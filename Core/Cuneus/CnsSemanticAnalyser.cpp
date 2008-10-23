#include "CnsSemanticAnalyser.h"

Meson_Cuneus_Compiler_BEGIN

using namespace ParserNodes;

SemanticAnalyser::SemanticAnalyser( CuneusParser* p_pParser )
{
	m_pDataTypeMgr = CuneusEngine::GetInstance()->GetDataTypeMgr();
	m_pParser = p_pParser;
}

SemanticAnalyser::~SemanticAnalyser( void )
{

}

void SemanticAnalyser::ReportError( String p_strMessage )
{
	m_pParser->ReportError( "Semantic Error - " + p_strMessage );
}

//*********************************************************************************************************

bool SemanticAnalyser::Check( IntegerLiteral* p_pLiteral )
{
	p_pLiteral->SetTypeCode( m_pDataTypeMgr->GetTypeCode( "long" ) );

	return true;
}

bool SemanticAnalyser::Check( RealLiteral* p_pLiteral )
{
	p_pLiteral->SetTypeCode( m_pDataTypeMgr->GetTypeCode( "double" ) );

	return true;
}

bool SemanticAnalyser::Check( CharLiteral* p_pLiteral )
{
	p_pLiteral->SetTypeCode( m_pDataTypeMgr->GetTypeCode( "char" ) );

	return true;
}

bool SemanticAnalyser::Check( StringLiteral* p_pLiteral )
{
	p_pLiteral->SetTypeCode( m_pDataTypeMgr->GetTypeCode( "String" ) );

	return true;
}

bool SemanticAnalyser::Check( BoolLiteral* p_pLiteral )
{
	p_pLiteral->SetTypeCode( m_pDataTypeMgr->GetTypeCode( "bool" ) );

	return true;
}

bool SemanticAnalyser::Check( RefLiteral* p_pLiteral )
{
	p_pLiteral->SetTypeCode( m_pDataTypeMgr->GetTypeCode( "reference" ) );

	return true;
}


//*********************************************************************************************************

bool SemanticAnalyser::Check( ParserNodes::ExpressionNode* p_pNode )
{
	if( !p_pNode )
		return false;

	int nSize = (int) p_pNode->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pNode)[ n ]->TypeCheck( this ) )
			return false;
	}

	return true;
}

bool SemanticAnalyser::Check( ParserNodes::UnaryOperator* p_pNode )
{

	if( !Check((*p_pNode)[0]) )
	{
		m_pParser->ReportError( "Semantic Error - UnaryOperator Operand." );
		return false;
	}

	p_pNode->SetTypeCode( (*p_pNode)[0]->GetTypeCode() );
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::BinaryOperator* p_pNode )
{

	if( !Check((*p_pNode)[0]) )
	{
		m_pParser->ReportError( "Semantic Error - Assignment LHS." );
		return false;
	}

	if( !Check((*p_pNode)[1]) )
	{
		m_pParser->ReportError( "Semantic Error - Assignment RHS." );
		return false;
	}

	if( (*p_pNode)[0]->GetTypeCode() != (*p_pNode)[1]->GetTypeCode() )
	{
		m_pParser->ReportError( "Semantic Error - BinaryOperator LHS Type does not match RHS." );
		return false;
	}

	p_pNode->SetTypeCode( (*p_pNode)[0]->GetTypeCode() );
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::TernaryOperator* p_pNode )
{
	if( !Check((*p_pNode)[0]) )
	{
		m_pParser->ReportError( "Semantic Error - TernaryOperator Contition." );
		return false;
	}

	if( !Check((*p_pNode)[1]) )
	{
		m_pParser->ReportError( "Semantic Error - TernaryOperator Then." );
		return false;
	}

	if( !Check((*p_pNode)[2]) )
	{
		m_pParser->ReportError( "Semantic Error - TernaryOperator Else." );
		return false;
	}

	if( (*p_pNode)[1]->GetTypeCode() != (*p_pNode)[2]->GetTypeCode() )
	{
		m_pParser->ReportError( "Semantic Error - TernaryOperator Then Type does not match Else." );
		return false;
	}

	p_pNode->SetTypeCode( (*p_pNode)[1]->GetTypeCode() );
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::RelationalOperator* p_pNode )
{
	if( !Check((*p_pNode)[0]) )
	{
		m_pParser->ReportError( "Semantic Error - Assignment LHS." );
		return false;
	}

	if( !Check((*p_pNode)[1]) )
	{
		m_pParser->ReportError( "Semantic Error - Assignment RHS." );
		return false;
	}

	if( (*p_pNode)[0]->GetTypeCode() != (*p_pNode)[1]->GetTypeCode() )
	{
		m_pParser->ReportError( "Semantic Error - RelationalOperator LHS Type does not match RHS." );
		return false;
	}

	p_pNode->SetTypeCode( m_pDataTypeMgr->GetTypeCode( "bool" ) );
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::TypeCast* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::TypeNode* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::Identifier* p_pNode )
{
	m_pParser->ReportError( "Semantic Error - IDENTIFIER." );
	return false;
}

bool SemanticAnalyser::Check( ParserNodes::QualifiedIdentifier* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::Variable* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::MethodNode* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::ClassNode* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::InterfaceNode* p_pNode )
{
	return true;
}

//*********************************************************************************************************

bool SemanticAnalyser::Check( ParserNodes::Assignment* p_pNode )
{
	if( Check((*p_pNode)[0]) )
	{
		m_pParser->ReportError( "Semantic Error - Assignment LHS." );
		return false;
	}

	if( !Check((*p_pNode)[1]) )
	{
		m_pParser->ReportError( "Semantic Error - Assignment RHS." );
		return false;
	}

	if( (*p_pNode)[0]->GetTypeCode() != (*p_pNode)[1]->GetTypeCode() )
	{
		m_pParser->ReportError( "Semantic Error - Assignment LHS Type does not match RHS." );
		return false;
	}

	p_pNode->SetTypeCode( (*p_pNode)[0]->GetTypeCode() );
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::IfNode* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::WhileNode* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::LabelNode* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::ReturnNode* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::GotoNode* p_pNode )
{
	return true;
}
	

//*********************************************************************************************************

bool SemanticAnalyser::Check( ParserNodes::Imports* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::NamespaceNode* p_pNode )
{
	return true;
}

bool SemanticAnalyser::Check( ParserNodes::CompilationUnit* p_pUnit )
{
	if( !p_pUnit )
		return false;

	int nSize = (int) p_pUnit->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pUnit)[ n ]->TypeCheck( this ) )
			return false;
	}

	p_pUnit->SetTypeCode( m_pDataTypeMgr->GetTypeCode( "void" ) );
	return true;
}


bool SemanticAnalyser::Check( ParserNode* p_pNode )
{
	if( !p_pNode )
		return false;

	int nSize = (int) p_pNode->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pNode)[ n ]->TypeCheck( this ) )
			return false;
	}

	return true;
}

Meson_Cuneus_Compiler_END
