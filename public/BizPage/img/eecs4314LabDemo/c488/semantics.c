/****************************************************************/
/*  Copyright 1995, David B. Wortman                            */
/*                                                              */
/*      semantics.c                                             */
/*      Semantics Analysis Module for CSC488S Project Compiler  */
/*                                                              */
/****************************************************************/

static char semanticsVersion[] 
         = "$Id: semantics.c,v 2.15 1996/03/23 02:02:25 singhdi Exp $" ;

/*  Include common definitions                                  */
#include "common.h"

/*  Include global variables                                    */
#include "globalvars.h"

/*  Include interface to this module                            */
#include "semantics.h"

/*  Include parser/semantic analysis interface                  */
#include "parsesemantics.h"

/*  Include interface to symbol table                           */
#include "symbol.h"
#ifndef SEMEXT
#define SEMEXT
   #include "semanticext.h"
#endif


/* Define order number */
/* keeps track of number of arguments and variables */
/*static int OrderLevel = 0;*/  /* AJM */

extern Push_FuncName( STPtr );

/* ************************************************************ */
/*  Utility procedures used for semantic analysis GO HERE       */
/* ************************************************************ */


/* Function to return symbol table pointer of symbol on top of symbol stack */
STPtr SymbolS_findPtr (char id[])
{
        return (FindToken(id));
}



/* Function to pop symbol stack */
SymbolS_Pop()
    {
    struct SymbolE *temp;
    temp = SymbolS_top;
    SymbolS_top = SymbolS_top->next;
    free(temp);
    }

/* Function to push an item onto symbol stack */    
SymbolS_Push(STPtr ptr)
    {
    struct SymbolE *temp;
    temp = malloc(sizeof(struct SymbolE)); 
    /*temp = (SymbolE *) malloc(sizeof(struct SymbolE)); *//* AJM */
    temp->ptr = ptr;
    temp->next = SymbolS_top;
    SymbolS_top = temp;  
    }

/* Function to return name of item on top of symbol stack */    
char *SymbolS_Name()
    {
    return (SymbolS_top->ptr)->Identifier ;
    }
    
/* Function to return the nth item on symbol stack */    
STPtr SymbolS_Check(int look)
    {
    int count;
    struct SymbolE *temp = SymbolS_top;
    for (count=0; count<look; count++)
      temp = temp->next;
    return (temp->ptr);
    }

/* Function to pop pop stack */
TypeS_Pop()
    {
    struct TypeE *temp;
    temp = TypeS_top;
    TypeS_top = TypeS_top->next;
    free(temp);
    }

/* Function to push an item onto type stack */        
TypeS_Push(enum Types type)
    {
    struct TypeE *temp;
    temp = malloc(sizeof(struct TypeE)); 
    /*temp = (TypeE *) malloc(sizeof(struct TypeE)); *//*AJM*/
    temp->type = type;
    temp->next = TypeS_top;
    TypeS_top = temp;
    }

/* Function to return the type of the nth item on type stack */    
TypeS_Check(int look)
    {
    int count;
    struct TypeE *temp = TypeS_top;
    for (count=0;count<look;count++)
      temp = temp->next;
    return (temp->type);
    }

CountS_Pop()
    {
    struct CountE *temp;
    temp = CountS_top;
    CountS_top = CountS_top->next;
    free (temp);
    }
    
CountS_Push(int count)
    {
    struct CountE *temp;
    temp = malloc(sizeof(struct CountE));
    /* temp = (CountE *) malloc(sizeof(struct CountE));*/ /*AJM*/
    temp->itemCount = count;
    temp->next = CountS_top;
    CountS_top = temp;
    }


struct SymbolE *SymbolS_top = NULL;
struct TypeE *TypeS_top = NULL;
struct CountE *CountS_top = NULL;
/* Generic error handler function */
ErrorHandler()
{
/* Generic error handler function.  Specific error
 * handling is added when need
 */
}

/* ************************************************************ */
/*  Procedures exported by the semantics module                 */
/* ************************************************************ */


/*  semanticsInitialize - called by the parser before compilation */

void semanticsInitialize() {

   /*   Initialize the symbol table                             */
   symbolInitialize();
   
   /*  Additional initialization code for the                   */
   /*  semantic analysis module                                 */
   /*  GOES HERE                                                */
   
}


/*  semanticsFinalize - called by the parser after compilation  */

void semanticsFinalize(){

  /*  Finalize the symbol table                                 */
  symbolFinalize();
  
  /*  Additional finalization code for the                      */
  /*  semantics analysis module                                 */
  /*  GOES here.                                                */
  
}

/*   semanticAction - called by the parser to perform one       */
/*                      semantic action                         */


static STPtr PrevPtr=NULL;
static STPtr PrevPtr_stack[11];
static PrevPtr_top = 0;

Push_PrevPtr()
{
    PrevPtr_stack[PrevPtr_top] = PrevPtr;
    PrevPtr = NULL;
    PrevPtr_top++;
}

Pop_PrevPtr()
{
    PrevPtr_top--;
    PrevPtr = PrevPtr_stack[PrevPtr_top];
}





void semanticAction( int actionNumber ) {


   if (traceSemantics )
       fprintf( traceFile, "semantics: S%d\n" , 
                     actionNumber - firstSemanticToken );




   /* ********************************************************** */                  
   /*  Codes to implement each semantic action GOES HERE         */
   /* ********************************************************** */

                     
   switch (actionNumber - firstSemanticToken)
       {
       
        /* -------------- */
        /* Scopes         */
        /* -------------- */
        
       case 0 : /* Enter a new Scope */
            PrevScope = ScopeLevel;
            ScopeLevel++;
            
            Push_PrevPtr();
            
            break;

       case 1 : /* Exit a scope that doen't have parameters */
            RemoveScope(ScopeLevel);
            PrevScope = ScopeLevel;
            ScopeLevel--;
            
            Pop_PrevPtr();
            
            break;

       case 2 : /* Exit scope that has parameters */
            RemoveScope(ScopeLevel);
            PrevScope = ScopeLevel;
            ScopeLevel--;
            
            Pop_PrevPtr();
            
            break;

       case 27 : /* Optionally print the symbol table for the current scope */
            if (dumpSymbols == TRUE) 
               PrintSymbolTable(ScopeLevel);
            break;

            
       /* ----------------- */
       /* Symbols and Types */
       /* ----------------- */

       case 3 : /* Check that the identifier is not already declared in the current scope */
            {
            STPtr ptr;
            char temp[MAX_IDENTIFIER];
            int len;
            strcpy(temp,previousString);
            
            do
                {
                ptr = FindToken(temp);

                if ( (ptr != NULL) && (ptr->ScopeNum == ScopeLevel) )
                  {
                  fprintf(errorFile, "Line %d:  The token '%s' has already been declared in the current scope\n", line, temp);

                  /** Error: So ignore current identifier **/
                  len = strlen(temp);
                  temp[len] = '_';
                  temp[len+1] = 0;
                  }
                }    
            while( (ptr != NULL) && (ptr->ScopeNum == ScopeLevel) );      
                  
            /* Add the current token to symbol table */
            ptr = Add(temp);
            
            /* push current token onto semantic symbol stack */
            SymbolS_Push(ptr);
            
            /* calc offsets, and set link lists for variables */
            if (PrevPtr != NULL)
            {
                ptr->Offset = PrevPtr->Offset + PrevPtr->Size;
                ptr->NextDecl = NULL;  
                PrevPtr->NextDecl = ptr;
            }
            else 
            {
                ptr->NextDecl = NULL;
                ptr->Offset=1;
            }
                
            /* set previous to current for next time through loop */
            PrevPtr = ptr;
            }    
            break;
            

       case 4 : /* Insert lexic level and order number into the symbol table */
            {
            STPtr ptr;

            /* Get current identifier's symbol tabel pointer */
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);
            
            /* Increment order number */
            OrderLevel ++;

            /* Add lexic level and order number to symbol table */
            ptr->ScopeNum = ScopeLevel;
            ptr->OrderNumber = OrderLevel;

            }
            break;

       case 5 : /* Insert type into the symbol table */
            {
            STPtr ptr;
            enum Types tempType;
            
            /* Get the type on top of type stack */
/*          tempType = TypeS_Check(0); */ tempType = (enum Types) TypeS_Check(0); /*AJM*/

            /* Get current identifier's symbol tabel pointer */
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);

            /* Add  the type to symbol table */
            ptr->TypeOf = tempType;
                
            }
            break;

       case 6 : /* check that the identifier is declared. Push its symbol table index onto Symbol Stack */
            {
            STPtr ptr;
            ptr = FindToken(previousString);

            if (ptr != NULL)
              SymbolS_Push(ptr);
            else
              {                 /* push previousString onto SymbolStack so 
                                   compiler can continue   */
              STPtr ptr;
              ptr = (STPtr) malloc (sizeof(ELEMENT));
              strcpy (ptr->Identifier,previousString);
              SymbolS_Push(ptr);
              fprintf(errorFile, "Line %d:  The token '%s' has not been declared\n",
                        line, previousString);
                        
              ErrorHandler();
              }
            }
            break;

       case 7 : /* Pop symbol stack */
            SymbolS_Pop();
            break;

       case 8 : /* Push type of top symbol */
            TypeS_Push(SymbolS_top->ptr->TypeOf);
            break;

       case 9 : /* Push type int */
            TypeS_Push(IntType);
            break;

       case 10 : /* Push type bool */
            TypeS_Push(BoolType);
            break;

       case 11 : /* Pop type */
            TypeS_Pop();
            break;

       case 12 : /* check that type is int */
            {
            enum Types Type;
/*          Type = TypeS_Check(0); */ Type = (enum Types) TypeS_Check(0); /*AJM*/
            if (Type != IntType)
              {
              fprintf(errorFile, "Line %d:  The expression, variable or array index used in this line\n\tmust be of type 'integer'. \n", line);
              ErrorHandler();
              }
            }
            break;

       case 13 : /* Check that type is bool */
            {
            enum Types Type;
/*          Type = TypeS_Check(0); */ Type = (enum Types) TypeS_Check(0); /*AJM*/
            if (Type != BoolType)
              {
              fprintf(errorFile, "Line %d:  The expression or the variable usedin this line\n\tmust be of type 'boolean'. \n", line);

              ErrorHandler();
              }
            }
            break;


      case 14 : /* Check types for equality comparison */

            /* Check to see if the top two type stack items are equal */
            if ( TypeS_Check(1) != TypeS_Check(0) )
              {
              fprintf(errorFile, "Line %d:  The two expressions being compared must be of the same type.\n", line);
              /* handle error */
              ErrorHandler();
              }
            break;

       case 15 : /* Check types for order comparison */

            /* Check to see if the top two type stack items are of type integer */
            if ((TypeS_Check(1) != IntType) || (TypeS_Check(0) != IntType))
              {
              fprintf(errorFile, "Line %d:  The two expressions being comparared \n\t must both be of type 'integer'.\n", line);

              /* handle error */
              ErrorHandler();
              }
            break;

       case 16 : /* Check type for assignment */
       
            /* Check to see if the top two type stack items are equal */
            if ( TypeS_Check(1) != TypeS_Check(0) )
              {
              fprintf(errorFile, "Line %d:  The expression and the variable it is being assigned to\n\tmust be of same type.\n", line);
              /* handle error */
              ErrorHandler();
              }
            break;

       case 17 : /* Check that variable type is integer */

            if (TypeS_top->type != IntType)
              {
              fprintf (errorFile, "Line %d:  '%s' must be of type 'integer'.\n", line,
              SymbolS_Name());
              /* handle error */
              ErrorHandler();
              }
            break;
            
       /* ------------------ */     
       /* Scalers and Arrays */     
       /* ------------------ */
            
       case 18 : /* Mark symbol table entry as scalar variable */
            {
            STPtr ptr;

            /* Get current identifier's symbol tabel pointer */ 
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);

            /* Change kind to scalar on symbol table */
            ptr->KindOf = scalar;
            ptr->Size = 1;
            }

            break;
       case 19 : /* Mark symbol table entry as array variable. Enter size in symbol table */
            {
            STPtr ptr;

            /* Get current identifier's symbol tabel pointer */ 
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);

            /* Change kind to array on symbol table */
            ptr->KindOf = array;
            
            /* Store array size into symbol table */
            ptr->Size = previousInteger;
                    
            }

            break;

       case 20 : /* Check that symbol is a scalar variable name */
            {
            STPtr ptr;

            /* Get current identifier's symbol table pointer */ 
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);
            /* Check kind of variable on symbol table */
            if (ptr != NULL)
            {
               if (ptr->KindOf != scalar) 
               { 
                 fprintf(errorFile, "Line %d: '%s' must be of scalar kind.\n",
                                   line, ptr->Identifier);
                   /* handle error */
                   ErrorHandler();
               }
            }

            }
            break;

       case 21 : /* Check that symbol is an array variable name */
            {
            STPtr ptr;

            /* Get current identifier's symbol tabel pointer */ 
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);
            /* Check kind of variable on symbol table */
            if (ptr->KindOf != array) {
                fprintf(errorFile, "Line %d: '%s' must be of array kind.\n",
                                line, ptr->Identifier);
              /* handle error */
              ErrorHandler();
            }

            }


            break;

       /* ------------------------ */     
       /* Functions and Procedures */     
       /* ------------------------ */
            
       case 22 : /* Insert lexic level and code address into the symbol table */
            {
            STPtr ptr;

            Push_PrevPtr();


            /* Get current identifier's symbol tabel pointer */
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);

            /* Add lexic level and code address to symbol table */
            ptr->ScopeNum = ScopeLevel;
            /* set code address */
            ptr->CodeAddr =  startMSP;

            Push_FuncName( ptr );

            }   

         break;

       case 23 : /* Check that type of expression matches type of function */
            {
            STPtr ptr;

            /* The top of the symbol table contains the function identifier */
            /* Get current function identifier's symbol tabel pointer */
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);

            /* The expression type is on top of type stack */
            /* If function type does not match expression type print error */
            if (ptr->TypeOf != TypeS_top->type){
              fprintf(errorFile, "Line %d:  The expression being returned from the function '%s'\n\tdoes not match the function type.\n", line, ptr->Identifier);
              /* handle error */
              ErrorHandler();
              }
            }
            break;
       

       case 24 : /* Set type of symbol table entry to procedure */
            {
            STPtr ptr;

            /* Get current identifier's symbol tabel pointer */
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);

            /* Set kind of symbol table entry to procedure */
            ptr->KindOf = procedure;
            }
            
            break;
       
       case 25 : /* Mark symbol table entry as being a parameter */
            {
            STPtr ptr;
            STPtr paramPtr;
            int counter = 0;

            /* Get current identifier's symbol table pointer */
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);

            /* Set kind of symbol table entry to parameter */
            ptr->KindOf = parameter;

            /* Keep track of parameter for the corresp funct/proc */
            /* the funct/proc or the prev param is the second item
             *  on top of stack (top of stack is item 0
             */
            
            paramPtr = (SymbolS_Check(1));
            /* Advance pointer to parameter field of func/proc */
            while (1)
            { 
           
              if (counter >= CountS_top->itemCount)
                break;
              paramPtr = paramPtr->Param;
              counter ++;
            }
            
            /* Assign parameter to function/proc */
            paramPtr->Param = ptr;
            ptr->ParamNumber=counter;
            ptr->Size = 1;
            }
               

         break;
       
       case 26 : /* Set type of symbol table entry to function */
            {
            STPtr ptr;

            /* Get current identifier's symbol tabel pointer */
            ptr = FindToken ( (SymbolS_top->ptr)->Identifier);

            /* Set kind of symbol table entry to function */
            ptr->KindOf = function;
            }
            
            break; 
            

       /* ---------- */     
       /* Parameters */     
       /* ---------- */
            
       case 28 : /* Check that identifier is a procedure name */
            if ( (SymbolS_top->ptr)->KindOf != procedure)
               {
                fprintf (errorFile, "Line %d:  Identifier '%s' is not declared as a procedure.\n", line,
                    SymbolS_Name());
                /* handle error */
                ErrorHandler();
               }
            break;
       case 29 : /* check that function or procedure has no parameters */
            if ( (SymbolS_top->ptr)->NumOfParam != 0)
               {
                fprintf (errorFile, "Line %d:  Function or procedure '%s' is declared with a parameter list.\n", line,
                    SymbolS_Name());
                /* handle error */
                ErrorHandler();
               }
            break;
       case 30 : /* Check that function or procedure has parameters */
            if ( (SymbolS_top->ptr)->NumOfParam == 0)
               {
                fprintf (errorFile, "Line %d:  Call to the Function or procedure '%s' must contain parameters.\n", line,
                    SymbolS_Name());
                /* handle error */
                ErrorHandler();
               }
            break;
       case 31 : /* Check argument type against parameter type */
            {
            STPtr ptr=NULL;
            int counter = 0;
            
            /* find the corresponding parameter */
            
            /* The argument number is on top of count stack */
            if (SymbolS_top)
                ptr = (SymbolS_top->ptr)->Param;
            if (!ptr)
            {
               fprintf (errorFile, "Line %d:  Parameter '%s' in '%s' does not match its argument type.\n", line, ptr->Identifier, SymbolS_Name());
               /* handle error */
               ErrorHandler();      
            
            }
            else
            {
                while (1)
                {
                if ((counter >= CountS_top->itemCount))
                    break;
                ptr = ptr->Param;
                counter ++;
                }
            
                if ( TypeS_top->type != ptr->TypeOf)
                {
                    fprintf (errorFile, "Line %d:  Parameter '%s' in '%s' does not match its argument type.\n", line, ptr->Identifier, SymbolS_Name());
                    /* handle error */
                    ErrorHandler();
               }
            }


          }
            break;
       case 32 : /* Check that all arguments have been seen */
            if ( (SymbolS_top->ptr)->NumOfParam != CountS_top->itemCount)
               {
                fprintf (errorFile, "Line %d:  Wrong number of arguments in '%s'.\n", line,
                    SymbolS_Name());
                /* handle error */
                ErrorHandler();
               }
            break;
       case 33 : /* check that identifier is a function name */
            if ( (SymbolS_top->ptr)->KindOf != function )
               {
                fprintf (errorFile, "Line %d:  Identifier '%s' must be a function.\n", line, SymbolS_Name());
                /* handle error */
                ErrorHandler();
               }
            break;
            break;
       case 34 : /* Enter parameter count into symbol table */
            (SymbolS_top->ptr)->NumOfParam = CountS_top->itemCount;
            PrevPtr = NULL;
            break;
            
       /* -------- */
       /* Counting */     
       /* -------- */
            
       case 35 : /* Push count and initialize to zero */
            /* Initialize order number to zero */
            OrderLevel = 0;

            /* push order number onto count stack */
            CountS_Push(OrderLevel);
            break;
       case 36 : /* Increment count by one */
            CountS_top->itemCount++;
            break;
       case 37 : /* Pop count */
            CountS_Pop();
                    
            break;
            
            
       /* misc */
            
       case 38 : /* Check that symbol is either a scalar variable ( S20 ) */
                 /* or a function with no parameters ( S33 and S29 ) */
            
            if ( (SymbolS_top->ptr)->KindOf == scalar || 
                  (SymbolS_top->ptr)->KindOf == function && (SymbolS_top->ptr)->NumOfParam == 0 )   
                { /* do nothing */ }
            else
                {
                fprintf (errorFile, "Line %d:  '%s' is not declared as a scaler variable or a function with no parameters.\n", line,
                    SymbolS_Name());
                ErrorHandler();
                }
 
            
            break;
            
            
            
       default : fprintf(errorFile, "Semantic Action S%d not defined\n", actionNumber );
            break;
       }            


                     
   return ;

}
