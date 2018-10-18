/***************************************************************************
                            inputStackErrors.h
                            ------------------
    Description         : Defines a generalised error structure for the
                          Input Stack & its modules.
    Duration            : Feb 2006 - May 2006
    Author              : Guillaume Renard, Universite de Bourgogne, France
                                            University of Northumbria, UK

 ***************************************************************************/
#ifndef _INPUT_STACK_ERRORS
  #define _INPUT_STACK_ERRORS

#include <list>
#include <string>

using namespace std;

/*
 * Defines the tokens that identifies the DBMS modules
 */
typedef enum { TOK, COM, PAR } T_MOD_IDENTIFIER;

/*
 * Defines the structure used to wrap error numbers.
 * Each error is identified by a source module token and
 * a number. 'Parameter' is a string that contains additional
 * information to display (where available).
 */
typedef struct
{
  T_MOD_IDENTIFIER  mod_id;
  int        number;
  char       parameter[256];
} Error;

/*
 * Defines an error list datatype
 */
typedef list<Error>  Errors;

#endif
