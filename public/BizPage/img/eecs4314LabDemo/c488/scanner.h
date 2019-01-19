#ifndef _SCANNER_H_
/************************************************/
/*						*/
/*	scanner.h				*/
/*	Interface to the scanner module		*/
/*						*/
/************************************************/
#define _SCANNER_H_

static char scannerInterfaceVersion[] =
	"$Id: scanner.h,v 1.1 1996/02/05 16:58:07 dw Exp $" ;

	/*  Initialize the scanner		*/
void scannerInitialize() ;
	
	/*   Advance to next scan token		*/
	/*   Set scanToken and (perhaps)	*/
	/*   scanString or scanInteger		*/
void scannerAdvance() ;

#endif
