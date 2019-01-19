#ifndef _GLOBAL_VARS_H_
/********************************************************/
/*  Copyright 1995, David B. Wortman                    */
/*                                                      */
/*  globalvars.h                                        */
/*  CSC488S Project Compiler Shared Global Variables    */
/*                                                      */
/*  This file contains the definition of global         */
/*  variables that are used for communication among     */
/*  the various compiler modules                        */
/*                                                      */
/********************************************************/
#define _GLOBAL_VARS_H_

#include "common.h"

        /*  These variables are extern except in main.c */
#ifndef  EXTERN_GLOBALS
#define  EXTERN_GLOBALS  extern
#endif

/********************************************************/
/*                                                      */
/*      FILE global variables.                          */
/*      Used to specify sinks for compiler output and   */
/*      sources for compiler input                      */
/*                                                      */
/********************************************************/

        /*  File used as an input source for the compiler  */
EXTERN_GLOBALS FILE * inputFile ;

        /*  File used as a sink for execution output    */
EXTERN_GLOBALS FILE * outputFile ;

        /*  File used as a sink for error messages      */
EXTERN_GLOBALS FILE *  errorFile ;

        /*  File used as a sink for table dumps         */
EXTERN_GLOBALS FILE *  dumpFile ;

        /*  File used as a sink for trace output        */
EXTERN_GLOBALS  FILE * traceFile ;

        /*  File used as input source during program execution */
EXTERN_GLOBALS  FILE * runInputFile ;

/********************************************************/
/*                                                      */
/*      Scanner global variables.                       */
/*      Contain information produced by the scanner     */
/*      and used by other modules                       */
/*                                                      */
/********************************************************/

        /*  Current line number in input.  Set by scanner  */
EXTERN_GLOBALS int line ;

        /*  Token produced by the scanner                  */
EXTERN_GLOBALS TOKEN scanToken ;

        /*  If scanToken = Tinteger then scanInteger    */
        /*  contains the value of the integer constant  */
EXTERN_GLOBALS int scanInteger ;

        /*  Maximum length ot text strings              */
#define MAX_STRING_VALUE  MAX_TEXT
        /*  MAX_TEXT defined in common.h                */
        
        /*  If scanToken = Tidentifier or               */
        /*     scanToken = Ttext then   */
        /*  scanString contains the value of the        */
        /*  identifier or text.                         */
EXTERN_GLOBALS char scanString[ MAX_STRING_VALUE] ;


/********************************************************/
/*                                                      */
/*      Parser global variables.                        */
/*      Used to pass information from the parser        */
/*      to semantics and code generation modules.       */
/*      This buffering of token values is necessary     */
/*      because the parser uses lookahead so the        */
/*      scanner might modify scanInteger or scanString  */
/*      before a semantic analysis or code generation   */
/*      action is applied that needs their value        */
/*                                                      */
/********************************************************/

        /*  Save scanString from scanner for semantic   */
        /*  analysis and code generation                */
EXTERN_GLOBALS char previousString[ MAX_STRING_VALUE ] ;

        /*  Save scanInteger from scanner for semantic  */
        /*  analysis and code generation                */
EXTERN_GLOBALS int previousInteger ;

/********************************************************/
/*                                                      */
/*      Code generator global variables                 */
/*      These global variables are used by the          */
/*      code generator module to communcate the         */
/*      initial value of the machine state to the       */
/*      pseudo machine interpreter.                     */
/*      See codegen.c for discussion of usage           */
/*      conventions.                                    */
/*                                                      */
/********************************************************/

        /* initial value for program counter            */
        /* i.e. address of first instruction in the     */
        /* compiled program                             */      
EXTERN_GLOBALS int startPC ;

        /* initial value for memory stack pointer (msp) */
EXTERN_GLOBALS  int startMSP ;

        /* initial value for memory limit pointer (mlP) */
EXTERN_GLOBALS  int startMLP ;
        

/***********************************************************/
/*                                                         */
/*      Control flags, set by main.c, used to cause        */
/*      various optional compiler actions                  */
/*                                                         */
/***********************************************************/

        /*  Flag used to indicate that an error occurred.  */
EXTERN_GLOBALS BOOLEAN errorOccurred ;

        /* Cause dump of parser director sets */
EXTERN_GLOBALS BOOLEAN dumpDirectorSets ;

        /* Cause dump of parser production tables       */
EXTERN_GLOBALS BOOLEAN dumpProductions  ;

        /*  Cause trace of source program as it is read */
EXTERN_GLOBALS BOOLEAN traceSource ;
        
        /*  Cause trace of scanner token stream         */
EXTERN_GLOBALS BOOLEAN traceScanner ;   

        /*  Cause trace of parser                       */
EXTERN_GLOBALS  BOOLEAN  traceParser ;

        /*  Cause trace of semantic analysis            */
EXTERN_GLOBALS  BOOLEAN  traceSemantics ;

        /*  Cause trace of symbol table module          */
EXTERN_GLOBALS  BOOLEAN  traceSymbols ;

        /*  Cause dump of symbol table information      */
EXTERN_GLOBALS  BOOLEAN  dumpSymbols ;

        /*  Cause trace of code generation              */
EXTERN_GLOBALS  BOOLEAN  traceCodeGen ;         

        /* Cause dump of instructions in memory before execution*/
EXTERN_GLOBALS BOOLEAN  dumpInstructions ;

        /* Cause execution to be traced                 */
EXTERN_GLOBALS BOOLEAN  traceExecution ;

        /* Suppress execution of compiled program       */
EXTERN_GLOBALS BOOLEAN  suppressExecution ;

#endif
