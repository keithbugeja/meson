
#include "CnsLexer.h"

#include <Converter.h>

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;


//*************************************************************************************************
// Helper Macros and Functions
//*************************************************************************************************

static const char* g_szLetter     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char* g_szIdentifier = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789";
static const char* g_szSymbol = "!#$%&()*+-/:<=>?@[\\]^`|~";

#define IsWhiteSpace( p_cChar ) ( (p_cChar < 33) || (p_cChar > 127) )
#define IsDigit( p_cChar ) ( (p_cChar >= '0') && (p_cChar <='9' ) )
#define IsOctDigit( p_cChar ) ( (p_cChar >= '0') && (p_cChar <'8' ) )
#define IsHexDigit( p_cChar ) ( ( (p_cChar >= '0') && (p_cChar <='9') ) || ( (p_cChar >= 'a') && (p_cChar <='f' ) ) || ( (p_cChar >= 'A') && (p_cChar <='F' ) ) )
#define IsLetter( p_cChar ) ( strchr( g_szLetter, p_cChar ) != NULL )
#define IsIdentifierChar( p_cChar ) ( strchr( g_szIdentifier, p_cChar ) != NULL )
#define IsSymbolChar( p_cChar ) ( strchr( g_szSymbol, p_cChar ) != NULL )

Meson_Cuneus_Compiler_BEGIN



Lexer::Lexer( SourceInfo* p_pSourceInfo )
{
	m_pSourceInfo = p_pSourceInfo;
	if( m_pSourceInfo == NULL )
		m_pSourceInfo = new SourceInfo();

	for( int n=0; n<0x0100; n++ )
	{
		BindScanlet( n, &Lexer::ScanletUnknown );
	}
	m_pInputStream = NULL;

	Reset();
}



Lexer::~Lexer( void )
{

}

SourceInfo* Lexer::GetSourceInfo( void )
{
	return m_pSourceInfo;
}


void Lexer::Init( IInputStream* p_pInputStream )
{
	m_pInputStream = p_pInputStream;
	
	int n,
		m;

	for( n='A', m='a'; n<='Z'; n++, m++)
	{
		BindScanlet( n, &Lexer::ScanletIdentifier );
		BindScanlet( m, &Lexer::ScanletIdentifier );
	}

	for( n='0'; n<='9'; n++)
	{
		BindScanlet( n, &Lexer::ScanletNumeric );
	}
	
	for( n='!'; n<='/'; n++)
	{
		BindScanlet( n, &Lexer::ScanletSymbol );
	}

	for( n=':'; n<='@'; n++)
	{
		BindScanlet( n, &Lexer::ScanletSymbol );
	}

	for( n='['; n<='`'; n++)
	{
		BindScanlet( n, &Lexer::ScanletSymbol );
	}

	for( n='{'; n<='~'; n++)
	{
		BindScanlet( n, &Lexer::ScanletSymbol );
	}

	BindScanlet( '/', &Lexer::ScanletComment );
	BindScanlet( '\'', &Lexer::ScanletCharLiteral );
	BindScanlet( '\"',  &Lexer::ScanletStringLiteral );

	BindScanlet( '_', &Lexer::ScanletIdentifier );
	BindScanlet( ';', &Lexer::ScanletTerminator );
	BindScanlet( ',', &Lexer::ScanletComma );
	BindScanlet( '.', &Lexer::ScanletDot );
	BindScanlet( '{', &Lexer::ScanletBlockBegin );
	BindScanlet( '}', &Lexer::ScanletBlockEnd );

	BindScanlet( '(', &Lexer::ScanletLParen );
	BindScanlet( ')', &Lexer::ScanletRParen );

	BindScanlet( '[', &Lexer::ScanletLBracket );
	BindScanlet( ']', &Lexer::ScanletRBracket );

	BindScanlet( 0xFE, &Lexer::ScanletEndOfStream );
	BindScanlet( 0xFF, &Lexer::ScanletError );

}



void Lexer::SetInputStream( IInputStream* p_pInputStream )
{
	m_pInputStream = p_pInputStream;
	Reset();
}



void Lexer::AddKeyword( const String& p_string, qword m_qwValue )
{
	TPointer< Token > pToken;
	pToken = new Token( TokenType::Keyword, p_string, m_qwValue );
	m_KeywordMap.Insert( p_string, pToken );
}



void Lexer::AddSymbol( const String& p_string, qword m_qwValue )
{
	TPointer< Token > pToken;
	pToken = new Token( TokenType::Keyword, p_string, m_qwValue );
	m_KeywordMap.Insert( p_string, pToken );
}


void Lexer::Reset( void )
{
	m_pSourceInfo->Reset(); 

	m_cCurrentChar = 0;
	m_bPushBack = false;
}



char Lexer::ReadChar( void )
{
	if( m_bPushBack )
	{
		m_bPushBack = false;
		return m_cCurrentChar;
	}

	if( !m_pInputStream->IsDataAvailable() )
	{
		m_cCurrentChar = _EOS;
		return m_cCurrentChar;
	}

	*m_pInputStream >> m_cCurrentChar;

	if( m_cCurrentChar == '\n' )
		m_pSourceInfo->NewLine();
	else
		if( m_cCurrentChar == '\\' )
		{
			*m_pInputStream >> m_cCurrentChar;
			switch( m_cCurrentChar )
			{
				case 'n':
					m_cCurrentChar = '\n';
					break;
				case 'r':
					m_cCurrentChar = '\r';
					break;
				case 't':
					m_cCurrentChar = '\t';
					break;
			}
		}


	m_pSourceInfo->NextColoumn();

	return m_cCurrentChar;
}



char Lexer::NextChar( void )
{
	do
	{
		if( m_bPushBack )
			m_bPushBack = false;
		else
		{
			if( !m_pInputStream->IsDataAvailable() )
			{
				m_cCurrentChar = _EOS;
				return m_cCurrentChar;
			}

			*m_pInputStream >> (char&)m_cCurrentChar;

			if( m_cCurrentChar == '\n' )
				m_pSourceInfo->NewLine();

			m_pSourceInfo->NextColoumn();
		}
	} while IsWhiteSpace( m_cCurrentChar );

	return m_cCurrentChar;
}



void Lexer::PushBack( void )
{
	m_pSourceInfo->PreviousColoumn();
	m_bPushBack = true;
}


TPointer< Token > Lexer::GetNextToken( void )
{
	m_cCurrentChar = NextChar();

	if( m_cCurrentChar < 0xFF )
	{
		return (this->*m_pScanlet[ m_cCurrentChar ])();
	}
	else
	{
		TPointer< Token > pToken;
		pToken = new Token( TokenType::Error );
		pToken->AppendChar( m_cCurrentChar );
		return pToken;
	}
}


void Lexer::GetNextToken( Token* p_pToken )
{
	TPointer< Token > pToken;
	pToken = GetNextToken();
	*p_pToken = *pToken;
}






//*************************************************************************************************
// Scanlets
//*************************************************************************************************

TPointer< Token > Lexer::ScanletUnknown( void )
{
	TPointer< Token > pToken; 
	pToken = new Token( TokenType::Undefined );
	return pToken;
}

TPointer< Token > Lexer::ScanletComment( void )
{
	char cChar = ReadChar();
	TPointer< Token > pToken;
	
	if( cChar == '/' )
	{
		pToken = new Token( TokenType::Comment );
		
		cChar = ReadChar();
		while( ( cChar != '\n' ) && ( cChar != _EOS ) )
		{
			pToken->AppendChar( cChar );
			cChar = ReadChar();
		}

		return pToken;
	}
	
	if( cChar == '*' )
	{
		pToken = new Token( TokenType::Comment );

ContinueComment:
		cChar = ReadChar();
		while( ( cChar != '*' ) && ( cChar != _EOS ) )
		{
			pToken->AppendChar( cChar );
			cChar = ReadChar();
		}

		cChar = ReadChar();
		if( ( cChar != '/' ) && ( cChar != _EOS ) )
		{
			pToken->AppendChar( cChar );
			goto ContinueComment;
		}

		return pToken;
	}

	PushBack();

	cChar = '/';
	pToken = new Token( TokenType::Symbol );
	pToken->AppendChar( cChar );

	if( m_KeywordMap.ContainsKey( pToken->GetStringValue(), pToken ) )
		return pToken;

	return pToken;
}

TPointer< Token > Lexer::ScanletIdentifier( void )
{

	char cChar = this->GetCurrentChar();

	TPointer< Token > pToken; 
	pToken = new Token( TokenType::Identifier );

	while( IsIdentifierChar( cChar ) )
	{
		pToken->AppendChar( cChar );
		cChar = ReadChar();
	}

	PushBack();

	if( m_KeywordMap.ContainsKey( pToken->GetStringValue(), pToken ) )
		return pToken;
	else
	{
		m_KeywordMap.Insert( pToken->GetStringValue(), pToken );
		return pToken;
	}
}


TPointer< Token > Lexer::ScanletNumeric( void )
{

	char cChar = this->GetCurrentChar();

	TPointer< Token > pToken; 
	pToken = new Token( TokenType::Integer );

	if( cChar == '0' )
	{
		// Hex or Octal or Binary
		cChar = ReadChar();
		switch( cChar )
		{
		case 'x':	
			// Hex
			cChar = ReadChar();
			while( IsHexDigit( cChar ) )
			{
				pToken->AppendChar( cChar );
				cChar = ReadChar();
			}
			pToken->SetValue( Converter::HexToQWord( pToken->GetStringValue() ) );
			PushBack();
			return pToken;

		case '_':	
			// Bin
			cChar = ReadChar();
			while( ( cChar == '0' ) || ( cChar == '1' ) )
			{
				pToken->AppendChar( cChar );
				cChar = ReadChar();
			}
			pToken->SetValue( Converter::BinToQWord( pToken->GetStringValue() ) );
			PushBack();
			return pToken;
		
		default:
			//Oct
			while( IsOctDigit( cChar ) )
			{
				pToken->AppendChar( cChar );
				cChar = ReadChar();
			}
			pToken->SetValue( Converter::OctToQWord( pToken->GetStringValue() ) );
			PushBack();
			return pToken;
		}

	}
	
	while( IsDigit( cChar ) )
	{
		pToken->AppendChar( cChar );
		cChar = ReadChar();
	}

	// Speculate Integer Type
	pToken->SetType( TokenType::Integer );
	
	if( cChar=='.' )
	{
		pToken->SetType( TokenType::Real );

		do
		{
			pToken->AppendChar( cChar );
			cChar = ReadChar();
		}
		while( IsDigit( cChar ) );
	}


	if( ( cChar | 32 ) == 'e' )
	{
		pToken->SetType( TokenType::Real );

		pToken->AppendChar( 'E' );
		cChar = ReadChar();
		if( cChar == '-' || cChar == '+' )
		{
			pToken->AppendChar( cChar );
			cChar = ReadChar();
		}

		while( IsDigit( cChar ) )
		{
			pToken->AppendChar( cChar );
			cChar = ReadChar();
		}
	}
	PushBack();

	if( pToken->GetType() == TokenType::Integer )
		pToken->SetValue( Converter::ToQWord( pToken->GetStringValue() ) );
	else
		pToken->SetValue( Converter::ToLongDouble( pToken->GetStringValue() ) );

	return pToken;
}



TPointer< Token > Lexer::ScanletSymbol( void )
{
	char cChar = this->GetCurrentChar();

	TPointer< Token > pToken; 
	pToken = new Token( TokenType::Symbol );

	while( IsSymbolChar( cChar ) )
	{
		pToken->AppendChar( cChar );
		cChar = ReadChar();
	}
	PushBack();

	if( m_KeywordMap.ContainsKey( pToken->GetStringValue(), pToken ) )
		return pToken;
	else
		pToken->SetType( TokenType::Undefined );

	return pToken;
}



TPointer< Token > Lexer::ScanletCharLiteral( void )
{
	char cChar = ReadChar();
	TPointer< Token > pToken; 
	pToken = new Token( TokenType::LiteralChar );
	pToken->AppendChar( cChar );
	cChar = ReadChar();

	if( cChar == '\'' )
		return pToken;
	
	PushBack();
	pToken->SetType( TokenType::Error );
	return pToken;
}



TPointer< Token > Lexer::ScanletStringLiteral( void )
{
	char cChar = 0;
	TPointer< Token > pToken; 
	pToken = new Token( TokenType::LiteralStr );

	while( cChar != '\"')
	{
		cChar = ReadChar();
		if( cChar == '\"')
			return pToken;

		pToken->AppendChar( cChar );
	}

	return pToken;
}


TPointer< Token > Lexer::ScanletTerminator( void )
{
	TPointer< Token > pToken; 

	if( m_KeywordMap.ContainsKey( ";", pToken ) )
		return pToken;

	pToken = new Token( TokenType::Terminator, ";" );
	return pToken;
}

TPointer< Token > Lexer::ScanletComma( void )
{
	TPointer< Token > pToken; 

	if( m_KeywordMap.ContainsKey( ",", pToken ) )
		return pToken;

	pToken = new Token( TokenType::Comma, "," );
	return pToken;
}

TPointer< Token > Lexer::ScanletDot( void )
{
	TPointer< Token > pToken; 

	if( m_KeywordMap.ContainsKey( ".", pToken ) )
		return pToken;

	pToken = new Token( TokenType::Dot, "." );
	return pToken;
}

TPointer< Token > Lexer::ScanletBlockBegin( void )
{
	TPointer< Token > pToken; 

	if( m_KeywordMap.ContainsKey( "{", pToken ) )
		return pToken;

	pToken = new Token( TokenType::BlockBegin, "{" );
	return pToken;
}

TPointer< Token > Lexer::ScanletBlockEnd( void )
{
	TPointer< Token > pToken; 

	if( m_KeywordMap.ContainsKey( "}", pToken ) )
		return pToken;

	pToken = new Token( TokenType::BlockEnd, "}" );
	return pToken;
}

TPointer< Token > Lexer::ScanletLParen( void )
{
	TPointer< Token > pToken; 

	if( m_KeywordMap.ContainsKey( "(", pToken ) )
		return pToken;

	pToken = new Token( TokenType::LParen, "(" );
	return pToken;
}

TPointer< Token > Lexer::ScanletRParen( void )
{
	TPointer< Token > pToken; 

	if( m_KeywordMap.ContainsKey( ")", pToken ) )
		return pToken;

	pToken = new Token( TokenType::RParen, ")" );
	return pToken;
}


TPointer< Token > Lexer::ScanletLBracket( void )
{
	TPointer< Token > pToken; 

	if( ReadChar() == ']' )
	{
		pToken = new Token( TokenType::ArrayDefn, "[]" );
		return pToken;
	}

	PushBack();
	pToken = new Token( TokenType::LBracket, "[" );
	return pToken;
}


Meson::Common::TPointer< Token > Lexer::ScanletRBracket( void )
{
	TPointer< Token > pToken; 

	pToken = new Token( TokenType::RBracket, "]" );
	return pToken;
}


TPointer< Token > Lexer::ScanletEndOfStream( void )
{
	TPointer< Token > pToken; 
	pToken = new Token( TokenType::EOS, GetCurrentChar() );
	return pToken;
}

TPointer< Token > Lexer::ScanletError( void )
{
	TPointer< Token > pToken; 
	pToken = new Token( TokenType::Error, GetCurrentChar() );
	return pToken;
}

Meson_Cuneus_Compiler_END
