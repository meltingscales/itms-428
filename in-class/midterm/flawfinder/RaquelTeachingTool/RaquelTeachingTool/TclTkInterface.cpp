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

//-----------------------------------------------------
// Preprocessor
//-----------------------------------------------------

//-----------------------------------------------------
// Includes
//-----------------------------------------------------
#include "TclTkInterface.h"

CTclTkInterface* CTclTkInterface::m_pInstance = 0;
CErrorManager CTclTkInterface::m_ErrorManager; 
CSink* CTclTkInterface::m_pGuiSink = 0;
//--------------------------
// Contructor
//--------------------------
CTclTkInterface::CTclTkInterface()	:	m_pTreeRoot( 0 )
{
	if(m_pInstance != NULL)
		std::cout << "\nStatic Instance alread Set\n";
	else
		m_pInstance = this;
}


//--------------------------
// Destructor
//--------------------------
CTclTkInterface::~CTclTkInterface()
{
	ClearQuery();
}
int CTclTkInterface::maxAttrWidth = 50;
// Initalise TclTk
int	CTclTkInterface::InitiliaseTclTk(int argc, char* argv[])
{
	// Initialise Input Stack
	m_pInputStack = new CInputStack;
	// Initialise Raquel
	if( !m_RaquelDBMS.Initialise( m_pInputStack ))
		return TCL_ERROR;
	// Register Extension Tokens
	RegisterRaquelModules();	

	// Initialise Error Manager
	m_ErrorManager.SetLanguage(UK);
	// Initialise Error manager with Natural Language errors
	m_ErrorManager.InitialiseModule("RaquelErrors/General", eMODULE_DBMS);
	m_ErrorManager.InitialiseModule("RaquelErrors/InputStackTokeniser", eMODULE_TOKENISER);
	m_ErrorManager.InitialiseModule("RaquelErrors/InputStackCompactor", eMODULE_COMPACTOR);
	m_ErrorManager.InitialiseModule("RaquelErrors/InputStackParser", eMODULE_PARSER);
	m_ErrorManager.InitialiseModule("RaquelErrors/ExecutionStack", eMODULE_EXECUTION);
	m_ErrorManager.InitialiseModule("RaquelErrors/StorageStack", eMODULE_STORAGE);
	// Initialise Sink
	m_pGuiSink = new CLocalSink;
	m_pGuiSink->SetSinkName( "s" );
	// Add Sink
	CRaquelDBMS::Get()->GetSinkHandler()->AddSink( m_pGuiSink );
	// Create TclTK Interpreter 
	interp = Tcl_CreateInterp();
	// Load Main linix 
	Tk_Main(argc, argv, Tk_AppInit );

	return TCL_OK;
}

//--------------------------
// Register Extensions
//--------------------------
void CTclTkInterface::RegisterExtensions()
{
}

//--------------------------
// Clear Query
//--------------------------
void CTclTkInterface::ClearQuery()
{
	m_QueryLexTokens.clear();
	m_QueryCompactorTokens.clear();
	m_QueryParserTokens.clear();
	m_pTreeRoot = 0;
}

//---------------------------
// Initialise TclTk Interface
//---------------------------
int Tk_AppInit(Tcl_Interp *interp)
{
	// Initialises packages 
  	if (Tcl_Init(interp) == TCL_ERROR)
 	{
    	return TCL_ERROR;
  	}
  	if (Tk_Init(interp) == TCL_ERROR)
  	{
    	return TCL_ERROR;
  	}

	// Registers application-specific commands. 
	Tcl_CreateCommand(interp, "exec", (Tcl_CmdProc *)&CTclTkInterface::executeCmd,
		(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
	Tcl_CreateCommand(interp, "exec_results", (Tcl_CmdProc *)&CTclTkInterface::exec_resultsCmd,
		(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
	Tcl_CreateCommand(interp, "select_lang", (Tcl_CmdProc *)&CTclTkInterface::select_langCmd, //declared 
		(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
	Tcl_CreateCommand(interp, "set_attrWidth", (Tcl_CmdProc *)&CTclTkInterface::set_attrWidthCmd, //declared 
		0, 0);
		
	// Defines start-up filename. 
	Tcl_SetVar(interp, "tcl_rcFileName", GUI, TCL_GLOBAL_ONLY);
	// Evaluate TclTk File 
	if(Tcl_EvalFile(interp,GUI) != TCL_OK)
	{
		std::cout << "TCL File Init Error...\n\n";
		return TCL_ERROR;
	}

	// return OK 
  	return TCL_OK;
}

//---------------------------
// Execute Command from TclTK
//---------------------------
int CTclTkInterface::executeCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
	// Clear return Buffer 
	Tcl_ResetResult(interp);
	// Clean up DBMS
	CRaquelDBMS::Get()->CleanUp();

	// Debug out to console where we are
	std::cout << "\nDebug Interface - executeCmd\n";

	if (argc > 2)
	{
		Tcl_AppendResult(interp, "Usage: !errors! exec ?statement?", (char *) NULL);
		return TCL_ERROR;
	}
	if (argc == 2)
	{
		std::vector<int> errors = m_pInstance->sendToDBMS(argv[1]);

		if( errors.size() != 0 )
		{
			// List Errors
			char szErrorCount[256];
			int iSizeOfErrors = errors.size();
			sprintf(szErrorCount, "Error Size %i\n", iSizeOfErrors);
			Tcl_AppendResult(interp, szErrorCount, (char *) NULL);	
		
			// Iterate over errors 
			for(unsigned int i = 0; i < errors.size(); i++ )
			{
				// Dtermine Module code
				unsigned int uiModuleID = m_ErrorManager.GetModuleCode(errors[i]);
				unsigned int uiErrorID  = m_ErrorManager.GetErrorCode(errors[i]);
				// Determine Natutral language error
				char szError[256];
	
				std::string szNLError =  m_ErrorManager.GetNLErrorFromCode(uiModuleID, uiErrorID);
				sprintf(szError, "Error [%i] [%i] %s", uiModuleID, uiErrorID, szNLError.c_str() );
				Tcl_AppendResult(interp, szError, (char *) NULL);		
			}
			CRaquelDBMS::Get()->CleanUp();
		}
		else
		{
			std::cout << "\nCommand Ok...\n";
			Tcl_AppendResult(interp, "", (char *) NULL);
		}
	}
	return TCL_OK;
}

//---------------------------------------------
// Execute Results command from TclTK Interface
//---------------------------------------------
int CTclTkInterface::exec_resultsCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
	// Vars
	CTuple SDesc;
	CTuple Tuple;
	CTable *pSTable = 0;
	// Clear return Buffer 
	Tcl_ResetResult(interp);
	// Buffer to format results in
	char sBuff[512];
	// Debug Out to console where we are in execution
	std::cout << "\nDebug Interface - exec_resultCmd\n";
	// Get Relation
	pSTable = m_pGuiSink->GetLocalRelation();

	// Check Sink
	if( pSTable->GetNumTuples() > 0 )
	{
		pSTable->GetTupleByIndex( 0, Tuple );
		/* this method of dynamic attribute length needs looked at
			when the new external interface is designed */
		int maxLength[Tuple.GetNumAttributes()];
		for( int i = 0; i < Tuple.GetNumAttributes(); i++ )
			maxLength[i] = 0;
		for( int i = 0; i < Tuple.GetNumAttributes(); i++ )
		{
			std::string hdr = Tuple.GetName( i ).c_str();
			if(hdr.size() > maxAttrWidth){
				maxLength[i] = maxAttrWidth;
			} else if( hdr.size() > maxLength[i] ){
				maxLength[i] = hdr.size();
			}
		}
		for( int i = 0; i < pSTable->GetNumTuples(); i++ )
		{
			// Get Tuple
			pSTable->GetTupleByIndex( i, Tuple );
			// Get Item Name
			std::string attr;// = *Tuple.GetAttribute( 0 );
			// Insert Tuple
			for( int j = 0; j < Tuple.GetNumAttributes(); j++ )
			{
				// Get Item Name
				attr = *Tuple.GetAttribute( j );
                //Truncate the column to fit the width
                if(attr.size() > maxAttrWidth){
					maxLength[j] = maxAttrWidth;
				} else if (attr.size() > maxLength[j]){
                    maxLength[j] = attr.size();
                }
			}
		}


		// Get Tuple to describe columns
		pSTable->GetTupleByIndex( 0, Tuple );
		// Set Column
		std::string szColoumns;
		// Add Other Columns
		for( int i = 0; i < Tuple.GetNumAttributes(); i++ )
		{
			// Get Header
			std::string  szValue = Tuple.GetName( i ).c_str();
			//Columns with a width of 'colWidth' (default 15)
            if(szValue.size() > maxAttrWidth){
                szValue.resize(maxAttrWidth);
            } else {
                szValue.resize(maxLength[i], ' ');
            }
			// Add to columns string
			szColoumns += szValue + " ";
		}
        //Add a divider post column headers
        szColoumns += "\n";
        for( int i = 0; i < Tuple.GetNumAttributes(); i++) {
           	for(int j=0; j < maxLength[i]; j++)
            	szColoumns += "-";
			szColoumns += " ";
        }
        //szColoumns += "\n";
		// Display Coloumn String
		sprintf(sBuff, "%s\n", szColoumns.c_str() );	
		std::cout << sBuff;
		Tcl_AppendResult(interp, sBuff, (char *) NULL);
		
		//---------------
		// Add Data
		//---------------
		for( int i = 0; i < pSTable->GetNumTuples(); i++ )
		{
			// Get Tuple
			pSTable->GetTupleByIndex( i, Tuple );
			// Get Item Name
			std::string szItem = *Tuple.GetAttribute( 0 );
			// Insert Item
			std::string	szData;
			// Insert Tuple
			for( int j = 0; j < Tuple.GetNumAttributes(); j++ )
			{
				// Get Item Name
				szItem = *Tuple.GetAttribute( j );
                //Truncate the column to fit the width
                if (szItem.size() > maxAttrWidth){
                    szItem.resize(maxAttrWidth);
                } else {
                    szItem.resize(maxLength[j], ' ');
                }
				// Add To data
				szData += szItem + " ";
			}
			// Display in Tcl
			sprintf(sBuff, "%s\n", szData.c_str() );
			Tcl_AppendResult(interp, sBuff, (char *) NULL);	
		}
		szColoumns = "";
        for( int i = 0; i < Tuple.GetNumAttributes(); i++) {
           	for(int j=0; j < maxLength[i]; j++)
            	szColoumns += "-";
			szColoumns += " ";
        }
		sprintf(sBuff, "%s", szColoumns.c_str() );	
		std::cout << sBuff;
		Tcl_AppendResult(interp, sBuff, (char *) NULL);
	}
	return TCL_OK;
}
int CTclTkInterface::set_attrWidthCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    // Clear return Buffer 
	Tcl_ResetResult(interp);
    int numIn = 0;
    sscanf(argv[1], "%d", &numIn);
	//Convert to a number and set as the new width
    if(numIn <= 0) {
        //error - no number (or 0) contained in the input
        //std::cout << "set_colWidthCmd -- Not a number\n";
        Tcl_AppendResult(interp, "0", (char *) NULL);
        return TCL_OK;
    } else {
        maxAttrWidth = numIn;
        //std::cout << " set_colWidthCmd -- Got number: " << colWidth << "\n";
       
        char ibuff[24]; // or what else, or use outer char array
        sprintf(ibuff,"%d",numIn); // use a proper format: "%04d"...
        

        Tcl_AppendResult(interp, ibuff, (char *) NULL);
        return TCL_OK;
    }

}

int CTclTkInterface::select_langCmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
	std::cout << "\nDebug Interface - select_langCmd\n";
/*
	if (argc > 2)
	{
		strcpy(interp->result, "Usage: select_lang ?lang? (two characters)");
		return TCL_ERROR;
	}
	if (argc == 2)
	{
		strncpy(lang, argv[1], 2);
	}
*/
	return TCL_OK;
}

//---------------------------------------------
// Send Initial Query String to DBMS
//---------------------------------------------
std::vector<int> CTclTkInterface::sendToDBMS(const char *szQuery)
{
	std::vector<int> errors;

	std::cout << "\nDebug Interface - Executing DBMS Command\n";

	std::cout << szQuery << std::endl;

	errors = m_RaquelDBMS.ProcessTextQuery(szQuery);
	
	return errors;
}

//---------------------------------------------
// Send Initial Query String to DBMS
//---------------------------------------------
void	CTclTkInterface::RegisterRaquelModules()
{
}

