#include "RENodeText.h"

#include <string.h>

using namespace Meson::Common;
using namespace Meson::Common::RE;

RENodeText::RENodeText(void)
{

}

RENodeText::RENodeText( char p_cValue ) : m_strValue( p_cValue )
{
	
}

RENodeText::~RENodeText(void)
{

}

//bool RENodeText::Parse( String &p_strExpr, String &p_strTermChars )
//{
//	while( ( p_strTermChars.IndexOf( p_strExpr(0) ) == -1 ) && ( p_strExpr.Length() > 0 ) )
//	{
//		if( p_strExpr(0) == '\\' )
//			p_strExpr.Delete(0);
//		else
//			m_strValue += p_strExpr.Pop(0);
//	}
//
//	return true;
//}

bool RENodeText::Parse( String &p_strExpr, String &p_strTermChars )
{
	char cData;
	while( true )
	{
		cData = p_strExpr.Pop(0);
		if( p_strExpr(0) == '\\' )
		{
			p_strExpr.Delete(0);
			m_strValue.Insert(cData, 0);
		}
		else if(( p_strTermChars.IndexOf( cData ) != -1 ) || ( p_strExpr.Length() == 0 ) )
		{
			p_strExpr.Insert( cData, 0 );
			return true;
		}
		else
			m_strValue.Insert(cData, 0);
	}
}

String RENodeText::ToString( void )
{
	String str = m_strValue;

	str += RENode::ToString();

	return str;
}