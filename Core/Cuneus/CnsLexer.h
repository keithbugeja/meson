//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::Lexer
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	04/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsNamespaces.h"
#include "CnsToken.h"

#include <InputStream.h>
#include <StringMap.h>
#include <Pointer.h>

#include "CnsErrorTracker.h"

Meson_Cuneus_Compiler_BEGIN

class Lexer
{
public:
	typedef Meson::Common::TPointer< Token > (Lexer::*Scanlet)( void );


protected:
	Scanlet m_pScanlet[ 0x0100 ];
	Meson::Common::IO::IInputStream* m_pInputStream;
	Meson::Common::Collections::TStringMap< Meson::Common::TPointer< Token > > m_KeywordMap;
	Token m_Token;
	char	m_cCurrentChar;

	SourceInfo* m_pSourceInfo;

	bool	m_bPushBack;

public:
	Lexer( SourceInfo* p_pSourceInfo );
	virtual ~Lexer( void );

	SourceInfo* GetSourceInfo( void );

	void SetInputStream( Meson::Common::IO::IInputStream* p_pInputStream );

	inline void BindScanlet( char p_cChar, Scanlet p_Scanlet ) 
	{
		m_pScanlet[ p_cChar ] = p_Scanlet;
	}

	void Init( Meson::Common::IO::IInputStream* p_pInputStream );

	void AddKeyword( const Meson::Common::Text::String& p_string, qword m_lValue );
	void AddSymbol( const Meson::Common::Text::String& p_string, qword m_lValue );

	void Reset( void );

	char ReadChar( void );
	char NextChar( void );

	void PushBack( void );

	inline char GetCurrentChar( void ) { return m_cCurrentChar; }

	Meson::Common::TPointer< Token > GetNextToken( void );
	void GetNextToken( Token* p_pToken );

	Meson::Common::TPointer< Token > ScanletUnknown( void );
	Meson::Common::TPointer< Token > ScanletComment( void );
	Meson::Common::TPointer< Token > ScanletIdentifier( void );
	Meson::Common::TPointer< Token > ScanletNumeric( void );
	Meson::Common::TPointer< Token > ScanletSymbol( void );
	Meson::Common::TPointer< Token > ScanletCharLiteral( void );
	Meson::Common::TPointer< Token > ScanletStringLiteral( void );
	Meson::Common::TPointer< Token > ScanletTerminator( void );
	Meson::Common::TPointer< Token > ScanletComma( void );
	Meson::Common::TPointer< Token > ScanletDot( void );
	Meson::Common::TPointer< Token > ScanletBlockBegin( void );
	Meson::Common::TPointer< Token > ScanletBlockEnd( void );

	Meson::Common::TPointer< Token > ScanletLParen( void );
	Meson::Common::TPointer< Token > ScanletRParen( void );

	Meson::Common::TPointer< Token > ScanletLBracket( void );
	Meson::Common::TPointer< Token > ScanletRBracket( void );
	
	Meson::Common::TPointer< Token > ScanletError( void );
	Meson::Common::TPointer< Token > ScanletEndOfStream( void );

};

Meson_Cuneus_Compiler_END
