//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ParserNodes::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	22/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsAccessModifiable.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class Variable
	: public AccessModifiable
{
protected:	
	int m_nArrayLen;

public:
	Variable( void );

	bool IsArray( void );
	
	int  GetArraySize( void );
	void SetArraySize( int p_nSize );


	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

Meson_Cuneus_Compiler_ParserNodes_END
