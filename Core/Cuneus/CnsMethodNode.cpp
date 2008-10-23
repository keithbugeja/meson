#include "CnsMethodNode.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

MethodNode::MethodNode(void)
	: AccessModifiable()
	, m_bInterface( false )
	, m_dwCodeOffset( 0 )
	, m_dwLocalVars( 0 )
{
}

MethodNode::MethodNode( Meson::Common::Text::String p_strName )
	: AccessModifiable( p_strName )
	, m_bInterface( false )
	, m_dwCodeOffset( 0 )
	, m_dwLocalVars( 0 )
{
}

bool MethodNode::IsInterface( void )
{
	return m_bInterface;
}

void MethodNode::SetInterface( bool p_bValue )
{
	m_bInterface = p_bValue;
}

dword MethodNode::GetByteCodeOffset( void )
{
	return m_dwCodeOffset;
}

void  MethodNode::SetByteCodeOffset( dword p_dwCodeOffset )
{
	m_dwCodeOffset = p_dwCodeOffset;
}

void MethodNode::IncLocalVars( void )
{
	m_dwLocalVars++;
}
	
dword MethodNode::GetLocalVars( void )
{
	return m_dwLocalVars;
}


bool MethodNode::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool MethodNode::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
