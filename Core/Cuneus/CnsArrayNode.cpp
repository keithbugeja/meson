#include "CnsArrayNode.h"

#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"
#include "CnsTypeCodes.h"


Meson_Cuneus_Compiler_ParserNodes_BEGIN

ArrayNode::ArrayNode( void ) 
	: ParserNode( "Array" )
{
}

ArrayNode::ArrayNode( ArrayNode* p_pArrayNode )
	: ParserNode( "Array" )
{
	AddChild( p_pArrayNode );
}


ArrayNode::ArrayNode( TypeNode* p_pTypeNode )
	: ParserNode( "Array" )
{
	SetType( p_pTypeNode );
}


ArrayNode::~ArrayNode( void )
{
}

bool ArrayNode::IsArray( void )
{
	return true;
}

bool ArrayNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool ArrayNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson::Common::Text::String ArrayNode::ToString( void )
{
	return m_pType->ToString() + "[]";
}


//---------------------------------------------------------------------------------------------------------

ElementNode::ElementNode( void ) : ParserNode( "Element" )
{
	SetTypeCode( CNS_TC_ARRAY );
}

ElementNode::~ElementNode( void )
{
}

int ElementNode::GetIndex( void )
{
	return m_nIndex;
}


void ElementNode::SetIndex( int p_nIndex )
{
	m_nIndex = p_nIndex;
}

Variable* ElementNode::GetVariable( void )
{
	return m_pVariable;
}

bool ElementNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool ElementNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
