#include "CnsBinaryOperator.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;
Meson_Cuneus_Compiler_ParserNodes_BEGIN

BinaryOperator::BinaryOperator( void )
	: ParserNode( "BinOp" )
{
}

BinaryOperator::BinaryOperator( Meson::Common::Text::String p_strName )
	: ParserNode( p_strName )
{
}

bool BinaryOperator::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool BinaryOperator::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



OpLogicOr::OpLogicOr( void ) : BinaryOperator( "||" )
{
}

bool OpLogicOr::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpLogicOr::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




OpBitwiseOr::OpBitwiseOr( void ) : BinaryOperator( "|" ) 
{
}

bool OpBitwiseOr::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpBitwiseOr::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




OpBitwiseXor::OpBitwiseXor( void ) : BinaryOperator( "^" )
{
}

bool OpBitwiseXor::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpBitwiseXor::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



OpPlus::OpPlus( void ) : BinaryOperator( "+" )
{
}

bool OpPlus::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpPlus::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



OpMinus::OpMinus( void ) : BinaryOperator( "-" )
{
}

bool OpMinus::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpMinus::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



//---------------------------------------------------------------------------------------------------------




OpLogicAnd::OpLogicAnd( void ) : BinaryOperator( "&&" )
{
}

bool OpLogicAnd::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpLogicAnd::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}





OpBitwiseAnd::OpBitwiseAnd( void ) : BinaryOperator( "&" )
{
}

bool OpBitwiseAnd::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpBitwiseAnd::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




OpShl::OpShl( void ) : BinaryOperator( "<<" )
{
}

bool OpShl::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpShl::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



OpShr::OpShr( void ) : BinaryOperator( ">>" )
{
}

bool OpShr::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpShr::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



OpUShr::OpUShr( void ) : BinaryOperator( ">>>" )
{
}

bool OpUShr::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpUShr::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




OpMul::OpMul( void ) : BinaryOperator( "*" )
{
}

bool OpMul::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpMul::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}


OpDiv::OpDiv( void ) : BinaryOperator( "/" )
{
}

bool OpDiv::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpDiv::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



OpMod::OpMod( void ) : BinaryOperator( "%" )
{
}

bool OpMod::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool OpMod::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



Meson_Cuneus_Compiler_ParserNodes_END
