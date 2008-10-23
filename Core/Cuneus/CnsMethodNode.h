//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ParserNodes::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	18/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsAccessModifiable.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class MethodNode :
	public AccessModifiable
{
protected:
	bool m_bInterface;
	dword m_dwCodeOffset;

	dword m_dwLocalVars;

public:
	MethodNode( void );
	MethodNode( Meson::Common::Text::String p_strName );

	bool IsInterface( void );
	void SetInterface( bool p_bValue );

	dword GetByteCodeOffset( void );
	void  SetByteCodeOffset( dword p_dwCodeOffset );

	void IncLocalVars( void );
	dword GetLocalVars( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );
};

Meson_Cuneus_Compiler_ParserNodes_END
