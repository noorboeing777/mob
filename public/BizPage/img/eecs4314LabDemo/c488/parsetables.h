#ifndef _PARSETABLES_H_
/************************************************************************/
/*  Copyright 1995, David B. Wortman                                    */
/*                                                                      */
/*      parsetables.h                                                   */
/*      Parse tables for CSC488S course project LL(1) parser            */
/*                                                                      */
/************************************************************************/
#define _PARSETABLES_H_

static char parseTablesVersion[] = 
                "$Id: parsetables.h,v 1.2 1996/02/28 14:48:29 wortman Exp $" ;

/*      Include global token definitions                                */
#include "tokenDef.h"

/*      Include scanner/parser interface                                */
#include "scanparse.h"

/*      Include parser/semantics interface                              */
#include "parsesemantics.h"

/*      Include parser/code generation interface                        */
#include "parsecodegen.h"

/*      number of rules in the grammar                                  */

#define NUMBER_OF_GRAMMAR_RULES   90

/*      length of the longest rule                                      */
/*      (including space for semantic and code generation actions       */

#define  LONGEST_GRAMMAR_RULE     15
                                        
/*************************************************************************/
/*                                                                      */
/*      Definitions for non-terminal symbols appearing in the grammar   */
/*                                                                      */
/*      NOTE: because of the algorithm used (via firstProduction) to    */
/*          locate the entries in the table production associated       */
/*          with a non-terminal, the following declarations             */
/*          MUST be in the same ORDER as in the revised translation     */
/*          grammar.                                                    */
/*                                                                      */
/************************************************************************/

enum nonTerminals { 

  Nprog = firstNonTerminal ,    /*  program             */
  Nscope     ,                  /*  scope               */
  Nstmts     ,                  /*  statements          */
  Nstmt      ,                  /*  statement           */
  Noels      ,                  /*  optElse             */
  Nasoc      ,                  /*  assignOrCall        */
  Nasexp     ,                  /*  assignExpression    */
  Nsubs      ,                  /*  subscript           */
  Nexpr      ,                  /*  expression          */
  Nmdsj     ,                   /*  moreDisjunctions    */
  Nconj     ,                   /*  conjunction         */
  Nmcnj     ,                   /*  moreConjunctions    */
  Nneg      ,                   /*  negation            */
  Nrexpr    ,                   /*  relExpression       */
  Norel     ,                   /*  optRelation         */
  Nlole     ,                   /*  lessOrLessE         */
  Ngoge     ,                   /*  greaterOrGreaterEq  */
  Nnexpr    ,                   /*  numericExpression   */
  Nmterm    ,                   /*  moreTerms           */
  Nterm     ,                   /*  term                */
  Nmfact    ,                   /*  moreFactors         */
  Nfact     ,                   /*  factor              */
  Nprime    ,                   /*  primary             */
  Nsocl     ,                   /*  subsOrCall          */
  Nargs     ,                   /*  arguments           */
  Nmargs    ,                   /*  moreArguments       */
  Ndcls     ,                   /*  declarations        */
  Nmdcl     ,                   /*  moreDeclarations    */
  Ndcl      ,                   /*  declaration         */
  Npfbdy     ,                  /*  pfBody              */
  Nfov      ,                   /*  funcOrVar           */
  Ntype     ,                   /*  type                */
  Noarbd    ,                   /*  optArrayBound       */
  Nparms    ,                   /*  parameters          */
  Nmparm    ,                   /*  moreParameters      */
  Nouts     ,                   /*  outputs             */
  Nmout     ,                   /*  moreOutput          */
  Nout      ,                   /*  output              */
  Ninpts    ,                   /*  inputs              */
  Nminpt    ,                   /*  moreInputs          */
  Ninput    ,                   /*  input               */
  Nosub                         /*  optSubscript        */
            }   ;

#define lastNonTerminal  Nosub

/************************************************************************/
/*                                                                      */
/*      The table firstProduction is used to index into the             */
/*      table productions which encodes the grammar being               */
/*      parsed.  For any nonterminal symbol  Nxxx,  The                 */
/*      first production rule defining that symbol is located           */
/*      at  firstProduction[ Nxxx ]  and the last rule defining         */
/*      the symbol is located at  firstProduction( Nxxx + 1 ) - 1       */
/*      Note dummy entry at end of table.                               */
/*      Note that the table productions[] is 1-origin not 0-origin      */
/*                                                                      */
/*      The entries in this table correspond directly to the number of  */
/*      the first rule for each nonterminal symbol in the revised       */
/*      translation grammar class handout.                              */
/*                                                                      */
/************************************************************************/

short  firstProduction[] = {
         1,      2,      3,      5,     15,     /*  Nprog  .. Noels     */
        17,     21,     22,     23,     24,     /*  Nasoc  .. Nmdsj     */
        26,     27,     29,     31,     32,     /*  Nconj  .. Norel     */
        37,     39,     41,     42,     45,     /*  Nlole  .. Nterm     */
        46,     49,     51,     57,     60,     /*  Nmfact .. Nargs     */
        61,     63,     64,     66,     68,     /*  Nargs  .. Npfbdy    */
        70,     72,     74,     76,     77,     /*  Nfov   .. Nmparm    */
        79,     80,     82,     85,     86,     /*  Nouts  .. Nminpt    */
        88,     89,                             /*  Ninput .. Nosub     */
        NUMBER_OF_GRAMMAR_RULES+1 /* dummy */   /*  Nosub+1             */
        } ;
        
/************************************************************************/
/*                                                                      */
/*      The table productionLength is used to hold the length           */
/*      (number of symbols + number of actions) in each                 */
/*      production.  Conceptually it is a constant, but it is           */
/*      initialized by parserInitialize to make maintenance and         */
/*      modification easier.                                            */
/*      Note that the table productions[] is 1-origin not 0-origin      */
/*                                                                      */
/************************************************************************/

short  productionLength[ NUMBER_OF_GRAMMAR_RULES + 1 ] ;

/************************************************************************/
/*                                                                      */
/*      director sets                                                   */
/*                                                                      */
/*      for each production  P,  the array element  directors(P)        */
/*      is the set of terminal symbols that can legally appear          */
/*      at the start of the right hand side of the production           */
/*                                                                      */      
/*                                                                      */
/*      conceptually the following table of director sets is a          */
/*      constant.  It is never modified once it has been initialized.   */
/*      It is a variable here to make its initialization easier.        */
/*      Note director[0] is unused.                                     */
/*                                                                      */
/************************************************************************/
         
   SET director[ NUMBER_OF_GRAMMAR_RULES + 1] ;
   
/************************************************************************/
/*                                                                      */       
/*        director array is initialized by parserInitialize             */
/*                                                                      */
/*      production table index vector                                   */
/*      The rules for the grammar along with associated context         */
/*      and code generation rules are stored as variable length         */
/*      sequences of tokens in the array productions.                   */
/*                                                                      */
/*      The table productions in intended to be a direct translation    */
/*      of the Revised Translation Grammar handed out in class.         */
/*      Any deviation between this table and the Revised Translation    */
/*      Grammar is almost certainly an error.                           */
/*                                                                      */
/*      Note the dummy entry at the end of production start.            */
/*        every rule ends with the constant EndRule                     */
/*      Note that the table productions[] is 1-origin not 0-origin      */
/*                                                                      */
/************************************************************************/

#define EndRule  -1

const short productions[NUMBER_OF_GRAMMAR_RULES + 2][LONGEST_GRAMMAR_RULE] 
  = {                                                           /*  RULE*/
 { EndRule },   /*  productions[0] UNUSED       */              /*   0*/
 { S0,Nscope,C5,S1,C38,C0,EndRule },                            /*   1*/
 {Tbegin,C1,Ndcls,S27,C3,Nstmts,Tend,EndRule },                 /*   2*/
 {EndRule },                                                    /*   3*/
 {Nstmt,Nstmts, EndRule },                                      /*   4*/
 {Tidentifier,S6,Nasoc,  EndRule },                             /*   5*/
 {Tif,Nexpr,S13,S11,C8,Tthen,Nstmts,Noels,C9,Tend,EndRule },    /*   6*/
 {Twhile,C10,Nexpr,S13,S11,C8,Tdo,Nstmts,C11,C9,Tend,EndRule }, /*   7*/
 {Tloop,C49,C10,Nstmts,C11,Tend,C50,EndRule },                  /*   8*/
 {Texit,C37,EndRule },                                          /*   9*/
 {Treturn,C5,C43,EndRule },                                     /*  10*/
 {Tresult,C4,Nexpr,S23,C33,S11,C5,C43,EndRule},                 /*  11*/ 
 {Tput,Nouts, EndRule },                                        /*  12*/
 {Tget,Ninpts,EndRule },                                        /*  13*/
 {S0,Nscope,C5,S1,EndRule },                                    /*  14*/
 {EndRule },                                                    /*  15*/
 {Telse,C7,C12,C9,Nstmts,EndRule },                             /*  16*/
 {S28,S29,C45,C44,S7,  EndRule },                               /*  17*/
 {Tlparen,S28,S30,C45,S35,Nargs,Trparen,S32,S37,C44,S7,EndRule },/*  18*/
 {Tless,Tminus,S20,C31,S8,Nasexp,EndRule },                     /*  19*/
 {Nsubs,S8,Tless,Tminus,Nasexp,EndRule },                       /*  20*/
 {Nexpr,S16,C33,S11,S11,S7, EndRule },                          /*  21*/
 {Tlsquare,S21,C40,Nnexpr,S12,S11,C41,Trsquare,EndRule },       /*  22*/
 {Nconj,Nmdsj,EndRule },                                        /*  23*/
 {EndRule },                                                    /*  24*/
 {Tor,S13,S11,Nconj,S13,C20,Nmdsj,EndRule },                    /*  25*/
 {Nneg,Nmcnj,EndRule },                                         /*  26*/
 {EndRule },                                                    /*  27*/
 {Tand,S13,S11,Nneg,S13,C19,Nmcnj,EndRule },                    /*  28*/
 {Nrexpr,EndRule },                                             /*  29*/
 {Tnot,Nneg,S13,C18,EndRule },                                  /*  30*/
 {Nnexpr,Norel,  EndRule },                                     /*  31*/
 {EndRule },                                                    /*  32*/
 {Tequal,Nnexpr,S14,S11,S11,S10,C21,  EndRule },                /*  33*/
 {Tnot,Tequal,Nnexpr,S14,S11,S11,S10,C22,EndRule },             /*  34*/
 {Tless, Nlole,  EndRule },                                     /*  35*/
 {Tgreater,Ngoge,  EndRule },                                   /*  36*/
 {Nnexpr,S15,S11,S11,S10,C23,EndRule },                         /*  37*/
 {Tequal,Nnexpr,S15,S11,S11,S10,C24,EndRule },                  /*  38*/
 {Nnexpr,S15,S11,S11,S10,C25,EndRule },                         /*  39*/
 {Tequal,Nnexpr,S15,S11,S11,S10,C26,EndRule },                  /*  40*/
 {Nterm,Nmterm,EndRule },                                       /*  41*/
 {EndRule },                                                    /*  42*/
 {Tplus,S12,S11,Nterm,S12,C14,Nmterm,EndRule },                 /*  43*/
 {Tminus,S12,S11,Nterm,S12,C15,Nmterm,EndRule },                /*  44*/
 {Nfact,Nmfact,  EndRule },                                     /*  45*/
 {EndRule },                                                    /*  46*/
 {Tstar,S12,S11,Nfact,S12,C16,Nmfact, EndRule },                /*  47*/
 {Tslash,S12,S11,Nfact,S12,C17,Nmfact, EndRule },               /*  48*/
 {Nprime,EndRule },                                             /*  49*/
 {Tminus,Nfact,S12,C13,  EndRule },                             /*  50*/
 {Tinteger,S9,C36,  EndRule },                                  /*  51*/
 {Ttrue,S10,C35,  EndRule },                                    /*  52*/
 {Tfalse,S10,C34,  EndRule },                                   /*  53*/
 {Tlparen,Nexpr,Trparen,  EndRule },                            /*  54*/
 {Tlcurley,S0,C2,Ndcls,S27,C3,Nstmts,Tyields,Nexpr,C6,S1,Trcurley,EndRule },/*  55*/
 {Tidentifier,S6,Nsocl,  EndRule },                             /*  56*/
 {S38,C51,S8,C52,S7,  EndRule },                                /*  57*/
 {Tlparen,S33,C46,S30,S35,Nargs,Trparen,S32,S37,S8,C47,S7,EndRule },/*  58*/
 {Nsubs,S8,C32,S7,  EndRule },                                  /*  59*/
 {Nexpr,S31,S11,S36,C48,Nmargs,  EndRule },                     /*  60*/
 {EndRule },                                                    /*  61*/
 {Tcomma,Nexpr,S31,S11,S36,C48,Nmargs,  EndRule },              /*  62*/
 {Ndcl,Nmdcl,EndRule },                                         /*  63*/
 {EndRule },                                                    /*  64*/
 {Ndcl,Nmdcl,EndRule },                                         /*  65*/
 {Ntype,Nfov,EndRule },                                         /*  66*/
 {Tproc,Tidentifier,C7,S3,S24,S22,S0,Npfbdy,S7,C9,EndRule },    /*  67*/
 {Nscope,C5,C43,S1,EndRule },                                   /*  68*/
 {Tlparen,S35,Nparms,Trparen,S34,S37,Nscope,C5,C43,S2,EndRule },/*  69*/
 {Tcolon,Tidentifier,S3,S4,S5,Noarbd,S11,S7,EndRule },          /*  70*/
 {Tfunc,Tidentifier,C7,S3,S26,S22,S5,S11,S0,Npfbdy,S7,C9,EndRule },/*  71*/
 {Tint,S9,EndRule },                                            /*  72*/
 {Tbool,S10,EndRule },                                          /*  73*/
 {S18,C42,EndRule },                                            /*  74*/
 {Tlsquare,Tinteger,S19,Trsquare,C39,EndRule },                 /*  75*/
 {Ntype,Tcolon,Tidentifier,S3,S4,S5,S11,S25,S7,S36,Nmparm,EndRule },/*  76*/
 {EndRule },                                                    /*  77*/
 {Tcomma,Ntype,Tcolon,Tidentifier,S3,S4,S5,S11,S25,S7,S36,Nmparm,EndRule },/*  78*/
 {Nout,Nmout,EndRule },                                         /*  79*/
 {EndRule },                                                    /*  80*/
 {Tcomma,Nout,Nmout,  EndRule },                                /*  81*/
 {Nnexpr,S12,C28,S11,EndRule },                                 /*  82*/
 {Ttext,C29,  EndRule },                                        /*  83*/
 {Tskip,C30,  EndRule },                                        /*  84*/
 {Ninput,Nminpt,  EndRule },                                    /*  85*/
 {EndRule },                                                    /*  86*/
 {Tcomma,Ninput,Nminpt,  EndRule },                             /*  87*/
 {Tidentifier,S6,S8,Nosub,S17,C27,S11,S7,EndRule },             /*  88*/
 {S20,C31,  EndRule },                                          /*  89*/
 {Nsubs,EndRule },                                              /*  90*/
 {EndRule }                                                     /*  91 (dummy)*/
} ;

/************************************************************************/
/*                                                                      */
/*      Table of names for non-terminal symbols, used only for          */
/*      optional grammar dump                                           */
/*                                                                      */
/*      NOTE  the following table must be in the same order as the      */
/*            definition of non-terminal constants (Nxxxx) given above. */
/*                                                                      */
/************************************************************************/

const char *nonTerminalNames[] = {
        "program",      "scope",        "statements",   "statement",
        "optElse",      "assignOrCall", "assignExpression",     "subscript",
        "expression",   "moreDisjunctions",
        "conjunction",  "moreConjunctions",     "negation",
        "relExpression",        "optRelation",  "lessOrLessEq",
        "greaterOrGreaterEq",   "numericExpression",    "moreTerms", "term",
        "moreFactors",  "factor",       "primary",      "subsOrCall",
        "arguments",    "moreArguments",        "declarations",
        "moreDeclarations",     "declaration",  "pfbody",       "funcOrVar",
        "type", "optArrayBound",        "parameters",   "moreParameters",
        "outputs",      "moreOutput",   "output",       "inputs",
        "moreInputs",   "input",        "optSubscript"          
        } ;
        
#endif
