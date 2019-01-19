#ifndef _SCANPARSE_H_
/************************************************************************/
/*  Copyright 1995, David B. Wortman                                    */
/*                                                                      */
/*      scanparse.h                                                     */
/*      This include file defines the language token interface          */
/*      that is used for communication between the scanner module       */
/*      and the parser module.                                          */
/************************************************************************/
#define  _SCANPARSE_H_

static const char *scannerDefVersion = 
               "$Id: scanparse.h,v 1.1 1996/02/05 17:01:01 dw Exp $";
               
/************************************************************************/
/*   To guarantee a unique definition for scanner/parser interface      */
/*   constants, the following mechanism is used.                        */
/*   In scanner.c  EXTERN_SCANNER is defined outside of this file       */
/*      This causes SCANNER_CONSTS to be defined which gives initial    */
/*      values to the interface constants                               */
/*   In parser.c EXTERN_SCANNER is not defined outside of this file     */
/*      This causes EXTERN_SCANNER to be defined as extern              */
/*      and initial values to omitted from the constants                */
/************************************************************************/

#ifndef EXTERN_SCANNER
#undef  SCANNER_CONSTS
#define EXTERN_SCANNER  extern
#else
#define SCANNER_CONSTS
#endif

/*    Include definitions for token classes             */
#include "tokenDef.h"

/************************************************************************/
/* The following two constants are manually derived from the grammar    */
/* for the language being compiled.  They are used to verify the        */
/* consistency of the definitions given below.                          */
/************************************************************************/

/* Number of special character tokens   */
#define  numberOfSpecialChars  15 

/* Number of reservved word tokens              */
#define  numberOfReservedWords 25       

/************************************************************************/
/* Token Encoding                                                       */
/*      The declarations given below describe the encoding of language  */
/*      tokens used for communication by the scanner and parser         */
/*************************************************************************/
typedef         /* NOTE this typedef continues to the end of token definitions */

  enum {                

/* Composite and Special Tokens         */

         Tidentifier =   1 ,    /* identifiers  */
         Tinteger    =   2 ,    /* integers     */
         Ttext       =   3 ,    /* text strings */
         TendFile    =   4 ,    /* end of file  */

/* Special character tokens                     */

/* Special Characters are given token numbers in the range              */
/* firstSpecialTerminal .. firstSpecialTerminal + numberOfSpecialChars -1 */

         Tcolon  = firstSpecialTerminal   , /* :  colon */
         Tcomma     ,           /* ,  comma     */
         Tplus      ,           /* +  plus      */
         Tminus     ,           /* -  minus     */
         Tstar      ,           /* *  star      */
         Tslash     ,           /* /  slash     */
         Tequal     ,           /* =  equal     */
         Tless      ,           /* <  less than */
         Tgreater   ,           /* >  greater than      */
         Tlparen    ,           /* (  left parenthesis  */
         Trparen    ,           /* )  right parenthesis */
         Tlcurley   ,           /* {  left curley bracket       */
         Trcurley   ,           /* }  right curley bracket      */
         Tlsquare   ,           /* [  left square bracket       */
         Trsquare   ,           /* ]  right square bracket      */

/*      NOTE the order of the definitions given above must be in        */
/*      EXACTLY the same order as the characters for the tokens         */
/*      appear in the string specialCharacters defined below            */
/*      The scanner uses the property to rapidly calculate              */
/*      the token number for special character tokens                   */

/* Reserved Word Tokens                                 */

/* Define tokens for reserved words                             */

          Tand  = firstReservedTerminal , /* and        */
          Tbegin    ,           /* begin        */
          Tbool     ,           /* boolean      */
          Tdo       ,           /* do           */
          Telse     ,           /* else         */
          Tend      ,           /* end          */
          Texit     ,           /* exit         */
          Tfalse    ,           /* false        */
          Tfunc     ,           /* function     */
          Tget      ,           /* get          */
          Tif       ,           /* if           */
          Tint      ,           /* integer      */
          Tloop     ,           /* loop         */
          Tnot      ,           /* not          */
          Tor       ,           /* or           */
          Tproc     ,           /* procedure    */
          Tput      ,           /* put          */
          Tresult   ,           /* result       */
          Treturn   ,           /* return       */
          Tskip     ,           /* skip         */
          Tthen     ,           /* then         */
          Ttrue     ,           /* true         */
          Twhile    ,           /* while        */
          Twith     ,           /* with         */
          Tyields               /* yields       */
          
     } terminalTokens ;         /*   END of  typedef enum { ... */
        
                                        
/* Printable names for composite tokens         */

EXTERN_SCANNER const char *compositeNames[] 
#ifdef SCANNER_CONSTS
                = { "", "identifier", "integer", "text", "|--" } 
                  /* compositeNames[0] unused   */
#endif
      ;

/* The scanner module uses their position in the string specialCharacters */
/* to rapidly calculate their token numbers using specialTokenOffset       */
/* tokenNumber(X)  is specialTokenOffset + strpos(specialCharacters, X ) */

EXTERN_SCANNER const char *specialCharacters 
#ifdef SCANNER_CONSTS
                              = ":,+-*/=<>(){}[]" 
                              /* 012345678901234        */
#endif
      ;

#define specialTokenOffset  firstSpecialTerminal

/************************************************************************/
/*                                                                      */
/*   Define constants that describe the range of various token types    */
/*                                                                      */
/************************************************************************/

#define firstCompositeToken  Tidentifier
#define lastCompositeToken   TendFile
#define firstSpecialToken    Tcolon
#define lastSpecialToken     Trsquare
#define firstReservedToken   Tand
#define lastReservedToken    Tyields

/***********************************************************************/
/*  NOTE, the following array of reserved wordss is SORTED into ascending*/
/*   order to allow the scanner to rapidly lookup keywords using        */
/*   binary search.                                                     */
/************************************************************************/

EXTERN_SCANNER const struct  reservedWordStruct {
                char *name ;            /* name of reserved word        */
                terminalTokens  value ; /* value of the token            */
                } reservedWords[] 
#ifdef SCANNER_CONSTS
    =   {
                {       "and",          Tand },
                {       "begin",        Tbegin },
                {       "boolean",      Tbool },
                {       "do",           Tdo },
                {       "else",         Telse },
                {       "end",          Tend },
                {       "exit",         Texit },
                {       "false",        Tfalse },
                {       "function",     Tfunc },
                {       "get",          Tget },
                {       "if",           Tif },
                {       "integer",      Tint },
                {       "loop",         Tloop },
                {       "not",          Tnot },
                {       "or",           Tor },
                {       "procedure",    Tproc },
                {       "put",          Tput },
                {       "result",       Tresult },
                {       "return",       Treturn },
                {       "skip",         Tskip },
                {       "then",         Tthen },
                {       "true",         Ttrue },
                {       "while",        Twhile },
                {       "with",         Twith } ,
                {       "yields",       Tyields}
        }
#endif 
        ;

#endif
