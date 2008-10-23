//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9AdapterDevice
//	The adapter device maps a graphics adapter to a Direct3D9 device and provides information
//	about supported modes, driver information and adapter identification.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "String8.h"
#include "VstNamespaces.h"

Meson_Vistas_BEGIN
	class DX9AdapterDevice
	{
	public:
		DX9AdapterDevice( LPDIRECT3D9 p_pD3D9, uint p_uiAdapterID );
		~DX9AdapterDevice(void);

		LPDIRECT3D9 GetDirect3D(void);

		void SetDirect3DDevice( LPDIRECT3DDEVICE9 p_d3dDevice9 );
		LPDIRECT3DDEVICE9 GetDirect3DDevice(void);

		uint GetAdapterID(void);
		Meson::Common::Text::String GetDescription(void);
		Meson::Common::Text::String GetDeviceName(void);
		Meson::Common::Text::String GetDriver(void);

		bool IsModeSupported(uint p_uiWidth, uint p_uiHeight, uint p_uiColourBufferSize, uint p_uiDepthBufferSize, uint p_uiStencilBufferSize, uint p_uiRefreshRate, bool p_bWindowed, D3DPRESENT_PARAMETERS* p_pD3DPresentParameters);

	protected:
		LPDIRECT3D9				m_pD3D9;
		LPDIRECT3DDEVICE9		m_pD3D9Device;
		D3DADAPTER_IDENTIFIER9	m_adapterIdentifier;
		
		uint m_uiAdapterID;
	};
Meson_Vistas_END
