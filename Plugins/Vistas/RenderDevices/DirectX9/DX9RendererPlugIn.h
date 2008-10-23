//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9RendererPlugIn
//	PlugIn interface for Vistas Direct3D9 renderer.
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
#include "DX9RenderDevice.h"

Meson_Vistas_BEGIN
	class DX9RendererPlugIn : public Meson::Common::PlugIn::IPlugIn
	{
	public:
		DX9RendererPlugIn();
		~DX9RendererPlugIn();

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

		DX9RenderDevice *m_pRenderDevice;
	};
Meson_Vistas_END
