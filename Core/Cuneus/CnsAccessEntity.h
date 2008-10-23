#pragma once

#include "CnsDataType.h"
#include "CnsTypeModifier.h"
#include "CnsAccessModifiable.h"

Meson_Cuneus_RTS_Reflection_BEGIN

class AccessEntity : public Meson::Cuneus::RTS::Typing::DataType
{
protected:
	///TypeModifier m_modifier;

	Meson::Cuneus::Compiler::ParserNodes::Modifier m_modifier;

public:
	AccessEntity( void );
	~AccessEntity( void );

	Meson::Cuneus::Compiler::ParserNodes::Modifier& GetModifier( void ) { return m_modifier; }

	dword GetModifierCode( void ) { return m_modifier.m_dwModifier; }
	void SetModifierCode( dword p_dwModifier ) { m_modifier.m_dwModifier = p_dwModifier; }

};

Meson_Cuneus_RTS_Reflection_END
