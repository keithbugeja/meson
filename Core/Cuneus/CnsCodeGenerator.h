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
#include "CnsClassWriter.h"
#include "CnsSymbolTable.h"

#include <ArrayList.h>

Meson_Cuneus_Compiler_BEGIN

class CuneusCompiler;

class CodeGenerator
{
protected:
	Meson::Cuneus::RTS::Typing::DataTypeMgr* m_pDataTypeMgr;
	CuneusCompiler* m_pCompiler;
	SymbolTable*  m_pSymbolTable;
	Meson::Cuneus::RTS::ClassWriter*  m_pClassWriter;

	Meson::Common::Collections::TArrayList< dword > m_lstAddress;
	byte m_byRegIndex;
	byte m_byPushCount;

public:
	CodeGenerator( CuneusCompiler* p_pCompiler );
	~CodeGenerator( void );

	Meson::Cuneus::RTS::ClassWriter*  GetClassWriter( void );
	void SetClassWriter( Meson::Cuneus::RTS::ClassWriter*  p_pClassWriter );

	
	byte IncRegIndex( void );
	byte DecRegIndex( void );
	byte GetRegIndex( void );
	byte GetPreviousRegIndex( void );
	byte GetNextRegIndex( void );

	dword MarkAddress( void );
	void  WriteRelativeAddress( dword p_dwAddressIndex, int p_nRelativeAddress );

public:
	bool Generate( ParserNodes::IntegerLiteral* p_pLiteral );
	bool Generate( ParserNodes::RealLiteral* p_pLiteral );
	bool Generate( ParserNodes::CharLiteral* p_pLiteral );
	bool Generate( ParserNodes::StringLiteral* p_pLiteral );
	bool Generate( ParserNodes::BoolLiteral* p_pLiteral );
	bool Generate( ParserNodes::RefLiteral* p_pLiteral );

	//--------------------------------------------------------------------------------------------------------
	
	bool Generate( ParserNodes::ExpressionNode* p_pNode );

	bool Generate( ParserNodes::UnaryOperator* p_pNode );

	bool Generate( ParserNodes::UnaryMinus* p_pNode );

	bool Generate( ParserNodes::UnaryLogicalNot* p_pNode );

	bool Generate( ParserNodes::UnaryInc* p_pNode );

	bool Generate( ParserNodes::UnaryDec* p_pNode );

	bool Generate( ParserNodes::UnaryBitwiseNot* p_pNode );

	bool Generate( ParserNodes::UnaryPlus* p_pNode );


	bool Generate( ParserNodes::BinaryOperator* p_pNode );

	bool Generate( ParserNodes::OpLogicOr* p_pNode );

	bool Generate( ParserNodes::OpBitwiseOr* p_pNode );

	bool Generate( ParserNodes::OpBitwiseXor* p_pNode );

	bool Generate( ParserNodes::OpPlus* p_pNode );

	bool Generate( ParserNodes::OpMinus* p_pNode );

	bool Generate( ParserNodes::OpLogicAnd* p_pNode );

	bool Generate( ParserNodes::OpBitwiseAnd* p_pNode );

	bool Generate( ParserNodes::OpShl* p_pNode );

	bool Generate( ParserNodes::OpShr* p_pNode );

	bool Generate( ParserNodes::OpUShr* p_pNode );

	bool Generate( ParserNodes::OpMul* p_pNode );

	bool Generate( ParserNodes::OpDiv* p_pNode );

	bool Generate( ParserNodes::OpMod* p_pNode );


	bool Generate( ParserNodes::TernaryOperator* p_pNode );

	bool Generate( ParserNodes::RelationalOperator* p_pNode );

	bool Generate( ParserNodes::RelOpEqEq* p_pNode );

	bool Generate( ParserNodes::RelOpNotEq* p_pNode );

	bool Generate( ParserNodes::RelOpLtEq* p_pNode );

	bool Generate( ParserNodes::RelOpGtEq* p_pNode );

	bool Generate( ParserNodes::RelOpLt* p_pNode );

	bool Generate( ParserNodes::RelOpGt* p_pNode );


	bool Generate( ParserNodes::TypeCast* p_pNode );

	//bool Generate( ParserNodes::TypeNode* p_pNode );

	bool Generate( ParserNodes::Identifier* p_pNode );

	bool Generate( ParserNodes::QualifiedIdentifier* p_pNode );

	bool Generate( ParserNodes::VarNode* p_pNode );

	bool Generate( ParserNodes::ArrayNode* p_pNode );

	bool Generate( ParserNodes::ElementNode* p_pNode );

	//--------------------------------------------------------------------------------------------------------

	bool Generate( ParserNodes::Assignment* p_pNode );

	bool Generate( ParserNodes::IfNode* p_pNode );

	bool Generate( ParserNodes::WhileNode* p_pNode );

	bool Generate( ParserNodes::LabelNode* p_pNode );

	bool Generate( ParserNodes::ReturnNode* p_pNode );

	bool Generate( ParserNodes::GotoNode* p_pNode );

	bool Generate( ParserNodes::CallNode* p_pNode );

	//--------------------------------------------------------------------------------------------------------

	bool Generate( ParserNodes::Variable* p_pNode );

	bool Generate( ParserNodes::BlockVar* p_pNode );

	bool Generate( ParserNodes::BlockNode* p_pNode );

	bool Generate( ParserNodes::MethodNode* p_pNode );
	
	bool Generate( ParserNodes::ClassNode* p_pNode );

	bool Generate( ParserNodes::InterfaceNode* p_pNode );
	
	//--------------------------------------------------------------------------------------------------------

	bool Generate( ParserNodes::Imports* p_pNode );

	bool Generate( ParserNodes::NamespaceNode* p_pNode );

	bool Generate( ParserNodes::CompilationUnit* p_pUnit );

	bool Generate( ParserNode* p_pNode );

};

Meson_Cuneus_Compiler_END
