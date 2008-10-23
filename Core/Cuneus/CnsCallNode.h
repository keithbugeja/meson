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

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class CallNode : public Meson::Cuneus::Compiler::ParserNode
{
public:
	CallNode( void );
	~CallNode( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};

Meson_Cuneus_Compiler_ParserNodes_END
