#ifndef _MACHINE_DEF_H_
/****************************************************************/
/*  Copyright 1995, David B. Wortman                            */
/*                                                              */
/*  Definition of csc488s Pseudo Machine                        */
/*                                                              */
/*  Interface between the Code Generator and Machine Interpreter*/
/*                                                              */
/****************************************************************/
#define _MACHINE_DEF_H_

static char machineDefVersion[] = 
  "$Id: machineDef.h,v 1.8 1996/02/02 14:04:47 dw Exp $" ;

#include "common.h"

/****************************************************************/
/* Machine Components                                           */
/*      normally these would be contained in the machine module */
/*      but this compiler cheats by compiling directly to memory*/
/*      so the code generator needs access to memory.           */
/*                                                              */
/* These machine components are visible to the code generator   */
/* but their real home is in the machine module                 */
/*                                                              */
/****************************************************************/
        
        /*  Size of machine memory in words                     */
        
#define  MEMORYSIZE  4096

        /* DISPLAY SIZE determines maximum depth of procedure nesting */
        
#define  DISPLAYSIZE  10

/************************************************************************/
/*                                                                      */
/*  To guarantee a unique initialization for the arrays that define     */
/*  machine constants, the following mechanism is used.                 */
/*  In machine.c                                                        */
/*      EXTERN_MACHINE is defined outside of this file                  */
/*      this causes MACHINE_CONSTS to be defined which in turn          */
/*      causes initial values to be given to various arrays             */
/*  In other files (specifically codegen.c)                             */
/*      EXTERN_MACHINE is not defined outside of this file              */
/*      This causes EXTERN_MACHINE to become extern and                 */
/*      causes initial values for various arrays to be omitted          */
/*                                                                      */
/************************************************************************/

#define MACHINE_CONSTS
#ifndef EXTERN_MACHINE
#define EXTERN_MACHINE  extern
#undef  MACHINE_CONSTS 
#endif

        /*  Machine main memory    addresses 0 .. MEMORYSIZE -1 */
        
EXTERN_MACHINE short memory[ MEMORYSIZE ] ;


/* Machine Instructions                                         */

enum InstructionCodes {
        HALT = 0, ADDR,  LOAD, STORE, PUSH, PUSHMT, SETD, POP,    
        POPN,     DUP,   DUPN, BR,    BF,   NEG,    ADD,  SUB,
        MUL,      DIV,   EQ,   LT,    OR,   SWAP,   READC,PRINTC, 
        READI,    PRINTI,TRON, TROFF 
        }  ; 

/*  Range of valid instructions                 */

#define firstInstruction  HALT
#define lastInstruction   TROFF

/*  Instruction names for dump and trace */

EXTERN_MACHINE const char *instructionNames[]
#ifdef MACHINE_CONSTS
    = { "HALT", "ADDR",  "LOAD", "STORE", "PUSH", "PUSHMT", "SETD", "POP",
        "POPN",  "DUP",  "DUPN", "BR",    "BF",   "NEG",    "ADD",  "SUB",
        "MUL",  "DIV",   "EQ",   "LT",    "OR",   "SWAP",   "READC","PRINTC", 
        "READI","PRINTI","TRON", "TROFF" 
      } 
#endif
;

EXTERN_MACHINE const  short undefined 
#ifdef MACHINE_CONSTS
                        = UNDEFINED   /* in binary 1000000000000000 */
#endif
        ;

/*  Definitions of TRUE and FALSE for the machine       */

#define MACHINE_TRUE    1
#define MACHINE_FALSE   0

/********************************************************/
/*                                                      */
/*  Table of lengths for each instruction.              */
/*  NOTE:  length of branch instructions is set to ZERO */
/*         since they directly change the pc            */
/*  NOTE:  length of HALT instruction is zero since once*/
/*         we reach halt, updating the pc is meaningless*/
/*                                                      */
/********************************************************/

EXTERN_MACHINE const unsigned char instructionLength[] 
#ifdef MACHINE_CONSTS
  =   { 0, 3, 1, 1, 2, 1, 2, 1, 1, 1, 1,        /* HALT .. DUPN  */
        0, 0,                                   /* BR   .. BF   */
        1, 1, 1, 1, 1, 1, 1, 1, 1,              /* NEG  .. SWAP */
        1, 1, 1, 1, 1, 1                        /* READC.. TROFF*/
    }
#endif
      ;

#endif
