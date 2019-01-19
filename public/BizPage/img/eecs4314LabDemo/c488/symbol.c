/*void exit (...);*/
/********************************************************/
/*  Copyright 1995, David B. Wortman                    */
/*                                                      */
/*    symbol.c                                          */
/*    Symbol table module for the CSC488S Project       */
/*                                                      */
/********************************************************/

static char symbolVersion[] = 
     "$Id: " ;
     
/*   Include common definitions                         */
#include "common.h"

/*   Include global variables                           */
#include "globalvars.h"

/*   Include interface to this module                   */
#include  "symbol.h"

/*   Utility procedures and functions used by the       */
/*   symbol table module GO HERE                        */

/*   Procedures and functions exported by the           */
/*   symbol table module                                */

/*   Initialize the symbol table                        */
void symbolInitialize() {
    int x=0;
    /*  Initialization code for the symbol table        */
    /*  GOES HERE                                       */
    for (x=0; x<SIZE_OF_HASH_ARRAY; x++)
    {
        HashTable[x]=NULL;
    }   
}

/*   Finalize the symbol table                          */
void symbolFinalize(){
    int x=0;
    int y=0;
    
    
    /* Finalization code for the symbol table           */
    /* GOES HERE                                        */
    for (x=0; x<SIZE_OF_HASH_ARRAY; x++)
    {
        if (HashTable[x] != NULL)
        {
            printf("\nERROR: Symbol Table not Empty\n");
            printf("\n  Symbol Table Dump \n");
            for (y=0; y<SIZE_OF_HASH_ARRAY; y++)
            {
               Display_List(HashTable[y]);
            }   
            exit(1);
        }
    }
    
}

/* global variable that stores scope level */

int ScopeLevel=-1; 
int PrevScope;
int OrderLevel=0;

ELEMENT *HashTable[SIZE_OF_HASH_ARRAY];  /* indexed 0->51, A->Z and a->z */


/*  Additional procedures and functions exported by     */
/*  the symbol table module GO HERE                     */

void Add_To_Beg ( STPtr *head, STPtr New)
{
  if ( *head == NULL )     
  {
     New->Next = NULL;
     *head = New;
  }
  else
  {
     New->Next = *head;
     *head = New;
  }
}


void Remove_From_Beg ( STPtr *head )
{
   STPtr temp;
   temp = *head;
   
   if ( *head != NULL )
   {
      *head = (*head)->Next;
   }
}

void Display_List ( STPtr head )
{
   STPtr temp;
   temp = head;
   while (temp != NULL)
   {
      printf ("Scope    Identifier\n");
      printf ("-----    ----------\n");
      printf ("%d       %s \n", temp->ScopeNum, temp->Identifier);
      temp = temp->Next;
   }
}

int find_index (char token[MAX_IDENTIFIER])
{
    int Index;
    
    
    /* For Upper Case ( 65->90, A->Z )
       Subtract by 65 to index array from 0->25 */
    Index = toascii( token[0] ) - 'A';

    /* For Lower Case ( 97->122, a->z ) 
       we are left with 97-65 to 122-65 or 32->57 */
    /* lower case: make array index 26->51 */
    if ( Index > 25 ) 
        Index = Index - 6;
        
    return Index;
}


void RemoveScope ()
{
   destroy(ScopeLevel);
}

void destroy(int ScopeNumber)
{
   int i;
   for (i=0; i < 52; i++)
   {
      if ( HashTable[i] != NULL)
      {
         while ( HashTable[i]->ScopeNum == ScopeNumber)
         {  
            Remove_From_Beg(&HashTable[i]); 
            if (HashTable[i] == NULL) break; 
         }
      }
   }
}


STPtr Add(char token[MAX_IDENTIFIER])
{
   STPtr New = NULL;
   New = (STPtr) malloc (sizeof(ELEMENT));
   strcpy(New->Identifier, token);                 /* copy token into identifier */
   Add_To_Beg( &HashTable[find_index(token)], New );
   
    #ifdef DEBUG
    printf( "\nAdding token %s, %p\n", New->Identifier , New);
    #endif
    
   return HashTable[find_index(token)];    /* New */         /* return pointer to structure
                                                       that is already in hash table */
}

void PrintSymbolTable(int val)
{
   int i,x;
   STPtr temp;
   
   printf ("\n Symbol Table ");
   printf ("\n---------------------\n");
   for (x=1; x <= ScopeLevel; x++)
   {
       printf("\nScope Level %d\n" , x);
       printf("----------\n");
       for (i=0; i < SIZE_OF_HASH_ARRAY; i++)
       {
          temp = HashTable[i];
          while ( temp != NULL)
          {
             if (temp->ScopeNum == x)
             {
                printf ("identifier: %s, %d\n",
                    temp->Identifier, temp->ScopeNum);
             }
             temp = temp->Next;
          
          }  /* end while */
          
       } /* end for */
    } 
}


STPtr FindToken (char token[MAX_IDENTIFIER])
{
   STPtr temp;
   temp = HashTable[find_index(token)];
   while ( temp != NULL )
   {
      if ( strcmp(temp->Identifier, token) == 0 )
      {
         return temp;
      }
      temp = temp->Next;
   }
   return NULL;
}

