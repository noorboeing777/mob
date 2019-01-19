/****************************************************************/
/*  Copyright 1995, David B. Wortman                            */
/*      machine.c                                               */              
/*      Pseudo machine interpreter for CSC488S Project          */
/*                                                              */
/****************************************************************/

static char machineVersion[] = 
        "$Id: machine.c,v 1.20 1996/03/19 14:24:42 dw Exp $" ;

/*   Include common definitions and global variables            */
#include "common.h"
#include "globalvars.h"

/*   Include interface to this module and machine defintions    */

#include "machine.h"

        /*  real home of machine memory                         */
#define EXTERN_MACHINE

#include "machineDef.h"

        /*  Display registers                                   */
int display[ DISPLAYSIZE ] ;
        /*  (approximate) top item in the display               */
int displayMax = -1 ;

BOOLEAN  executing = TRUE ;

/****************************************************************/
/*                                                              */
/*      Internal procedure to dump state of machine to a file   */
/*                                                              */
/*      Note: pc, msp, mlp are passed explicitly to             */
/*      dumpMachineState and runError so they can be register   */
/*      variables for speed in the interpreter.                 */
/*                                                              */
/****************************************************************/

static void dumpMachineState( FILE * sinkFile, char * msg,
                int pc, int msp, int mlp  ) {
        int i, mspmin, mspmax  ;
        fprintf( sinkFile, "%s", msg );
        if( strlen( msg) > 50 )
            fprintf( sinkFile, "\n\t" );
        fprintf(sinkFile, "\t pc = %5d, msp = %5d, mlp = %5d",
                pc, msp, mlp );
        if( displayMax >= 0 ){          
            fprintf(sinkFile, "\t display[ 0 .. %d ] = ", displayMax );
            for( i = 0 ; i <= displayMax ; i++ )
                fprintf( sinkFile, "%8d", display[i] ); 
        };
        if( 0 <= msp && msp < MEMORYSIZE ){
            mspmin = ( msp - 8 >= 0 ? msp -8 : 0 ) ;
            mspmax = ( msp < MEMORYSIZE ? msp : MEMORYSIZE -1 );
            fprintf(sinkFile, "\n\tmemory[ %d .. %d ] = ", mspmin, mspmax );
            for( i = mspmin ; i <= mspmax ; i++ )
                fprintf( sinkFile, "%8hd", memory[i] ); 
            fprintf( sinkFile, "\n" );
        };
    }
    
/************************************************************************/
/*                                                                      */
/*      Internal procedure to print error message and stop execution    */
/*                                                                      */
/************************************************************************/

static void runError( char *msg , int pc, int msp, int mlp ){
        char msgBuff[121] = "Execution Error - " ;
        strncat( msgBuff, msg, 120 -strlen(msgBuff)  );
        dumpMachineState( errorFile, msg, pc, msp, mlp );
        errorOccurred = TRUE ;
        executing = FALSE ;
    }


void machineInitialize() {
        /* When the electricity is turned on,            */
        /* all storage contains undefined                */
        int i ;
        for( i = 0 ; i < MEMORYSIZE ; i++ )
            memory[i] = undefined ;
        for( i = 0 ; i < DISPLAYSIZE ; i++ )
            display[i] = undefined ;
}

/************************************************************************/
/*                                                                      */
/*      Utility procedure for formatting one instruction for a dump     */
/*                                                                      */
/************************************************************************/

static char instructionBuffer[ 64 ] ;   /* output from formatInstruction */

static int formatInstruction( int addr ) {
        /* format the instruction at addr and leave the result in       */
        /* instructionBuffer                                            */
        /* For convenience, returns the length of the instruction       */
        int opCode = memory[ addr ] ;
        instructionBuffer[0] = 0 ;      /* null terminate */
        /* pack instructions onto one line */
        if( firstInstruction <= opCode && opCode <= lastInstruction ){
           switch( instructionLength[ opCode ] ){
              case  0:          /*  include BR and BF  */
              case  1:
                sprintf( instructionBuffer, "%8s", instructionNames[ opCode ] );
                return 1 ;
                
              case 2 :
                sprintf( instructionBuffer, "%8s%8hd", 
                instructionNames[ opCode ], memory[addr+1] );
                return 2 ;
                
              case 3:
                sprintf( instructionBuffer, "%8s%8hd%8hd", instructionNames[ opCode ], 
                       memory[ addr+1], memory[addr+2] );
                return 3 ;
                
             default :
                assert( FALSE );    /* error in instructionLength[] */
           } ;
         }
         else{  /*  not an instruction */
            sprintf( instructionBuffer, "%8d", opCode );
            return 1 ;
         }
   }
   
/************************************************************************/
/*                                                                      */      
/*      instruction dump  - print the object code                       */
/*                              in memory[ 0 .. startMSP -1 ]           */
/*                                                                      */
/************************************************************************/

static void  instructionDump() {
        char  lineBuffer[81] ;
        int addr, firstAddr, iLength ;
        
        lineBuffer[0] = 0 ;     /* null terminate */
        addr = 0 ;
        firstAddr = 0 ;
        while( addr < startMSP ) {
            iLength = formatInstruction( addr );
            strcat( lineBuffer, instructionBuffer );
            
            if( strlen( lineBuffer ) >= 64 ){
                /* print current line of dump */
                fprintf( dumpFile, "memory[ %4d .. %4d ] = %s\n",
                    firstAddr, addr, lineBuffer );
                lineBuffer[0] = 0 ;     /* null terminate */
                firstAddr = addr + iLength ;
            };
            addr += iLength ;
        };    
        if( strlen( lineBuffer ) > 0 ) 
            fprintf( dumpFile, "memory[ %4d .. %4d ] = %s\n",
                   firstAddr, addr, lineBuffer );
   }

/************************************************************************/
/*                                                                      */
/*  Interpreter for CSC488S Pseudo Machine                              */
/*                                                                      */
/************************************************************************/

void machineExecute() {
   /*********************************************************************/
   /* executes the object code in memory                                */
   /* Sets initial machine state from global variables                  */
   /*   startPC, startMSP, startMLP                                     */
   /* It changes the state of the machine,                              */
   /*    i.e. memory, display, pc, and msp.                             */
   /*                                                                   */
   /* Attention: we ought to check the following:                       */
   /*   arithmetic overflow                                             */
   /*********************************************************************/

   /*  Real machine registers                                           */
        /*  Program counter                                     */
   register int pc ;
        /*  Memory stack pointer                                */
   register int msp ;
        /*  Memory limit pointer                                */
   register int mlp ;
   
   int adr ;            /* memory address temp          */
   int n ;              /* count temp                   */
   int i ;              /* loop index                   */
   int v ;              /* value temp                   */
   int ll ;             /* lexical level temp           */
   int opCode ;         /* current isntruction code     */
   BOOLEAN tracing ;    /* true if tracing program execution */

   /*************************************************************/   
   /*   Macro for validating machine registers                  */
   /*   If an error is detected it does an immediate return     */
   /*   from executeMachine                                     */
   /*************************************************************/

#define rangeCheck( val, lowBound, highBound, msg  )\
  {\
        if( (val) < (lowBound) || (val) > (highBound) )\
        {\
                runError( msg , pc, msp, mlp );\
                return ; \
        }\
  }

#define MEMORY_TOP  ( MEMORYSIZE - 1 )
#define DISPLAY_TOP ( DISPLAYSIZE - 1 )                        

   /*************************************************************/   
   /*   Macro for manipulating the machines stack               */
   /*                                                           */
   /*   NOTE the convention that msp points at                  */
   /*   the first UNUSED entry in the stack.                    */
   /*   The TOP item on the stack is memory[msp-1].             */
   /*                                                           */
   /*************************************************************/

#define  SPUSH   msp++ 
#define  SPOP    msp--
#define  TOP    memory[msp-1]
#define  TOPP1  memory[msp]
#define  TOPM1  memory[msp-2]

   /*************************************************************/  
   /*   Initialize state of the machine for execution           */
   /*************************************************************/
   pc = startPC ;
   msp = startMSP ;
   mlp = startMLP ;
   tracing = traceExecution ;
   rangeCheck( pc, 0, MEMORY_TOP, "Initial value of pc outside memory");
   rangeCheck( mlp,0, MEMORYSIZE, "Initial value of mlp out of range" );
   rangeCheck( msp,0, mlp - 1, "Initial value of msp out of range" );
    

   if( dumpInstructions ) 
       instructionDump() ;
       
   if( suppressExecution ) {
       fprintf( outputFile, "Execution suppressed by control flag");
       return ;
   } else
       dumpMachineState( outputFile, "Start Exectution" , pc, msp, mlp );
   
   /*   During the execution of each instruction:               */
   /*   opCode  contains the instruction code                   */
   /*   pc      points at the instructon                        */
   
   while( executing ){ 
      /*  Execute one instruction from memory                   */
      
      rangeCheck( pc, 0, MEMORY_TOP, "Program counter outside memory" );
      opCode = memory[ pc] ;
      
      if( msp < startMSP )
          runError( "Run stack underflow", pc, msp, mlp );
      if( msp >= startMLP )
          runError( "Run stack overflow", pc, msp, mlp );
          
      if( tracing ) {
          formatInstruction( pc );
          fprintf( traceFile, "%d: %s\n", pc, instructionBuffer );
       }; 
          
      switch( opCode ){ 
      
        case ADDR:  ll = memory[pc+1] ;
                    rangeCheck( ll, 0, displayMax , 
                                        "ADDR Display index out of range" );
                    SPUSH ;
                    TOP = display[ ll ] + memory[pc+2] ;
                    break ;
                     
        case LOAD:   adr = TOP ;
                     rangeCheck( adr, 0, MEMORY_TOP,
                        "LOAD address out of range" );
                     if( memory[adr] == undefined ){
                          runError("Attempt to LOAD undefined value ",
                                     pc, msp, mlp  );
                     }
                     else{
                          TOP = memory[adr] ;
                     };
                     break ;
                     
        case STORE:  v = TOP ;    SPOP ;
                     adr = TOP ;  SPOP ;
                     rangeCheck( adr, 0, MEMORY_TOP,
                                        "STORE address out of range" );
                     memory[ adr ] = v ;
                     break ;
                     
        case PUSH:   SPUSH ;
                     TOP = memory[pc+1] ;
                     break ;
                     
        case PUSHMT: SPUSH ;
                     TOP = msp - 1 ;
                     break ;
                     
        case SETD:  adr = TOP ;
                    SPOP;
                    ll = memory[ pc+1 ] ;
                    rangeCheck( ll, 0, DISPLAY_TOP, 
                              "SETD display index out of range" );
                    rangeCheck( adr, startMSP, mlp, 
                              "SETD display entry out of range");
                    displayMax = ( displayMax > ll ? displayMax : ll );
                    display[ ll ] = adr ;
                    break ;
                    
        case POPN:  msp -= TOP ;
                    SPOP ; 
                    break ;
                    
        case POP:   SPOP ;
                    break ;
                    
        case DUPN:  n = TOP ;  SPOP ;
                    v = TOP ;  SPOP ;
                    rangeCheck( msp+n, startMSP, startMLP,
                           "DUPN stack overflow" );
                    for(i = msp ; i <=  msp-1+n ; i++ )
                         memory[i]= v ;
                    msp += n ;
                    break ;
                    
        case DUP:   SPUSH ;
                    TOP = TOPM1 ;
                    break ;
                    
        case BR:    pc= TOP ;   /* BR sets pc directly */
                    SPOP ;
                    break ;
                    
        case BF:    adr = TOP ;    SPOP ;
                    v   = TOP ;    SPOP ;
                    if( v == MACHINE_FALSE )
                       pc = adr ;       
                    else                /* BF sets pc directly */
                       pc++ ;
                    break ;
                    
        case NEG:   TOP = - TOP ;
                    break ;
                    
        case ADD:   SPOP ;
                    TOP += TOPP1 ;
                    break ;
                    
        case SUB:   SPOP ;
                    TOP -= TOPP1 ;
                    break ;
                    
        case MUL:   SPOP  ;
                    TOP *= TOPP1 ;
                    break ;
                    
        case DIV:   v = TOP ;   SPOP ;
                    if( v != 0 )
                        TOP /= v ;
                    else
                        runError( "Attempt to divide by zero", pc, msp, mlp );
                    break ;
                    
        case EQ:    SPOP  ;
                    TOP = ( TOP == TOPP1 ? MACHINE_TRUE : MACHINE_FALSE ) ;
                    break ;
                   
        case LT:   SPOP ;
                   TOP = ( TOP < TOPP1 ? MACHINE_TRUE : MACHINE_FALSE ) ;
                   break ;
                   
        case OR:   SPOP ;
                   TOP = (( memory[msp-1] == MACHINE_TRUE 
                         || memory[msp] == MACHINE_TRUE ) ?
                                   MACHINE_TRUE : MACHINE_FALSE );
                   break ;
                   
        case SWAP: v = TOPM1 ;
                   TOPM1 = TOP ;
                   TOP = v ;
                   break ;
                   
        case READC: SPUSH ;
                    if( (v = fgetc( runInputFile )) == EOF )
                        runError("Character input (READC) read EOF", 
                                  pc, msp, mlp );
                    else
                        TOP = v ;
                    break ;
                    
        case PRINTC: fputc( TOP & 0x7F, outputFile );  
                     SPOP ;
                     break ;
                     
        case READI:  SPUSH ;
                     if( fscanf( runInputFile, " %hd", & TOP  ) !=  1)
                        runError("Integer input (READI) failed", pc, msp, mlp );
                     break ;
                     
        case PRINTI: fprintf( outputFile, "%hd", TOP );
                     SPOP ;
                     break ;
                     
        case HALT:  executing = FALSE ;         /* end of execution */
        
        case TRON:  tracing = TRUE ;
                    dumpMachineState(traceFile,  "Start trace (TRON)", 
                                          pc, msp, mlp );
                    break ;
                    
        case TROFF: tracing = FALSE ;
                    dumpMachineState( traceFile, "End trace (TROFF)", 
                                       pc, msp, mlp  );
                    break ;
                    
        default:   runError("Illegal instruction code", pc, msp, mlp  );
                   break ;
      
         }      /* end switch on instruction code */
        
        pc += instructionLength[ opCode ];      /* update pc    */
        
     }  ;       /* end interpreter main loop      */
     
     dumpMachineState( outputFile, "End Execution", pc, msp, mlp );
     
  }
