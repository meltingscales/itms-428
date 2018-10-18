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
#ifndef LINUXINPUTSTACK_H
#define LINUXINPUTSTACK_H
//-----------------------------------------------------
// Includes
//-----------------------------------------------------
#include <iostream>
#include "InputStack.h"
//-----------------------------------------------------
// Types
//-----------------------------------------------------

//-----------------------------------------------------
// Input Stack Class
//-----------------------------------------------------
class CLinuxInputStack : public CInputStack
{
//--------------------------
// Protected Members
//--------------------------
protected:

	// Output Vars For Dialog
	LexTokenVector					m_QueryLexTokens;
	std::vector< CRaquelToken * >	m_QueryCompactorTokens;
	CRaquelToken					*m_pTreeRoot;

//--------------------------
// Public Operations
//--------------------------
public:


	// Constructor
	CLinuxInputStack();

	// Destructor
	~CLinuxInputStack();

	// Create Tree
	CRaquelToken* CreateTree( const std::string &szQuery, std::vector<int> *Errors );

	// Clear Query
	void ClearQuery();

	// Get Lex Tokens
	LexTokenVector& GetLexTokens() { return m_QueryLexTokens; }

	// Get Compactor Tokens
	std::vector< CRaquelToken * >& GetCompactorTokens() { return m_QueryCompactorTokens; }

	// Get Lex Tokens
	CRaquelToken* GetTreeRoot() { return m_pTreeRoot; }

};

#endif
