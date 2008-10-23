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

class BinaryOperator
	: public ParserNode
{
public:
	BinaryOperator( void );
	BinaryOperator( Meson::Common::Text::String p_strName );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};


class OpLogicOr
	: public BinaryOperator
{
public:
	OpLogicOr( void );
	
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class OpBitwiseOr
	: public BinaryOperator
{
public:
	OpBitwiseOr( void );
	
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};

class OpBitwiseXor
	: public BinaryOperator
{
public:
	OpBitwiseXor( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class OpPlus
	: public BinaryOperator
{
public:
	OpPlus( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};


class OpMinus
	: public BinaryOperator
{
public:
	OpMinus( void );
	
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};


//---------------------------------------------------------------------------------------------------------


class OpLogicAnd
	: public BinaryOperator
{
public:
	OpLogicAnd( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class OpBitwiseAnd
	: public BinaryOperator
{
public:
	OpBitwiseAnd( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class OpShl
	: public BinaryOperator
{
public:
	OpShl( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class OpShr
	: public BinaryOperator
{
public:
	OpShr( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class OpUShr
	: public BinaryOperator
{
public:
	OpUShr( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class OpMul
	: public BinaryOperator
{
public:
	OpMul( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};


class OpDiv
	: public BinaryOperator
{
public:
	OpDiv( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};

class OpMod
	: public BinaryOperator
{
public:
	OpMod( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};


Meson_Cuneus_Compiler_ParserNodes_END
