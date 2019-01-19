#ifndef _COMMON_H_
/********************************************************/
        /*  Copyright 1995, David B. Wortman */
/* common.h                                                     */
/* common definitions that are used throughout the      */
/* compiler                                             */
/*                                                      */
/********************************************************/
#define _COMMON_H_

static char commonVersion[] =
        "$Id: common.h,v 1.1 1996/02/05 16:58:07 dw Exp $" ;

/*  C libraries that are widely used            */
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*  Special types  */

typedef short TOKEN ;

typedef unsigned char  BOOLEAN ;
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/*  Limits on the source language.              */
/*  Enforced in scanner.c                       */

   /*  Limit on size of identifier              */
#define  MAX_IDENTIFIER  32

   /*  Limit on size of text (string)           */
#define  MAX_TEXT   256
    /* 255 characters plus 1 for terminating null */

   /*  Limit on size of integer value           */
   /*  Target machine has 16 bit signed integers */
   
#define  MAX_INTEGER  32767                     
#define  MIN_INTEGER  -32767

#define  UNDEFINED  -32768
   /* most negative 16-bit integer value is used to  */
   /* denote undefined                               */


#endif
