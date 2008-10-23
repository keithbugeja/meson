//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VCPU::Registers
//	This class defines and contains the Virtual Machine Registers.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	23/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include <Types.h>
#include <String8.h>
#include <Pointer.h>

#include "CnsNamespaces.h"
#include "CnsDataType.h"
#include "CnsTypeDWord.h"


Meson_Cuneus_VCPU_BEGIN

// Virtual Machine Registers class
class Registers
{

public:
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > R[ 0x20 ];
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::TypeDWord > PC, SP, FP;

public:
	Registers( void );
	Registers( Registers &p_vmRegisters );
	~Registers( void );

	void Reset( void );

	void Copy( Registers &p_vmRegisters );

	Registers &operator=( Registers &p_vmRegisters );
	bool operator==( Registers &p_vmRegisters );
	bool operator!=( Registers &p_vmRegisters );

	void Dump( Meson::Common::Text::String& p_str );
};

Meson_Cuneus_VCPU_END
