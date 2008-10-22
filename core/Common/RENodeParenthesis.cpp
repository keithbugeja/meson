#include "RENodeParenthesis.h"

using namespace Meson::Common;
using namespace Meson::Common::RE;


RENodeParenthesis::RENodeParenthesis(void)
{
}

RENodeParenthesis::~RENodeParenthesis(void)
{
}



String RENodeParenthesis::ToString( void )
{
	String str = "(";

	for(size_t un=0; un<m_lstNode.Size(); un++)
		str += m_lstNode(un)->ToString();

	return str + ")";
}