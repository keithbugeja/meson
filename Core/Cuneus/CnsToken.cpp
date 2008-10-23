
#include "CnsToken.h"
#include <stdlib.h>


Meson_Cuneus_Compiler_BEGIN

Token::Token( void ) : m_strValue()
{
	m_eType =  TokenType::Undefined;
	m_qwValue = 0;
}

Token::Token( TokenType::TokenType p_eType ) 
	: m_strValue()
{
	m_eType =  p_eType;
	m_qwValue = 0;
}

Token::Token( TokenType::TokenType p_eType, const Meson::Common::Text::String& p_strValue ) 
	: m_strValue( p_strValue )
{
	m_eType =  p_eType;
	m_qwValue = 0;
}


Token::Token( TokenType::TokenType p_eType, const Meson::Common::Text::String& p_strValue, qword& p_qwValue ) 
	: m_strValue( p_strValue )
{
	m_eType =  p_eType;
	m_qwValue = p_qwValue;
}

Token::Token( const Token& p_Token )  : m_strValue( p_Token.m_strValue )
{
	m_eType =  p_Token.m_eType;
	m_qwValue = p_Token.m_qwValue;
}


Token::~Token( void )
{
}

void Token::Clear( void )
{
	m_strValue.Clear();
	m_eType = TokenType::Undefined;
	m_qwValue = 0;
}

Token& Token::operator=( const Token& p_Token )
{
	m_eType =  p_Token.m_eType;
	m_qwValue = p_Token.m_qwValue;
	m_strValue = p_Token.m_strValue;
	return *this;
}

Meson_Cuneus_Compiler_END
