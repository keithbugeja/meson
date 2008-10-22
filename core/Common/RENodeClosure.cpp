#include "RENodeClosure.h"
#include "RENodeText.h"

using namespace Meson::Common;
using namespace Meson::Common::RE;

RENodeClosure::RENodeClosure( char p_cClosureType )
{
	m_cType = p_cClosureType;
}

RENodeClosure::~RENodeClosure(void)
{
}

bool RENodeClosure::Parse( String &p_strExpr, String &p_strTermChars )
{
	if( String( "()*+?" ).IndexOf( p_strExpr(0) ) == -1 )
	{
		RENode *pNode = new RENodeText( p_strExpr.Pop(0) );
		AddNode( pNode );
	}
	else
	{
		RENode *pNode = new RENode();
		pNode->ParseNode( p_strExpr, String( '(' ) );
		AddNode(pNode);
	}

	return true;
}

String RENodeClosure::ToString( void )
{
	String str(m_cType);

	str += '[';
	str += m_lstNode(0)->ToString();
	str += ']';

	return str;
}