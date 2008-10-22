//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "RegularExpression.h"
#include <stdlib.h>
#include <string.h>

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Common_RegularExpressions_BEGIN



RegularExpression::RegularExpression( void )
{
	m_pRENode = NULL;
}

RegularExpression::RegularExpression( String &p_str )
{
	m_pRENode = NULL;
	Set( p_str );
}


RegularExpression::~RegularExpression( void )
{
}


void RegularExpression::Reset( void )
{
	if( m_pRENode )
	{
		delete m_pRENode;
		m_pRENode = NULL;
	}
}

RENode *RegularExpression::Get( void )
{
	return m_pRENode;
}

bool RegularExpression::Set( String &p_str )
{
	if( !m_parser.Parse( p_str ) )
	{
		m_pRENode = NULL;
		return false;
	}

	if( m_pRENode )
	{
		delete m_pRENode;
		m_pRENode = NULL;
	}

	m_pRENode = m_parser.GetRegularExpression();
	return true;
}


bool RegularExpression::Matches( String &p_str )
{
	if( m_pRENode )
	{
		size_t unIndex = 0;
		return m_pRENode->Matches( p_str, unIndex );
	}

	return false;
}

int RegularExpression::Fetch( String &p_str )
{
	if( !m_pRENode )
		return -1;

	size_t unIndex = 0;
	int nLen = (int)p_str.Size();

	for( int n=0; n<nLen; n++ )
	{
		unIndex = n;
		if( m_pRENode->Matches( p_str, unIndex ) )
			return n;
	}

	return -1;
}


String RegularExpression::ToString( void )
{
	if( m_pRENode )
		return m_pRENode->ToString();
	else
		return String("Undefined");
}

Meson_Common_RegularExpressions_END
