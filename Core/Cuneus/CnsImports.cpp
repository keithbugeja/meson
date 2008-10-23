#include "CnsImports.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

Imports::Imports( void ) 
	: ParserNode( "Imports" ) 
{
}

bool Imports::TypeCheck( SemanticAnalyser* p_pAnalyser )
{
	return p_pAnalyser->Check( this );
}

bool Imports::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END