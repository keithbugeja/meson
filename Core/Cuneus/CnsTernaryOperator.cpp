#include "CnsTernaryOperator.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

TernaryOperator::TernaryOperator( void ) : ParserNode()
{
}

TernaryOperator::TernaryOperator( Meson::Common::Text::String p_strName )
	: ParserNode( p_strName )
{
}

Meson::Common::Text::String TernaryOperator::ToString( void )
{
	Meson::Common::Text::String str = (*m_plstChildNode)[0]->ToString() + "?";
	str += (*m_plstChildNode)[1]->ToString() + ":";
	str += (*m_plstChildNode)[2]->ToString();

	return str;
}

bool TernaryOperator::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool TernaryOperator::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
