#ifndef _SYMBOL_H_
/********************************************************/
/*  Copyright 1995, David B. Wortman                    */
/*                                                      */
/*      symbol.h                                        */
/*      Interface to the symbol table module            */
/*                                                      */
/********************************************************/
#define _SYMBOL_H

static char symbolInterfaceVersion[]
     = "$Id: symbol.h,v 1.21 1996/03/21 03:50:22 kovan Exp singhdi $" ;
     
/*      Symbol table initialization                     */
void symbolInitialize();

/*      Symbol table finalization                       */
void symbolFinalize();


/*      Additional functions and procedures in the      */
/*      interface to the symbol table module            */
/*      GO HERE                                         */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>


#define SIZE_OF_HASH_ARRAY 52

extern int ScopeLevel;  /* global variable that stores scope level */
extern int OrderLevel;
extern int PrevScope;

enum Types { IntType, BoolType };
enum boolean { True, False };       
enum Kind { array, scalar, function, procedure, parameter };

union ValueOfId  {
   int IntValue;
   enum boolean BoolValue;
};

struct SymTable {
   char Identifier[MAX_IDENTIFIER];  /*MAX_IDENTIFIER declared in common.h file of project */
   int Size;                
   int ScopeNum;           /*Scope number declared in  */
   int OrderNumber;        /*Order of declaration in its scope. */ 
   int Offset;
   union ValueOfId Value;
   enum Types TypeOf;
   enum Kind KindOf;
   int NumOfParam;         /*If TypeOf is procedure or function, this var stores # of param. */
   int ParamNumber;
   short CodeAddr;
   struct SymTable *Param; /*If TypeOf is procedure or function, this points to its parameters. */
   struct SymTable *Next;
   struct SymTable *NextDecl;
};

typedef struct SymTable ELEMENT;
typedef struct SymTable *STPtr;



/* This is the hash table for the symbol table      */
extern ELEMENT *HashTable[SIZE_OF_HASH_ARRAY];  /* indexed 0->51, A->Z and a->z */


/*     Adds  a symbol table entry 'new' to the appropriate  
       head of the linked list.                            */
void Add_To_Beg ( STPtr *head, STPtr New);  


/*     Removes the first element from the linked list.   */
void Remove_From_Beg ( STPtr *head );

/*     Displays the linked list specified   */
void Display_List ( STPtr head );

/*     Function returns an index into the hash table.  */
int find_index ( char token[MAX_IDENTIFIER]);

/*     destroys all the symbol table entries with the 
       specified 'ScopeNumber                         */
void destroy(int ScopeNumber);

/*     Removes all elements in hash table that is a member of the
       current scope. It then decrements the 'ScopeLevel' variable.  */
void RemoveScope ();  /*void RemoveScope (...); IRB *//* AJM */


/*    Takes as parameter a token, allocates memory, copies token and 
      scope number into structure then inserts the structure into hash
      table.                                          */
STPtr Add(char token[MAX_IDENTIFIER]);

/*   Prints all tokens of the symbol table with the Scope value of 'val'.  */       
void PrintSymbolTable(int val);

/* Searches the HashTable for the token entered as a paramete. If the token
   is found a pointer to the structure of the token is returned. Else 
   return NULL                                                  */
STPtr FindToken (char token[MAX_IDENTIFIER]);


/*  This was placed here so when compiled using -Wall option no warning
    will be given about using printf statements.      */
/*int printf();*/

#endif

