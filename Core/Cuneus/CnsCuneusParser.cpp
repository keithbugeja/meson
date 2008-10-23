#include "CnsCuneusParser.h"

#include "CnsCuneusCompiler.h"
#include "CnsParserNodes.h"
#include "CnsAllDataTypes.h"
#include "CnsTypeCodes.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Cuneus::Compiler;
using namespace Meson::Cuneus::Compiler::ParserNodes;

using namespace Meson::Cuneus::RTS::Typing;

Meson_Cuneus_Compiler_BEGIN

CuneusParser::CuneusParser( CuneusCompiler* p_pCompiler ) 
	: m_pToken()
	, m_pTokenStream( p_pCompiler->GetTokenStream() )
	, m_labelMgr( *LabelMgr::GetInstance() )
{
	m_pErrorTracker = p_pCompiler->GetErrorTracker();
	m_pSymbolTable  = p_pCompiler->GetSymbolTable();

	Init();
}

CuneusParser::~CuneusParser( void )
{
}

void CuneusParser::Init( void )
{
	m_dwScope = 0;
	m_wParen = 0;
}


TPointer< Token > CuneusParser::NextToken( void )
{
	m_pToken = m_pTokenStream->NextToken();
	return m_pToken;
}

qword CuneusParser::NextTokenValue( void )
{
	m_pToken = m_pTokenStream->NextToken();
	return m_pToken->GetValue();
}

TokenType::TokenType CuneusParser::NextTokenType( void )
{
	m_pToken = m_pTokenStream->NextToken();
	return m_pToken->GetType();
}

void CuneusParser::PushBack( void )
{
	m_pTokenStream->PushBack(1);
	//NextToken();
}

void CuneusParser::PushBack( int p_nCount )
{
	m_pTokenStream->PushBack( p_nCount+1 );
	NextToken();
}


int CuneusParser::StreamMark( void )
{
	return m_pTokenStream->Mark();
}

void CuneusParser::StreamRestore( int p_nRestorePoint )
{
	m_pTokenStream->Restore( p_nRestorePoint );
	m_pToken = m_pTokenStream->NextToken();
}



void CuneusParser::ReportError( String p_strMessage )
{
	m_pErrorTracker->ReportError( m_pTokenStream->GetTokenPosition(), p_strMessage );
}

void CuneusParser::ReportError( PositionInfo p_position, String& p_strMessage )
{
	m_pErrorTracker->ReportError( p_position, p_strMessage );
}


void CuneusParser::IncScope( Meson::Common::Text::String& p_strName )
{
	if( m_dwScope == m_lstScope.Size() )
		m_lstScope.Add( p_strName );
	else
		m_lstScope.SetElement( m_dwScope, p_strName );
	m_dwScope++;
}

void CuneusParser::DecScope( void )
{
	m_dwScope--;
}

dword CuneusParser::GetScope( void )
{
	return m_dwScope;
}

void CuneusParser::SetScope( dword p_dwScope )
{
	m_dwScope = p_dwScope;
}

//dword CuneusParser::MarkScope( void )
//{
//	return (dword) m_lstScope.Size();
//}
//
//dword CuneusParser::GetCurrentScope( void )
//{
//	return (dword) m_lstScope.Size() - 1;
//}
//
//void CuneusParser::RestoreScope( dword p_unScope )
//{
//	if( p_unScope == 0 )
//		m_lstScope.Clear();
//	else if( p_unScope < m_lstScope.Size() )
//		m_lstScope.RemoveRange( p_unScope + 1, m_lstScope.Size() - p_unScope - 1 );
//}

dword CuneusParser::GetScopeString( String& p_string )
{
	for( dword dw=0; dw < m_dwScope; dw++ )
	{
		p_string += m_lstScope[ dw ];
		if( dw < m_dwScope - 1 )
			p_string += '.';
	}

	return m_dwScope;
}



ParserNode* CuneusParser::Parse( void )
{
	CuneusParser::ParseState state = Ok;

	ParserNodes::CompilationUnit* pNode = new ParserNodes::CompilationUnit();

	ParserNodes::Imports* pImports = new ParserNodes::Imports();
	

	//int nMark = StreamMark();

	while( NextTokenValue() == KeywordImport )
	{
		state = ParseImport( pImports );
		if( state == Fail )
		{
			ReportError( "Skipping Import declaration." );
		}
	}

	// Test if Any import statements have been found
	if( pImports->HasChildren() )
	{
		pNode->SetImports( pImports );
	}
	else
	{
		delete pImports;
		//StreamRestore( nMark );
	}

	if( state == Fail )
	{
		delete pNode;
		return NULL;
	}

	PushBack();
	while( !m_pToken->IsEOS() && state == Ok )
	{
		state = ParseDeclarations( pNode );
	}

	if( state == Fail )
	{
		delete pNode;
		return NULL;
	}

	return pNode;
}


ParserNode* CuneusParser::Parse( TokenStream* p_pTokenStream )
{
	if( !p_pTokenStream )
		return NULL;

	m_pTokenStream = p_pTokenStream;
	m_pTokenStream->Init();
	return Parse();
}

CuneusParser::ParseState CuneusParser::ParseImport( ParserNode* p_pParserNode )
{
	if( ParseQualifiedIdentifier( p_pParserNode ) )
	{
		if( NextTokenType() == TokenType::Terminator )
			return Ok;
	}

	ReportError( "Import declaration rejected." );

	return Fail;
}




CuneusParser::ParseState CuneusParser::ParseNodeName( ParserNode* p_pParserNode )
{
	if( NextTokenType() == TokenType::Identifier )
	{
		p_pParserNode->SetName( m_pToken->GetStringValue() );
		return Ok;
	}

	ReportError( "Identifier expected found '" + m_pToken->GetStringValue() + "' instead." );

	return Fail;
}


CuneusParser::ParseState CuneusParser::ParseIdentifier( ParserNode* p_pParserNode )
{
	if( NextTokenType() == TokenType::Identifier )
	{
		ParserNode* pNode = new ParserNodes::Identifier( m_pToken->GetStringValue() );
		p_pParserNode->AddChild( pNode );

		return Ok;
	}

	ReportError( "Identifier expected found '" + m_pToken->GetStringValue() + "' instead." );

	return Fail;
}

CuneusParser::ParseState CuneusParser::ParseQualifiedIdentifier( ParserNode* p_pParserNode )
{
	ParserNode* pNode = new ParserNodes::QualifiedIdentifier();
	
	if( ParseIdentifier( pNode ) == Ok )
	{
		while( NextToken()->GetStringValue() == "." )
		{
			if( !ParseIdentifier( pNode ) )
				goto PQI_Error;
		}
		
		PushBack();
		p_pParserNode->AddChild( pNode );	
		pNode->SetName( pNode->ToString() );
		return Ok;
	}

PQI_Error:
	delete pNode;

	ReportError( "Qualified Identifier not parsed correctly." );
	
	return Fail;
}


CuneusParser::ParseState CuneusParser::ParseQualifiedName( ParserNode* p_pParserNode )
{

	String strName;

	if( NextTokenType() == TokenType::Identifier )
	{
		strName += m_pToken->GetStringValue();

		while( NextToken()->GetStringValue() == '.' )
		{
			strName += '.';

			if( NextTokenType() == TokenType::Identifier )
				strName += m_pToken->GetStringValue();
			else
				goto PQN_Error;
		}

		PushBack();
		p_pParserNode->SetName( strName );
		return Ok;

	}

	return Skip;

PQN_Error:
	ReportError( "Qualified Name not parsed correctly." );
	
	return Fail;
}

CuneusParser::ParseState CuneusParser::ParseDeclarations( ParserNode* p_pParserNode )
{
	
	CuneusParser::ParseState state = Ok;

	while( !NextToken()->IsEOS() && state == Ok )
	{
		if( m_pToken->GetType() == TokenType::BlockEnd )
		{
			PushBack();
			return Skip;
		}

		//PushBack();
		state = ParseDeclaration( p_pParserNode );
		
	}

	return state;
}


CuneusParser::ParseState CuneusParser::ParseDeclaration( ParserNode* p_pParserNode )
{
	//CuneusParser::ParseState state = ParseClassDeclaration( p_pParserNode );

	//if( state < Skip )
	//	return state;
	//
	//state = ParseInterfaceDeclaration( p_pParserNode );

	//if( state < Skip )
	//	return state;

	if( m_pToken->GetType() == TokenType::BlockEnd || m_pToken->GetType() == TokenType::Terminator )
	{
		PushBack();
		return Skip;
	}

	int nStreamMark = StreamMark();

	CuneusParser::ParseState state = ParseNamespace( p_pParserNode );

	if( state < Skip )
		return state;

	StreamRestore( nStreamMark );

	state = ParseMethodDeclaration( p_pParserNode );

	if( state < Skip )
		return state;

	StreamRestore( nStreamMark );

	return ParseVariableDeclaration( p_pParserNode );

}


CuneusParser::ParseState CuneusParser::ParseNamespace( ParserNode* p_pParserNode )
{
	if( m_pToken->GetValue() != KeywordNamespace )
		return Skip;

	NamespaceNode* pNamespace = new NamespaceNode();

	if( ParseNodeName( pNamespace ) != Ok )
	{
		ReportError( "Namespace declaration rejected." );
		delete pNamespace;
		return Fail;
	}
	else
	{
		p_pParserNode->AddChild( pNamespace );
		IncScope( pNamespace->GetName() );
		
		SymbolTableEntry* pEntry = NULL;
		if( !m_pSymbolTable->Contains( pNamespace->GetQualifiedName(), pEntry ) )
		{
			pEntry = m_pSymbolTable->Insert( pNamespace->GetQualifiedName() );
			pEntry->SetNode( pNamespace );
			pEntry->SetScope( GetScope() );
		}
	}
	

	if( NextTokenType() != TokenType::BlockBegin )
	{
		ReportError( " '{' expected." );
		p_pParserNode->RemoveLastChild();
		DecScope();
		delete pNamespace;
		return Fail;
	}
	
	if( m_pToken->GetType() == TokenType::BlockEnd )
	{
		DecScope();
		return Ok;
	}

	if( ParseDeclarations( pNamespace ) == Fail )
	{
		ReportError( "Namespace declaration rejected." );
		p_pParserNode->RemoveLastChild();
		DecScope();
//TODO: Check for error
		delete pNamespace;
		return Fail;
	}
	
	if( NextTokenType() != TokenType::BlockEnd )
	{
		ReportError( " '}' expected." );
		p_pParserNode->RemoveLastChild();
		DecScope();
		delete pNamespace;
		return Fail;
	}

	DecScope();
	
	return Ok;
}




CuneusParser::ParseState CuneusParser::ParseVariableDeclaration( ParserNode* p_pParserNode )
{
	Variable* pVariable = new Variable();
	
	ParseAccessModifier( pVariable );

	if( ParseType( pVariable ) != Ok )
	{
		delete pVariable;
		return Skip;
	}

	if( ParseNodeName( pVariable ) != Ok )
	{
		delete pVariable;
		return Fail;
	}

	if( NextTokenType() != TokenType::Terminator )
	{
		delete pVariable;
		return Fail;
	}
	
	p_pParserNode->AddChild( pVariable );

	SymbolTableEntry* pEntry = NULL;
	if( m_pSymbolTable->Contains( pVariable->GetQualifiedName(), pEntry ) )
	{
		ReportError( "Variable already exists!" );
		p_pParserNode->RemoveLastChild();
		delete pVariable;
		return Fail;
	}

	pEntry = m_pSymbolTable->Insert( pVariable->GetQualifiedName() );
	pEntry->SetNode( pVariable );
	pEntry->SetTypeCode( pVariable->GetTypeCode() );
	pEntry->SetScope( GetScope() );
	pEntry->SetEntryType( SymbolTableEntryType::Global );

	return Ok;
}


CuneusParser::ParseState CuneusParser::ParseAccessModifier( ParserNodes::AccessModifiable* p_pAccessModifiableNode )
{
	//int nStreamMark = StreamMark();

	switch( m_pToken->GetValue() )
	{
		case KeywordPublic:
			p_pAccessModifiableNode->SetPublic( true );
			//NextToken();
			return Ok;
		
		case KeywordProtected:
			p_pAccessModifiableNode->SetProtected( true );
			//NextToken();
			return Ok;
		
		case KeywordPrivate:
			p_pAccessModifiableNode->SetPrivate( true );
			//NextToken();
			return Ok;
	}

	//StreamRestore(nStreamMark );
	PushBack();
	return Skip;
}


CuneusParser::ParseState CuneusParser::ParseModifiers( ParserNodes::AccessModifiable* p_pAccessModifiableNode )
{
	bool bFound = false;

	if( NextTokenValue() == KeywordStatic )
	{
		p_pAccessModifiableNode->SetStatic( true );
		bFound = true;
	}
	else
	{
		PushBack();
	}

	if( NextTokenValue() == KeywordSynchronized )
	{
		p_pAccessModifiableNode->SetSync( true );
		bFound = true;
	}
	else
	{
		PushBack();
	}


	if( NextTokenValue() == KeywordAbstract )
	{
		p_pAccessModifiableNode->SetAbstract( true );
		bFound = true;
	}
	else
	{
		PushBack();
	}


	if( NextTokenValue() == KeywordFinal )
	{
		p_pAccessModifiableNode->SetFinal( true );
		bFound = true;
	}
	else
	{
		PushBack();
	}


	if( NextTokenValue() == KeywordNative )
	{
		p_pAccessModifiableNode->SetNative( true );
		bFound = true;
	}
	else
	{
		PushBack();
	}

	
	if( !bFound )
	{
		//PushBack();
		return Skip;
	}

	return Ok;
}



CuneusParser::ParseState CuneusParser::ParseBasicType( ParserNode* p_pParserNode )
{
	TypeNode* pType = NULL;

	//int nStreamMark = StreamMark();

	switch( NextTokenValue() )
	{
	case KeywordByte: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_BYTE );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordChar: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_CHAR );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordWord: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_WORD );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordShort: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_SHORT );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordDword: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_DWORD );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordInt: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_INTEGER );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordQword: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_QWORD );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordLong: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_LONG );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordFloat: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_FLOAT );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordDouble: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_DOUBLE );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordString:
		pType = new TypeNode( "string" );
		pType->SetTypeCode( CNS_TC_STRING );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordBoolean: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_BOOLEAN );
		p_pParserNode->SetType( pType );
		return Ok;

	case KeywordVoid: 
		pType = new TypeNode( m_pToken->GetStringValue() );
		pType->SetTypeCode( CNS_TC_VOID );
		p_pParserNode->SetType( pType );
		return Ok;
	}

	PushBack();

	//StreamRestore( nStreamMark );

	return Fail;
}

CuneusParser::ParseState CuneusParser::ParseType( ParserNode* p_pParserNode )
{
	TypeNode* pType;
	if( ParseBasicType( p_pParserNode ) != Ok )
	{

		pType = new TypeNode();

		if( ParseQualifiedIdentifier( pType ) != Ok )
			return Fail;

	}
	
	pType = p_pParserNode->GetType();

	ArrayNode* pArrayNode;

	if( NextTokenType() == TokenType::LBracket )
	{	
		pArrayNode = new ArrayNode( pType );

		if( ParseExpression( pArrayNode ) != Ok )
		{
			ReportError( "Error in array length definition." );
			return Fail;
		}

		if( NextTokenType() != TokenType::RBracket )
		{
			ReportError( "Missing ] in array definition." );
			return Fail;
		}
		p_pParserNode->AddChild( pArrayNode );
	}
	else
		PushBack();
	
	
	/*while( NextTokenType() == TokenType::ArrayDefn )
	{	
		pType = new ArrayNode( pType );
	}

	PushBack();*/


	//if( pType->IsArray() )
	//{
	//	p_pParserNode->AddChild( pType );
	//	p_pParserNode->SetType( pType );
	//}
	//else
	//{
	//	p_pParserNode->SetType( pType );
	//}

	return Ok;
}




CuneusParser::ParseState CuneusParser::ParseMethodDeclaration( ParserNode* p_pParserNode )
{
	MethodNode* pMethod = new MethodNode();
	pMethod->SetInterface( false );
	p_pParserNode->AddChild( pMethod );

	CuneusParser::ParseState state = ParseMethodSignature( pMethod );

	if( state == Skip || state == Fail )
	{
		p_pParserNode->RemoveLastChild();
		return state;
	}
	else 
	{
		if( NextTokenType() == TokenType::Terminator )
			return Ok;
		else
			PushBack();

		IncScope( pMethod->GetName() );
		state = ParseBlock( pMethod );
		DecScope();
		return state;
	}
}

CuneusParser::ParseState CuneusParser::ParseMethodSignature( MethodNode* p_pMethodNode )
{
	ParseAccessModifier( p_pMethodNode );
	ParseModifiers( p_pMethodNode );

	if( ParseType( p_pMethodNode ) != Ok )
	{
		ReportError( "Type expected!" );
		return Skip;
	}

	if( ParseNodeName( p_pMethodNode ) != Ok )
	{
		ReportError( "Identifier expected!" );
		return Skip;
	}

	if( NextTokenType() != TokenType::LParen )
	{
		//ReportError( " '(' expected!" );
		return Skip;
	}	

	SymbolTableEntry* pEntry = NULL;
	if( m_pSymbolTable->Contains( p_pMethodNode->GetQualifiedName(), pEntry ) )
	{
		ReportError( "Method already defined!" );
		return Fail;
	}

	pEntry = m_pSymbolTable->Insert( p_pMethodNode->GetQualifiedName() );
	pEntry->SetNode( p_pMethodNode );
	pEntry->SetScope( GetScope() );

	ParserNode* pParams = new ParserNode( "Params" );

	p_pMethodNode->AddChild( pParams );

	if( ParseFormalParameters( pParams ) == Fail )
	{
		p_pMethodNode->RemoveLastChild();
		delete pParams;
		return Fail;
	}

	if( NextTokenType() != TokenType::RParen )
	{
		ReportError( " ')' expected!" );
		return Fail;
	}	

	return Ok;
}

CuneusParser::ParseState CuneusParser::ParseFormalParameters( ParserNode* p_pParserNode )
{
	if( NextTokenType() == TokenType::RParen )
	{
		PushBack();
		return Ok;
	}

	PushBack();
	
	do
	{
		ParserNode* pParam = new ParserNode( "Param" );
		

		if( !( ParseType( pParam ) && ParseNodeName( pParam ) ) )
		{
			delete pParam;
			ReportError( "Error in param declaration." );
			return Fail;
		}
		else
		{
			p_pParserNode->AddChild( pParam );

			ParserNode* pNode = p_pParserNode->GetParent();

			SymbolTableEntry* pEntry = m_pSymbolTable->Insert(  pNode->GetQualifiedName() + "." + pParam->GetName() );

			pEntry->SetNode( pParam );
			pEntry->SetTypeCode( pParam->GetTypeCode() );
			pEntry->SetScope( GetScope() );
			
			pEntry->SetEntryType( SymbolTableEntryType::Param );

		}

	} while( NextTokenType() == TokenType::Comma );


	PushBack();
	return Ok;
}

//---------------------------------------------------------------------------------------------------------

CuneusParser::ParseState CuneusParser::ParseBlock( ParserNode* p_pParserNode )
{

	BlockNode* pBlock = new BlockNode();
	p_pParserNode->AddChild( pBlock );

	pBlock->SetName( m_labelMgr.CreateLabel( "$BLK" ) );
	IncScope( pBlock->GetName() );

	SymbolTableEntry* pEntry = m_pSymbolTable->Insert( pBlock->GetQualifiedName() );
	pEntry->SetNode( pBlock );
	pEntry->SetScope( GetScope() );

	if( NextTokenType() != TokenType::BlockBegin )
	{
		ReportError( " '{' expected." );
		DecScope();
		return Fail;
	}

	ParseState state = Ok;

	while( !NextToken()->IsEOS() && state == Ok )
	{
		if( m_pToken->GetType() == TokenType::BlockEnd )
		{
			DecScope();
			return Ok;
		}
		
		PushBack();
		if( ParseStatement( pBlock ) != Ok )
		{
			DecScope();
			return Fail;
		}
	}

	DecScope();
	if( m_pToken->GetType() != TokenType::BlockEnd )
		return Fail;

	return Ok;
}

CuneusParser::ParseState CuneusParser::ParseStatement( ParserNode* p_pParserNode )
{
	int nStreamMark = StreamMark();

	ParseState state = ParseGoto( p_pParserNode );
	
	if( state == Ok )
		return state;
	
	StreamRestore( nStreamMark );
	
	state = ParseIf( p_pParserNode );
	
	if( state == Ok )
		return state;
	
	StreamRestore( nStreamMark );

	state = ParseReturn( p_pParserNode );
	
	if( state == Ok )
		return state;
	
	StreamRestore( nStreamMark );
	
	state = ParseWhile( p_pParserNode );
	
	if( state == Ok )
		return state;

	StreamRestore( nStreamMark );

	state = ParseAssignment( p_pParserNode );

	if( state < Skip )
		return state;

	StreamRestore( nStreamMark );

	state = ParseVariableDeclarationStatement( p_pParserNode );

	if( state < Skip )
		return state;

	StreamRestore( nStreamMark );

	state = ParseLabel( p_pParserNode );

	if( state < Skip )
		return state;

	StreamRestore( nStreamMark );

	state = ParseExpression( p_pParserNode );

	if( state == Ok )
	{
		if( NextTokenType() == TokenType::Terminator )
			return Ok;
		else
			PushBack();
	}

	ReportError( "Statement Expected." );
	return Fail;
}



CuneusParser::ParseState CuneusParser::ParseIf( ParserNode* p_pParserNode )
{
	if( NextTokenValue() != KeywordIf )
		return Skip;

	if( NextTokenType() != TokenType::LParen )
	{
		ReportError( " '(' expected! " );
		return Fail;
	}

	IfNode* pIf = new IfNode();
	p_pParserNode->AddChild( pIf );

	CuneusParser::ParseState state = ParseExpression( pIf );

	if( state == Skip || state == Fail )
	{
		p_pParserNode->RemoveLastChild();
		delete pIf;
		ReportError( " Error while parsing condition! " );
		return Fail;
	}
	
	if( NextTokenType() != TokenType::RParen )
	{
		ReportError( " ')' expected! " );
		return Fail;
	}

	state = ParseBlock( pIf );
	if( state == Skip || state == Fail )
	{
		p_pParserNode->RemoveLastChild();
		delete pIf;
		ReportError( " Error while parsing Then block! " );
		return Fail;
	}

	if( NextTokenValue() != KeywordElse )
	{
		PushBack();
		return Ok;
	}

	return ParseBlock( pIf );
}

CuneusParser::ParseState CuneusParser::ParseWhile( ParserNode* p_pParserNode )
{
	if( NextTokenValue() != KeywordWhile )
		return Skip;

	if( NextTokenType() != TokenType::LParen )
	{
		ReportError( " '(' expected! " );
		return Fail;
	}

	WhileNode* pWhile = new WhileNode();
	p_pParserNode->AddChild( pWhile );

	CuneusParser::ParseState state = ParseExpression( pWhile );

	if( state == Skip || state == Fail )
	{
		p_pParserNode->RemoveLastChild();
		delete pWhile;
		ReportError( " Error while parsing condition! " );
		return Fail;
	}
	
	if( NextTokenType() != TokenType::RParen )
	{
		ReportError( " ')' expected! " );
		return Fail;
	}

	return ParseBlock( pWhile );
}

CuneusParser::ParseState CuneusParser::ParseReturn( ParserNode* p_pParserNode )
{
	if( NextTokenValue() != KeywordReturn )
		return Skip;
	
	ReturnNode* pReturn = new ReturnNode();

	if( NextTokenType() == TokenType::Terminator )
	{
		p_pParserNode->AddChild( pReturn );
		return Ok;
	}

	PushBack();

	if( ParseExpression( pReturn ) != Ok )
	{
		delete pReturn;
		return Fail;
	}

	if( NextTokenType() != TokenType::Terminator )
	{
		delete pReturn;
		return Fail;
	}

	p_pParserNode->AddChild( pReturn );

	return Ok;
}

CuneusParser::ParseState CuneusParser::ParseGoto( ParserNode* p_pParserNode )
{
	if( NextTokenValue() != KeywordGoto )
		return Skip;

	GotoNode* pGoto = new GotoNode();

	if( ParseNodeName( pGoto ) != Ok )
	{
		delete pGoto;
		return Skip;
	}

	if( NextTokenType() != TokenType::Terminator )
	{
		delete pGoto;
		return Skip;
	}

	p_pParserNode->AddChild( pGoto );

	return Ok;
}

CuneusParser::ParseState CuneusParser::ParseLabel( ParserNode* p_pParserNode )
{
	LabelNode* pLabel = new LabelNode();

	if( ParseNodeName( pLabel ) != Ok )
	{
		delete pLabel;
		return Skip;
	}

	if( NextToken()->GetStringValue() != ":" )
	{
		delete pLabel;
		return Skip;
	}
	
	p_pParserNode->AddChild( pLabel );

	SymbolTableEntry* pEntry = NULL;
	if( m_pSymbolTable->Contains( pLabel->GetQualifiedName(), pEntry ) )
	{
		ReportError( "Label name already defined!" );
		return Fail;
	}

	pEntry = m_pSymbolTable->Insert( pLabel->GetQualifiedName() );
	pEntry->SetNode( pLabel );
	pEntry->SetScope( GetScope() );
	pEntry->SetTargetOffset( 0xFFFFFFFF );

	return Ok;
}



CuneusParser::ParseState CuneusParser::ParseVariableDeclarationStatement( ParserNode* p_pParserNode )
{
	BlockVar* pVariable = new BlockVar();
	
	//ParseAccessModifier( pVariable );

	if( ParseType( pVariable ) != Ok)
	{
		delete pVariable;
		return Skip;
	}

	if( NextTokenType() == TokenType::Identifier )
	{
		pVariable->SetName( m_pToken->GetStringValue() );
	}
	else
	{
		delete pVariable;
		return Skip;
	}

	if( NextTokenType() != TokenType::Terminator )
	{
		delete pVariable;
		return Skip;
	}
	
	p_pParserNode->AddChild( pVariable );

	SymbolTableEntry* pEntry = NULL;
	if( m_pSymbolTable->Contains( pVariable->GetQualifiedName(), pEntry ) )
	{
		ReportError( "Variable already exists!" );
		p_pParserNode->RemoveLastChild();
		delete pVariable;
		return Fail;
	}

	pEntry = m_pSymbolTable->Insert( pVariable->GetQualifiedName() );
	pEntry->SetNode( pVariable );
	pEntry->SetTypeCode( pVariable->GetTypeCode() );
	pEntry->SetScope( GetScope() );
	pEntry->SetEntryType( SymbolTableEntryType::Local );
	pEntry->SetTargetOffset( p_pParserNode->GetLocalVars() );

	p_pParserNode->IncLocalVars();

	return Ok;
}



CuneusParser::ParseState CuneusParser::ParseAssignment( ParserNode* p_pParserNode )
{

	Assignment* pAssignment = new Assignment( "=" );
	
	ParseState state = ParseQualifiedIdentifier( pAssignment );

	if( state != Ok )
	{
		delete pAssignment;
		return Skip;
	}

	String strScope;
	GetScopeString( strScope );
	strScope += '.';
	strScope += pAssignment->GetChild(0)->ToString();

	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( strScope );
	if( !pEntry )
	{
		ReportError( "Undefined Variable/Method in LHS" );
		return Fail;
	}


	if( NextTokenType() == TokenType::LBracket )
	{
		ElementNode* pElementNode = new ElementNode();
		pElementNode->SetName( pEntry->GetName() );

		if( ParseExpression( pElementNode ) != Ok )
		{
			ReportError( " Error in array Index!" );
			delete pElementNode;
			return Fail;
		}

		if( NextTokenType() != TokenType::RBracket )
		{
			ReportError( " ']' expected!" );
			delete pElementNode;
			return Fail;
		}	

		pAssignment->RemoveLastChild();
		pAssignment->AddChild( pElementNode );
		pEntry->SetNode(pElementNode);
	}
	else
	{
		PushBack();
	}

	if( NextTokenValue() != OperatorEq )
	{
		delete pAssignment;
		return Skip;
	}


	state = ParseExpression( pAssignment );
	if( state != Ok )
	{
		delete pAssignment;
		return Fail;
	}

	
	if( NextTokenType() != TokenType::Terminator )
	{
		delete pAssignment;
		ReportError(" ';' expected.");
		return Fail;
	}

	p_pParserNode->AddChild( pAssignment );
	return Ok;
}

//---------------------------------------------------------------------------------------------------------

CuneusParser::ParseState CuneusParser::ParseExpression( ParserNode* p_pParserNode )
{
	ExpressionNode* pExpression = new ExpressionNode();

	if( ParseSimpleExpression( pExpression ) != Ok )
	{
		delete pExpression;
		return Fail;
	}

	if( NextTokenType() == TokenType::Terminator )
	{
		p_pParserNode->AddChild( pExpression );
		PushBack();
		return Ok;
	}

	if( m_pToken->GetStringValue() == "?" )
	{

		TernaryOperator* pOperator = new TernaryOperator();
		pOperator->AddChild( pExpression );

		pExpression = new ExpressionNode();
		if( ParseSimpleExpression( pExpression ) != Ok )
		{
			delete pExpression;
			delete pOperator;
			return Fail;
		}
		pOperator->AddChild( pExpression );

		if( NextToken()->GetStringValue() == ":" )
		{
			pExpression = new ExpressionNode();
			if( !ParseSimpleExpression( pExpression ) )
			{
				delete pExpression;
				delete pOperator;
				return Fail;
			}
			pOperator->AddChild( pExpression );
			p_pParserNode->AddChild( pOperator );
			return Ok;
		}
		else
		{
			ReportError( " ':' expected in ternary operator." );
			delete pExpression;
			delete pOperator;
			return Fail;
		}

	}
	else
	{
		RelationalOperator* pOperator = NULL;
		switch( m_pToken->GetValue() )
		{
			case OperatorEqEq:
				pOperator = new RelOpEqEq();
				pOperator->AddChild( pExpression );
				break;

			case OperatorNotEq:
				pOperator = new RelOpNotEq();
				pOperator->AddChild( pExpression );
				break;
			case OperatorLtEq:
				pOperator = new RelOpLtEq();
				pOperator->AddChild( pExpression );
				break;

			case OperatorGtEq:
				pOperator = new RelOpGtEq();
				pOperator->AddChild( pExpression );
				break;
			case OperatorLt:
				pOperator = new RelOpLt();
				pOperator->AddChild( pExpression );
				break;

			case OperatorGt:
				pOperator = new RelOpGt();
				pOperator->AddChild( pExpression );
				break;

			default:
				if( m_pToken->GetType() == TokenType::RParen ||
					m_pToken->GetType() == TokenType::RBracket ||
					m_pToken->GetType() == TokenType::Comma ||
					m_pToken->GetType() == TokenType::Terminator )
				{
					p_pParserNode->AddChild( pExpression );
					PushBack();
					return Ok;
				}
				else
				{
					ReportError( "Error in expression" );
					delete pExpression;
					return Fail;
				}
		}

		pExpression = new ExpressionNode();
		if( ParseSimpleExpression( pExpression ) != Ok )
		{
			delete pExpression;
			delete pOperator;
			return Fail;
		}

		pOperator->AddChild( pExpression );
		p_pParserNode->AddChild( pOperator );
		return Ok;
	}

	p_pParserNode->AddChild( pExpression );
	return Ok;
}

CuneusParser::ParseState CuneusParser::ParseSimpleExpression( ParserNode* p_pParserNode )
{
	ExpressionNode* pExpression = new ExpressionNode();
	
	if( ParseTerm( pExpression ) != Ok )
	{
		ReportError( "Error While parsing Simple Expression." );
		delete pExpression;
		return Fail;
	}

	BinaryOperator* pOperator = NULL;
	switch( NextTokenValue() )
	{
	
	case OperatorOr:
		pOperator = new OpLogicOr();
		pOperator->AddChild( pExpression );
		break;

	case OperatorBitOr:
		pOperator = new OpBitwiseOr();
		pOperator->AddChild( pExpression );
		break;

	case OperatorBitXor:
		pOperator = new OpBitwiseXor();
		pOperator->AddChild( pExpression );
		break;

	case OperatorAdd:
		pOperator = new OpPlus();
		pOperator->AddChild( pExpression );
		break;

	case OperatorSub:
		pOperator = new OpMinus();
		pOperator->AddChild( pExpression );
		break;

	default:
		/*PushBack();
		p_pParserNode->Merge( pExpression );
		delete pExpression;*/
		
		PushBack();
		p_pParserNode->AddChild( pExpression );
		return Ok;
	}

	pExpression = new ExpressionNode();
	//ParseTerm vs ParseExpression
	if( ParseExpression( pExpression ) != Ok)
	{
		ReportError( "Error while parsing Simple expression." );
		delete pOperator;
		delete pExpression;
		return Fail;
	}

	pOperator->AddChild( pExpression );
	p_pParserNode->AddChild( pOperator );
	return Ok;
}

CuneusParser::ParseState CuneusParser::ParseTerm( ParserNode* p_pParserNode )
{
	ExpressionNode* pExpression = new ExpressionNode();
	
	if( ParseFactor( pExpression ) != Ok )
	{
		ReportError( "Error while parsing expression term." );
		delete pExpression;
		return Fail;
	}

	BinaryOperator* pOperator = NULL;
	UnaryOperator*  pUniOperator = NULL;
	switch( NextTokenValue() )
	{

		case OperatorInc:
			pUniOperator = new UnaryInc( true );
			pUniOperator->AddChild( pExpression );
			p_pParserNode->AddChild( pUniOperator );
			return Ok;

		case OperatorDec:
			pUniOperator = new UnaryDec( true );
			pUniOperator->AddChild( pExpression );
			p_pParserNode->AddChild( pUniOperator );
			return Ok;

		case OperatorAnd:
			pOperator = new OpLogicAnd();
			pOperator->AddChild( pExpression );
			break;

		case OperatorBitAnd:
			pOperator = new OpBitwiseAnd();
			pOperator->AddChild( pExpression );
			break;

		case OperatorShl:
			pOperator = new OpShl();
			pOperator->AddChild( pExpression );
			break;

		case OperatorShr:
			pOperator = new OpShr();
			pOperator->AddChild( pExpression );
			break;

		case OperatorUShr:
			pOperator = new OpUShr();
			pOperator->AddChild( pExpression );
			break;

		case OperatorMul:
			pOperator = new OpMul();
			pOperator->AddChild( pExpression );
			break;

		case OperatorDiv:
			pOperator = new OpDiv();
			pOperator->AddChild( pExpression );
			break;

		case OperatorMod:
			pOperator = new OpMod();
			pOperator->AddChild( pExpression );
			break;
	
		default:
			PushBack();
			p_pParserNode->AddChild( pExpression );
			return Ok;
	}

	pExpression = new ExpressionNode();
	//ParseFactor vs ParseExpression
	if( ParseExpression( pExpression ) != Ok )
	{
		ReportError( "Error while parsing expression term." );
		delete pOperator;
		delete pExpression;
		return Fail;
	}

	pOperator->AddChild( pExpression );
	p_pParserNode->AddChild( pOperator );
	return Ok;
}

CuneusParser::ParseState CuneusParser::ParseLiteral( ParserNode* p_pParserNode )
{
	Literal* pLiteral = NULL;
	String str;

	switch( NextTokenType() )
	{
		case TokenType::Integer:
			
			long long llValue;
			m_pToken->GetValue( llValue );
			
			str+=llValue;
			pLiteral = new IntegerLiteral( str );
			pLiteral->SetDataType( new TypeLong( llValue ) );

			p_pParserNode->AddChild( pLiteral );
			return Ok;

		case TokenType::Real:
			
			double dfValue;
			m_pToken->GetValue( dfValue );
			
			str+=dfValue;
			pLiteral = new RealLiteral( str );

			pLiteral->SetDataType( new TypeDouble( dfValue ) );
			p_pParserNode->AddChild( pLiteral );
			return Ok;

		case TokenType::LiteralChar:
			m_pToken->GetValue( llValue );

			str += (char)llValue;
			pLiteral = new CharLiteral( str );
			pLiteral->SetDataType( new TypeChar( (char)llValue ) );

			p_pParserNode->AddChild( pLiteral );
			return Ok;

		case TokenType::LiteralStr:
			pLiteral = new StringLiteral( m_pToken->GetStringValue() );
			pLiteral->SetDataType( new TypeString( m_pToken->GetStringValue() ) );

			p_pParserNode->AddChild( pLiteral );
			return Ok;

		case TokenType::Keyword:
			{
				if( m_pToken->GetStringValue() == "true" )
				{
					pLiteral = new BoolLiteral("True");
					pLiteral->SetDataType( new TypeBool( true ) );
					p_pParserNode->AddChild( pLiteral );
					return Ok;
				}
				else if( m_pToken->GetStringValue() == "false" )
				{
					pLiteral = new BoolLiteral("False");
					pLiteral->SetDataType( new TypeBool( false ) );
					p_pParserNode->AddChild( pLiteral );
					return Ok;
				}
				else if( m_pToken->GetStringValue() == "null" )
				{
					pLiteral = new RefLiteral("NULL");
					pLiteral->SetDataType( new TypeRef( NULL ) );
					p_pParserNode->AddChild( pLiteral );
					return Ok;
				}
			}
	}

	PushBack();
	return Skip;
}


CuneusParser::ParseState CuneusParser::ParseFactor( ParserNode* p_pParserNode )
{

	ParseState state = Ok;

RightFactor:
	if( ParseLiteral( p_pParserNode ) == Ok )
		return Ok;

	if( ParseComplexExpression( p_pParserNode ) == Ok )
		return Ok;

	if( NextTokenType() == TokenType::LParen )
	{

		//int nMark = this->StreamMark();

		//StreamRestore( nMark + 1 );
		TypeCast* pTypeCast = new TypeCast();

		if( ParseType( pTypeCast ) == Ok )
		{
			if( NextTokenType() == TokenType::RParen )
			{
				p_pParserNode->AddChild( pTypeCast );
				return Ok;
			}
			else
			{
				delete pTypeCast;
				PushBack();
			}
		}
		else
		{
			delete pTypeCast;
			PushBack();
		}

		
		
		ExpressionNode* pExpresson = new ExpressionNode();

		if( ParseExpression( pExpresson ) == Ok )
		{
			p_pParserNode->AddChild( pExpresson );

			if( NextTokenType() != TokenType::RParen )
			{
				ReportError( " ')' expected!" );
				delete pExpresson;
				return Fail;
			}

			return Ok;
		}
		else
			delete pExpresson;
	}


	UnaryOperator* pUniOperator = NULL;

	switch( m_pToken->GetValue() )
	{
		case OperatorInc:
			pUniOperator = new UnaryInc( false );
			p_pParserNode->AddChild( pUniOperator );
			p_pParserNode = pUniOperator;
			goto RightFactor;

		case OperatorDec:
			pUniOperator = new UnaryDec( false );
			p_pParserNode->AddChild( pUniOperator );
			p_pParserNode = pUniOperator;
			goto RightFactor;

		case OperatorNot:
			pUniOperator = new UnaryLogicalNot();
			p_pParserNode->AddChild( pUniOperator );
			p_pParserNode = pUniOperator;
			goto RightFactor;

		case OperatorBitNot:
			pUniOperator = new UnaryBitwiseNot();
			p_pParserNode->AddChild( pUniOperator );
			p_pParserNode = pUniOperator;
			goto RightFactor;

		case OperatorAdd:
			pUniOperator = new UnaryPlus();
			p_pParserNode->AddChild( pUniOperator );
			p_pParserNode = pUniOperator;
			goto RightFactor;

		case OperatorSub:
			pUniOperator = new UnaryMinus();
			p_pParserNode->AddChild( pUniOperator );
			p_pParserNode = pUniOperator;
			goto RightFactor;
	}

	return Fail;
}

CuneusParser::ParseState CuneusParser::ParseComplexExpression( ParserNode* p_pParserNode )
{
	return ParseCallExpression( p_pParserNode );
}


CuneusParser::ParseState CuneusParser::ParseCallExpression( ParserNode* p_pParserNode )
{
	//TODO: Replace Parser Node with CallNode
	ParserNode* pNode = new ParserNode();

	if( ParseQualifiedName( pNode ) != Ok )
	{
		PushBack();
		return Skip;
	}

	//SymbolTableEntry* pEntry = NULL;
	String strScope;
	GetScopeString( strScope );
	strScope += '.';
	strScope += pNode->GetName();

	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( strScope );
	if( !pEntry )
	{
		ReportError( "Undefined Variable/Method " + pNode->GetName() );
		return Fail;
	}


	switch( NextTokenType() )
	{
		case TokenType::LParen:
			{
				CallNode* pCallNode = NULL;
				if( NextTokenType() == TokenType::RParen )
				{
					CallNode* pCallNode = new CallNode();
					pCallNode->SetName( pNode->GetName() );
					p_pParserNode->AddChild( pCallNode );
					delete pNode;
					return Ok;
				}

				PushBack();

				pCallNode = new CallNode();
				pCallNode->SetName( pNode->GetName() );
				delete pNode;

				if( ParseActualParameters( pCallNode ) == Fail )
				{
					delete pCallNode;
					return Fail;
				}

				if( NextTokenType() != TokenType::RParen )
				{
					ReportError( " ')' expected!" );
					delete pCallNode;
					return Fail;
				}	

				p_pParserNode->AddChild( pCallNode );
				return Ok;
			}

		case TokenType::LBracket:
			{
				ElementNode* pElementNode = new ElementNode();
				pElementNode->SetName( strScope );
				

				if( ParseExpression( pElementNode ) != Ok )
				{
					ReportError( " Error in array Index!" );
					delete pElementNode;
					return Fail;
				}

				if( NextTokenType() != TokenType::RBracket )
				{
					ReportError( " ']' expected!" );
					delete pElementNode;
					return Fail;
				}	
				
				pEntry->SetNode( pElementNode );
				p_pParserNode->AddChild( pElementNode );
				return Ok;
			}
			break;

		default:
			PushBack();
			VarNode* pVarNode = new VarNode();
			pVarNode->SetName( pNode->GetName() );
			p_pParserNode->AddChild( pVarNode );
			delete pNode;
			return Ok;
	}

	return Fail;
}



//CuneusParser::ParseState CuneusParser::ParseCallExpression( ParserNode* p_pParserNode )
//{
//	//TODO: Replace Parser Node with CallNode
//	ParserNode* pNode = new ParserNode();
//
//	if( ParseQualifiedName( pNode ) != Ok )
//	{
//		PushBack();
//		return Skip;
//	}
//
//	//SymbolTableEntry* pEntry = NULL;
//	String strScope;
//	GetScopeString( strScope );
//	strScope += '.';
//	strScope += pNode->GetName();
//
//	if( !m_pSymbolTable->Lookup( strScope ) )
//	{
//		ReportError( "Undefined Variable/Method " + pNode->GetName() );
//		return Fail;
//	}
//
//	if( NextTokenType() != TokenType::LParen )
//	{
//		PushBack();
//		VarNode* pVarNode = new VarNode();
//		pVarNode->SetName( pNode->GetName() );
//		p_pParserNode->AddChild( pVarNode );
//		delete pNode;
//		return Ok;
//	}	
//
//	if( NextTokenType() == TokenType::RParen )
//	{
//		CallNode* pCallNode = new CallNode();
//		pCallNode->SetName( pNode->GetName() );
//		p_pParserNode->AddChild( pCallNode );
//		delete pNode;
//		return Ok;
//	}	
//
//
//	PushBack();
//
//	CallNode* pCallNode = new CallNode();
//	pCallNode->SetName( pNode->GetName() );
//	delete pNode;
//
//	if( ParseActualParameters( pCallNode ) == Fail )
//	{
//		delete pCallNode;
//		return Fail;
//	}
//
//	if( NextTokenType() != TokenType::RParen )
//	{
//		ReportError( " ')' expected!" );
//		delete pCallNode;
//		return Fail;
//	}	
//
//	p_pParserNode->AddChild( pCallNode );
//	return Ok;
//}


CuneusParser::ParseState CuneusParser::ParseActualParameters( ParserNode* p_pParserNode )
{
	if( ParseExpression( p_pParserNode ) == Fail )
		return Ok;

	while( NextToken()->GetStringValue() == "," )
	{
		if( ParseExpression( p_pParserNode ) != Ok )
			goto PAP_Error;
	}
	
	PushBack();
	return Ok;

PAP_Error:
	PushBack();
	return Fail;
}


//---------------------------------------------------------------------------------------------------------


Meson_Cuneus_Compiler_END
