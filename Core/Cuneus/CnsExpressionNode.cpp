#include "CnsExpressionNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

ExpressionNode::ExpressionNode( void ) : ParserNode( "Expr" )
{
}

ExpressionNode::ExpressionNode( Meson::Common::Text::String p_strName ) 
	: ParserNode( p_strName ) 
{
}

bool ExpressionNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool ExpressionNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
