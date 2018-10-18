/***************************************************************************
                            commands.h
                            ----------
    Description         : Defines the commands used by the GUI to execute
                          statements and retrieve errors from the DBMS.
    Duration            : Feb 2006 - May 2006
    Author              : Guillaume Renard, Universite de Bourgogne, France
                                            University of Northumbria, UK

 ***************************************************************************/
#include <iostream>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <tcl.h>
#include <tk.h>

#include "tcltk/inputStackErrors.h" /* Defines the Input Stack's error structure */
#include "tcltk/fifo.h"             /* Definitions for the pipe communication */

/*
 * Defines the program's constants.
 *    MAX_SIZE is the maximum number of characters that can be
 *             returned to the GUI.
 *    LANGUAGE is the language used to display error messages.
 *             It is used to retrieve the error files which are named
 *             with common language abbreviation (eg: 'en')
 *             (should not be stored as a constant but set from
 *             the GUI - not yet implemented)
 */
#define MAX_SIZE 200
#define LANGUAGE "en"

using namespace std;

/*
 * Initialises the variables used by the program.
 * 'errorPath' is the root path (witheout final slash '/')
 * where the modules' error directories are located. 
 */
void CmdInit(char* errorPath);

/*
 * Tcl/Tk execute command.
 * Executes a user statement (from the GUI) by sending it to
 * the Input Stack.
 * Returns a character string containing the Input Stack errors.
 */
int executeCmd(ClientData clientData, Tcl_Interp *interp,
               int argc, char *argv[]);

/*
 * Tcl/Tk exec_results command.
 * Returns the results from the previous statement execution.
 *
 * Not yet implemented.
 */
int exec_resultsCmd(ClientData clientData, Tcl_Interp *interp,
                    int argc, char *argv[]);

/*
 * Tcl/Tk select_lang command.
 * Select the lang to display for the error messages.
 */
int select_langCmd(ClientData clientData, Tcl_Interp *interp,
                    int argc, char *argv[]);
