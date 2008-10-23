#include "CnsRelationalOperator.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

RelationalOperator::RelationalOperator( void )
	:ParserNode()
{
}

RelationalOperator::RelationalOperator( Meson::Common::Text::String p_strName )
	: ParserNode( p_strName )
{
}

bool RelationalOperator::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool RelationalOperator::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}


//---------------------------------------------------------------------------------------------------------


RelOpEqEq::RelOpEqEq( void ) : RelationalOperator( "==" ) 
{
}

bool RelOpEqEq::TypeCheck( SemanticAnalyser* p_pAnalyser )
{ 
	return p_pAnalyser->Check( this ); 
}


bool RelOpEqEq::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




RelOpNotEq::RelOpNotEq( void ) : RelationalOperator( "!=" )
{
}

bool RelOpNotEq::TypeCheck( SemanticAnalyser* p_pAnalyser )
{ 
	return p_pAnalyser->Check( this ); 
}


bool RelOpNotEq::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



RelOpLtEq::RelOpLtEq( void ) : RelationalOperator( ">=" ) 
{
}

bool RelOpLtEq::TypeCheck( SemanticAnalyser* p_pAnalyser )
{ 
	return p_pAnalyser->Check( this );
}


bool RelOpLtEq::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




RelOpGtEq::RelOpGtEq( void ) : RelationalOperator( "<=" )
{
}

bool RelOpGtEq::TypeCheck( SemanticAnalyser* p_pAnalyser )
{ 
	return p_pAnalyser->Check( this ); 
}


bool RelOpGtEq::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}




RelOpLt::RelOpLt( void ) : RelationalOperator( "<" )
{
}

bool RelOpLt::TypeCheck( SemanticAnalyser* p_pAnalyser )
{
	return p_pAnalyser->Check( this ); 
}


bool RelOpLt::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



RelOpGt::RelOpGt( void ) : RelationalOperator( ">" )
{ 
}

bool RelOpGt::TypeCheck( SemanticAnalyser* p_pAnalyser )
{ 
	return p_pAnalyser->Check( this ); 
}


bool RelOpGt::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}





Meson_Cuneus_Compiler_ParserNodes_END
