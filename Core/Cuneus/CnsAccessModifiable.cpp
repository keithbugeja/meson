#include "CnsAccessModifiable.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

AccessModifiable::AccessModifiable( void ) 
	: m_modifier()
{
}

AccessModifiable::AccessModifiable( Meson::Common::Text::String p_strName ) 
	: ParserNode( p_strName )
	, m_modifier()
{
}



Meson_Cuneus_Compiler_ParserNodes_END
