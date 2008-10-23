#include "CnsVariable.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

Variable::Variable( void ) 
	: AccessModifiable()
	, m_nArrayLen( -1 )
{
}

bool Variable::IsArray( void )
{
	return ( m_nArrayLen > -1 );
}
	
int  Variable::GetArraySize( void )
{
	return m_nArrayLen;
}

void Variable::SetArraySize( int p_nSize )
{
	m_nArrayLen = p_nSize;
}


bool Variable::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool Variable::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
