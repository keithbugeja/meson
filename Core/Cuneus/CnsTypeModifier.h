#pragma once

#include "CnsDataType.h"

Meson_Cuneus_RTS_Reflection_BEGIN

class TypeModifier : public Meson::Cuneus::RTS::Typing::DataType
{
protected:
	dword m_dwModifier;

public:
	TypeModifier( void );
	~TypeModifier( void );

	dword GetModifier( void ) { return m_dwModifier; }
	void SetModifier( dword p_dwValue ) { m_dwModifier = p_dwValue; }
};

Meson_Cuneus_RTS_Reflection_END
