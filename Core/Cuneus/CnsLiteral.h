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
#include "CnsDataType.h"


Meson_Cuneus_Compiler_ParserNodes_BEGIN

class Literal
	: public ParserNode
{
private:
	Meson::Cuneus::RTS::Typing::DataType* m_pDataType;

public:
	Literal( void );
	Literal( Meson::Common::Text::String p_strName );
	~Literal( void );

	Meson::Cuneus::RTS::Typing::DataType* GetDataType( void );
	void SetDataType( Meson::Cuneus::RTS::Typing::DataType* p_pDataType );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

	Meson::Common::Text::String ToString( void );

};

//*********************************************************************************************************

//TODO: Store the literal values

using namespace Meson::Cuneus::Compiler;

class IntegerLiteral 
	: public Literal 
{ 
public: 
	IntegerLiteral( Meson::Common::Text::String p_strName ) : Literal( p_strName ) {} 

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//*********************************************************************************************************

class RealLiteral
	: public Literal
{ 
public:
	RealLiteral( Meson::Common::Text::String p_strName ) : Literal( p_strName ) {}

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//*********************************************************************************************************

class CharLiteral
	: public Literal 
{ 
public: 
	CharLiteral( Meson::Common::Text::String p_strName ) : Literal( p_strName ) {}

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//*********************************************************************************************************

class StringLiteral
	: public Literal 
{ 
public:
	StringLiteral( Meson::Common::Text::String p_strName ) : Literal( p_strName ) {}
	
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//*********************************************************************************************************

class BoolLiteral
	: public Literal 
{ 
public: 
	BoolLiteral( Meson::Common::Text::String p_strName ) : Literal( p_strName ) {} 

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//*********************************************************************************************************

class RefLiteral
	: public Literal 
{ 
public: 
	RefLiteral( Meson::Common::Text::String p_strName ) : Literal( p_strName ) {} 
	
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

Meson_Cuneus_Compiler_ParserNodes_END
