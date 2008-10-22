//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "REParser.h"
#include <stdlib.h>
#include <string.h>

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Common_RegularExpressions_BEGIN

REParser::REParser(void) : m_pRegExp( NULL )
{

}

REParser::~REParser( void )
{
	if( m_pRegExp )
		delete m_pRegExp;
}


bool REParser::Parse( const char *p_pszRegExp )
{
	String strRegExp( p_pszRegExp );
	m_pRegExp = ParseExpressions( strRegExp );
	return m_pRegExp!=NULL;
}

bool REParser::Parse( String &p_strRegExp )
{
	m_pRegExp = ParseExpressions( p_strRegExp );
	return m_pRegExp!=NULL;
}

RENode *REParser::GetRegularExpression( void )
{
	return m_pRegExp;
}


String REParser::GetExpression( void )
{
	if( m_pRegExp )
		return m_pRegExp->ToString();
	else
		return "ErRoR";
}


int REParser::ParseNumber( String &p_strRegExp )
{
	char cData = p_strRegExp( 0 );

	String strNum( (size_t)40 );

	while( cData >= '0' && cData <= '9' )
	{
		strNum.Append( cData );
		p_strRegExp.Delete( 0 );
		cData = p_strRegExp( 0 );
	} 

	int nVal = atoi( strNum );

	return ( errno < 0 ) ? -1 : nVal;
}


StringPtr g_pstrHex(new String("0123456789abcdef"));

int REParser::ParseHex( String &p_strRegExp )
{
	int  nVal = -1;
	int  nTmp = -1;

	char cData = p_strRegExp.Pop( 0 ) | 32;
	
	nTmp = g_pstrHex->IndexOf( cData );
	if( nTmp == -1 )
		return -1;

	nVal = nTmp << 4;

	cData = p_strRegExp.Pop( 0 ) | 32;
	
	nTmp = g_pstrHex->IndexOf( cData );
	if( nTmp == -1 )
		return -1;

	return ( nVal | ( nTmp & 0x0F ) );
}


RENode *REParser::ParseEscape( String &p_strRegExp )
{
	RENode *pNode = new RENode( RENodeType::CHAR );
	char cData = p_strRegExp.Pop( 0 );
	switch( cData )
	{
	case 'n':
		pNode->SetChar( '\n' );
		break;

	case 'r':
		pNode->SetChar( '\r' );
		break;
	
	case 't':
		pNode->SetChar( '\t' );
		break;

	case 'd':
		{
			int n = ParseNumber( p_strRegExp );
			if( n != -1 )
				pNode->SetChar( n );
			else
			{
				delete pNode;
				return NULL;
			}
		}
		break;

	case 'x':
		{
			int n = ParseHex( p_strRegExp );
			if( n != -1 )
				pNode->SetChar( n );
			else
			{
				delete pNode;
				return NULL;
			}
		}
		break;

	default:
		pNode->SetChar( cData );
	}
	return pNode;
}



bool REParser::ParseParenthesis( RENode *p_pNode, String &p_strRegExp )
{
	while( p_strRegExp(0) != ')' )
	{
		if( !ParseExpression( p_pNode, p_strRegExp ) )
			return false;
	}
	p_strRegExp.Delete(0);
	return true;
}


RENode *REParser::ParseBraces( String &p_strRegExp )
{
	RENode *pNode = new RENode( RENodeType::BRACES );

	pNode->SetMin( ParseNumber( p_strRegExp ) );

	if( p_strRegExp(0) == ',' )
	{
		p_strRegExp.Delete(0);
		if( p_strRegExp(0) == '}' )
			pNode->SetMax(0);
		else
			pNode->SetMax( ParseNumber( p_strRegExp ) );

	}
	else if( p_strRegExp(0) == '}' )
	{
		pNode->SetMax(-1);
	}

	if( pNode->GetMax() == -1 && pNode->GetMin() == -1 )
	{
		delete pNode;
		return NULL;
	}

	p_strRegExp.Delete(0);
	return pNode;
}



bool SetClass(RENode *p_pNode, String &p_str)
{
	if( p_str == "digit" )
	{
		p_pNode->GetBitSet()->SetRange( '0', '9' );
		return true;
	}

	if( p_str == "xdigit" )
	{
		p_pNode->GetBitSet()->SetRange( '0', '9' );
		p_pNode->GetBitSet()->SetRange( 'a', 'f' );
		p_pNode->GetBitSet()->SetRange( 'A', 'F' );
		return true;
	}

	if( p_str == "alpha" )
	{
		p_pNode->GetBitSet()->SetRange( 'a', 'z' );
		p_pNode->GetBitSet()->SetRange( 'A', 'Z' );
		return true;
	}

	if( p_str == "alnum" )
	{
		p_pNode->GetBitSet()->SetRange( 'a', 'z' );
		p_pNode->GetBitSet()->SetRange( 'A', 'Z' );
		p_pNode->GetBitSet()->SetRange( '0', '9' );
		return true;
	}

	if( p_str == "lower" )
	{
		p_pNode->GetBitSet()->SetRange( 'a', 'z' );
		return true;
	}

	if( p_str == "upper" )
	{
		p_pNode->GetBitSet()->SetRange( 'A', 'Z' );
		return true;
	}

	if( p_str == "blank" )
	{
		p_pNode->GetBitSet()->Set( ' ' );
		p_pNode->GetBitSet()->Set( '\t' );
		return true;
	}

	if( p_str == "space" )
	{
		p_pNode->GetBitSet()->Set( ' ' );
		p_pNode->GetBitSet()->Set( '\t' );
		p_pNode->GetBitSet()->Set( 11 );
		p_pNode->GetBitSet()->Set( 12 );
		return true;
	}

	if( p_str == "cntrl" )
	{
		p_pNode->GetBitSet()->SetRange( 0, 31 );
		p_pNode->GetBitSet()->Set( 127 );
		return true;
	}

	if( p_str == "graph" )
	{
		p_pNode->GetBitSet()->SetRange( 33, 126 );
		return true;
	}

	if( p_str == "print" )
	{
		p_pNode->GetBitSet()->SetRange( 32, 126 );
		return true;
	}

	if( p_str == "punct" )
	{
		p_pNode->GetBitSet()->SetRange( 33, 126 );
		p_pNode->GetBitSet()->ClearRange( 'a', 'z' );
		p_pNode->GetBitSet()->ClearRange( 'A', 'Z' );
		p_pNode->GetBitSet()->ClearRange( '0', '9' );
		return true;
	}

	return false;
}

RENode *REParser::ParseBrackets( String &p_strRegExp )
{
	RENode *pNode = new RENode( RENodeType::BRACKETS );

	bool bNegate = false;
	
	char cData = p_strRegExp.Pop(0);
	if( cData == '^' )
	{
		bNegate = true;
		cData = p_strRegExp.Pop(0);
	}
	
	if( cData == ']' )
	{
		pNode->GetBitSet()->Set( ']' );
		cData = p_strRegExp.Pop(0);
	}

	while ( cData != ']' )
	{
		if( cData == ':' )
		{
			String str;
			do
			{
				cData = p_strRegExp.Pop(0);
				str += (char)(cData | 32);
			}
			while ( cData != ':' && ( cData != ']' ) );
			str.Delete( str.Length() - 1 );

			if( !SetClass( pNode, str ) )
			{
				delete pNode;
				return NULL;
			}
		}
		else
		{
			pNode->GetBitSet()->Set( cData );
			if( ( p_strRegExp(0) == '-' ) && ( p_strRegExp(1) != ']' ) )
			{
				p_strRegExp.Pop(0);
				char cLast = p_strRegExp.Pop(0);
				if( cData > cLast )
				{
					char cTmp = cData;
					cData = cLast;
					cLast = cTmp;
				}
				pNode->GetBitSet()->SetRange( cData, cLast );
			}

			if( p_strRegExp.Length() == 0 )
			{
				delete pNode;
				return NULL;
			}
		}

		cData = p_strRegExp.Pop(0);
	}


	if( bNegate )
		pNode->GetBitSet()->Toggle();

	return pNode;
}



bool REParser::ParseExpression( RENode *p_pNode, String &p_strRegExp )
{
	RENode *pSubNode = NULL;
	char cData = 0;

	if( p_strRegExp.Length() > 0 )
	{
		cData = p_strRegExp.Pop( 0 );
		switch( cData )
		{
		case ')':
			return false;

		case '$':
			pSubNode = new RENode( RENodeType::END );
			p_pNode->AddNode( pSubNode );
			break;

		case '^':
			pSubNode = new RENode( RENodeType::BEGIN );
			p_pNode->AddNode( pSubNode );
			break;

		case '.':
			pSubNode = new RENode( RENodeType::DOT );
			p_pNode->AddNode( pSubNode );
			break;

		case '(':
			pSubNode = new RENode( RENodeType::PARENTHESIS );
			if( !ParseParenthesis( pSubNode, p_strRegExp ) )
			{
				delete pSubNode;
				return false;
			}
			p_pNode->AddNode( pSubNode );
			break;

		case '{':
			pSubNode = ParseBraces( p_strRegExp );
			if( !pSubNode )
			{
				delete pSubNode;
				return false;
			}
			pSubNode->AddNode( p_pNode->PopNode() );
			p_pNode->AddNode( pSubNode );
			break;

		case '[':
			pSubNode = ParseBrackets( p_strRegExp );
			if( !pSubNode )
			{
				delete pSubNode;
				return false;
			}
			p_pNode->AddNode( pSubNode );
			break;

		case '*':
			pSubNode = new RENode( RENodeType::STAR );
			pSubNode->AddNode( p_pNode->PopNode() );
			p_pNode->AddNode( pSubNode );
			break;

		case '+':
			pSubNode = new RENode( RENodeType::PLUS );
			pSubNode->AddNode( p_pNode->PopNode() );
			p_pNode->AddNode( pSubNode );
			break;

		case '?':
			pSubNode = new RENode( RENodeType::QMARK );
			pSubNode->AddNode( p_pNode->PopNode() );
			p_pNode->AddNode( pSubNode );
			break;

		case '|':
			pSubNode = new RENode( RENodeType::PIPE );
			pSubNode->AddNode( p_pNode->PopNode() );
			p_pNode->AddNode( pSubNode );
			if( !ParseExpression( pSubNode, p_strRegExp ) )
				return false;
			break;

		case '\\':
			pSubNode = ParseEscape( p_strRegExp );
			if( !pSubNode )
			{
				delete pSubNode;
				return false;
			}
			p_pNode->AddNode( pSubNode );
			break;

		default:
			pSubNode = new RENode( RENodeType::CHAR );
			pSubNode->SetChar( cData );
			p_pNode->AddNode( pSubNode );
			break;
		}

	}

	return true;
}



RENode *REParser::ParseExpressions( String &p_strRegExp )
{
	RENode *pNode = new RENode();
	while( p_strRegExp.Length() > 0 )
	{
		if( !ParseExpression( pNode, p_strRegExp ) )
		{
			delete pNode;
			return NULL;
		}

	}
	return pNode;
}

Meson_Common_RegularExpressions_END
