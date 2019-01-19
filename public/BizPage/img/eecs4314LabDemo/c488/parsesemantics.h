#ifndef _PARSESEMANTICS_H_
/****************************************************************/
/*  Copyright 1995, David B. Wortman                            */
/*                                                              */
/*      parsesemantics.h                                        */
/*      This include file defines the interface between the     */
/*      parser and semantic analysis module                     */
/*                                                              */
/****************************************************************/
#define _PARSESEMANTICS_H_

static char parsesemanticsVersion[] 
     = "$Id: parsesemantics.h,v 1.1 1996/02/05 17:01:01 dw Exp $"       ;


/* For documentation purposes, use names for the semantic analysis      */
/*  actions that correspond directly to the class handouts              */
/*  (more mnemonic names would be preferable).                          */

#include "tokenDef.h"

enum  SemanticActions {

 S0  = firstSemanticToken ,
 S1     ,  
 S2     ,  
 S3     ,  
 S4     ,  
 S5     ,  
 S6     ,  
 S7     ,  
 S8     ,  
 S9     ,  
 S10    ,  
 S11    ,  
 S12    ,  
 S13    ,  
 S14    ,  
 S15    ,  
 S16    ,  
 S17    ,  
 S18    ,  
 S19    ,  
 S20    ,  
 S21    ,  
 S22    ,  
 S23    ,  
 S24    ,  
 S25    ,  
 S26    ,  
 S27    ,  
 S28    ,  
 S29    ,  
 S30    ,  
 S31    ,  
 S32    ,  
 S33    ,  
 S34    ,  
 S35    ,  
 S36    ,  
 S37    ,  
 S38    
 
 } ; 

#define lastSemanticToken S38

#endif

