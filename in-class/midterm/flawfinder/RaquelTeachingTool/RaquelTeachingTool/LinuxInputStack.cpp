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
#include "LinuxInputStack.h"
//--------------------------
// Contructor
//--------------------------
CLinuxInputStack::CLinuxInputStack()	:	m_pTreeRoot( 0 )
{
}

//--------------------------
// Destructor
//--------------------------
CLinuxInputStack::~CLinuxInputStack()
{
	ClearQuery();
}

//--------------------------
// Tokenise
//--------------------------
CRaquelToken* CLinuxInputStack::CreateTree( const std::string &szQuery, std::vector<int> *Errors )
{

	std::cout << "\nEntry Create Tree - Linux Input Stack\n";

	// Vars
	LexTokenVector LexTokens;
	std::vector< CRaquelToken * > RaquelTokens;
	CRaquelToken *pTreeRoot = 0, *pToken = 0;
	// Reset Error Manager
	m_pErrors = Errors;

	// Invoke Tokeniser
	TokeniseQuery( szQuery, LexTokens );

	std::cout << "\nAdd Lex Tokens - Linux Input Stack\n";

	// Add Lex Tokens
	for( size_t i = 0; i < LexTokens.size(); i++ )
		m_QueryLexTokens.push_back( LexTokens[ i ] );

	// if we have errors return empty token
	if( m_pErrors->size() > 0 )
	{
		// Return
		return 0;
	}

	std::cout << "\nCall Compactor- Linux Input Stack\n";

	// Call Compactor
	CompactLexTokens( LexTokens, RaquelTokens );

	std::cout << "\nCompactor Called...\n";

	// if we have errors return empty token
	if( m_pErrors->size() > 0 )
	{
		// Perform Cleanup
		for( unsigned int i = 0; i < RaquelTokens.size(); i++ )
			RaquelTokens[ i ]->DeleteRef();
		RaquelTokens.clear();
		// Return
		return 0;
	}

	std::cout << "\nAdd Compactor Tokens- Linux Input Stack\n";

	// Add Compactor Tokens
	for( size_t i = 0; i < RaquelTokens.size(); i++ )
	{
		m_QueryCompactorTokens.push_back( RaquelTokens[ i ] );
		RaquelTokens[ i ]->AddRef();
	}

	std::cout << "\nCall Parser- Linux Input Stack\n";

	// Call Parser
	pTreeRoot = CreateParseTree( RaquelTokens );

	// if we have errors return empty token
	if( m_pErrors->size()> 0 )
	{
		// Perform Cleanup
		for( unsigned int i = 0; i < RaquelTokens.size(); i++ )
			DeleteTree( RaquelTokens[ i ] );
		RaquelTokens.clear();
		// Return
		return 0;
	}
	
	std::cout << "\nWibble\n";

	// Store Tree Root
	m_pTreeRoot = pTreeRoot;
	// Return Root 
	return pTreeRoot;
}

//--------------------------
// Clear Query
//--------------------------
void CLinuxInputStack::ClearQuery()
{
	// Clear Lex Tokens
	m_QueryLexTokens.clear();
	// Clear Compactor Tokens
	for( unsigned int i = 0; i < m_QueryCompactorTokens.size(); i++ )
	{
		CRaquelToken *pToken = m_QueryCompactorTokens[ i ];
		pToken->DeleteRef();
	}

	m_QueryCompactorTokens.clear();
	// Reset Tree Root
	m_pTreeRoot = 0;
}
