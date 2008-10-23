//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ParserNodes::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	17/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsParserNode.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class RelationalOperator
	: public ParserNode
{
public:
	RelationalOperator( void );
	RelationalOperator( Meson::Common::Text::String p_strName );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};




class RelOpEqEq : public RelationalOperator
{
public:
	RelOpEqEq( void );
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};



class RelOpNotEq : public RelationalOperator
{
public:
	RelOpNotEq( void );
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};



class RelOpLtEq : public RelationalOperator
{
public:
	RelOpLtEq( void );
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};



class RelOpGtEq : public RelationalOperator
{
public:
	RelOpGtEq( void );
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};



class RelOpLt : public RelationalOperator
{
public:
	RelOpLt( void );
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};



class RelOpGt : public RelationalOperator
{
public:
	RelOpGt( void );
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

Meson_Cuneus_Compiler_ParserNodes_END
