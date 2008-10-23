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

namespace AccessType
{
	enum AccessType
	{
		None, Public, Protected, Private
	};
}




union Modifier
{
	struct
	{
		dword m_bPublic		: 1;
		dword m_bProtected	: 1;
		dword m_bPrivate	: 1;
		dword m_bNamespace	: 1;

		dword m_bStatic		: 1;
		dword m_bFinal      : 1;
		dword m_bNative		: 1;
		dword m_bAbstract	: 1;
		
		dword m_bSync		: 1;
		dword m_bInterface	: 1;
	};

	dword m_dwModifier;

public:
	Modifier( void ) { m_dwModifier = 0; }
};




class AccessModifiable 
	: public Meson::Cuneus::Compiler::ParserNode
{

#define BOOL_ACCESSORS( X )	inline bool Is##X( void ) { return m_modifier.m_b##X; } \
						inline void Set##X( bool p_bValue ) { m_modifier.m_b##X = p_bValue; }

protected:
	Modifier m_modifier;
	

public:
	AccessModifiable( void ); 
	AccessModifiable( Meson::Common::Text::String p_strName );

	BOOL_ACCESSORS( Public )
	BOOL_ACCESSORS( Protected )
	BOOL_ACCESSORS( Private )
	BOOL_ACCESSORS( Namespace )

	BOOL_ACCESSORS( Static )
	BOOL_ACCESSORS( Final )
	BOOL_ACCESSORS( Native )
	BOOL_ACCESSORS( Abstract )
	
	BOOL_ACCESSORS( Sync )
	BOOL_ACCESSORS( Interface )

	inline dword GetModifierCode( void ) { return m_modifier.m_dwModifier; }
	inline void SetModifierCode( dword p_dwValue ) { m_modifier.m_dwModifier = p_dwValue; }

	inline bool Test( dword p_dwValue ) { return m_modifier.m_dwModifier == p_dwValue; }
};

#undef BOOL_ACCESSORS

Meson_Cuneus_Compiler_ParserNodes_END
