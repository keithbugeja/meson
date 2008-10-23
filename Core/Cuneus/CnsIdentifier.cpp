#include "CnsIdentifier.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

Identifier::Identifier( void ) 
{
}

Identifier::Identifier( Meson::Common::Text::String p_strName ) 
	: ParserNode( p_strName ) 
{
}

Meson::Common::Text::String Identifier::ToString( void )
{
	return m_strName;
}

bool Identifier::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool Identifier::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END