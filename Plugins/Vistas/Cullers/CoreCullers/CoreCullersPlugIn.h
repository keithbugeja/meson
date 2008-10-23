//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include "VstNamespaces.h"
#include "BasicCullerFactory.h"

Meson_Vistas_BEGIN
	class CoreCullersPlugIn : public Meson::Common::PlugIn::IPlugIn
	{
	public:
		CoreCullersPlugIn();
		~CoreCullersPlugIn();

		const Meson::Common::Text::String& GetName(void) const;
		const Meson::Common::Version& GetVersion(void) const;

		void Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap);
		void Unregister();

		void Initialise(void);
		void Shutdown(void);

	private:
		Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;

		Meson::Common::Text::String	m_pName;
		Meson::Common::Version	m_version;

		BasicCullerFactory* m_pBasicCullerFactory;		
	};
Meson_Vistas_END
