#include "CnsStatementNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

StatementNode::StatementNode( void ): ParserNode( "Statement" )
{
}

StatementNode::StatementNode( Meson::Common::Text::String p_strName ): ParserNode( p_strName )
{
}

StatementNode::~StatementNode( void )
{
}

bool StatementNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool StatementNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//---------------------------------------------------------------------------------------------------------

IfNode::IfNode( void ): StatementNode( "If" )
{
}

IfNode::~IfNode( void )
{
}

bool IfNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool IfNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//---------------------------------------------------------------------------------------------------------

WhileNode::WhileNode( void ): StatementNode( "While" )
{
}

WhileNode::~WhileNode( void )
{
}

bool WhileNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool WhileNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//---------------------------------------------------------------------------------------------------------

ReturnNode::ReturnNode( void ): StatementNode( "Return" )
{
}

ReturnNode::~ReturnNode( void )
{
}

bool ReturnNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool ReturnNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//---------------------------------------------------------------------------------------------------------

LabelNode::LabelNode( void ): StatementNode( "Label" )
{
}

LabelNode::~LabelNode( void )
{
}

bool LabelNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool LabelNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//---------------------------------------------------------------------------------------------------------

GotoNode::GotoNode( void ): StatementNode( "Goto" )
{
	m_dwAddressReference = 0xFFFFFFFF;
}

GotoNode::~GotoNode( void )
{
}

dword GotoNode::GetAddress( void )
{
	return m_dwAddress;
}

void  GotoNode::SetAddress( dword p_dwAddress )
{
	m_dwAddress = p_dwAddress;
}

dword GotoNode::GetAddressReference( void )
{
	return m_dwAddressReference;
}

void  GotoNode::SetAddressReference( dword p_dwAddressReference )
{
	m_dwAddressReference = p_dwAddressReference;
}

bool GotoNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool GotoNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//---------------------------------------------------------------------------------------------------------



Meson_Cuneus_Compiler_ParserNodes_END
