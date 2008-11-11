#pragma once

#include <String8.h>
#include "CnsTypes.h"
#include "CnsNamespaces.h"
#include "CnsContext.h"

Meson_Cuneus_Util_BEGIN

class CUNEUS_API Disasm
{
private:
	byte* m_pByteCodeBase;

	Meson::Cuneus::VCPU::Registers* m_pRegisters;
	Meson::Cuneus::VCPU::Flags* m_pFlags;
	Meson::Cuneus::Util::DataTypeStack* m_pStack;

	

public:
	Disasm( void );
	Disasm( Meson::Cuneus::VCPU::Registers* p_pRegisters, Meson::Cuneus::VCPU::Flags* p_pFlags,	Meson::Cuneus::Util::DataTypeStack* p_pStack );
	~Disasm(void);

	Meson::Common::Text::String Register( byte p_byReg );

	void SetByteCodeBase( byte* p_pByteCodeBase );
	dword Disassemble( dword p_dwOffset, String& p_str );

	//Meson::Common::Text::String Disassemble( byte* p_pbyCode );

};

Meson_Cuneus_Util_END
