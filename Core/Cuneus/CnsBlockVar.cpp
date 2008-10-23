#include "CnsBlockVar.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

BlockVar::BlockVar( void ) : Variable()
{
}

BlockVar::~BlockVar( void )
{
}

bool BlockVar::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool BlockVar::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END