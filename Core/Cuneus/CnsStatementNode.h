//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ParserNodes::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	07/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsParserNode.h"
#include "CnsImports.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class StatementNode : public Meson::Cuneus::Compiler::ParserNode
{
public:
	StatementNode( void );
	StatementNode( Meson::Common::Text::String p_strName );
	~StatementNode(void);

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//----------------------------------------------------------------------------------------------

class IfNode : public StatementNode
{
public:
	IfNode(void);
	~IfNode(void);

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//----------------------------------------------------------------------------------------------

class WhileNode : public StatementNode
{
public:
	WhileNode(void);
	~WhileNode(void);

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//----------------------------------------------------------------------------------------------

class ReturnNode : public StatementNode
{
public:
	ReturnNode(void);
	~ReturnNode(void);

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//----------------------------------------------------------------------------------------------

class LabelNode : public StatementNode
{
public:
	LabelNode(void);
	~LabelNode(void);

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//----------------------------------------------------------------------------------------------

class GotoNode : public StatementNode
{
protected:
	dword m_dwAddress;
	dword m_dwAddressReference;

public:
	GotoNode(void);
	~GotoNode(void);

	dword GetAddress( void );
	void  SetAddress( dword p_dwAddress );

	dword GetAddressReference( void );
	void  SetAddressReference( dword p_dwAddressReference );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

//----------------------------------------------------------------------------------------------


Meson_Cuneus_Compiler_ParserNodes_END
