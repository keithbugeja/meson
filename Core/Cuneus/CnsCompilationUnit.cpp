#include "CnsCompilationUnit.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

Meson_Cuneus_Compiler_ParserNodes_BEGIN


CompilationUnit::CompilationUnit( void ) 
	: ParserNode( "CompilationUnit" )
	, m_pImports( NULL )
{
}

Imports* CompilationUnit::GetImports( void )
{
	return m_pImports;
}

void CompilationUnit::SetImports( Imports* p_pImports )
{
	m_pImports = p_pImports;
}

bool CompilationUnit::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool CompilationUnit::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END

