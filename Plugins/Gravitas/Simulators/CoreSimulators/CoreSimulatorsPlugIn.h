//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreSimulatorsPlugIn
//	PlugIn interface for the Gravitas core simulator implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		28/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include "GrvNamespaces.h"

#include "BasicSimulatorFactory.h"

Meson_Gravitas_BEGIN

class CoreSimulatorsPlugIn : public Meson::Common::PlugIn::IPlugIn
{
private:
	Meson::Common::Text::String	m_pName;
	Meson::Common::Version	m_version;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;

	// simulator factories
	Meson::Gravitas::Simulation::BasicSimulatorFactory *m_pBasicSimulatorFactory;


public:
	CoreSimulatorsPlugIn();
	~CoreSimulatorsPlugIn();

	const Meson::Common::Text::String& GetName(void) const;
	const Meson::Common::Version& GetVersion(void) const;

	void Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap);
	void Unregister();

	void Initialise(void);
	void Shutdown(void);
};

Meson_Gravitas_END
