//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	05/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "CnsLexer.h"
#include "CnsErrorTracker.h"

#include <ArrayList.h>
#include <Pointer.h>

Meson_Cuneus_Compiler_BEGIN

class TokenStream
{
protected:
	Lexer* m_pLexer;
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< Token > > m_lstToken;
	Meson::Common::Collections::TArrayList< PositionInfo > m_lstPosition;
	int m_nIndex;
		//m_nRestorePoint;

public:
	TokenStream( Lexer* p_pLexer );
	~TokenStream( void );

	void Init( void );

	bool HasMoreTokens( void );

	void PushBack( void );
	void PushBack( int p_nCount );
	Meson::Common::TPointer< Token > NextToken( void );
	PositionInfo GetTokenPosition( void );

	int Mark( void );
	void Restore( int p_nRestorePoint );
};

Meson_Cuneus_Compiler_END
