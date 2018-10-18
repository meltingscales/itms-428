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

// TokeniserTest.cpp : Defines the entry point for the console application.
//

//-----------------------------------------------------
// Preprocessor
//-----------------------------------------------------
#ifdef WIN32
	#include "stdafx.h"
#endif
//-----------------------------------------------------
// Includes
//-----------------------------------------------------
#include <iostream>

#include "TclTkInterface.h"


int main(int argc, char* argv[])
{
	//Version 1.3 - Richard Ockerby 3/11/2009
	//Version 1.31 - Richard Ockerby 8/11/2009
	//Version 1.32 - Richard Ockerby 10/11/2009
	//Version 1.33 - Richard Ockerby 15/11/2009
	std::cout << "\n\n--------------------------\n";
	std::cout << "Raquel Test Linux Ver 1.33\n";
	std::cout << "--------------------------\n\n";

	// Initialise Raquel DBMS 
	std::cout << "Initialising Raquel DBMS...\n\n";

	// Initialise Interface
	CTclTkInterface					*m_pInputStack = new CTclTkInterface;
	
	// Create TclTK Interface 
	std::cout << "Initiating TclTk Interface...\n\n";
	m_pInputStack->InitiliaseTclTk(argc, argv);

	// Process Query 
	//std::string						szQuery = "S <--Retrieve  Part Join[ SNo ] Supplier Restrict[ Price < 9]";
	//ErrorList Errors 			= 	m_RaquelDBMS.ProcessTextQuery(szQuery);

	// Get Storage Interface
	//CStorageStack *pStorageIF 	=	CRaquelDBMS::Get()->GetExternalInterface()->GetStorageStack( "s" );
	
/*
	// Debug Out Lex Tokens
	LexTokenVector vLexResult = m_pInputStack->GetLexTokens();
	std::cout << "Size of Lex Tokens = " << vLexResult.size() << std::endl;
	for(int c = 0; c < vLexResult.size(); c++)
	{
		std::cout << c << " " << vLexResult[c].szToken << std::endl;	
	}
	
	// Get Comapction tokens
	//std::vector< CRaquelToken * >& GetCompactorTokens()
	std::vector< CRaquelToken * > vCompactorTokens = m_pInputStack->GetCompactorTokens();
	std::cout << "Size of Compactor Tokens = " << vCompactorTokens.size() << std::endl;
	for(int c = 0; c < vCompactorTokens.size(); c++)
	{
		std::cout << c << " " << vCompactorTokens[c]->GetTokenTypeAsString() << std::endl;	
	}	

	std::cout << "Number of Resulting Tuples = " << pStorageIF->GetNumTuples() << std::endl;
	std::cout << "Size of Errors = " << Errors.size() << std::endl;
*/

	return 0;
}
