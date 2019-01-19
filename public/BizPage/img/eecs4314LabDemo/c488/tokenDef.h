#ifndef _TOKENDEF_H_
#define _TOKENDEF_H_

/*  Copyright 1995, David B. Wortman */
/************************************************************************/
/*      define global constants that specify the encoding used          */
/*      for various types of tokens                                     */
/************************************************************************/

/*   RCS Version Identifier                     */

static char tokenDefVersion[] = "$Id: tokenDef.h,v 1.1 1996/02/05 16:58:07 dw Exp $" ;

/*      token classes                           */

/*      Terminal Symbols                        */
/*      terminalClass MUST be zero, see scanparse.h     */

#define terminalClass   ( 0 )   

/*      Non Terminal Symbols                    */
                                
#define nonTerminalClass ( 1 )

/*      Semantic Analysis Tokens                */

#define semanticClass   ( 2 )

/*      Code Generation Tokens                  */

#define codeGenClass    ( 3 )

/************************************************************************/
/*                                                                      */
/*      Define range of values for each token class.                    */
/*      Gaps have been left between the classes to allow                */
/*      for future expansion (hint Assignment 6)                        */
/*                                                                      */
/************************************************************************/

/*      TokenStride determines the spacing between token classes        */

#define tokenStride     ( 100 )

/*      Define starting point for each class of tokens                  */

#define firstTerminal           ( terminalClass * tokenStride )

        /*  leave gap  before defining tokens for special characters    */
#define firstSpecialTerminal    ( firstTerminal + 8           )

        /*  leave gap  before defining tokens for reserved words        */
#define firstReservedTerminal   ( firstSpecialTerminal + 25  )

#define firstNonTerminal        ( nonTerminalClass * tokenStride )

#define firstSemanticToken      ( semanticClass  *  tokenStride )

#define firstCodeGenToken       ( codeGenClass  *  tokenStride )

#endif
