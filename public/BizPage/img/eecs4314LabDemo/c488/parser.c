/********************************************************/
/*                                                      */
/*  Copyright 1995, David B. Wortman                    */
/*      parser.c                                        */
/*      Parser module for CSC488S Project Compiler      */
/*                                                      */
/********************************************************/

static char parserVersion[] =
      "$Id: parser.c,v 1.2 1996/02/07 13:38:30 wortman Exp $" ;
   
    /*  include common definitions and global variables */
#include "common.h"
#include "globalvars.h"

    /* include parser interface                 */
#include "parser.h"

    /*  include interfaces for procedures we call       */
#include "scanner.h"
#include "semantics.h"
#include "codegen.h"
#include "tset.h"

    /************************************************************/
    /*                                                          */
    /*  include parse tables and token class definitions        */
    /*  The parsetables file contains the parser/semantics      */
    /*  interface and the parser/codegen interface              */
    /*                                                          */
    /************************************************************/

#include  "tokenDef.h"    
#include  "parsetables.h"

static BOOLEAN  parsing  ;      /* true while parsing           */

/*      Internal procedure to print message for syntax error    */                      
/*      sets errorOccurred to TRUE, parsing to FALSE            */

static void parseError( char *msg ){
        fprintf(errorFile, "Parser Error (line %d) - %s\n", 
           line, msg );
        errorOccurred = TRUE ;
        parsing = FALSE ;
        return ;
   } ;


/*   Internal function to return the printable name of a terminal */
/*   symbol token for use in error messages.                    */

#define  TOKEN_RESULT_SIZE  50
/* output of getTokenName       */
static char tokenNameResult[TOKEN_RESULT_SIZE + 1]  ;   

static  void getTokenName( TOKEN inToken ) {
        /* For simplicity we assume that the caller of this function    */
        /* will use its result once and immediately so returning        */
        /* a value in a static array  will not cause problems.          */
        
        /*  The following if cases should be mutually exclusive         */
        /*  Composite token                             */
        if( inToken >= firstCompositeToken && inToken <= lastCompositeToken ){
             strncpy(tokenNameResult, compositeNames[ inToken ], 
                                        TOKEN_RESULT_SIZE ) ;
             return ;
         };
        /* Special character                            */
        if( inToken >= firstSpecialToken && inToken <= lastSpecialToken ){
            tokenNameResult[0] = 
                          specialCharacters[inToken - specialTokenOffset];
            tokenNameResult[1] = '\0' ; /* null terminate */
            return ;
         };
        /*  Reserved Word       */
        if( inToken >= firstReservedToken && inToken <= lastReservedToken ){
            strncpy( tokenNameResult,
                  reservedWords[ inToken - firstReservedToken ].name, 
                                           TOKEN_RESULT_SIZE ) ;
            return ;
         };
        /*  Nonterminal symbol  */
        if( inToken >= firstNonTerminal && inToken <= lastNonTerminal ){
            strncpy( tokenNameResult, 
                  nonTerminalNames[ inToken - firstNonTerminal ],
                                           TOKEN_RESULT_SIZE );
            return ;
         };
        if( inToken >= firstSemanticToken && inToken <= lastSemanticToken ){
             sprintf( tokenNameResult, "S%d", inToken - firstSemanticToken );
             return ;
        } ;
        if( inToken >= firstCodeGenToken && inToken <= lastCodeGenToken ){
             sprintf( tokenNameResult, "C%d", inToken - firstCodeGenToken );
             return ;
         };               
         /* inToken value out of range, return integer value */
         sprintf(tokenNameResult, "%7d", inToken );
        return ;
    } ;

/*   Internal function to dump scanToken to traceFile           */

void scanTokenTrace() {
        getTokenName( scanToken ) ;
        if( scanToken == Tidentifier )
            fprintf( traceFile, "scan: %s (%s)\n", tokenNameResult, scanString );
        else if(  scanToken == Ttext )
            fprintf( traceFile, "scan: %s (\"%s\")\n", tokenNameResult, 
                        scanString );
        else if( scanToken == Tinteger )
            fprintf( traceFile, "scan: %s (%d)\n", tokenNameResult, scanInteger);
        else
            fprintf( traceFile, "scan: %s\n", tokenNameResult );
    } 
                
        /****************************************/
        /*  parse stack                         */
        /****************************************/
    
#define parseStackSize  400
TOKEN  parseStack[ parseStackSize ] ;
int    pst ;                    /*  parser stack pointer  */

        /**************************************************/
        /*  rule stack runs parallel to the parser stack. */
        /*  It records the rule that caused each token to */
        /*  be pushed onto the parse stack.  Used to give */
        /*  better syntax error messages.                 */
        /**************************************************/
unsigned char ruleStack[ parseStackSize ] ;

        /********************************************************/
        /*                                                      */
        /*  initialize parser tables and parser stack           */
        /*                                                      */
        /********************************************************/

void parserInitialize() {


        int i, j ;
        int normToken ;         /* normalized token value       */

        /********************************************************/      
        /*   Initialize the table for director sets.            */
        /*   NOTE the last argument to every call of setCreate  */
        /*   MUST be the constant SETEND                        */
        /*                                                      */
        /*   First define some useful set constants             */
        /********************************************************/
        
        /*  terminal symbols that begin statements              */      
        SET firstStmts = 
           setCreate(Tidentifier,Tbegin,Texit,Tget,Tif, Tloop,
                            Tput,Treturn,Tresult,Twhile, SETEND);

        /*  terminal symbols that follow statements             */
        SET followStmts = setCreate(Telse, Tend ,  Tyields, SETEND ) ; 

        /*  terminal symbols that follow statement              */
        SET followStmt  = setUnion( firstStmts, followStmts ) ;

        /*  terminal symbols that begin types                   */
        SET firstType =  setCreate( Tbool, Tint, SETEND ); 

        /*  terminal symbols that begin declarations            */
        SET firstDcls = setUnion( firstType, setCreate( Tproc, SETEND ) ) ;

        /*  terminal symbols that begin expressions             */
        SET firstExpr = 
           setCreate(Tidentifier,Tinteger,Tlparen,Tlcurley,Tminus,
                     Tfalse,Tnot,Ttrue, SETEND) ; 

        /*  terminal symbols that follow expressions            */
        SET followExpr = 
           setUnion( followStmt, setCreate(Tcomma,Trparen,Trcurley,Trsquare,
                                        Tdo, Tthen, SETEND ) ) ;

        /*  terminal symbols that begin numeric expressions     */
        SET firstNumExpr = 
           setCreate(Tidentifier,Tinteger,Tlparen,Tlcurley,Tminus,
                     Tfalse,Ttrue, SETEND ) ;

        /*  terminal symbols that follow conjunctions           */
        SET followConjunct = setUnion( followExpr, setCreate( Tor, SETEND ) ) ;

        /*  terminal symbols that follow relational expressions */
        SET followRelExpr  = setUnion( followConjunct, setCreate( Tand, SETEND )) ;

        /*  terminal symbols that follow numeric expressions    */
        SET followNumExpr = 
           setUnion( followRelExpr, setCreate( Tequal, Tless, Tgreater, 
                                              Trsquare,Tnot, SETEND ) ) ;

        /*  terminal symbols that begin primaries               */
        SET firstPrimary = 
           setCreate(Tidentifier,Tinteger,Tlparen,Tlcurley,Tfalse,
                     Ttrue, SETEND ) ;

        /*  terminal symbols that follow terms                  */
        SET followTerm  = setUnion( followNumExpr , 
                                   setCreate( Tplus, Tminus, SETEND ) ) ; 

        /*  terminal symbols that follow factors                */
        SET followFactor  = setUnion( followTerm , 
                                   setCreate( Tstar, Tslash, SETEND ) ) ;
                                   
        /*  Useful singelton sets                               */      
        SET Scolon =  setCreate( Tcolon, SETEND ) ; 
        SET Slsquare = setCreate( Tlsquare, SETEND ) ; 
        SET Slcurley = setCreate( Tlcurley, SETEND ) ; 
        SET Scomma = setCreate( Tcomma, SETEND ) ; 
        SET Slparen = setCreate( Tlparen, SETEND ) ; 
        SET Srparen = setCreate( Trparen, SETEND ) ; 
        SET Sident =  setCreate( Tidentifier, SETEND ); 
        SET Sbegin = setCreate( Tbegin, SETEND ) ;
        SET Sequal = setCreate( Tequal, SETEND ); 

        /********************************************************/      
        /*    Validate tables and constants used by the parser  */
        /*    Assumes validations performed in scannerInitialize*/
        /********************************************************/
        assert( lastReservedToken <= SETELEMENTMAX );
        assert( firstNonTerminal > lastReservedToken
               && firstNonTerminal <= lastNonTerminal );
        assert( (sizeof firstProduction  / sizeof( short ))
                == (lastNonTerminal - firstNonTerminal  + 2) );
                /*  One dummy entry in table */
        assert( ( sizeof nonTerminalNames / sizeof( char * ) )
                == lastNonTerminal - firstNonTerminal + 1 ) ;
        
        /********************************************************/
        /*                                                      */
        /*    Initialize the array director[]                   */
        /*    fill in values derived from the grammar.          */
        /*                                                      */
        /********************************************************/
        
        director[ 1]    = Sbegin ;                      /*  program:    */
        director[ 2]    = Sbegin ;                      /*  scope:      */
        director[ 3]    = followStmts;                  /*  statements: */
        director[ 4]    = firstStmts;
        director[ 5]    = Sident ;                      /*  statement:  */
        director[ 6]    = setCreate( Tif, SETEND );
        director[ 7]    = setCreate( Twhile, SETEND );
        director[ 8]    = setCreate( Tloop, SETEND );
        director[ 9]    = setCreate( Texit, SETEND );
        director[10]    = setCreate( Treturn, SETEND );
        director[11]    = setCreate( Tresult, SETEND );
        director[12]    = setCreate( Tput, SETEND );
        director[13]    = setCreate( Tget, SETEND );
        director[14]    = Sbegin;
        director[15]    = setCreate( Tend, SETEND );            /*  optElse:    */
        director[16]    = setCreate( Telse, SETEND );
        director[17]    = followStmt;                   /*  assignOrCall:*/
        director[18]    = Slparen;
        director[19]    = setCreate( Tless, SETEND );
        director[20]    = Slsquare;
        director[21]    = firstExpr;                    /*  assignExpression:*/
        director[22]    = Slsquare;                     /*  subscript:  */
        director[23]    = firstExpr;                    /*  expression: */
        director[24]    = followExpr;                   /*  moreDisjunctions:*/
        director[25]    = setCreate( Tor, SETEND );
        director[26]    = firstExpr;                    /*  conjunction:*/
        director[27]    = followConjunct;               /*  moreConjunctions:*/
        director[28]    = setCreate( Tand, SETEND );
        director[29]    = firstNumExpr  ;               /*  negation:   */
        director[30]    = setCreate( Tnot, SETEND );
        director[31]    = firstNumExpr  ;               /*  relExpression:*/
        director[32]    = followRelExpr ;               /*  optRelation */
        director[33]    = Sequal ;
        director[34]    = setCreate( Tnot, SETEND );
        director[35]    = setCreate( Tless, SETEND );
        director[36]    = setCreate( Tgreater, SETEND );
        director[37]    = firstNumExpr  ;               /*  lessOrLessEq:*/
        director[38]    = Sequal ;
        director[39]    = firstNumExpr  ;               /*  greaterOrGreaterEq:*/
        director[40]    = Sequal ;
        director[41]    = firstNumExpr  ;               /*  numericExpression:*/
        director[42]    = followNumExpr ;               /*  moreTerms:  */
        director[43]    = setCreate( Tplus, SETEND );
        director[44]    = setCreate( Tminus, SETEND );
        director[45]    = firstNumExpr  ;               /*  term:       */
        director[46]    = followTerm;                   /*  moreFactors:*/
        director[47]    = setCreate( Tstar, SETEND );
        director[48]    = setCreate( Tslash, SETEND );
        director[49]    = firstPrimary  ;               /*  factor:     */
        director[50]    = setCreate( Tminus, SETEND );
        director[51]    = setCreate( Tinteger, SETEND ) ;       /*  primary:    */
        director[52]    = setCreate( Ttrue, SETEND );
        director[53]    = setCreate( Tfalse, SETEND );
        director[54]    = Slparen;
        director[55]    = Slcurley;
        director[56]    = Sident;
        director[57]    = followFactor;                 /*  subsOrCall: */
        director[58]    = Slparen;
        director[59]    = Slsquare;
        director[60]    = firstExpr;                    /*  arguments:  */
        director[61]    = Srparen;                      /*  moreArguments:*/
        director[62]    = Scomma;
        director[63]    = firstDcls;                    /*  declarations:*/
        director[64]    = followStmt;                   /*  moreDeclarations:*/
        director[65]    = firstDcls;
        director[66]    = firstType ;                   /*  declaration:*/
        director[67]    = setCreate( Tproc, SETEND );
        director[68]    = Sbegin;                       /*  pfBody:     */
        director[69]    = Slparen;
        director[70]    = Scolon;                       /*  funcOrVar:  */
        director[71]    = setCreate( Tfunc, SETEND );
        director[72]    = setCreate( Tint, SETEND );    /*  type:       */
        director[73]    = setCreate( Tbool, SETEND);
        director[74]    = setUnion(followStmt,firstDcls);/*  optArrayBound:*/
        director[75]    = Slsquare;
        director[76]    = firstType ;                   /*  parameters: */
        director[77]    = Srparen;                      /*  moreParameters*/
        director[78]    = Scomma;
        director[79]    = setUnion(firstNumExpr,        /*  outputs:    */
                              setCreate(Ttext, Tskip, SETEND ) ) ;
        director[80]    = followStmt ;                  /*  moreOutput: */
        director[81]    = Scomma ;
        director[82]    = firstNumExpr  ;               /*  output:     */
        director[83]    = setCreate( Ttext, SETEND ) ;
        director[84]    = setCreate( Tskip, SETEND ) ;
        director[85]    = Sident ;                      /*  inputs:     */
        director[86]    = followStmt ;                  /*  moreInputs: */
        director[87]    = Scomma ;
        director[88]    = Sident ;                      /*  input:      */
        director[89]    = setUnion( followStmt, Scomma );/*  optSubscript:*/                            
        director[90]    = setCreate( Tlsquare, SETEND );
         
        /*  Initialize productionLength table           */
        for( i = 1 ; i <= NUMBER_OF_GRAMMAR_RULES ; i++ ){
            for( j = 0 ; productions[i][j] != EndRule ; j++ ) ;
            productionLength[i] = j ;
        } ;
            
        if( dumpDirectorSets ){
                fprintf(dumpFile, "Dump of Director Sets\n" );
                for( i = 1 ; i <= NUMBER_OF_GRAMMAR_RULES ; i++ ){
                   fprintf(dumpFile,"director[%3d ] =  " , i );
                   for(j = 1 ; j <=  lastReservedToken ; j++ )
                        if( setMember( j, director[i] ) ){
                           getTokenName( j ) ;
                           fprintf(dumpFile, "%s ", tokenNameResult );
                        };
                  fprintf(dumpFile, " \n" );
                };
        };

        if( dumpProductions ) {
            int  k  ;
            
            fprintf( dumpFile, "Dump of firstProduction table\n" );
            for( i = firstNonTerminal ; i <= lastNonTerminal; i++ ) {
                getTokenName( i ); 
                normToken = i - firstNonTerminal ;
                fprintf( dumpFile, "firstProduction[ %s ] = %d\n" 
                    , tokenNameResult  , firstProduction[ normToken ] ) ;
            };
        
            fprintf( dumpFile, "Dump of productionLength table\n" );
            for( i = 1 ; i <= NUMBER_OF_GRAMMAR_RULES ; i += 8 ){
                int minr ;
                minr = ( i + 7 > NUMBER_OF_GRAMMAR_RULES ? 
                                  NUMBER_OF_GRAMMAR_RULES : i + 7 ) ;
                fprintf( dumpFile, "productionLength[%3d ..%3d ] = ",
                        i, minr );
                for( j = i ; j <= minr ; j++ )
                    fprintf( dumpFile, "%4d  ", productionLength[j] );
                fprintf(dumpFile,"\n");
            };
            
            fprintf( dumpFile, "Dump of productions table\n" );
            /*  dump in order of definition of nonterminal symbols */
            for( i = firstNonTerminal ; i <= lastNonTerminal ; i++ ){
                getTokenName( i ) ;
                normToken = i - firstNonTerminal ;
                fprintf( dumpFile, "%s\t->\t", tokenNameResult );
                for( j = firstProduction[ normToken ] ; 
                     j < firstProduction[ normToken + 1 ] ; j++ ){
                     for( k = 0 ; k < productionLength[j] ; k++ ){
                        getTokenName( productions[j][k] );
                        fprintf( dumpFile, "%s ", tokenNameResult );
                    };
                    fprintf( dumpFile, "\n" );
                    if( j + 1 < firstProduction[ normToken + 1 ] )
                          /* another production for this non terminal  */
                          fprintf( dumpFile, "\t    ->\t" );
                };
             };            
        };   /* if( dumpProductions ..  */
        
        /*  initialize parse stack                              */

        parseStack[ 0 ] = TendFile ;            /* end of file */
        parseStack[ 1 ] = Nprog ;               /* goal symbol */
        ruleStack[0] = firstProduction[ Nprog - firstNonTerminal ];
        ruleStack[1] = ruleStack[0] ;           
        pst = 1 ;
        parsing = TRUE ;                
        
        scannerAdvance() ;                      /*  scan first token    */
        if( traceScanner )
                scanTokenTrace() ;
        
    } ;         /* end initialize */

void parser() {
        int  tokenClass ;               /* kind of top parse stack token */
        register TOKEN tok ;            /* value of top parse stack token */
        register int  p, t;             /* indices in production[][]    */
        register int normToken ;        /* normalized token number      */
        int  lastProduction ;           /* lastProduction for a nonterminal */
        char errBuffer[120], tokenNameBuffer[40] ; /* error msg buffers */
        
        
        /************************************************/
        /* main loop of the parser                      */
        /*      stay in this loop until an error        */
        /*      occurs or we reach end of program       */
        /************************************************/
        
        while( parsing ) {      /*  loop until end of file or error */
            if( pst < 0 )
                parseError( "parse stack underflow" );

        /****************************************************************/              
        /* NOTE use of division by tokenStride to efficiently           */
        /* classify tokens.  The major token classes are defined in     */
        /* tokenDef.h so that                                           */
        /*      0 * tokenStride <= terminalSymbol       < 1 * tokenStride*/
        /*      1 * tokenStride <= nonterminal symbol   < 2 * tokenStride*/
        /*      2 * tokeStride  <= semantic action      < 3 * tokenStride*/   
        /*      3 * tokenStride <= codegen action       < 4 * tokenStride*/
        /*                                                              */
        /****************************************************************/
        
            /************************************************************/              
            /* save top of parse stack in tokenClass, tok               */
            /************************************************************/
            
            tokenClass = parseStack[pst] / tokenStride ;
            tok  = parseStack[pst] ;
            pst-- ;             /*  POP parser stack    */
            
            if( traceParser ){
                getTokenName( tok ) ;
                fprintf( traceFile, "Parser pop: %s\n", tokenNameResult );
            };
            
            switch( tokenClass ){ /* select action based on kind of token */
            
                /************************************************/
                /*                                              */
                /*  Process terminal symbols                    */
                /*                                              */
                /************************************************/

            case  terminalClass :       /*  terminal symbol     */
           
                if( tok != scanToken ) {
                     /***************************************************/
                     /*  Syntax error, incoming token (scanToken)       */
                     /*  doesn't match expected terminal symbol (tok)   */
                     /***************************************************/
                     getTokenName( tok );
                     strncpy( tokenNameBuffer, tokenNameResult, 40 );
                     getTokenName( scanToken );
                     sprintf(errBuffer, 
                        "Syntax error, token %s expected, token %s in input",
                                tokenNameBuffer, tokenNameResult );
                     parseError( errBuffer );
                }else if( tok == TendFile ){    /* end of parse */
                    /****************************************************/
                    /*  Reached end of file on input, check that we've  */
                    /*  also reached end of parse                       */
                    /****************************************************/
                    if( pst >= 0 )
                        parseError("unexpected end of file");
                    /****************************************************/
                    /*  Everything OK, ACCEPT programs as syntactically */
                    /*  correct.                                        */
                    /****************************************************/
                    parsing = FALSE ;           /* end of parse */
                }else if( tok == Tidentifier || tok == Ttext ){
                    /****************************************************/
                    /*  scanToken is identifier or text.                */
                    /*  save value of scanToken (scanString)            */
                    /*  for use by semantic analysis and code generation*/
                    /****************************************************/
                    strcpy( previousString, scanString );
                }else if( tok == Tinteger ){
                    /****************************************************/
                    /*  scanToken is integer constant                   */
                    /*  save value of scanToken (scanInteger) for use   */
                    /*  by semantic analaysis and code generation       */
                    /****************************************************/
                    previousInteger = scanInteger ;
                } ;
                /********************************************************/
                /*  Normal case, terminal symbol on parse stack (tok)   */
                /*  matched incoming terminal symbol (scanToken)        */
                /*  tok has been popped from the parse stack, advance   */
                /*  the scanner to the next input token                 */
                /********************************************************/
                scannerAdvance();       /*  discard input token  */
                if( traceScanner )
                    scanTokenTrace() ;
                break ;
            
                /************************************************/
                /*                                              */
                /*  Process nonterminal symbols                 */
                /*                                              */
                /************************************************/


            case  nonTerminalClass :    /*  non-terminal symbol         */
        
                /********************************************************/
                /*  search production table for applicable rule         */
                /*  to replace non-terminal symbol                      */
                /*                                                      */
                /*  index in production table of last production for    */
                /*  this nonterminal                                    */
                /********************************************************/
                
                normToken = tok - firstNonTerminal ;
                lastProduction = firstProduction[normToken+1] - 1       ;

                for( p = firstProduction[ normToken ] ; 
                                  p <= lastProduction ; p++ ){
                    /*  use director set to select rule */
                    if( setMember( scanToken, director[ p ])  ) {
                        /***************************************************/   
                        /* perform production p                            */
                        /* push items for rule onto stack in reverse order */
                        /* so first item is on top of stack                */
                        /* use productionLength table to find end of prod. */
                        /***************************************************/
                        
                        if( traceParser )
                            fprintf( traceFile, "Parser (rule %d) push: ",
                                      p  );
                        for( t = productionLength[ p ] - 1; t >= 0 ; t-- ){
                            pst++ ;     /*  PUSH parse stack    */
                            if( pst == parseStackSize ){
                                parseError( "parse stack overflow" );
                                break ;
                            }else{
                                parseStack[pst] = productions[p][t] ;
                                ruleStack[pst]  = p ;
                                if( traceParser ){
                                   getTokenName( parseStack[ pst ] );
                                   fprintf(traceFile," %s",tokenNameResult );
                                };
                           } ;
                        };
                        if( traceParser )
                            fprintf( traceFile, "\n" );
                        break ;  /* exit production search loop */
                    }else if( p == lastProduction ) {   
                        /***************************************************/
                        /* Syntax Error                                    */
                        /* reached last rule and no match on director set  */
                        /***************************************************/
                        getTokenName( scanToken );
                        sprintf( errBuffer, 
                            "syntax error (rule %d), token %s cannot appear here",
                             ruleStack[pst], tokenNameResult );
                        parseError( errBuffer );
                        break ; /* exit production search loop          */
                    };
                };
                break ;
            
                /************************************************/
                /*                                              */
                /*  Process semantic actions                    */
                /*                                              */
                /************************************************/

                
            case  semanticClass :       /*  semantic token      */
                
                /* perform semantic analysis action                     */
                semanticAction( tok );
                break ;
            
                /************************************************/
                /*                                              */
                /*  Process code generation actions             */
                /*                                              */
                /************************************************/


           case  codeGenClass :         /*  code generation token       */

                /* perform code generation action               */
                generateCode( tok ) ;
                break ;

                /*  Process rubbish                             */
                
           default :
                sprintf( errBuffer, 
                    "unknown token (%d) on parse stack" , tok );
                parseError( errBuffer );
                break ;

           } ;   /* end switch */
        } ;     /*  end parser main loop */
    }
