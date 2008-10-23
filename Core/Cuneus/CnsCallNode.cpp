#include "CnsCallNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

CallNode::CallNode( void ) : ParserNode( "Call" )
{
}

CallNode::~CallNode( void )
{
}

bool CallNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool CallNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
