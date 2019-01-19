/****************************************************************/
/*  Copyright 1995, David B. Wortman                            */
/*                                                              */
/*  tset.c                                                      */
/*  A quick and dirty  implementation of finite bitsets         */
/*  Provide the minimal functionality necessary to implement    */
/*     director sets for the CSC488S LL(1) parser.              */
/*  The only operations provided are set create, union and      */
/*      membership. The speed of set membership is optimized    */
/*      to make the parser run fast                             */
/*                                                              */
/****************************************************************/

/*  This implementation is intended to be used by parser.c      */

/*  Include common definitions and global variables             */
#include "common.h"
#include "globalvars.h"

/*  Variable length argument list used in setCreate             */
#include <stdarg.h>

/*  Include the interface for this code                         */

/*  The real storage for sets                                   */
/*  SETMAX determines the maximum number of sets available      */
/*  We need enough for approximately 100 director sets plus     */
/*  some temps used during director set initialization          */

#define  EXTERN_SETS

#include "tset.h"

/*   Index of next available entry in setStorage                */
int  nextSet  = 1 ;

/*  all sets are stored in the setStorage array.  externally    */
/*  sets are represented by their index into this array         */
/*  Space is setStorage is allocated as needed and never freed  */

/*   This implementation provides for sets of up to 64 members  */
/*   The 64 bits in the 2 unsigned words are used for set membership */
/*   For any value V                                            */
/*      V / SETBITS     selects word[0] or word[1]              */
/*      (1 << ( V & SETMASK ))  selects the bit in the word     */

/*   Internal procedure to allocate entries in setStorage       */

static SET setAlloc() {
        if( nextSet++ < SETMAX )
                return nextSet - 1  ;
        else {
            fprintf( errorFile, 
            "setAlloc: Error - too many sets - increase SETMAX\n");
            return SETMAX - 1 ;
        }
    } 

/*   set union function                                 */

SET setUnion(  SET setA, SET setB ){
        /*  return  setA union setB     */
        SET outSet = setAlloc() ;
        setStorage[outSet][0] = setStorage[setA][0] | setStorage[setB][0] ;
        setStorage[outSet][1] = setStorage[setA][1] | setStorage[setB][1] ;
        return outSet ;
   } ;
   
   /*  Internal procedure to add one element to a set  */
   
static void addElement( SET oSet , int arg ){
    /* add value arg to oSet */
    if( 1 <= arg && arg <= SETMAXMEMBER )
        setStorage[oSet][ arg / SETBITS ] |= ( 1 << ( arg & SETMASK )) ;
    else
        fprintf(errorFile,
          "setCreate: Error, invalid value (%d) ignored\n", arg );
 };
        
SET setCreate( int firstArg , ... /* VAR ARGS */ ) {
        int e ;
        va_list  argptr ;               /* pointer to variable arg list */
        SET outSet = setAlloc() ;
        if( firstArg != SETEND )
            addElement( outSet, firstArg );
        else
            return outSet ;             /* nothing to add */

        /* Start processing with first argument  */
        va_start( argptr, firstArg );
        /* loop through argument list, adding values to newSet */
        while(( e = va_arg(argptr, int)) != SETEND ){
            addElement( outSet, e ) ;
        }
        va_end( argptr );
        return outSet ;
   } ;

/*  Macro to validate assumptions about code above      */

#define VALIDATE_TSET  \
   assert( SETWORDS == 2 )
