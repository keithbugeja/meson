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

#include "CnsParserNode.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class TypeNode
	: public ParserNode
{
protected:
	dword m_dwTypeCode;

public:
	TypeNode( void );
	TypeNode( Meson::Common::Text::String p_strName );

	dword GetTypeCode( void );
	void SetTypeCode( dword p_dwTypeCode );

	bool BindQualifiedIdentifier( void );

	virtual bool IsArray( void );
	
	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};

Meson_Cuneus_Compiler_ParserNodes_END