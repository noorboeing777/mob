void semanticsInitialize (); /* AJM */
void semanticsFinalize (); /* AJM */
void codegenInitialize (); /* AJM */
void codegenFinalize (); /* AJM */
/************************************************************************/
/*  Copyright 1995, David B. Wortman                                    */
/*                                                                      */
/*      main.c                                                          */
/*      This is the main driver program for the CSC488S course project  */
/*      compiler.  It handles compiler command line options and         */
/*      initialization, calls the parser to run the compilation         */
/*      and then invokes the pseudo machine interpreter to run          */
/*      the compiled program.                                           */
/************************************************************************/

static char mainVersion[] = 
                "$Id: main.c,v 1.1 1996/02/05 17:02:59 dw Exp $" ;      

/************************************************************************/
/* The compiler has the following parts:                                */
/*      language declarations           scanparse.h, parsetables.h      */
/*      communication variables         common.h, globalvars.h          */
/*      module interfaces               scanner.h, parser.h, semantics.h*/
/*                                      codegen.h, machine.h            */
/*      scanner module                  scanner.h, scanner.c            */
/*      symbol table module             symbol.h, symbol.c              */
/*      parser module                   parser.h, parser.c              */
/*      semantics analysis module       semantics.h, semantics.c        */
/*      code generator module           codegen.h, codegen.c            */
/*      machine interpreter module      machine.h, machineDef.h,        */
/*                                      machine.c                       */
/************************************************************************/


/************************************************************************/
/* Interfaces                                                           */
/*      The following include files are used to define interfaces       */
/*      between compiler passes.  See the appropriate include file      */
/*      or source file for further information                          */
/*                                                                      */
/*      scanner/parser interface        scanparse.h                     */ 
/*      parser/semantics interface      parsesemantics.h                */ 
/*      parser/codegen interface        parsecodgen.h                   */
/*      generate/machine interface      machineDef.h                    */  
/*      parser tables                   parsetables.h                   */
/*      encoding of tokens              tokenDef.h                      */
/*                                                                      */
/************************************************************************/

/*      Defintions common to all modules in the compiler                */
#include "common.h"

/*      Global variables used for communication between compiler        */
/*      modules.  This is the home for those variables.                 */
#define EXTERN_GLOBALS
#include "globalvars.h"

/* Include interface definitions for external procedures that we call */
/* Scanner Interface */
#include  "scanner.h"

/* Parser Interface */
#include  "parser.h"

/* Machine Interpreter Interface */
#include  "machine.h"

/* Default values for various files                     */
/* Note assumption that default files are not closed at */
/* the end of compilation                               */
#define DEFAULT_INPUT_FILE      stdin 
#define DEFAULT_OUTPUT_FILE     stdout
#define DEFAULT_ERROR_FILE      stderr
#define DEFAULT_DUMP_FILE       stdout
#define DEFAULT_TRACE_FILE      stdout
#define DEFAULT_RUN_INPUT_FILE  stdin
 
/* Internal utility procedure for opening optional files */
static FILE * fileOpen( char * fileName, char * fileMode, 
                                      FILE * defaultFile ){
    /*  open file "fileName" with mode "mode"           */
    /*  print error message and return default file     */
    /*  if open fails                                   */
    FILE * fTemp ;
    if( (fTemp = fopen( fileName, fileMode )) != NULL) 
         /* successfully opened the file        */
         return fTemp ;
    else {
        fprintf(errorFile, "Unable to open file %s\n", fileName );
        return defaultFile ;
    };   
};

/*********************************************************/
/*                                                       */  
/* Main program for the Compiler                         */
/*                                                       */
/*********************************************************/

main( int argc , char *argv[] ){
    
    /*  Initialize global variables to default values   */
    /*  before processing arguments and flags           */
    
    char optch ;        /* sub option character         */
    
    /*  Initialize files                                */
    inputFile  = DEFAULT_INPUT_FILE ;
    outputFile = DEFAULT_OUTPUT_FILE ;
    errorFile  = DEFAULT_ERROR_FILE ;
    dumpFile   = DEFAULT_DUMP_FILE ;
    traceFile  = DEFAULT_TRACE_FILE ;
    runInputFile = DEFAULT_RUN_INPUT_FILE ;
    /*  Initialize control flags                        */
    errorOccurred    = FALSE ;
    dumpDirectorSets = FALSE ;
    dumpProductions  = FALSE ;
    traceSource      = FALSE ;
    traceScanner     = FALSE ;
    traceParser      = FALSE ;
    traceSemantics   = FALSE ;
    traceSymbols     = FALSE ;
    dumpSymbols      = FALSE ;
    traceCodeGen     = FALSE ;
    traceExecution   = FALSE ;
    dumpInstructions = FALSE ;
    suppressExecution = FALSE ;
    
    
    /*  Process compiler option flags           */
    while( --argc > 0 && (*++argv)[0] == '-' ){
        /* process one -X argument              */

        switch( *++argv[0] ){
        case 'D' :  /* Dump options     */
                    while( optch = *++argv[0] ){
                        /* process to end of option letters */
                        switch( optch ) {
                        case 'd' :  dumpDirectorSets = TRUE ;
                                    break ;
                        case 'p' :  dumpProductions = TRUE ;
                                    break ;
                        case 'x':   dumpInstructions = TRUE ;
                                    break ;
                        case 'y':   dumpSymbols = TRUE ;
                                    break ;
                        default:    fprintf(errorFile, 
                                   "Invalid dump option %c ignored\n", optch );
                                   break ;
                        } ;
                    };
                    break ;
        case 'T' :  /* Trace options    */
                    while( optch = *++argv[0] ){
                        /* process to end of option letters */
                        switch( optch ) {
                        case 'c' :  traceCodeGen = TRUE ;
                                    break ;
                        case 'i' :  traceSource = TRUE ;
                                    break ;
                        case 'l' :  traceScanner = TRUE ;
                                    break ;
                        case 'p' :  traceParser = TRUE ;
                                    break ;
                        case 's' :  traceSemantics = TRUE ;
                                    break ;
                        case 'x':   traceExecution = TRUE ;
                                    break ;
                        case 'y':   traceSymbols = TRUE ;
                                    break ;
                        default:    fprintf(errorFile, 
                                   "Invalid trace option %c ignored\n",
                                   *(argv[0]-1) );
                                   break ;
                        } ;
                    };
                    break ;
                    
        case 'O' :      /* Alternative output file */
                    argc-- ;
                    outputFile = fileOpen( *++argv, "w", DEFAULT_OUTPUT_FILE );
                    break ;
                    
        case 'E'  :     /* Alternative error message file */
                    argc-- ;
                    errorFile = fileOpen( *++argv, "w", DEFAULT_ERROR_FILE );
                    break ;
                    
        case 'R' :      /* Alternative sink for  traces */
                    argc-- ;
                    traceFile = fileOpen( *++argv, "w", DEFAULT_TRACE_FILE );
                    break ;
                    
        case 'U' :      /* Alternative sink for dumps   */
                    argc-- ;
                    dumpFile = fileOpen( *++argv, "w", DEFAULT_DUMP_FILE );
                    break ;
                    
        case 'I' :      /* Alternative input during execution   */
                    argc-- ;
                    runInputFile = fileOpen( *++argv, "r", DEFAULT_RUN_INPUT_FILE);
                    break ;
                    
        case 'X' :      /* supress execution flag               */
                    suppressExecution = TRUE ;
                    break ;
                    
        default:        /* Anything else        */
                    fprintf(stderr,"Unknown option character %c (ignored)\n",
                        *(argv[0] - 1) );
                    break ;
        } ;
      };
     /* end of -X argument processing  */

    if( argc > 0 ) {
        /*  user supplied source file name on input line */
        inputFile = fileOpen( *argv, "r", DEFAULT_INPUT_FILE );
        fprintf(outputFile, "Compiling from source file %s\n", *argv );
     };
            
    fprintf(outputFile,"Compilation begins\n");
                                       
    /*  initialize the scanner  */
    scannerInitialize() ;
    /*  initialize the parser   */
    parserInitialize() ;
    /*  initialize the semantic analyzer        */
    semanticsInitialize() ;
    /*  initialize the code generator           */
    codegenInitialize() ;
    
    /*   start compiler running */
    parser();
    
    fprintf(outputFile, "\nCompilation Ends\n" );
    /*  clean up compiler modules               */
    semanticsFinalize();
    codegenFinalize();
     
    if( errorOccurred  )
        fprintf(outputFile, 
          "Execution suppressed due to compilation errors\n");
    else
        /*  machineExecute handles suppressExecution flag  */
        machineExecute();
    
    /*  Clean up files if necessary             */
    if( inputFile != DEFAULT_INPUT_FILE )
        fclose( inputFile );
    if( errorFile != DEFAULT_ERROR_FILE )
        fclose( errorFile );
    if( dumpFile != DEFAULT_DUMP_FILE )
        fclose( dumpFile );
    if( traceFile != DEFAULT_TRACE_FILE )
        fclose( traceFile );
    if( outputFile != DEFAULT_OUTPUT_FILE )
        fclose( outputFile );
    if( runInputFile != DEFAULT_RUN_INPUT_FILE )
        fclose( runInputFile );
  
}
