#include "CnsVarNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

VarNode::VarNode( void ) : ParserNode( "VarNode" )
{
}

VarNode::~VarNode( void )
{
}

bool VarNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool VarNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END