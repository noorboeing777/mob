void ErrorHandler ();  /* AJM */

/****************************************************************/
/*  Copyright 1995, David B. Wortman */
/*                                                              */
/*      codegen.c                                               */
/*      Code Generation module for the CSC488S Project Compiler */
/*                                                              */
/****************************************************************/

static char codegenVersion[] = 
        "$Id: codegen.c,v 1.4 1996/04/23 18:58:48 singhdi Exp $" ;

/****************************************************************/
/*                                                              */
/*  Code Generation Conventions                                 */
/*                                                              */
/*  To simplify the course project, this code generator is      */
/*  designed to compile directly to pseudo machine memory       */
/*  which is available as the array memory[] in machineDef.h    */
/*                                                              */
/*  It is assumed that the code generator places instructions   */
/*  in memory in locations                                      */
/*      memory[ 0 .. startMSP - 1 ]                             */
/*  The code generator may also place instructions and/or       */
/*  constants in high memory at locations                       */
/*      memory[ startMLP .. MEMORYSIZE - 1 ]                    */
/*  During program exection the memory area                     */
/*      memory[ startMSP .. startMLP - 1 ]                      */
/*  is used as a dynamic stack for storing activation records   */
/*  and temporaries used during expression evaluation.          */
/*  A hardware execption (stack overflow) occurs if the pointer */
/*  for this stack reaches the memory limit register (mlp).     */
/*                                                              */
/*  The code generator is responsible for setting the global    */
/*  variables:                                                  */
/*      startPC         initial value for program counter       */
/*      startMSP        initial value for msp                   */
/*      startMLP        initial value for mlp                   */
/*                                                              */
/****************************************************************/

/*  Include common definitions and global variables             */
#include "common.h"
#include "globalvars.h"

/*   Include parser/codegen interface                           */
#include "parsecodegen.h"

/*  Include machine definitions for code generation             */
#include "machineDef.h"

/*  Include interface to machine module                         */
#include "machine.h"

#include "symbol.h"

#ifndef SEMEXT
#define SEMEXT
   #include "semanticext.h"
#endif
/*  Utility procedures used for code generation GO HERE         */



/**************************/
/*    FBS stack           */
/**************************/
struct FBSstack 
    {
    int address;
    struct FBSstack *next;
    };
struct FBSstack *FBS_top = NULL;

/* Function to pop FBS stack */
FBS_Pop()
{
    struct FBSstack *temp;
    temp = FBS_top;
    FBS_top = FBS_top->next;
    free(temp);
}

/* Function to push an item onto FBS stack */    
void FBS_Push(int addr)
{
    struct FBSstack *temp;
    temp = (struct FBSstack *) malloc(sizeof(struct FBSstack));
    temp->address = addr;
    temp->next = FBS_top;
    FBS_top = temp;  
}

void FBS_Swap()
{
  int temp1, temp2;
  temp1 = FBS_top->address;
  FBS_Pop();
  temp2 = FBS_top->address;
  FBS_Pop();
  FBS_Push(temp1);
  FBS_Push(temp2);

}

/**************************/
/*    BBS stack           */
/**************************/

struct BBSstack 
{
    int address;
    struct BBSstack *next;
};

struct BBSstack *BBS_top = NULL;

/* Function to pop BBS stack */
BBS_Pop()
{
    struct BBSstack *temp;
    temp = BBS_top;
    BBS_top = BBS_top->next;
    free(temp);
}

/* Function to push an item onto BBS stack */    
BBS_Push()
{
    struct BBSstack *temp;
    temp = ( struct BBSstack *) malloc(sizeof(struct BBSstack));
    temp->address = startMSP;
    temp->next = BBS_top;
    BBS_top = temp;  
}

/***************************/
/*     LES stack           */
/***************************/
struct LESstack 
    {
    int address;
    struct LESstack *next;
    };
struct LESstack *LES_top = NULL;

/* Function to pop LES stack */
LES_Pop()
{    
    struct LESstack *temp;
    temp = LES_top;
    LES_top = LES_top->next;
    free(temp);
}

/* Function to push an item onto LES stack */    
LES_Push(int addr)
{
    struct LESstack *temp;
    temp = ( struct LESstack *) malloc(sizeof(struct LESstack));
    temp->address = addr;
    temp->next = LES_top;
    LES_top = temp;  
}


void Code(int IorD)
{
    memory[startMSP] = IorD;
    startMSP++;
    
}



/************************************/
/*     array stack */
/************************************/
struct array_stack 
    {
    STPtr ptr;
    struct array_stack *next;
    };
struct array_stack *array_top = NULL;

/* Function to pop stack */
Pop_arrayName()
{    
    struct array_stack *temp;
    temp = array_top;
    if (temp != NULL)
      {
      array_top = array_top->next;
      free(temp);
      }
}

/* Function to push an item onto stack */    
Push_arrayName(STPtr addr)
{
    struct array_stack *temp;
    temp = ( struct array_stack *) malloc(sizeof(struct array_stack));
    temp->ptr = addr;
    temp->next = array_top;
    array_top = temp;  
}



/************************************/
/*     Function and procedure stack */
/************************************/
struct Func_stack 
    {
    STPtr address;
    struct Func_stack *next;
    };
struct Func_stack *Func_top = NULL;

/* Function to pop stack */
Pop_FuncName()
{    
    struct Func_stack *temp;
    temp = Func_top;
    if (temp != NULL)
      {
      Func_top = Func_top->next;
      free(temp);
      }
}

/* Function to push an item onto stack */    
Push_FuncName(STPtr addr)
{
    struct Func_stack *temp;
    temp = ( struct Func_stack *) malloc(sizeof(struct Func_stack));
    temp->address = addr;
    temp->next = Func_top;
    Func_top = temp;  
}

int Return_Func_NumArgs()
{
    int value;
    if (Func_top != NULL)
      value = (Func_top->address)->NumOfParam;
    else
      value = 0;
    return value;
}



/*************************************/
/*            Types                  */
/*************************************/
typedef enum InstructionCodes INSTR;

/**************************************/
/*           Global Variables         */
/**************************************/

/****************************************/
/*          #define                     */
/****************************************/

/* initial value for program counter            */
/* i.e. address of first instruction in the     */
/* compiled program                             */      
int startPC;

/* initial value for memory stack pointer (msp) */
int startMSP ;

/* initial value for memory limit pointer (mlP) */
int startMLP ;

void allocate_storage()
{
   STPtr pr;
   int z;
   pr = FindToken(previousString);
   for(z=1; z<=pr->Size; z++)
   {
      Code(PUSH);
      Code(UNDEFINED);
   }
}

Code31()
{
   Code( (INSTR) ADDR);
   Code((SymbolS_top->ptr)->ScopeNum);
   if ( (SymbolS_top->ptr)->KindOf != parameter )
      Code((SymbolS_top->ptr)->Offset);
   else      
      Code((SymbolS_top->ptr)->Offset - (Return_Func_NumArgs() + 1) );
}

Code32()
{
  /* assume that the address is on the top of the stack */
  Code( (INSTR) LOAD);
}


Code46()
{
   /* Allocate space for keeping track of the result*/
   Code ((INSTR) PUSH);
   Code (UNDEFINED);
   
   /* Allocate space for return value on AR */
   Code ((INSTR) PUSH);
   FBS_Push(startMSP);
   Code (UNDEFINED);

   
}

Code47()
{
   /* branch to fcn location in memory */
   Code ((INSTR) PUSH);
   Code ((SymbolS_top->ptr)->CodeAddr);  
   Code( (INSTR)BR);

   /* write the startMSP as the return address */
           
   memory[FBS_top->address] = startMSP;
   FBS_Pop();


}


void codegenInitialize(){

        /* Initialization code for the code generator GOES HERE */
        /* This procedure is called once before codeGeneration  */
        
        /* This code generator generates code directly to memory*/
        /* so initialize machine memory before code generation  */
        /* begins.                                              */
        startMSP=0; /* first instruction stored at memory[0]     */
        startPC=0; /* first instruction stored at memory[0]     */
        startMLP = MEMORYSIZE-1;
        machineInitialize();
        return ;
}

void codegenFinalize() {

        /* Finalization code for the code generator GOES HERE.  */
        
        /* This procedure is called once at the end of code generation  */
        
        /* Do something simple in this skeleton */
        startPC = 0 ;
        /* startMSP=startMSP-1; */
        return ;
}


void generateCode( int actionNumber )
{

    if( traceCodeGen == TRUE )
       fprintf( traceFile, "codegen: C%d\n",
                   actionNumber - firstCodeGenToken );
              
    /*  Code to implement the code generation actions GOES HERE     */
    /*  This dummy code generator just prints the actionNumber      */
    
     
    switch (actionNumber - firstSemanticToken - 100 )
       {
       
        /* ------------------------ */
        /* Code Gen Numbers         */
        /* ------------------------ */
        
       case 0 :  /* Set pc and mt to values for execution start */
            startPC = 0;          /*    initial value for program counter   */
            break;

       case 1 :  /* Check LL and emit */
         {
            if ( ScopeLevel < DISPLAYSIZE && ScopeLevel >0 )
            {
                /* stack top is now address of where DISPLAY should be */
                
                if ( ScopeLevel > PrevScope)
                {
                   Code( (INSTR) PUSHMT);
                   Code( (INSTR) SETD);
                   Code(ScopeLevel);
                   
                   
                   Code((INSTR) ADDR);  /* puts current display[LL] onto stack for return */
                   Code(ScopeLevel);
                   Code(0);
                }
                else
                {
                   Code((INSTR) ADDR);  /* puts current display[LL] onto stack for return */
                   Code(ScopeLevel);
                   Code(0);
                   Code( (INSTR) PUSHMT);
                   Code( (INSTR) PUSH);
                   Code( 1 );
                   Code( (INSTR) SUB);
                   Code( (INSTR) SETD);
                   Code(ScopeLevel);
                }
                
            }
            else if ((ScopeLevel < DISPLAYSIZE) && (ScopeLevel == 0 ))
            {
                Code( (INSTR)PUSHMT);
                Code((INSTR)SETD);
                Code(0);

                Code((INSTR) ADDR);  /* puts current display[LL] onto stack for return */
                Code(ScopeLevel);
                Code(0);
                
            }
            else 
            {
                fprintf(errorFile, "Display Size not large enough, too many lexic levels\n");
                ErrorHandler();
            }
         }
         break;
         
       case 2 : 
         {
            if ( ScopeLevel < DISPLAYSIZE )
            {
                STPtr ptr = FindToken(previousString);         
                /* top now address of where DISPLAY should be */
                Code(UNDEFINED); /* allocate space for return value */
                Code((INSTR) ADDR);  /* puts current display[LL] onto stack for return */
                Code(ScopeLevel);
                Code(0);
   
            }
            else 
            {
              fprintf(errorFile, "Display Size not large enough, too many lexic levels\n");
              ErrorHandler();
            }
         }
         break;

       case 3 : 
            break;
       case 4 : 
          {      
            Code((INSTR) ADDR); /* place DISPLAY[LL-RETURN_ADDR] on top of stck*/
            Code(ScopeLevel);
            Code(-(Return_Func_NumArgs()+2));
          }  
          break;
       case 5 : 
          {
            Code((INSTR) PUSHMT);
            Code((INSTR) ADDR);  /* puts current display[LL] onto stack for return */
            Code(ScopeLevel);
            Code(0);
            Code((INSTR) SUB);

            Code(PUSH);   /* PUSH 1 onto stack then subtract   */
            Code(1);           /* this results in the POPN pointing */
            Code(SUB);         /* to the right mem. location        */
            Code((INSTR) POPN);  /* top of stck contains size of vars+args */
            Code((INSTR) SETD);  /* restore old DISPLAY [LL] */
            Code(ScopeLevel);
            
            /* Remove the arguments */
            Code ( (INSTR) PUSH);
            Code (Return_Func_NumArgs()); 
            Code ( (INSTR) POPN);
          }
          break;

       case 6 : 
          {
            STPtr ptr  =  FindToken(previousString);         
            
            /* free vars */
            Code((INSTR) PUSHMT);
            Code((INSTR) ADDR);  /* puts current display[LL] onto stack for return */
            Code(ptr->ScopeNum);
            Code(0);
            Code((INSTR) SUB);
            Code((INSTR) POPN);  /* top of stck contains size of vars */
            Code((INSTR) SETD);  /* restore old DISPLAY [LL] */
          }  
           break;

       case 7 :   /* Emit forward unconditional branch. Record address of instruction in FBS.  */
            Code( (INSTR) PUSH);
            Code(UNDEFINED);    /* the value will be determined later */
            FBS_Push(startMSP);
            Code( (INSTR) BR);
            break;
       case 8 :   /* Emit forward branch on false. Record address of instruction in FBS. */
            Code ( (INSTR) PUSH);
            Code (UNDEFINED);     /*  address to be determined later */
            FBS_Push(startMSP);
            Code (BF);            
            break;

       case 9 :   /* Fix up address of forward branch identified by top entry of FBS to current location. Pop FBS.  */
            Pop_FuncName();
            memory [ FBS_top->address-1 ]  =   startMSP;     
            FBS_Pop();
            break;

       case 10 :  /* Push current location onto BBS stack. */
            BBS_Push ();
            break;

       case 11 :  /* Emit unconditional branch to address on top of BBS. Pop BBS.  */
            Code( (INSTR) PUSH);
            Code(BBS_top->address);
            Code( (INSTR) BR);
            BBS_Pop();   
            break;
            
       case 12 :  /* Interchange top two entries in FBS. */
            FBS_Swap();
            break;
       case 13 : /* make top of stack Negative */
            Code( (INSTR) NEG);
            break;

       case 14 : 
            Code( (INSTR) ADD);
            break;

       case 15 : 
            Code( (INSTR) SUB);
            break;
       case 16 : 
            Code( (INSTR) MUL);
            break;

       case 17 : 
            Code( (INSTR) DIV);
            break;

       case 18 : /* do negation ie. NOT relExpression */
            /* assume top of stack has True=1 or False=0 */
            Code ( (INSTR) PUSH);
            Code (1);
            Code ( (INSTR) SWAP);
            Code ( (INSTR) SUB);
            break;

       case 19 : /* Emit instr for 'and' */
           
            /* x and y == not ( not x or not y) */   
            
            /* do a NOT now */
            Code (1);
            Code ( (INSTR) SWAP);
            Code ( (INSTR) SUB);

            Code ( (INSTR) SWAP);
            /* do a NOT now */
            Code (1);
            Code ( (INSTR) SWAP);
            Code ( (INSTR) SUB);

            Code ( (INSTR) OR);

            /* do a NOT now */
            Code ( (INSTR) PUSH);
            Code (1);
            Code ( (INSTR) SWAP);
            Code ( (INSTR) SUB);
            break;
       
       case 20 : /* Emit instr for 'or' */
            Code ( (INSTR) OR);
            break;

       case 21 : /* Emit instr for '=' */
            Code ( (INSTR) EQ);
            break;

       case 22 : /* Emit instr for 'not' '=' */
            Code ( (INSTR) EQ );      
            
            /* do a NOT now */
            Code ( (INSTR) PUSH);
            Code ( 1 );
            Code ( (INSTR) SWAP);
            Code ( (INSTR) SUB);

            break;

       case 23 : /* Emit instr for '<' */
            Code ( (INSTR) LT);
            break;

       case 24 : /* Emit instr for '<''=' */
            /* e1 <= e2  --> !(e1 > e2) --> !(e2 < e1) */
            Code ( (INSTR) SWAP);
            Code ( (INSTR) LT);
            
            /* do a NOT now */
            Code ( (INSTR) PUSH);
            Code (1);
            Code ( (INSTR) SWAP);
            Code ( (INSTR) SUB);
            break;

       case 25 :  /* greater than */
            Code ( (INSTR) SWAP );   /*     x < y  is same as y < x     */
            Code ( (INSTR) LT );
            break;

       case 26 :   /* greater than or equal  */

            Code ( (INSTR) LT );      /*   x >= y  is same as   !( x < y )   */
            Code ( (INSTR) PUSH);
            Code ( 1 );
            Code ( (INSTR) SWAP);
            Code ( (INSTR) SUB);
            break;

       case 27 :   /* Emit instructions to read and store integer */
            Code ( (INSTR) READI);   /* Reads in digits converts to integer and pushes to top of mem stack */
            Code ( (INSTR) STORE);   
            break;
            
       case 28 :   /* Emit instructions to print integer.  */
            Code ( (INSTR) PRINTI );
            break;

       case 29 :  /* Emit instructions to print text */
          {
            int TextLength;
            int u;
            TextLength = strlen(previousString);
            for (u=0; u < TextLength; u++)
            {
               Code ( (INSTR) PUSH);
               Code (previousString[u]);
               Code ( (INSTR) PRINTC);
            }  
         }
            break;
       case 30 :  /* Emit instructions to skip to new line on output. */
            Code ( (INSTR) PUSH);
            Code ('\n');
            Code ( (INSTR) PRINTC);
            break;
 
   /* Expressions, Input & Output */
       

       case 31 : /* Emit instructions to obtain address of variable */ 

            Code31();
            break;

       case 32 :  /* Emit instructions to obtain value of variable */
       
            Code32();
            break;

       case 33 : /* Emit STORE */
       
            Code( (INSTR) STORE);
            break;
            
       case 34 : /* Emit PUSH 0 */
       
            Code( (INSTR) PUSH);
            Code(0);
            break;

       case 35 : /* Emit PUSH 1 */
       
            Code( (INSTR) PUSH);
            Code(1);
            break;

       case 36 : /* Emit PUSH intergervalue */
       
            Code( (INSTR) PUSH);
            Code(scanInteger);
            break;

  
       case 37 :  /* Emit forward branch for exit. Record instruction address in LES.  */
            Code( (INSTR) PUSH);
            LES_Push(startMSP);
            Code(UNDEFINED);  /*  'address to be determined later' */
            Code( (INSTR) BR);
            break;

       case 38 : 
            Code( (INSTR) HALT);
            break;
            
       /* Scalars and Arrays */


       case 39 : /* Emit instructions to allocate storage for an array variable */
       
            allocate_storage();
            break;
            
       case 40 : /* Emit instructions to obtain address of an array variable */
        
            Push_arrayName(FindToken(previousString));
            /* push the address of the beginning of the array onto the stack */
            Code( (INSTR) ADDR);
            Code((SymbolS_top->ptr)->ScopeNum);
            Code((SymbolS_top->ptr)->Offset);
            break;

       case 41 : /* Emit instructions to check that index is in bounds */
                 /* Emit instructions to create address of array element */
       
            {
            STPtr pr  =  array_top->ptr;         
            Pop_arrayName();
            /* emit instruction to check the index */
            Code( (INSTR) DUP);
            Code( (INSTR) PUSH);
            Code(pr->Size);
            Code( (INSTR) LT);
            Code( (INSTR) PUSH);
            FBS_Push(startMSP);            
            Code( UNDEFINED);       /* the address of <stop> */
            Code( (INSTR) BF);
            Code( (INSTR) DUP);
            Code( (INSTR) PUSH);
            Code(UNDEFINED);
            Code( (INSTR) LT);
            Code( (INSTR) PUSH);
            FBS_Push(startMSP);            
            Code( UNDEFINED);       /* the address of <CONTINUE> */
            Code( (INSTR) BF);
            Code( (INSTR) HALT);     /* this is the location of <STOP> */
            FBS_Swap();
            memory[FBS_top->address] = startMSP-1;
            FBS_Pop();
            /* emit instruction to create address */
            Code( (INSTR) ADD);      /* this is the location of <CONTINUE> */
            memory[FBS_top->address] = startMSP-1;
            FBS_Pop();
            }
            break;

       case 42 : /* Emit instructions to allocate storage for a scalar variable */
             
            Code(PUSH);
            Code(UNDEFINED);
            break;              

       /* ********************************** */
       /* Functions Procedures and arguments */
       /* ********************************** */

       case 43 : /* Emit instr to return from fcn or proc */

           #ifdef NO_DEF
           Code((INSTR) ADDR);
           Code(ScopeLevel);
           Code(-1);             /* offset for return addr of PC */
           Code( (INSTR) LOAD);
           #endif
           Code( (INSTR) BR);
           break;


       case 44 :  /* Emit instr to call a proc */  


           /* branch to procedure location in memory */
           Code((INSTR) PUSH);
           Code ((SymbolS_top->ptr)->CodeAddr);  
           Code( (INSTR)BR);
           
           /* write the startMSP as the return address */
           
           memory[FBS_top->address] = startMSP;
           FBS_Pop();

           break;

       case 45 : /* Emit instr to generate block mark for proc call */
           
           /* Put return address onto AR */
           Code ((INSTR) PUSH);
           FBS_Push(startMSP);
           Code (UNDEFINED);
           break;

       case 46 : /* Emit instr to generate block mark for fcn call */
       
           Code46();
           break;

       case 47 : /* Emit instr to call fcn */

           Code47();
           break;

       case 48 : /* Emit instr to save args for call */
                   
           break;

       case 49 :  /* Push LES */
            /* Pushing UNDEFINED onto LES stack because you can have multiple
               exit statements within a loop.  Therefore there can be multiple
               places in a loop where you branch out of the loop.  These places
               are stored on the LES stack.  
            */ 
            LES_Push(UNDEFINED); 
            break;
            
       case 50 :  /* Fixup branches described by top entry in LES to current location. Pop LES.  */
           
            while ( LES_top->address != UNDEFINED )    
            {
               memory [ LES_top->address ] = startMSP;     /* 'current memory location' */
               LES_Pop();
            }
            break;
          

       /* ambiguity resolution */


       case 51 : /* if symbol is a scalar variable do C31 */
                 /* if symbol is a function do C46 */
            {
            STPtr pr  =  FindToken(previousString);         
            if (pr->KindOf == function)
                Code46();
            else
                Code31();
            }
            break;
            
       case 52 : /* if symbol is a scalar variable to C32 */
                 /* if symbol is a function do C47 */
       
            {
            STPtr pr  =  FindToken(previousString);         
            if (pr->KindOf == function)
                Code47();
            else
                Code32();
            }
            break;

        }  /* end switch */
       
       return ;
}


