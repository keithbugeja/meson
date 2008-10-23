#pragma once

#include "CnsToken.h"
#include "CnsTokenStream.h"
#include "CnsSymbolTable.h"
#include "CnsLabelMgr.h"
#include "CnsAccessModifiable.h"
#include "CnsCompilationUnit.h"



#include <ArrayList.h>

// Forward Declaration
class CnsDataTypeMgr;



Meson_Cuneus_Compiler_BEGIN

// Forward Declaration
class CuneusCompiler;

namespace ParserNodes
{
	class MethodNode;
};

class CuneusParser
{
private:
	Meson::Common::TPointer< Token > m_pToken;
	TokenStream*  m_pTokenStream;
	ErrorTracker* m_pErrorTracker;
	SymbolTable*  m_pSymbolTable;
	LabelMgr&	  m_labelMgr;

	
	
	Meson::Common::Collections::TArrayList< Meson::Common::Text::String > m_lstScope;
	dword m_dwScope;

	enum ParseState { Fail = 0, Ok, Skip };

	word m_wParen;

public:
	CuneusParser( CuneusCompiler* p_pCompiler );
	virtual ~CuneusParser( void );
	
	void Init( void );

private:
	Meson::Common::TPointer< Token > NextToken( void );
	qword NextTokenValue( void );
	TokenType::TokenType NextTokenType( void );
	void PushBack( void );
	void PushBack( int p_nCount );

	int StreamMark( void );
	void StreamRestore( int p_nRestorePoint );

public:
	void ReportError( Meson::Common::Text::String p_strMessage );
	void ReportError( PositionInfo p_position, Meson::Common::Text::String& p_strMessage );

public:
	ParserNode* Parse( void );
	ParserNode* Parse( TokenStream* p_pTokenStream );

	void  IncScope( Meson::Common::Text::String& p_strName );
	void  DecScope( void );
	dword GetScope( void );
	void  SetScope( dword p_dwScope );

	
	//inline void IncParen( void ) { m_wParen++; }
	//inline void DecParen( void ) { m_wParen--; }
	//inline void ClearParen( void ) { m_wParen == 0; }
	//inline bool IsZeroParen( void ) { return (m_wParen == 0); }

	/*dword MarkScope( void );
	dword GetCurrentScope( void );
	void RestoreScope( dword p_unScope );*/

	dword GetScopeString( String& p_string );

private:

	ParseState ParseNodeName( ParserNode* p_pParserNode );

	ParseState ParseIdentifier( ParserNode* p_pParserNode );

	ParseState ParseQualifiedName( ParserNode* p_pParserNode );

	ParseState ParseQualifiedIdentifier( ParserNode* p_pParserNode );

	ParseState ParseImport( ParserNode* p_pParserNode );

	ParseState ParseDeclarations( ParserNode* p_pParserNode );

	ParseState ParseDeclaration( ParserNode* p_pParserNode );

	ParseState ParseNamespace( ParserNode* p_pParserNode );

	ParseState ParseVariableDeclaration( ParserNode* p_pParserNode );

	ParseState ParseAccessModifier( ParserNodes::AccessModifiable* p_pAccessModifiableNode );

	ParseState ParseModifiers( ParserNodes::AccessModifiable* p_pAccessModifiableNode );

	ParseState ParseBasicType( ParserNode* p_pParserNode );

	ParseState ParseType( ParserNode* p_pParserNode );

	ParseState ParseMethodDeclaration( ParserNode* p_pParserNode );

	ParseState ParseMethodSignature( ParserNodes::MethodNode* p_pMethodNode );

	ParseState ParseFormalParameters( ParserNode* p_pParserNode );

	ParseState ParseBlock( ParserNode* p_pParserNode );

	ParseState ParseStatement( ParserNode* p_pParserNode );

	ParseState ParseVariableDeclarationStatement( ParserNode* p_pParserNode );

	ParseState ParseAssignment( ParserNode* p_pParserNode );

	ParseState ParseIf( ParserNode* p_pParserNode );

	ParseState ParseWhile( ParserNode* p_pParserNode );

	ParseState ParseReturn( ParserNode* p_pParserNode );

	ParseState ParseGoto( ParserNode* p_pParserNode );

	ParseState ParseLabel( ParserNode* p_pParserNode );

	ParseState ParseExpression( ParserNode* p_pParserNode );

	ParseState ParseSimpleExpression( ParserNode* p_pParserNode );

	ParseState ParseTerm( ParserNode* p_pParserNode );

	ParseState ParseLiteral( ParserNode* p_pParserNode );

	ParseState ParseFactor( ParserNode* p_pParserNode );

	ParseState ParseComplexExpression( ParserNode* p_pParserNode );

	ParseState ParseCallExpression( ParserNode* p_pParserNode );

	ParseState ParseActualParameters( ParserNode* p_pParserNode );

	/*
	
	ParseState ParseNodeName( ParserNode* p_pParserNode, bool p_bIncreaseScope );

	ParseState ParseIdentifier( ParserNode* p_pParserNode, bool p_bIncreaseScope );

	ParseState ParseQualifiedIdentifier( ParserNode* p_pParserNode, bool p_bIncreaseScope );

	ParseState ParseImport( ParserNode* p_pParserNode );



	ParseState ParseQualifiedIdentifierList( ParserNode* p_pParserNode );

	ParseState ParseBasicType( ParserNode* p_pParserNode );

	ParseState ParseType( ParserNode* p_pParserNode );

	

	ParseState ParseNamespace( ParserNode* p_pParserNode );

	ParseState ParseDeclarations( ParserNode* p_pParserNode );

	ParseState ParseDeclaration( ParserNode* p_pParserNode );

	ParseState ParseClassDeclaration( ParserNode* p_pParserNode );

	ParseState ParseClassBody( ParserNode* p_pParserNode );

	ParseState ParseInterfaceDeclaration( ParserNode* p_pParserNode );

	ParseState ParseInterfaceBody( ParserNode* p_pParserNode );

	ParseState ParseMethodDeclaration( ParserNode* p_pParserNode );

	ParseState ParseMethodSignature( ParserNode* p_pParserNode );

	ParseState ParseFormalParameters( ParserNode* p_pParserNode );

	ParseState ParseVariableDeclaration( ParserNode* p_pParserNode );

	ParseState ParseAccessModifier( ParserNodes::AccessModifiable* p_pAccessModifiableNode );

	ParseState ParseModifiers( ParserNodes::AccessModifiable* p_pAccessModifiableNode );

	ParseState ParseExpression( ParserNode* p_pParserNode );

	ParseState ParseSimpleExpression( ParserNode* p_pParserNode );

	ParseState ParseTerm( ParserNode* p_pParserNode );

	ParseState ParseLiteral( ParserNode* p_pParserNode );

	ParseState ParseFactor( ParserNode* p_pParserNode );

	ParseState ParseComplexExpression( ParserNode* p_pParserNode );

	ParseState ParseCallExpression( ParserNode* p_pParserNode );

	ParseState ParseActualParameters( ParserNode* p_pParserNode );

	ParseState ParseBlock( ParserNode* p_pParserNode );

	ParseState ParseStatement( ParserNode* p_pParserNode );

	ParseState ParseAssignment( ParserNode* p_pParserNode );
	*/
};


Meson_Cuneus_Compiler_END
