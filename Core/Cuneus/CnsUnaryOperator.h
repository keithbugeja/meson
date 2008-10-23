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

class UnaryOperator
	: public ParserNode
{
private:
	bool m_bPostfix;

public:
	UnaryOperator( bool p_bPostfix = false );
	UnaryOperator( Meson::Common::Text::String p_strName, bool p_bPostfix = false );

	bool IsPostfix( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};



class UnaryInc : public UnaryOperator
{
public:
	UnaryInc( bool p_bPostfix );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class UnaryDec : public UnaryOperator
{
public:
	UnaryDec( bool p_bPostfix );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};



class UnaryLogicalNot : public UnaryOperator
{
public:
	UnaryLogicalNot( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class UnaryBitwiseNot : public UnaryOperator
{
public:
	UnaryBitwiseNot( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};


class UnaryPlus : public UnaryOperator
{
public:
	UnaryPlus( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

class UnaryMinus : public UnaryOperator
{
public:
	UnaryMinus( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};






Meson_Cuneus_Compiler_ParserNodes_END
