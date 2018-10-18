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
// Top Level Include ( Precompiled Header )
//#include "StdAfx.h"
#include "TeachingTool.h"

#include "Error.h"

//--------------------------
// Contructor
//--------------------------
CErrorManager::CErrorManager()
{
	m_CurrentLanguage = NONE;
	m_ErrorMap.clear();
}

//--------------------------
// Contructor
//--------------------------
CErrorManager::CErrorManager(LANGUAGE_REGION Language)
{
	m_CurrentLanguage = Language;
	m_ErrorMap.clear();
}

//--------------------------
// Destructor
//--------------------------
CErrorManager::~CErrorManager()
{
}


//--------------------------
// Initialise Module Errors 
//--------------------------
bool CErrorManager::InitialiseModule(std::string szFileName, const unsigned int &uiModuleID)
{
	// create language specific help file name 
	std::string szLangFileName = GenerateLanguageFileName(szFileName);

	// Create file stream in 
	std::ifstream ifp;	
	ifp.open(szLangFileName.c_str(), std::ios::in);

	// Close if fails
	if(!ifp)
	{
		// Close Stream
		ifp.close();
		// return false
		return false;
	}

	// Init vars
	std::string szErrorTxt;
	int			iCnt = 1;

	// See if current module errors exist
	ModuleErrors::iterator sPos;
	sPos = m_ErrorMap.find(uiModuleID);

	// Check insert on map by defining return type 
	std::pair<ModuleErrors::iterator,bool> ret;

	// Check to see if we are trying to add errors to an existing module
	if(sPos != m_ErrorMap.end())
	{
		return false;
	}
	else		// Not found - insert new module code
	{
		ErrorList	tempList;
		ret = m_ErrorMap.insert(ModuleErrors::value_type(uiModuleID, tempList));
	}

	if(ret.second == true)
		sPos = ret.first;
	else
		return false;

	// Read errors
	while( !ifp.eof() )
	{
		// Read Error Text from File 
		szErrorTxt.clear();
		ReadLine(ifp, szErrorTxt);

		if(szErrorTxt.length() > 0)
		{
			// insert into local error map
			sPos->second.push_back(ErrorDefs::value_type(iCnt, szErrorTxt));
		}

		// increment error
		iCnt++;
	}
	// Ensure we close file 
	ifp.close();
	// return true
	return true;
}

//------
// Reset
//------
void CErrorManager::Reset()
{
	// Reset Errors
	m_Errors.clear();
	m_NumericErrors.clear();
}

//---------------------------------------
// Get String Error in language from Code
//---------------------------------------
std::string CErrorManager::GetNLErrorFromCode(const unsigned int &uiModuleID, const unsigned int &uiErrorID)
{
	// Find Module Code
	// See if current module errors exist
	ModuleErrors::iterator sPos;
	sPos = m_ErrorMap.find(uiModuleID);

	// Module Found
	if(sPos != m_ErrorMap.end())
	{
		// Get Zero Indexed Error ID
		unsigned int uiErrorZI = uiErrorID - 1;
		// Check error is in a valid range
		if( uiErrorZI < sPos->second.size())
		{
			ErrorList errorList = sPos->second;
			return errorList[uiErrorZI].second;
		}
		else
		{
			return "ErrorManager->GetNLErrorFromCode - error";
		}
	}
	else
	{
		return "ErrorManager->GetNLErrorFromCode - error";
	}

}


//----------------------------------
// Generate Language Specific Errors
//----------------------------------
std::string	CErrorManager::GenerateLanguageFileName(std::string &szFileName)
{
	// Create initial File Name 
	std::string szLanFileName = szFileName;

	// Find Current Language 
	switch(m_CurrentLanguage)
	{
		case FR:
		{
			szLanFileName += ".fr";
			break;
		}

		case NL:
		{
			szLanFileName += ".nl";
			break;
		}
		case UK:
		default:			// Default to UK if no other langugae file found + display error
		{
			szLanFileName += ".uk";
			break;
		}
	};

	return szLanFileName;
}

//--------------------------
// Read Line
//--------------------------
bool CErrorManager::ReadLine( std::ifstream &ifp, std::string &szLine )
{
	// Vars
	char szBuffer[ 512 ];
	// Nullify Buffer
	szBuffer[ 0 ] = 0;
	// Read Line
	ifp.getline( szBuffer, 512 );
	// Copy String
	szLine = szBuffer;
	// Check Length
	if( szLine.size() < 1 )
		return false;
	// Remove Trailing /r on Linux
	#ifndef WIN32
	szLine.resize( szLine.size() - 1 );
	#endif
	// Return
	return true;
}
