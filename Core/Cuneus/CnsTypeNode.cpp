#include "CnsTypeNode.h"
#include "CnsQualifiedIdentifier.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

TypeNode::TypeNode( void ) 
	: ParserNode()
	, m_dwTypeCode( 0 )
	//, m_dwArraySize( 0 )
	//, m_bArray( false )
{
}

TypeNode::TypeNode( Meson::Common::Text::String p_strName ) 
	: ParserNode( p_strName )
	, m_dwTypeCode( 0 )
	//, m_dwArraySize( 0 )
	//, m_bArray( false )
{

}


dword TypeNode::GetTypeCode( void )
{
	return m_dwTypeCode;
}

void TypeNode::SetTypeCode( dword p_dwTypeCode )
{
	m_dwTypeCode = p_dwTypeCode;
}

bool TypeNode::BindQualifiedIdentifier( void )
{
	/*if( !m_plstChildNode || ( m_plstChildNode->Size() == 0 ) )
		return false;

	QualifiedIdentifier* pQualifiedIdentifier = (QualifiedIdentifier*) (*m_plstChildNode)[0];
	m_strName = pQualifiedIdentifier->ToString();*/

	return true;
}

bool TypeNode::IsArray( void )
{
	return false;
}

//void TypeNode::SetArray( bool p_bIsArray )
//{
//	m_bArray = p_bIsArray;
//}
//
//dword TypeNode::GetArraySize( void )
//{
//	return m_dwArraySize;
//}
//
//void  TypeNode::SetArraySize( dword p_dwArraySize )
//{
//	m_dwArraySize = p_dwArraySize;
//}

bool TypeNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool TypeNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END