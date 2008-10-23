//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::MemoryHandle
//	Defines the MemoryHandle that is used to access either native memory or memory allocated by
//	a garbage collector of the virtual machine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	14/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "CnsTypes.h"

// |-Native-|-NativePointer-|
// |    1   |       63      |
// |--------|---------------|
// |	 qwValue   64       |
// |------------------------|
typedef union
{
	struct
	{
		qword NativePointer : 63;
		qword Native        : 1;
	};

	qword qwValue;	

} MemoryHandle;

