//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Scheduler
//	This class implements the virtual machine execution scheduler.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	23/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include <ArrayList.h>

#include <CnsTypes.h>
#include <CnsNamespaces.h>
#include <CnsScheduler.h>

#include "Task.h"

Meson_Cuneus_BEGIN

class Scheduler : public Meson::Cuneus::IScheduler
{
private:
	Meson::Common::Collections::TArrayList< Task* > m_lstTasks;
	dword m_dwTaskIndex;
	Task *m_pCurrentTask;

public:
	Scheduler(void);
	~Scheduler(void);

	virtual void Startup( Meson::Common::Config::Configuration &p_config );
	virtual void Shutdown( void );

	Meson::Cuneus::VCPU::Context *CreateTask( Meson::Cuneus::RTS::Reflection::TypeMethod *p_pMethod, int p_nPriority );

	Meson::Cuneus::VCPU::Context *NextTask( void );
	Meson::Cuneus::VCPU::Context *GetCurrentTask( void );

	void TerminateTask( Meson::Cuneus::VCPU::Context *p_pContext );
};

Meson_Cuneus_END
