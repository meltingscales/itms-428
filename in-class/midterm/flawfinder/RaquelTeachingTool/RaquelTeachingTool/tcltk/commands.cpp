/***************************************************************************
                            commands.cpp
                            ------------
    Description         : Implementation of the commands used by the GUI 
                          to execute statements and retrieve errors from
                          the DBMS.
    Duration            : Feb 2006 - May 2006
    Author              : Guillaume Renard, Universite de Bourgogne, France
                                            University of Northumbria, UK

 ***************************************************************************/
#include "tcltk/commands.h"
#include "tcltk/inputStackErrors.h"

/************************************************************
 * DECLARATIONS OF DATA TYPES
 ************************************************************/
/*
 * Defines the modules informations.
 * Each module is identified by a module token, a name
 */
typedef struct
{
  T_MOD_IDENTIFIER   mod_id;
  char*       name;
  char*       dir;
} Module;

/*
 * Defines a module list datatype
 */
typedef list<Module> Modules;


/************************************************************
 * DECLARATIONS OF VARIABLES
 ************************************************************/
/*
 * Declares the module list to be used by the program to
 * retrieve module information
 */
Modules modules;
/*
 * Declares the variable used to store the results of the last
 * statement execution.
 */
char* lastResults = new char[MAX_SIZE];
/*
 * Declares the variable to store the language for the error
 * messages.
 */
char* lang = new char[3];

/************************************************************
 * DECLARATIONS OF PROCEDURES
 ***********************************************************/
/*
 * Initialises the variables used by the program.
 * 'errorPath' is the root path (witheout final slash '/')
 * where the modules' error directories are located. 
 */
void CmdInit(char* errorPath)
{
  /* Initialises the module list */
  Module tokeniser, compactor, parser;

  tokeniser.mod_id = TOK;
  tokeniser.name    = "Tokeniser";
  tokeniser.dir = new char[256];
  sprintf(tokeniser.dir, "%s%s", errorPath, "/tokeniser/");
  modules.push_back(tokeniser);

  compactor.mod_id = COM;
  compactor.name    = "Compactor";
  compactor.dir = new char[256];
  sprintf(compactor.dir, "%s%s", errorPath, "/compactor/");
  modules.push_back(compactor);

  parser.mod_id = PAR;
  parser.name    = "Parser";
  parser.dir = new char[256];
  sprintf(parser.dir, "%s%s", errorPath, "/parser/");
  modules.push_back(parser);
  
  /* Sets the default language to english */
  strncpy(lang, LANGUAGE, 2);
}

/*
 * Formats error information from a module + an error struct
 * + an error message (desc) to an human-readable message
 * and stores the result string at the address of the 'result'
 * parameter.
 *
 * Modify this function to display the message in your own
 * way.
 */
void formatError(Module module, Error error, char* desc, char** result)
{
  strcpy(*result, module.name);
  
  if ( strcmp(lang, "fr") == 0 ) strcat(*result, " Erreur:");
  else                           strcat(*result, " Error:"); 
  strcat(*result, desc);
  if (strlen(error.parameter) > 0)
  {
    strcat(*result, "(");
    strcat(*result, error.parameter);
    strcat(*result, ")\n");
  }
}

/*
 * Returns the error number contained into the string 'buffer'
 * and extracts the following error message to the address
 * of the 'result' parameter.
 *
 * (Note: The buffer parameter should be a single line from a
 *        module error file)
 */
int extractError(char* buffer, char** result)
{
  return strtol(buffer, result, 10);
}

/*
 * Extracts the description of an error that occured into
 * a specified module and stores it as a string of 
 * characters at the address of the 'desc' parameter.
 *
 * (Note : The description is extracted from the error file
 *         specified into the module structure and regarding
 *         the specified language)
 */
void getErrorDesc(Module module, Error error, char** desc)
{
  char fname[MAX_SIZE];
  strcpy(fname, module.dir);
  strcat(fname, lang);
  FILE* file;
  
  if ( (file = fopen(fname, "rt")) == NULL )
  {
    strcpy(*desc, " [unexpected error] cannot retrieve error information (file missing)\n");
    return;
  }
  
  int  err_num;
  bool found = false;
  char buffer[MAX_SIZE];
  while ( (!found) && (fgets( buffer, sizeof(buffer), file) != NULL) )
  {
    err_num = extractError(buffer, desc);
    if (err_num == error.number) found = true;
  }
  fclose(file);
    
  if (!found) strcpy(*desc, " [unexpected error] unknown error\n"); 
}

/*
 * Returns the module represented by its token into the
 * specified error data.
 */
Module getErrorSource(Error error)
{
  bool found = false;
  Module module;
  Modules::reverse_iterator i = modules.rbegin();
  while ( (!found) && (i != modules.rend()) )
  {
    module = *i;
    if (module.mod_id == error.mod_id) found = true;
    ++i;
  }
  return module;
}

/*
 * Extracts the information of the specified error data and
 * stores it as a string of characters at the address of the
 * 'result' parameter.
 * Information contains a module name and an error message.
 */
void getError(Error error, char** result)
{
  Module module = getErrorSource(error);
  char* desc = new char[MAX_SIZE];
  getErrorDesc(module, error, &desc);
  formatError(module, error, desc, result);
  delete desc;
}

/*
 * Converts an error list structure into a string of characters
 * and stores it at the address of the 'results' parameter.
 * Each error message is put into a new line.
 */
void getErrors(Errors errors, char** results)
{
  strcpy(*results, "");
  /* Browses the errors' list */
  char* buffer = new char[MAX_SIZE];
  for (Errors::reverse_iterator it = errors.rbegin();
       it != errors.rend();
       it++)
  {
    Error e = *it;
    getError(e, &buffer);
    strncat(*results, buffer, (MAX_SIZE - strlen(*results) - 1) );
  }
  delete buffer;
}

/*
 * Sends a statement (from the GUI) to the DBMS
 * and returns errors from its execution.
 */
Errors sendToDBMS(const char* input)
{
    std::cout << "in sendToDBMS in commands.cpp\n";
  Errors errors;
  int input_fifo, errors_fifo;
  
  /* Opens the input pipe to send the statement to the DBMS */
  if ( (input_fifo = open(CLIENT_TO_DBMS, O_WRONLY)) == -1 )
  {
    /* If the pipe cannot be open, returns an error to the GUI */
    strcpy(lastResults, "No Result, the DBMS did not answered.\n");
    strcat(lastResults, "(CLIENT_TO_DBMS Pipe broken)");
  }
  else
  {
    /* Writes the statement into the input pipe */
    write(input_fifo, input, strlen(input)+1);
    close(input_fifo);
    /* Gets the errors from the DBMS */
    if ( (errors_fifo = open(DBMS_TO_CLIENT_ERROR, O_RDONLY)) == -1 )
    {
      /* If the errors' pipe cannot be open, returns an error to the GUI */
      strcpy(lastResults, "No Result, the DBMS did not answered.\n");
      strcat(lastResults, "(DBMS_TO_CLIENT_ERROR Pipe broken)");
    }
    else
    {
      /* Reads the errors one-by-one from the DBMS and adds them to the list */
      Error e;
      while ( read(errors_fifo, (char*) &e, sizeof(e)) >0 )
        errors.push_back(e);
    }
  }
  return errors;
}

/*
 * Tcl/Tk execute command.
 * Executes an user statement (from the GUI) by sending it to
 * the DBMS.
 * Returns a character string containing the DBMS errors.
 */
int executeCmd(ClientData clientData, Tcl_Interp *interp,
               int argc, char *argv[])
{
  if (argc > 2)
  {
    strcpy(interp->result, "Usage: !errors! exec ?statement?");
    return TCL_ERROR;
  }
  if (argc == 2)
  {
    Errors errors = sendToDBMS(argv[1]);
    if ( errors.empty() )
    {
      strcpy(interp->result, "");
    }
    else
    {
      char* str_errors = new char[MAX_SIZE];
      getErrors(errors, &str_errors);
      strncpy(interp->result, str_errors, MAX_SIZE);
      delete str_errors;
    }
  }
  return TCL_OK;
}

/*
 * Tcl/Tk exec_results command.
 * Returns the results from the previous statement execution.
 *
 * Not yet implemented.
 */
int exec_resultsCmd(ClientData clientData, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc > 1)
  {
    strcpy(interp->result, "Usage: ?output? exec_results");
    return TCL_ERROR;
  }
  if (argc == 1)
  {
    strncpy(interp->result, lastResults, MAX_SIZE);
  }
  return TCL_OK;
}

/*
 * Tcl/Tk select_lang command.
 * Select the lang to display for the error messages.
 */
int select_langCmd(ClientData clientData, Tcl_Interp *interp,
                    int argc, char *argv[])
{
  if (argc > 2)
  {
    strcpy(interp->result, "Usage: select_lang ?lang? (two characters)");
    return TCL_ERROR;
  }
  if (argc == 2)
  {
    strncpy(lang, argv[1], 2);
  }
  return TCL_OK;
}
