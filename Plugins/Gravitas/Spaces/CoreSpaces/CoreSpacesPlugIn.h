//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreSpacesPlugin
//	PlugIn interface for the Gravitas core space implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		20/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include "GrvNamespaces.h"

#include "BasicSpaceFactory.h"
#include "GridSpaceFactory.h"
#include "KDTreeSpaceFactory.h"
#include "SweepAndPruneSpaceFactory.h"

Meson_Gravitas_BEGIN

class CoreSpacesPlugIn : public Meson::Common::PlugIn::IPlugIn
{
private:
	Meson::Common::Text::String	m_pName;
	Meson::Common::Version	m_version;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;

	// space factories
	Meson::Gravitas::Space::BasicSpaceFactory* m_pBasicSpaceFactory;
	Meson::Gravitas::Space::GridSpaceFactory* m_pGridSpaceFactory;
	Meson::Gravitas::Space::KDTreeSpaceFactory* m_pKDTreeSpaceFactory;
	Meson::Gravitas::Space::SweepAndPruneSpaceFactory* m_pSweepAndPruneSpaceFactory;

public:
	CoreSpacesPlugIn();
	~CoreSpacesPlugIn();

	const Meson::Common::Text::String& GetName(void) const;
	const Meson::Common::Version& GetVersion(void) const;

	void Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap);
	void Unregister();

	void Initialise(void);
	void Shutdown(void);
};

Meson_Gravitas_END
