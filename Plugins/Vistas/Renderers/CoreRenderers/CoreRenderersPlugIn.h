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
#include "BasicRendererFactory.h"
#include "InstrumentationRendererFactory.h"

Meson_Vistas_BEGIN

	class CoreRenderersPlugIn : public Meson::Common::PlugIn::IPlugIn
	{
	public:
		CoreRenderersPlugIn();
		~CoreRenderersPlugIn();

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

		BasicRendererFactory* m_pBasicRendererFactory;
		InstrumentationRendererFactory* m_pInstrumentationRendererFactory;
	};

Meson_Vistas_END
