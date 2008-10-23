#include "CnsTypeCast.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

TypeCast::TypeCast( void )
	: ParserNode()
{
}

TypeCast::TypeCast( Meson::Common::Text::String p_strName )
	: ParserNode( p_strName )
{
}

bool TypeCast::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool TypeCast::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
