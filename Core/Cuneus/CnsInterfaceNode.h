//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ParserNodes::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	12/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsAccessModifiable.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class InterfaceNode
	: public AccessModifiable
{
protected:
	ParserNode* m_pExtends;

public:
	InterfaceNode( void );
	InterfaceNode( Meson::Common::Text::String p_strName );
	~InterfaceNode( void );

	ParserNode* GetExtends( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};

Meson_Cuneus_Compiler_ParserNodes_END
