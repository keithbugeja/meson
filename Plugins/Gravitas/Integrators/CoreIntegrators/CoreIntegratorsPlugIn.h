//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreIntegratorsPlugin
//	PlugIn interface for the Gravitas core integrator implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		30/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include "GrvNamespaces.h"

#include "EulerKineticIntegrator.h"
#include "VerletKineticIntegrator.h"
#include "RK4KineticIntegrator.h"

Meson_Gravitas_BEGIN

class CoreIntegratorsPlugIn
	: public Meson::Common::PlugIn::IPlugIn
{
private:
	Meson::Common::Text::String m_strName;
	Meson::Common::Version m_version;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;

	// kinetic integrators
	Meson::Gravitas::Kinetics::KineticIntegratorPtr m_pEulerKineticIntegrator;
	Meson::Gravitas::Kinetics::KineticIntegratorPtr m_pVerletKineticIntegrator;
	Meson::Gravitas::Kinetics::KineticIntegratorPtr m_pRK4KineticIntegrator;

public:
	CoreIntegratorsPlugIn();
	~CoreIntegratorsPlugIn();

	const Meson::Common::Text::String& GetName(void) const;
	const Meson::Common::Version& GetVersion(void) const;

	void Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap);
	void Unregister();

	void Initialise(void);
	void Shutdown(void);
};

Meson_Gravitas_END
