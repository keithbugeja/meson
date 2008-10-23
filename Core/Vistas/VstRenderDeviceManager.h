//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"

#include "VstTypes.h"
#include "VstRenderDevice.h"

Meson_Vistas_BEGIN

	class RenderDeviceManager : public Meson::Common::TSingleton<RenderDeviceManager>
	{
		friend class Meson::Common::TSingleton<RenderDeviceManager>;

	private: 
		RenderDeviceManager(void);
		~RenderDeviceManager(void);

	public:
		void RegisterRenderDevice(Meson::Vistas::IRenderDevice* p_pRenderDevice);
		void UnregisterRenderDevice(Meson::Vistas::IRenderDevice* p_pRenderDevice);

		IRenderDevice* GetActive(void) const;
		bool IsAvailable(const Meson::Common::Text::String& p_strRenderDeviceName);
		Meson::Common::Config::Configuration* GetConfiguration(const Meson::Common::Text::String& p_strRenderDeviceName);
		WindowRenderTarget* Startup(const Meson::Common::Text::String& p_strRenderDeviceName);
		void Shutdown(void);
	
	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String,IRenderDevice*> m_renderDeviceMap;
		IRenderDevice* m_pActiveRenderDevice;
	};

Meson_Vistas_END
