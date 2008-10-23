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

class ClassNode
	: public AccessModifiable
{
protected:
	ParserNode* m_pExtends;
	ParserNode* m_pImplements;
	

public:
	ClassNode( void );
	ClassNode( Meson::Common::Text::String p_strName );
	~ClassNode( void );
	
	ParserNode* GetExtends( void );
	ParserNode* GetImplements( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

Meson_Cuneus_Compiler_ParserNodes_END
