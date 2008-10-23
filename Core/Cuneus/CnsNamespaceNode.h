//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ParserNodes::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	10/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "CnsParserNode.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class NamespaceNode 
	:	public Meson::Cuneus::Compiler::ParserNode
{
public:
	NamespaceNode( void );
	NamespaceNode( Meson::Common::Text::String p_strName );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};

Meson_Cuneus_Compiler_ParserNodes_END
