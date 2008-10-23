//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9AdapterDevice
//	The adapter device maps a graphics adapter to a Direct3D9 device and provides information
//	about supported modes, driver information and adapter identification.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "MesonException.h"
#include "DX9AdapterDevice.h"
#include "DX9Types.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
DX9AdapterDevice::DX9AdapterDevice( LPDIRECT3D9 p_pD3D9, uint p_uiAdapterID )
	: m_pD3D9(p_pD3D9), m_pD3D9Device(NULL), m_uiAdapterID(p_uiAdapterID)
{
	if (D3D_OK != p_pD3D9->GetAdapterIdentifier( p_uiAdapterID, NULL, &m_adapterIdentifier))
		throw new MesonException("Cannot instantiate DX9AdapterDevice : failed retrieving adapter identifier.");
}
//----------------------------------------------------------------------------------------------
DX9AdapterDevice::~DX9AdapterDevice(void)
{
	// Releases the Direct3D9 device
	SAFE_RELEASE(m_pD3D9Device);
}
//----------------------------------------------------------------------------------------------
LPDIRECT3D9 DX9AdapterDevice::GetDirect3D(void) 
{
	return m_pD3D9;
}
//----------------------------------------------------------------------------------------------
void DX9AdapterDevice::SetDirect3DDevice( LPDIRECT3DDEVICE9 p_d3dDevice9 ) 
{
	m_pD3D9Device = p_d3dDevice9;
}
//----------------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 DX9AdapterDevice::GetDirect3DDevice(void) 
{
	return m_pD3D9Device;
}
//----------------------------------------------------------------------------------------------
uint DX9AdapterDevice::GetAdapterID(void) 
{
	return m_uiAdapterID;
}
//----------------------------------------------------------------------------------------------
String DX9AdapterDevice::GetDescription(void) 
{
	return String(m_adapterIdentifier.Description);
}
//----------------------------------------------------------------------------------------------
String DX9AdapterDevice::GetDeviceName(void) 
{
	return String(m_adapterIdentifier.DeviceName);
}
//----------------------------------------------------------------------------------------------
String DX9AdapterDevice::GetDriver(void) 
{
	return String(m_adapterIdentifier.Driver);
}
//----------------------------------------------------------------------------------------------
bool DX9AdapterDevice::IsModeSupported(uint p_uiWidth, uint p_uiHeight, uint p_uiColourBufferSize, uint p_uiDepthBufferSize, uint p_uiStencilBufferSize, uint p_uiRefreshRate, bool p_bWindowed, D3DPRESENT_PARAMETERS* p_pD3DPresentParameters)
{
	D3DFORMAT d3dFormat;
	D3DDISPLAYMODE d3dDisplayMode;
	
	// Clear presentation parameters
	ZeroMemory(p_pD3DPresentParameters, sizeof(D3DPRESENT_PARAMETERS));

	/** Assign a format on the basis of 
		back buffer depth.
	 */
	switch(p_uiColourBufferSize)
	{
		case 32:
		case 24:
			d3dFormat = D3DFMT_X8R8G8B8;
			break;

		case 16:
		default:
			d3dFormat = D3DFMT_R5G6B5;
			break;
	}

	/** When running in full screen, check if the
		required display mode is supported.
	 */
	bool bAdapterModeSupported = p_bWindowed;

	if (!p_bWindowed)
	{
		uint uiAdapterModeCount = m_pD3D9->GetAdapterModeCount(m_uiAdapterID, d3dFormat);
		
		/** Enumerate the back buffer modes and look for
			one which matches the desired values. 
		 */

		for (uint uiMode = 0; uiMode < uiAdapterModeCount; uiMode++)
		{
			m_pD3D9->EnumAdapterModes(m_uiAdapterID, d3dFormat, uiMode, &d3dDisplayMode);

			if (d3dDisplayMode.Width != p_uiWidth || d3dDisplayMode.Height != p_uiHeight)
				continue;

			if (p_uiRefreshRate != 0 && p_uiRefreshRate != d3dDisplayMode.RefreshRate)
				continue;

			bAdapterModeSupported = true;
			break;
		}
	}

	/** If adapter is supported or we are running in windowed mode, 
		set presentation parameters to required settings. If adapter
		is not supported, set presentation parameters to a fallback
		mode.
	 */
	if (bAdapterModeSupported)
	{
		p_pD3DPresentParameters->Windowed = p_bWindowed;
		p_pD3DPresentParameters->BackBufferCount = 1;
		p_pD3DPresentParameters->BackBufferFormat = d3dFormat;
		p_pD3DPresentParameters->BackBufferHeight = p_uiHeight;
		p_pD3DPresentParameters->BackBufferWidth = p_uiWidth;
		p_pD3DPresentParameters->SwapEffect = D3DSWAPEFFECT_COPY;
		p_pD3DPresentParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		p_pD3DPresentParameters->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
		p_pD3DPresentParameters->EnableAutoDepthStencil = (p_uiDepthBufferSize != 0 || p_uiStencilBufferSize != 0);

		if (p_uiStencilBufferSize == 0)
		{
			if (p_uiDepthBufferSize == 32)
				p_pD3DPresentParameters->AutoDepthStencilFormat = D3DFMT_D32;
			else if (p_uiDepthBufferSize == 24)
				p_pD3DPresentParameters->AutoDepthStencilFormat = D3DFMT_D24X8;
			else
				p_pD3DPresentParameters->AutoDepthStencilFormat = D3DFMT_D16;
		}
		else
			p_pD3DPresentParameters->AutoDepthStencilFormat = D3DFMT_D24S8;
	}
	else
	{
		D3DDISPLAYMODE d3dDisplayMode;

		m_pD3D9->GetAdapterDisplayMode(m_uiAdapterID, &d3dDisplayMode);

		p_pD3DPresentParameters->BackBufferFormat = d3dDisplayMode.Format;
		p_pD3DPresentParameters->BackBufferWidth = d3dDisplayMode.Width;
		p_pD3DPresentParameters->BackBufferHeight = d3dDisplayMode.Height;
		p_pD3DPresentParameters->EnableAutoDepthStencil = FALSE;
		p_pD3DPresentParameters->SwapEffect = D3DSWAPEFFECT_DISCARD;
		p_pD3DPresentParameters->PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		p_pD3DPresentParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		p_pD3DPresentParameters->Windowed = true;
	}

	return bAdapterModeSupported;
}

Meson_Vistas_END
