#pragma once

#include "CnsNamespaces.h"

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include <CnsGarbageCollector.h>

#include "Scavenger.h"

Meson_Cuneus_BEGIN

class ScavengerPlugIn : public Meson::Common::PlugIn::IPlugIn
{
	IGarbageCollector *m_pGarbageCollector;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;
	Meson::Common::Text::String	m_strName;
	Meson::Common::Version	m_version;
public:
	ScavengerPlugIn( void );
	~ScavengerPlugIn( void );

	const Meson::Common::Text::String &GetName( void ) const;
	const Meson::Common::Version &GetVersion( void ) const;

	void Register( Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap );
	void Unregister( void );

	void Initialise( void );
	void Shutdown( void );
};

Meson_Cuneus_END
