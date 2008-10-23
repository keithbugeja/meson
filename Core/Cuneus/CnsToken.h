//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::Token
//	Lexer's token class.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	20/02/2008	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsNamespaces.h"
#include "String8.h"

Meson_Cuneus_Compiler_BEGIN


#define _ERR -1
#define _EOS -2

namespace TokenType
{
	// Enumeration of the possible token types
	enum TokenType 
	{ 
		EOS = -2, Error = -1, Undefined = 0, Control, 
		Keyword, Identifier, 
		Integer, Real, 
		Symbol,
		LiteralChar, LiteralStr, Terminator, Comma, Dot, BlockBegin, BlockEnd, LParen, RParen, LBracket, RBracket, ArrayDefn,
		Comment
	};
};


class Token : public Meson::Common::TReferenceCounter< Token >
{
protected:
	TokenType::TokenType m_eType;
	Meson::Common::Text::String m_strValue;

	union
	{
		qword		m_qwValue;
		long long	m_llValue;
		long double	m_ldfValue;
	};

public:
	Token( void );
	Token( TokenType::TokenType p_eType );
	Token( TokenType::TokenType p_eType, const Meson::Common::Text::String& p_strValue );
	Token( TokenType::TokenType p_eType, const Meson::Common::Text::String& p_strValue, qword& p_qwValue );

	Token( const Token& p_Token );
	virtual ~Token( void );

	inline TokenType::TokenType GetType( void ) { return m_eType; }
	inline void SetType( TokenType::TokenType p_eType ) { m_eType = p_eType; }

	inline qword GetValue( void ) { return m_qwValue; }
	inline void GetValue( qword& p_qwValue ) { p_qwValue = m_qwValue; }
	inline void GetValue( long long& p_llValue ) { p_llValue = m_llValue; }
	inline void GetValue( double& p_ldfValue ) { p_ldfValue = m_ldfValue; }

	inline void SetValue( qword p_qwValue ) { m_qwValue = p_qwValue; }
	inline void SetValue( long long p_llValue ) { m_llValue = p_llValue; }
	inline void SetValue( double p_ldfValue ) { m_ldfValue = p_ldfValue; }

	inline void AppendChar( char p_cValue ) { m_strValue += p_cValue; }
	inline Meson::Common::Text::String& GetStringValue( void ) { return m_strValue; }
	inline void SetStringValue( const Meson::Common::Text::String& p_strValue ) { m_strValue = p_strValue; }


	// End of Source test
	inline bool IsEOS( void )
	{
		return ( m_eType == TokenType::EOS );
	}

	void Clear( void );

	Token& operator=( const Token& p_Token );
};



Meson_Cuneus_Compiler_END
