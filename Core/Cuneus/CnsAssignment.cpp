#include "CnsAssignment.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN


Assignment::Assignment( void ) : ParserNode( "=" )
{
}

Assignment::Assignment( Meson::Common::Text::String p_strName ) 
	: ParserNode( p_strName )
{
}

bool Assignment::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool Assignment::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
