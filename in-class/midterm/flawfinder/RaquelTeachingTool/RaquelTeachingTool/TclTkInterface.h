/***************************************************************************
 *   Copyright (C) $YEAR$ by $AUTHOR$   *
 *   $EMAIL$   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//-----------------------------------------------------------------
// Preprocessor
//-----------------------------------------------------------------
#ifndef TCLTKINTERFACE_H
#define TCLTKINTERFACE_H
//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "lib/tk.h"
#include "lib/tcl.h"
#include <string>
#include "InputStack.h"
#include "RaquelDBMS.h"
#include "TabDelimitedStack.h"
#include "LocalSink.h"
#include "Error.h"
//-----------------------------------------------------------------
// Types
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GUI "tcltk/Main.tk"

// Declaration of the External C Tk functions
extern "C" int Tk_AppInit(Tcl_Interp *interp);

//-----------------------------------------------------------------
// Input Stack Class
//-----------------------------------------------------------------
class CTclTkInterface : public CInputStack
{
	
private:
	static int maxAttrWidth;
	
//--------------------------
// Protected Members
//--------------------------
protected:
	// Static instance of TclTkInterface
	static CTclTkInterface 				*m_pInstance;

	// Output Vars For Dialog
	LexTokenVector					m_QueryLexTokens;
	std::vector< CRaquelToken * >			m_QueryCompactorTokens;
	std::vector< CRaquelToken * >			m_QueryParserTokens;
	std::vector<int>				m_QueryErrors;
	CRaquelToken					*m_pTreeRoot;
	Tcl_Interp 					*interp;

	std::vector< CRaquelToken * >			m_TokenList;

	// Raquel DBMS System
	CRaquelDBMS					m_RaquelDBMS;
	// Input Stack 
	CInputStack					*m_pInputStack;
	// Gui Sink
	static CSink					*m_pGuiSink;
	// Error Manager 
	static CErrorManager				m_ErrorManager;
	

//--------------------------
// Public Operations
//--------------------------
public:
	// Constructor
	CTclTkInterface();

	// Destructor
	~CTclTkInterface();

	// Get Static Interface Pointer
	static CTclTkInterface*	Get()		{return m_pInstance;}

	// Initalise TclTk
	int	InitiliaseTclTk(int argc, char* argv[]);

	// Register Raquel Extensions
	void RegisterExtensions();

	// Clear Query
	void ClearQuery();

	// Get Lex Tokens
	LexTokenVector& GetLexTokens() { return m_QueryLexTokens; }

	// Get Compactor Tokens
	std::vector< CRaquelToken * >& GetCompactorTokens() { return m_QueryCompactorTokens; }

	// Get Parser Tokens
	std::vector< CRaquelToken * >& GetParserTokens() { return m_QueryParserTokens; }

	// Get Errors
	std::vector<int>& GetErrors() { return m_QueryErrors; }

	// Get Lex Tokens
	CRaquelToken* GetTreeRoot() { return m_pTreeRoot; }

	

//--------------------------
// Tcl Tk Public Operation
//--------------------------
public:
	
	// Execute Raqel Command
	static int executeCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

	// Get Results
	static int exec_resultsCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

	// Set Language
	static int select_langCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

	//Set the width of each column for the results pane
	static int set_attrWidthCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
	
//--------------------
// Protected Operation
//--------------------
protected:
	std::vector<int> sendToDBMS(const char *szQuery);

	void			 RegisterRaquelModules();

	
};
#endif
