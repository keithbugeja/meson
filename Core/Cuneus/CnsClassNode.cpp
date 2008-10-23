#include "CnsClassNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

ClassNode::ClassNode( void )
	: AccessModifiable()
{
	m_pExtends = m_pImplements = NULL;
}

ClassNode::ClassNode( Meson::Common::Text::String p_strName )
	: AccessModifiable( p_strName )
{
	m_pExtends = m_pImplements = NULL;
}

ClassNode::~ClassNode(void)
{
	ParserNode::~ParserNode();
	if( m_pExtends )
		delete m_pExtends;

	if( m_pImplements )
		delete m_pImplements;
}

ParserNode* ClassNode::GetExtends( void )
{
	if( !m_pExtends )
		m_pExtends = new ParserNode();

	return m_pExtends;
}

ParserNode* ClassNode::GetImplements( void )
{
	if( !m_pImplements )
		m_pImplements = new ParserNode();

	return m_pImplements;
}

bool ClassNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool ClassNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
