#include "CnsBlockNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

BlockNode::BlockNode( void ) : ParserNode( "Block" )
{

}

BlockNode::~BlockNode( void )
{
}



bool BlockNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool BlockNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
