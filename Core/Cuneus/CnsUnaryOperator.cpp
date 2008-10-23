#include "CnsUnaryOperator.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

UnaryOperator::UnaryOperator( bool p_bPostfix )
	: ParserNode()
{
	m_bPostfix = p_bPostfix;
}

UnaryOperator::UnaryOperator( Meson::Common::Text::String p_strName, bool p_bPostfix )
	: ParserNode( p_strName )
{
	m_bPostfix = p_bPostfix;
}


bool UnaryOperator::IsPostfix( void )
{
	return m_bPostfix;
}

bool UnaryOperator::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool UnaryOperator::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//---------------------------------------------------------------------------------------------------------


UnaryInc::UnaryInc( bool p_bPostfix ) 
	: UnaryOperator( "++", p_bPostfix )
{
}


bool UnaryInc::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool UnaryInc::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



UnaryDec::UnaryDec( bool p_bPostfix ) 
	: UnaryOperator( "--", p_bPostfix )
{
}

bool UnaryDec::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool UnaryDec::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




UnaryLogicalNot::UnaryLogicalNot( void ) 
	: UnaryOperator( "!", false ) 
{
}

bool UnaryLogicalNot::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool UnaryLogicalNot::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



UnaryBitwiseNot::UnaryBitwiseNot( void )
	: UnaryOperator( "~", false ) 
{
}

bool UnaryBitwiseNot::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this );
}

bool UnaryBitwiseNot::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



UnaryPlus::UnaryPlus( void ) 
	: UnaryOperator( "+", false )
{
}

bool UnaryPlus::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool UnaryPlus::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



UnaryMinus::UnaryMinus( void ) 
	: UnaryOperator( "-", false ) 
{
}

bool UnaryMinus::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool UnaryMinus::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




Meson_Cuneus_Compiler_ParserNodes_END
