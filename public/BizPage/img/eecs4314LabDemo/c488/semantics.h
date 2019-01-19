#ifndef _SEMANTICS_H_
/****************************************************************/
/*  Copyright 1995, David B. Wortman                            */
/*                                                              */
/*      semantics.h                                             */
/*      Interface to the                                        */
/*      Semantics Analysis Module for CSC488S Project Compiler  */
/*                                                              */
/****************************************************************/
#define _SEMANTICS_H_

static char semanticsInterfaceVersion[] = 
        "$Id: semantics.h,v 1.2 1996/01/25 20:07:48 dw Exp $" ;

/*  semanticsInitialize - called by the parser before compilation */

void semanticsInitialize() ;

/*  semanticsFinalize - called by the parser after compilation  */

void semanticsFinalize() ;


/*   semanticAction - called by the parser to perform one       */
/*                      semantic action                         */

void semanticAction( int actionNumber ) ;

#endif
