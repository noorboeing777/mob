/********************************************************/
/*							*/
/*	scanner.c					*/
/*	lexical analyzer for CSC488S project compiler	*/
/*							*/
/********************************************************/

   static char *scannerVersion = 
       "$Id: scanner.c,v 1.1 1996/03/26 17:41:47 wortman Exp $" ;

   /* include interface definitions	*/
   
   		/*  Common definitions	*/
   #include "common.h"
   		/*  Global shared variables	*/
   #include "globalvars.h"
   		/*  Interface to this module	*/
   #include "scanner.h"
		
   /*************************************************************/
   /*								*/
   /*  This is the real home for the scanner/parser interface 	*/
   /*  tables.							*/
   /*								*/
   /*************************************************************/

   #define EXTERN_SCANNER
   
   		/*  Scanner/parser interface	*/
   #include "scanparse.h"

   /* classify input characters by type		*/
   typedef enum { ignore, illegal, startComment, letter, digit,
	startText, special, endLine, endFile }     charClass ;

   /* character classification array for fast scanning	*/
   charClass classify[ 256 ];

   /* special character constants	*/
   const char endFileChar =  EOF ;		/* defined in stdio.h */
   const char newLineChar = '\n' ;
   const char tabChar 	= '\t' ;
   const char nullChar	= '\000' ;
   const char blankChar = ' ' ;
   const char zeroChar  = '0' ;		/* digit zero */

   /* variables set by readch and used by advance	*/

   char ch = ' ' ;		/* actual value of next character  */
   charClass  classch ;		/* classify[ ch ]		*/
   BOOLEAN  endFileSeen = FALSE ;	/* end of file on inputFile */ 
   
   /*  Scanner buffers one line of input to improve performance */           
#define LINE_BUFFER_SIZE  256
   char lineBuffer[LINE_BUFFER_SIZE] ;	/* buffer for one input line      */
   char *lp = NULL ;			/* pointer to next char in buffer */
   
   /*************************************************************/
   /*								*/   
   /*  The scanner reads all input from the file specified	*/
   /*  by the global variable inputFile which is set in main.c	*/
   /*								*/
   /*  Internal procedure for generating scanner error messages	*/
   /*  Writes message to errorFile and sets global variable	*/
   /*  errorOccurred to TRUE.					*/
   /*								*/
   /*************************************************************/

   
static void scannerError( char *msg ){
   fprintf(errorFile, "Lexical Error (line %d) - %s\n", line, msg );
   errorOccurred = TRUE ;
}
   /*************************************************************/
   /*								*/   
   /*  Internal utility procedure to handle file input,  	*/
   /*  line buffering and end of file processing		*/
   /*  Sets  ch to next character and classch to its charClass	*/
   /*  Sets endFileSeen and errorOCcurred.			*/
   /*								*/
   /*************************************************************/
   
static  void readch() {
      if( endFileSeen ){
      	    scannerError( "Attempt to read past end of inputFile");
      	    ch = endFileChar ;
      	    classch = endFile ;
      	    return ;
      } ;
      if( ! *lp ) {
          /*********************************************/
          /*  line buffer pointer points at null char  */
          /*  read next line of input from inputFile   */
          /*********************************************/
          lp = fgets( &lineBuffer[0], LINE_BUFFER_SIZE, inputFile ) ;
          if( !lp ){
          	/*  fgets returned null, assume end of file  */
          	endFileSeen = TRUE ;
          	lp = &lineBuffer[0] ;   /* just in case */
          	*lp = nullChar ;
          	ch = endFileChar ;
          	classch = endFile ;
          	return ;
          };
          
          /* fgets returned a non empty string and lp points at the 1st char */

          if( traceSource )
          	fprintf( traceFile, "%4d:%s\n", line, lineBuffer );
       };
      /***********************************************************************/
      /* Set ch to the next input character and classch to its classification*/
      /* This is the normal case	 				     */
      /***********************************************************************/
      ch = *lp++ ;	   
      classch = classify[ ch ] ;
   }
   
/*************************************************************************/
/*									*/
/*  Internal function to return the relative position of character c 	*/
/*  in string s.  Must succeed since it's only called to classify	*/
/*  known special characters.						*/
/*									*/
/************************************************************************/

static int strpos( const char * s, char c ) {
    char * tmp ;
    tmp = strchr( s, c );
    assert( tmp != NULL );
    return  (int)( tmp - s );
}

/****************************************************************/
/*								*/
/*	scanner initialization procedure			*/
/*								*/
/****************************************************************/

void scannerInitialize() {
	int i ;
	const char * cp ;
	
	/*  Define some useful character constants	*/
	const char *lowerLetters = "abcdefghijklmnopqrstuvwxyz" ;
	const char *upperLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
	const char *digitChars   = "0123456789" ;


	/*  Validate the constants and tables used by the scanner	*/
	assert( firstCompositeToken >= 1 
		&& firstCompositeToken <= lastCompositeToken );
	assert( ( sizeof compositeNames / sizeof( char *) ) ==
		lastCompositeToken - firstCompositeToken  + 2  );
		/* compositeNames[0] unused */
	assert( firstSpecialToken > lastCompositeToken 
		&& lastSpecialToken - firstSpecialToken + 1
		   == numberOfSpecialChars );
	assert( strlen(specialCharacters) == numberOfSpecialChars );
	assert( firstReservedToken > lastSpecialToken 
		&& firstReservedToken <= lastReservedToken );
	assert( numberOfReservedWords ==
		   lastReservedToken - firstReservedToken + 1 );
	assert( ( sizeof reservedWords / sizeof( struct reservedWordStruct ))
		== numberOfReservedWords ) ;
	assert( reservedWords[0].value == firstReservedToken
		&& reservedWords[numberOfReservedWords -1 ].value
		    == lastReservedToken );
		    
   	/* Initialize tables used by the scanner */
	/******************************************************/
   	/* initialize scanner character classification table  */
	/* first make all unknown characters illegal	      */
	/******************************************************/
	for( i = 0 ; i <= 255 ; i++ )
	    classify[ i ] = illegal ;
	    
	/********************************************************/
	/* characters that are ignored				*/
	/********************************************************/
	classify[ blankChar ] = ignore ;	/* blanks	*/
	classify[ tabChar ] = ignore ;		/* tabs		*/
	classify[ newLineChar ] = endLine ;	/* new line	*/
	
	/********************************************************/
	/* letters 						*/
	/********************************************************/
	for( cp = lowerLetters ; *cp ; cp++ )
	    classify[ *cp ] = letter ;
	for( cp = upperLetters ; *cp ; cp++ )
	    classify[ *cp ] = letter ;
	classify[ '_' ] = letter ;		/* underscore */
	/********************************************************/
	/* digits						*/
	/********************************************************/
	for( cp = digitChars ; *cp ; cp++ )
	    classify[ *cp ] = digit ;
	/********************************************************/
	/* special characters					*/
	/********************************************************/
	for( cp = specialCharacters ; *cp ; cp++ )
	    classify[ *cp ] = special ;
	/********************************************************/
	/* miscellaneous					*/
	/********************************************************/
	classify[ '%' ] = startComment	;
	classify[ '"' ] = startText ;
	/********************************************************/
	/*  classification of endFileChar is handled in readch	*/
	/********************************************************/

	/********************************************************/
	/*  initialize line buffer, and ch			*/
	/********************************************************/		
	/* initialize the line input buffer */
	lp = &lineBuffer[0] ;
	*lp = nullChar ;	/*  Force a line to be read */

	/*  initialize the line number counter	*/	
	line = 1 ;
	/* set ch to blank, will force read of first line of	*/
	/* input file in scannerAdvance				*/
	/* establishes precondition for scannerAdvance		*/
	
	ch = blankChar ;
	classch = ignore ;

    }

/****************************************************************/
/*								*/
/*  advance scanner by one token.				*/
/*  accumulate token value as required				*/
/*								*/
/*  precondition: ch = next character (unprocessed)		*/
/*								*/
/****************************************************************/

void scannerAdvance() {
      char idTemp[MAX_IDENTIFIER + 1] ;	/* buffer for identifiers */
      char *idp ;		/* pointer to idTemp buffer */
      char *svp ;		/* pointer to scanString */
      int  left, mid, right ;	/* binary search indices */
      int  comp ;		/* comparison result */
      int  charCount ;		/* length of identifers and texts */
      
      while(TRUE) { 	/* main character scanning loop		*/
			/* scan until one token has been found	*/
	switch( classch ) {

	case ignore:			/* characters that are ignored */
		readch();
		break ;

	case endLine :			/* end of line character */
		line++ ;
		readch() ;		/* ignore the character */
		break  ;
	
	case startComment :		/* start of comment */
		/* discard characters in comment */
		while( ch != newLineChar && ch != endFileChar )
		   readch() ;
		break ;
		
	case letter :			/* identifier or reserved word */
		/* accumulate text of identifier	*/
		idp = &idTemp[0] ;
		*idp++ = ch ;
		charCount = 1 ;
		readch();	/* next character in identifier */
		while( classch == letter || classch == digit ) {
		    *idp++ = ch ;
		    if( ++charCount == MAX_IDENTIFIER ){
		    	scannerError("Identifier too long");
		    	*idp = nullChar ;
		    	break ;
		    };
		    readch() ;
		};
		*idp = nullChar ;	/* null terminate idTemp */
		
		/************************************************/
		/* use binary search to look identifier up in 	*/
		/* reserved word table 				*/
		/************************************************/
		left = 0 ; 				/* first element */
		right = numberOfReservedWords - 1 ;	/* last element */
		while( left <= right ){
		    mid = (left+right) / 2 ;
		    comp = strcmp(reservedWords[mid].name,idTemp) ;
		    if(  comp < 0 )
			left = mid + 1 ;
		    else  if( comp > 0 ) 
			right = mid -1 ;
		    else {
		        /*************************************************/
			/* found a reserved word  in reserved word table */
			/*************************************************/
			scanToken = reservedWords[mid].value ;
			return ;
		    } 
		} ;

		/**********************************************************/
		/* if we reach here, we have an identifier not a reserved */
		/* word we know that idTemp will fit in scanString	  */
		/**********************************************************/
		strcpy( scanString, idTemp );
	 	scanToken = Tidentifier ;
		return ;

	case digit :			/* integer constant	*/
		/* integer, accumulate value		*/
		scanInteger  = ch - zeroChar ;	/* normalize digit */
		readch();
		while( classch == digit ){
		    scanInteger = 10 * scanInteger + ( ch - zeroChar ) ;
		    if( scanInteger > MAX_INTEGER ){
		        scannerError( "integer constant too large" );
		        scanInteger = MAX_INTEGER ;
		        break ;
		    };
		    readch() ;
		};
		scanToken = Tinteger ;
		return ;

	case startText :		/* text string	*/
		/* text, accumulate value of string */
		svp = &scanString[0] ;	/* start at beginning of buffer */
		charCount = 0 ;		/* count characters */
		while( TRUE ){
		    readch();		/* read next character */
		    if( classch == startText ) {		
			/* possible end of string 	*/
			readch() ;
			if( classch != startText ) 	
			    /* not "" so end of string	*/
			    break ;
			 /* otherwise fall through and one " gets added */
		    } ;
		    if( classch == endLine || classch == endFile ) {
			scannerError( "Missing closing text quote");
			break ;
		     };
		     /* normal case, accumulate string value */
		     if( charCount++ == MAX_TEXT ){
		         scannerError("Text string too long");
		         svp-- ;
		         break ;
		      };  
		      /* Add character ch to text string */
		     *svp++ =  ch ;
		};
		*svp = nullChar ;		/* null terminate string */
		scanToken = Ttext ;		
		return ;

	case special :			/*   special characters */
		/*********************************************************/
		/*  Compute token value quickly by doing a table lookup  */
		/*  in the string specialCharacters.  This lookup must	 */
		/*  succeed since we only get to this case (via classify)*/
		/*  for the strings that are in specialCharacters.  See  */
		/*  scannerInitialize				 	 */
		/*********************************************************/
		 
 		scanToken = strpos( specialCharacters, ch ) + specialTokenOffset ;
		readch();
		return ;

	case endFile :			/*  end of file character */
		scanToken = TendFile ;
		return ;

	default :			/* all other characters		*/
		scannerError( "Illegal character - ignored" );
		readch() ;
		break ;
	} ;		/* end switch( classch )	*/

      } ;		/* end main scanner loop	*/

   }

