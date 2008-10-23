#pragma once

#include "CnsNamespaces.h"

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include <CnsScheduler.h>

#include "Scheduler.h"

Meson_Cuneus_BEGIN

class SchedulerPlugIn : public Meson::Common::PlugIn::IPlugIn
{
	IScheduler *m_pScheduler;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;
	Meson::Common::Text::String	m_strName;
	Meson::Common::Version	m_version;
public:
	SchedulerPlugIn( void );
	~SchedulerPlugIn( void );

	const Meson::Common::Text::String &GetName( void ) const;
	const Meson::Common::Version &GetVersion( void ) const;

	void Register( Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap );
	void Unregister( void );

	void Initialise( void );
	void Shutdown( void );
};

Meson_Cuneus_END
