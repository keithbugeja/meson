//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreForcesPlugin
//	PlugIn interface for Gravitas core force implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		30/07/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include "GrvNamespaces.h"

#include "BasicForceFactory.h"
#include "GravityForceFactory.h"
#include "VortexForceFactory.h"

Meson_Gravitas_BEGIN

class CoreForcesPlugIn
	: public Meson::Common::PlugIn::IPlugIn
{
private:
	Meson::Common::Text::String	m_pName;
	Meson::Common::Version	m_version;
	Meson::Common::PlugIn::SharedObjectMap* m_pSharedObjectMap;

	// force factories
	Meson::Gravitas::Force::BasicForceFactory* m_pBasicForceFactory;
	Meson::Gravitas::Force::GravityForceFactory* m_pGravityForceFactory;
	Meson::Gravitas::Force::VortexForceFactory* m_pVortexForceFactory;

public:
	CoreForcesPlugIn();
	~CoreForcesPlugIn();

	const Meson::Common::Text::String& GetName(void) const;
	const Meson::Common::Version& GetVersion(void) const;

	void Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap);
	void Unregister();

	void Initialise(void);
	void Shutdown(void);
};

Meson_Gravitas_END
