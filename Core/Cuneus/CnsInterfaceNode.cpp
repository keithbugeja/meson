#include "CnsInterfaceNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

InterfaceNode::InterfaceNode( void )
	: AccessModifiable()
{
	m_pExtends = NULL;
}

InterfaceNode::InterfaceNode( Meson::Common::Text::String p_strName )
	: AccessModifiable( p_strName )
{
	m_pExtends = NULL;
}

InterfaceNode::~InterfaceNode(void)
{
	ParserNode::~ParserNode();
	if( m_pExtends )
		delete m_pExtends;
}

ParserNode* InterfaceNode::GetExtends( void ) 
{ 
	if( !m_pExtends ) 
		m_pExtends = new ParserNode();
		
	return m_pExtends; 
}

bool InterfaceNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool InterfaceNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
