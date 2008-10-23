//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9WindowRenderTarget
//	Render target implementation for Direct3D9 renderer.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "VstNamespaces.h"
#include "VstDefinitions.h"
#include "VstTypes.h"
#include "VstWindowRenderTarget.h"
#include "DX9AdapterDevice.h"

Meson_Vistas_BEGIN
	class DX9WindowRenderTarget : public WindowRenderTarget
	{
	protected:
		void InitialiseDirect3D(void);
		void ShutdownDirect3D(void);

		void SetFullScreen(bool p_bEnable);
	public: 
		DX9WindowRenderTarget(HINSTANCE p_hInstance, DX9AdapterDevice *p_pAdapterDevice);
		void Create(const Meson::Common::Text::String& p_strName, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, Meson::Common::Config::Configuration* p_pConfiguration);
		void Destroy(void);

		void Present(void);
		void ResetDevice(void);

		void OnWindowMoved(void);
		void OnWindowResized(void);

		LPDIRECT3DSURFACE9 GetRenderTargetSurface(void) const;
		LPDIRECT3DSURFACE9 GetDepthStencilSurface(void) const;

	protected:
		HWND m_hWnd;
		HINSTANCE m_hInstance;

		D3DPRESENT_PARAMETERS	m_presentParameters;
		
		LPDIRECT3DSWAPCHAIN9	m_pSwapChain;
		LPDIRECT3DSURFACE9		m_pRenderTargetSurface,
								m_pDepthStencilSurface;

		DX9AdapterDevice* m_pAdapterDevice;
		
		bool m_bIsDepthBufferEnabled,
			 m_bIsStencilBufferEnabled,
			 m_bIsPerformanceHudEnabled;

		uint m_uiDepthBufferSize,
			 m_uiStencilBufferSize;
	};
Meson_Vistas_END
