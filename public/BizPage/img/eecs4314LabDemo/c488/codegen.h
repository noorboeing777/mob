#ifndef _CODEGEN_H_
/****************************************************************/
/*  Copyright 1995, David B. Wortman */
/*      codegen.h                                               */
/*      Interface to the                                        */
/*      Code Generation module for the CSC488S Project Compiler */
/*                                                              */
/****************************************************************/
#define _CODEGEN_H_

static char codegenInterfaceVersion[] = 
        "$Id: codegen.h,v 1.3 1996/01/28 17:05:04 dw Exp $" ;

        /* Initialize the code generator                        */
void codegenInitialize() ;

        /*  Finalize the code generator                         */
void codegenFinalize() ;

        /*  Perform one code generation action                  */      
void generateCode( int actionNumber ) ;

     
#endif
