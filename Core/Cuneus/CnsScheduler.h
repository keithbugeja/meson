//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Scheduler
//	This class implements the virtual machine execution scheduler.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	23/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include <Types.h>
#include <ArrayList.h>
#include <Configuration.h>

#include "CnsNamespaces.h"
#include "CnsTypeMethod.h"
#include "CnsContext.h"

Meson_Cuneus_BEGIN

class IScheduler
{

public:
	virtual void Startup( Meson::Common::Config::Configuration &p_config ) = 0;
	virtual void Shutdown( void ) = 0;

	virtual Meson::Cuneus::VCPU::Context *CreateTask( Meson::Cuneus::RTS::Reflection::TypeMethod *p_pMethod, int p_nPriority = 1 ) = 0;

	virtual Meson::Cuneus::VCPU::Context *NextTask( void ) = 0;
	virtual Meson::Cuneus::VCPU::Context *GetCurrentTask( void ) = 0;

	virtual void TerminateTask( Meson::Cuneus::VCPU::Context *p_pContext ) = 0;
};

Meson_Cuneus_END
