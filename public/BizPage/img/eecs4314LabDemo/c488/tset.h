#ifndef _TSET_H_
/****************************************************************/
/*  Copyright 1995, David B. Wortman                            */
/*                                                              */
/*  tset.h                                                      */
/*  Interface to                                                */
/*  tset.c - a quick and dirty  implementation of finite bitsets*/
/*                                                              */
/****************************************************************/
#define _TSET_H_

/*  Definition of the type SET                                  */
/*  In this implementation SETs are indices into the setStorage */
/*  array defined below.                                        */

typedef  short  SET ;

#define SETWORDS        2
#define SETBITS         32
#define SETMASK         0x7F
#define SETMAXMEMBER   ( SETWORDS * 32 )
#define SETMAX          175
#define SETELEMENTMAX  (( SETWORDS * SETBITS ) - 1 )

/*  The real storage for sets                                   */
/*  SETMAX determines the maximum number of sets available      */
/*  We need enough for approximately 100 director sets plus     */
/*  some temps used during director set initialization          */
/*  The real setStorage is defined in tset.c                    */

#ifndef EXTERN_SETS
#define EXTERN_SETS  extern
#endif

EXTERN_SETS unsigned  setStorage[ SETMAX ] [ SETWORDS ] ;

/*  marker for end of argument list to setCreate                */
#define SETEND          -1

/*  To improve performance setMember is a macro that uses       */
/*  the global variable setStorage directly                     */

#define setMember( val, set )  \
    ( 1 <= val && val <= SETMAXMEMBER ? \
    ( setStorage[set][ val / SETBITS ] & ( 1 << ( val & SETMASK ))) : FALSE )

        /*  Compute the union of two sets       */
        
SET setUnion( SET setA, SET setB ) ;
        
        /*  Create a set from a list of elements  */
        /*  NOTE: accepts a variable length argument list       */
        
SET setCreate( int firstArg , ... /* VAR ARGS */ ) ;

#endif
