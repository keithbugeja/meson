//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VCPU::CuneusInterpreter
//	This is the actual Virtual Machine's byte code interpreter implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	27/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include <Configuration.h>

#include "CnsNamespaces.h"
#include "CnsContext.h"

Meson_Cuneus_BEGIN

class ICuneusInterpreter
{
public:
	virtual void Startup( Meson::Common::Config::Configuration &p_config ) = 0;
	virtual void Shutdown( void ) = 0;

	virtual dword Execute( Meson::Cuneus::VCPU::Context *p_pContext ) = 0;
	virtual dword ExecuteInstructions( Meson::Cuneus::VCPU::Context *p_pContext, dword p_dwInstructions ) = 0;
	virtual dword ExecuteTimespan( Meson::Cuneus::VCPU::Context *p_pContext, dword p_dwTimespan ) = 0;
};

Meson_Cuneus_END
