//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "RENode.h"

#include <stdlib.h>
#include <string.h>

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Common_RegularExpressions_BEGIN

RENode::RENode(void)
{
	m_plstNode = NULL;
	SetType( RENodeType::NONE );
}

RENode::RENode( RENodeType::RENodeType p_eType )
{
	m_plstNode = NULL;
	SetType( p_eType );
}

RENode::~RENode(void)
{
	if( m_plstNode )
	{
		for(size_t un=0; un<m_plstNode->Size(); un++)
		{
			delete (*m_plstNode)(un);
		}
		delete m_plstNode;
	}

	if( ( m_eType == RENodeType::BRACKETS ) && ( m_pBitSet ) )
	{
		delete m_pBitSet;
	}

}


RENodeType::RENodeType RENode::GetType( void )
{
	return m_eType;
}

void RENode::SetType( RENodeType::RENodeType p_eType )
{
	m_eType = p_eType;
	if( m_eType == RENodeType::BRACKETS )
	{
		m_pBitSet = new BitSet( 256 );
	}
}


char RENode::GetChar( void )
{
	return m_cChar;
}

void RENode::SetChar( char p_cChar )
{
	m_cChar = p_cChar;
}


int RENode::GetMin( void )
{
	return m_nMin;
}

void RENode::SetMin( int p_nMin )
{
	m_nMin = p_nMin;
}

int RENode::GetMax( void )
{
	return m_nMax;
}

void RENode::SetMax( int p_nMax )
{
	m_nMax = p_nMax;
}


BitSet *RENode::GetBitSet( void )
{
	return m_pBitSet;
}

void RENode::AddNode( RENode* p_pNode )
{
	if( !m_plstNode )
		m_plstNode = new TStack< RENode* >();

	m_plstNode->Push( p_pNode );
}

RENode *RENode::PopNode()
{
	if( m_plstNode )
		return m_plstNode->Pop();
	else
		return NULL;
}



bool RENode::Matches( String &p_str, size_t &p_unIndex )
{
	if( p_unIndex > p_str.Length() )
		return false;

	size_t unTmp;

	switch( m_eType )
	{
	case RENodeType::FAULT:
		return false;

	case RENodeType::DOT:
		p_unIndex++;
		return true;

	case RENodeType::CHAR:
		if( p_str(p_unIndex) == m_cChar )
		{
			p_unIndex++;
			return true;
		}
		break;

	case RENodeType::BEGIN:
		if( p_str(p_unIndex) == '\n' )
		{
			p_unIndex++;
			return true;
		}
		else if ( p_unIndex == 0 )
			return true;
		break;

	case RENodeType::END:
		if( p_str(p_unIndex) == '\n' )
		{
			p_unIndex++;
			return true;
		}
		else if ( p_unIndex == p_str.Length() )
			return true;
		break;

	case RENodeType::PIPE:
		unTmp = p_unIndex;

		if( (*m_plstNode)(0)->Matches( p_str, p_unIndex ) )
			return true;
		
		p_unIndex = unTmp;
		if( (*m_plstNode)(1)->Matches( p_str, p_unIndex ) )
			return true;
		break;

	case RENodeType::BRACKETS:	
		if( m_pBitSet->IsSet( p_str(p_unIndex) ) )
		{
			p_unIndex++;
			return true;
		}
		break;

	case RENodeType::STAR:
		unTmp = p_unIndex;
		while( (*m_plstNode)(0)->Matches( p_str, p_unIndex ) )
		{
			unTmp = p_unIndex;
		}
		p_unIndex = unTmp;
		return true;

	case RENodeType::PLUS:
		if( (*m_plstNode)(0)->Matches( p_str, p_unIndex ) )
		{
			unTmp = p_unIndex;
			while( (*m_plstNode)(0)->Matches( p_str, p_unIndex ) )
			{
				unTmp = p_unIndex;
			}
			p_unIndex = unTmp;
			return true;
		}
		break;

	case RENodeType::QMARK:
		if( (*m_plstNode)(0)->Matches( p_str, p_unIndex ) )
			return true;
		break;

	case RENodeType::BRACES:
		{
			int n;

			for( n = GetMin(); n >= 0; n--)
			{
				if( !(*m_plstNode)(0)->Matches( p_str, p_unIndex ) )
					return false;
			}

			n = GetMax();
			if( n > 0 )
			{
				for( n = GetMin(); n >= 0; n--)
				{
					if( !(*m_plstNode)(0)->Matches( p_str, p_unIndex ) )
						return false;
				}
			}
			
			return true;
		}
		break;

	case RENodeType::PARENTHESIS:
	default:
		if(m_plstNode)
		{
			for(size_t un=0; un<m_plstNode->Size(); un++)
			{
				if( !( (*m_plstNode)(un)->Matches( p_str, p_unIndex ) ) )
					return false;
			}
		}
		return true;
	}

	return false;
}



String RENode::ToString( void )
{
	String str;

	switch( m_eType )
	{
	case RENodeType::FAULT:
		str += "ERROR";
		break;

	case RENodeType::DOT:
		str += '.';
		break;

	case RENodeType::CHAR:
		str += m_cChar;
		break;

	case RENodeType::BEGIN:
		str += '^';
		break;

	case RENodeType::END:
		str += '$';
		break;


	case RENodeType::PARENTHESIS:
		str += '(';
		if(m_plstNode)
		{
			for(size_t un=0; un<m_plstNode->Size(); un++)
			{
				str += (*m_plstNode)(un)->ToString();
			}
		}
		str += ')';
		break;

	case RENodeType::BRACES:
		str += (*m_plstNode)(0)->ToString();
		str += '{';

		if( m_nMin > 0 )
			str.Append( m_nMin );

		if( m_nMax > -1 )
			str.Append( ',' );

		if( m_nMax > 0 )
			str.Append( m_nMax );

		str += '}';
		break;

	case RENodeType::BRACKETS:
		str += '[';
		for(int n=0; n<32; n++)
		{
			if( m_pBitSet->IsSet( n ) )
			{
				str += '.';
				str += n;
			}
		}
		for(int n=32; n<129; n++)
		{
			if( m_pBitSet->IsSet( n ) )
				str += (char)n;
		}
		for(int n=129; n<256; n++)
		{
			if( m_pBitSet->IsSet( n ) )
			{
				str += '.';
				str += n;
			}
		}
		str += ']';
		break;

	case RENodeType::STAR:
		str += (*m_plstNode)(0)->ToString();
		str += '*';
		break;

	case RENodeType::PLUS:
		str += (*m_plstNode)(0)->ToString();
		str += '+';
		break;

	case RENodeType::QMARK:
		str += (*m_plstNode)(0)->ToString();
		str += '?';
		break;

	case RENodeType::PIPE:
		str += (*m_plstNode)(0)->ToString();
		str += '|';
		str += (*m_plstNode)(1)->ToString();
		break;

	default:
		if(m_plstNode)
		{
			for(size_t un=0; un<m_plstNode->Size(); un++)
			{
				str += (*m_plstNode)(un)->ToString();
			}
		}
	}

	//if(m_plstNode)
	//{
	//	for(size_t un=0; un<m_plstNode->Size(); un++)
	//	{
	//		str += (*m_plstNode)(un)->ToString();
	//	}
	//}

	return str;
}


RENode::operator const char*( void )
{
	return ToString();
}

Meson_Common_RegularExpressions_END
