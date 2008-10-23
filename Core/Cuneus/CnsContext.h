//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VCPU::Context
//	This class represents the execution contexts and contains the information required by the
//	Virtual Machine's interpreter to execute the task.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	27/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "CnsNamespaces.h"
#include "CnsRegisters.h"
#include "CnsFlags.h"
#include "CnsDataTypeStack.h"
#include "CnsDataType.h"

#include <ArrayList.h>

Meson_Cuneus_VCPU_BEGIN

class Context
{
private:
	dword m_dwIndex;

public:
	Meson::Cuneus::VCPU::Registers Registers;
	Meson::Cuneus::VCPU::Flags     Flags;
	Meson::Cuneus::Util::DataTypeStack Stack;
	Meson::Common::Collections::TArrayList< Meson::Cuneus::RTS::Typing::DataType > ConstList;
	Meson::Common::Collections::TArrayList< Meson::Cuneus::RTS::Typing::DataType > DataList;
	Meson::Common::Collections::TArrayList< Meson::Cuneus::RTS::Typing::DataType > RefList;
	

public:
	Context( int p_nStackSize );
	~Context( void );

	inline dword GetIndex( void ) { return m_dwIndex; }
	inline void SetIndex( dword p_dwIndex ) { m_dwIndex = p_dwIndex; }

	void Reset( void );

	dword EnterMethodFrame( dword p_dwSize );
	void  LeaveMethodFrame( dword p_dwPosition );
};

Meson_Cuneus_VCPU_END
