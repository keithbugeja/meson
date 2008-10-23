#include "CnsImports.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

NamespaceNode::NamespaceNode( void ) 
	: ParserNode( "NamespaceNode" )
{
}

NamespaceNode::NamespaceNode( Meson::Common::Text::String p_strName )
	: ParserNode( p_strName )
{
}

bool NamespaceNode::TypeCheck( SemanticAnalyser* p_pAnalyser )
{
	return p_pAnalyser->Check( this );
}

bool NamespaceNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
