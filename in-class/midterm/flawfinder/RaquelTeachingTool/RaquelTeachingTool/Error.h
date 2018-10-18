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
#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H
//-----------------------------------------------------
// Includes
//-----------------------------------------------------
#include <vector>
#include <fstream>
#include <map>

//--------------------------
// Error List Typedef
//--------------------------
typedef std::vector< int >			ErrorListNumeric;
typedef std::map<int, std::string>	ErrorDefs;
typedef std::vector<std::pair<int, std::string> > ErrorList;
typedef std::map<int, ErrorList>	ModuleErrors;

enum LANGUAGE_REGION {NONE = -1, UK = 0, FR, NL};

class CErrorManager
{
//--------------------------
// Protected Members
//--------------------------
protected:

	// Numeric list of Errors
	ErrorListNumeric 	m_NumericErrors;

	// Language / Region
	LANGUAGE_REGION		m_CurrentLanguage;

	// Error Definitions
	ModuleErrors			m_ErrorMap;

	// Error List (Number and Langauge String)
	ErrorList			m_Errors;


//--------------------------
// Public Operations
//--------------------------
public:
	
	// Constructor
	CErrorManager();

	// Constructor
	CErrorManager(LANGUAGE_REGION Language);

	// Destructor
	~CErrorManager();

	// Initialise Errors into Overall Error module
	bool InitialiseModule(std::string szFileName, const unsigned int &iStartPoint);

	// Reset Errors
	void Reset();

	// Find Error via Code
	std::string GetNLErrorFromCode(const unsigned int &uiModuleID, const unsigned int &uiErrorID);

	// Set Language
	inline void SetLanguage(LANGUAGE_REGION Language) {m_CurrentLanguage = Language;}

	// Get Language
	inline LANGUAGE_REGION GetLanguage()			   {return m_CurrentLanguage;}

	// Clear Errors
	inline void ClearErrors() {m_Errors.clear();}

	// Return size of current Error Vector
	inline int GetNumberOfErrors()	{return static_cast<int>( m_Errors.size() );}

	// return Error List
	inline ErrorList GetErrorList()	{return m_Errors;}

	// Generate Module ID from error Code
	inline int GetModuleCode(unsigned int uiError)	{return (uiError & 0xffff0000) >> 16;}

	// Generate Error ID from error Code
	inline int GetErrorCode(unsigned int uiError)	{return (uiError & 0x0000ffff);}

//--------------------------
// Protected Operations
//--------------------------
public:

	// Generate language specific filename 
	std::string	GenerateLanguageFileName(std::string &szFileName);

	// Read File Line
	bool ReadLine( std::ifstream &ifp, std::string &szLine );

};

#endif