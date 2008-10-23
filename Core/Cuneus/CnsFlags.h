//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VCPU::Flags
//	This class defines and contains the Virtual Machine Flags.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	23/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include <Types.h>

#include "CnsNamespaces.h"

Meson_Cuneus_VCPU_BEGIN

// Flags Register Definition
struct Flags
{
	dword brk : 1;
	dword dbl : 1;

	union
	{
		struct
		{
			dword eq : 1;
			dword gt : 1;
			dword lt : 1;
		};
		dword cmp : 3;
	};

	dword tru : 1;


	inline void Reset( void ) 
	{ 
		brk = 0;
		dbl = 0;
		cmp = 0;
		tru = 0;
	}

};

Meson_Cuneus_VCPU_END
