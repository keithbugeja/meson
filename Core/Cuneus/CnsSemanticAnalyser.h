#pragma once

//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::SemanticAnalyser
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	26/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "CnsNamespaces.h"
#include "CnsCuneusEngine.h"
#include "CnsParserNodes.h"
#include "CnsCuneusParser.h"

Meson_Cuneus_Compiler_BEGIN

class SemanticAnalyser
{
protected:
	Meson::Cuneus::RTS::Typing::DataTypeMgr* m_pDataTypeMgr;
	CuneusParser* m_pParser;

public:
	SemanticAnalyser( CuneusParser* p_pParser );
	~SemanticAnalyser(void);

private:
	void ReportError( Meson::Common::Text::String p_strMessage );

public:
	bool Check( ParserNodes::IntegerLiteral* p_pLiteral );
	bool Check( ParserNodes::RealLiteral* p_pLiteral );
	bool Check( ParserNodes::CharLiteral* p_pLiteral );
	bool Check( ParserNodes::StringLiteral* p_pLiteral );
	bool Check( ParserNodes::BoolLiteral* p_pLiteral );
	bool Check( ParserNodes::RefLiteral* p_pLiteral );

	//--------------------------------------------------------------------------------------------------------
	
	bool Check( ParserNodes::ExpressionNode* p_pNode );

	bool Check( ParserNodes::UnaryOperator* p_pNode );

	bool Check( ParserNodes::BinaryOperator* p_pNode );

	bool Check( ParserNodes::TernaryOperator* p_pNode );

	bool Check( ParserNodes::RelationalOperator* p_pNode );

	bool Check( ParserNodes::TypeCast* p_pNode );

	bool Check( ParserNodes::TypeNode* p_pNode );

	bool Check( ParserNodes::Identifier* p_pNode );

	bool Check( ParserNodes::QualifiedIdentifier* p_pNode );

	//--------------------------------------------------------------------------------------------------------

	bool Check( ParserNodes::Assignment* p_pNode );

	bool Check( ParserNodes::IfNode* p_pNode );

	bool Check( ParserNodes::WhileNode* p_pNode );

	bool Check( ParserNodes::LabelNode* p_pNode );

	bool Check( ParserNodes::ReturnNode* p_pNode );

	bool Check( ParserNodes::GotoNode* p_pNode );

	//--------------------------------------------------------------------------------------------------------

	bool Check( ParserNodes::Variable* p_pNode );

	bool Check( ParserNodes::MethodNode* p_pNode );
	
	bool Check( ParserNodes::ClassNode* p_pNode );

	bool Check( ParserNodes::InterfaceNode* p_pNode );
	
	//--------------------------------------------------------------------------------------------------------

	bool Check( ParserNodes::Imports* p_pNode );

	bool Check( ParserNodes::NamespaceNode* p_pNode );

	bool Check( ParserNodes::CompilationUnit* p_pUnit );

	bool Check( ParserNode* p_pNode );

};

Meson_Cuneus_Compiler_END
