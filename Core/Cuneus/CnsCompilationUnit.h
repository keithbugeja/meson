//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ParserNodes::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	07/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsParserNode.h"
#include "CnsImports.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class CompilationUnit :	public Meson::Cuneus::Compiler::ParserNode
{
private:
	Imports* m_pImports;

public:
	CompilationUnit( void );

	Imports* GetImports( void );
	void SetImports( Imports* p_pImports );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

Meson_Cuneus_Compiler_ParserNodes_END
