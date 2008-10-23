#include "CnsTokenStream.h"

#include<iostream>

using namespace Meson::Common;

Meson_Cuneus_Compiler_BEGIN

#define INITIAL_SIZE 4096

TokenStream::TokenStream( Lexer* p_pLexer ) 
	: m_lstToken( INITIAL_SIZE )
	, m_lstPosition( INITIAL_SIZE )
{
	m_pLexer = p_pLexer;
	m_nIndex = 0;
	//m_nRestorePoint = 0;
}

TokenStream::~TokenStream( void )
{

}


void TokenStream::Init( void )
{
	TPointer< Token > pToken;
	do
	{
		pToken = m_pLexer->GetNextToken();
std::cout << pToken->GetStringValue() << std::endl;


		while( pToken->GetType() == TokenType::Comment )
			pToken = m_pLexer->GetNextToken();

		if( pToken->IsEOS() )
			return;

		m_lstToken.Add( pToken );
		m_lstPosition.Add( m_pLexer->GetSourceInfo()->GetPosition() );
	}
	while( !pToken->IsEOS() );
}

bool TokenStream::HasMoreTokens( void )
{
	return ( m_nIndex < (int)m_lstToken.Size() );
}

void TokenStream::PushBack( void )
{
	if( m_nIndex > 0 )
		m_nIndex--;
}

void TokenStream::PushBack( int p_nCount )
{
	m_nIndex -= p_nCount;
	if( m_nIndex < 0 )
		m_nIndex = 0;
}

TPointer< Token > TokenStream::NextToken( void )
{
	if( HasMoreTokens() )
	{
		TPointer< Token > pToken( m_lstToken[ m_nIndex ] );
		m_nIndex++;
		return pToken;
	}
	else
		return m_lstToken[ m_nIndex - 1 ];
}

PositionInfo TokenStream::GetTokenPosition( void )
{
	if( m_nIndex <= 0 )
		return PositionInfo();

	if( m_nIndex >= (int)m_lstPosition.Size() )
		return m_lstPosition[ m_nIndex - 1 ];

	return m_lstPosition[ m_nIndex ];
}

int TokenStream::Mark( void )
{
	return m_nIndex;
}

void TokenStream::Restore( int p_nRestorePoint )
{
	if( p_nRestorePoint == 0 )
	{
		m_nIndex = 0;
		return;
	}

	m_nIndex = p_nRestorePoint - 1;
}

Meson_Cuneus_Compiler_END
