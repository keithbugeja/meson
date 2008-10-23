//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9WindowRenderTarget
//	Render target implementation for Direct3D9 renderer.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <d3dx9math.h>
#include "VstVistasEngine.h"
#include "VstWindowMessageManager.h"
#include "DX9RenderDevice.h"
#include "DX9WindowRenderTarget.h"

using namespace Meson::Common;
using namespace Meson::Common::Config;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
///	Create window render target.
///	\param p_hInstance Module instance containing WndProc
/// \param p_pAdapterDevice Pointer to an adapter-device object
//----------------------------------------------------------------------------------------------
DX9WindowRenderTarget::DX9WindowRenderTarget( HINSTANCE p_hInstance, DX9AdapterDevice *p_pAdapterDevice ) 
	: m_hInstance(p_hInstance), 
	  m_pAdapterDevice(p_pAdapterDevice), 
	  m_hWnd(NULL), 
	  m_pSwapChain(NULL),
	  m_pRenderTargetSurface(NULL),
	  m_pDepthStencilSurface(NULL)
{
}

//----------------------------------------------------------------------------------------------
/// Creates a window render target object.
/// \param p_strName Name used to index render target with render device object
/// \param p_uiWidth Width of target buffer
/// \param p_uiHeight Height of target buffer
/// \param p_bFullScreen Determines whether the window will be opened in full screen mode or not
/// \param p_pConfiguration Pointer to a configuration object used to supply additional creation information
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::Create(const String& p_strName, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, Meson::Common::Config::Configuration* p_pConfiguration)
{
	/** Initialise the render target by setting its name,
		width, height and full screen settings.
	 */
	m_strName		= p_strName;
	m_uiWidth		= p_uiWidth;
	m_uiHeight		= p_uiHeight;
	m_bIsFullScreen = p_bFullScreen;

	/** Parse the render target configuration
	 */
	String strWindowTitle;

	if (p_pConfiguration->Contains("Window.Title"))
		strWindowTitle = *(p_pConfiguration->Get("Window.Title")->GetString());
	else
		strWindowTitle = "Vistas DirectX9 Application";

	m_nLeft = p_pConfiguration->Contains("Window.Left") ? p_pConfiguration->Get("Window.Left")->GetLong() : 0;
	m_nTop = p_pConfiguration->Contains("Window.Top") ? p_pConfiguration->Get("Window.Top")->GetLong() : 0;

	m_uiColourBufferSize = p_pConfiguration->Contains("Device.ColourBuffer.Size") ? p_pConfiguration->Get("Device.ColourBuffer.Size")->GetLong() : 32;

	m_bIsDepthBufferEnabled = p_pConfiguration->Contains("Device.DepthBuffer.Enabled") ? p_pConfiguration->Get("Device.DepthBuffer.Enabled")->GetBool() : true;
	m_uiDepthBufferSize = p_pConfiguration->Contains("Device.DepthBuffer.Size") ? p_pConfiguration->Get("Device.DepthBuffer.Size")->GetLong() : 16;

	m_bIsStencilBufferEnabled = p_pConfiguration->Contains("Device.StencilBuffer.Enabled") ? p_pConfiguration->Get("Device.StencilBuffer.Enabled")->GetBool() : false;
	m_uiStencilBufferSize = p_pConfiguration->Contains("Device.StencilBuffer.Size") ? p_pConfiguration->Get("Device.StencilBuffer.Size")->GetLong() : 8;
	
	m_bIsPerformanceHudEnabled = p_pConfiguration->Contains("Device.PerformanceHud.Enabled") ? p_pConfiguration->Get("Device.PerformanceHud.Enabled")->GetBool() : false;
	
	/** Check if window class has already been registered. If it 
		has not, create a new class and register it.
	 */
	WNDCLASSEX windowClassEX;

	// Register window class if first time
	if (!GetClassInfoEx(m_hInstance, "VistasD3D9Window", &windowClassEX))
	{
		windowClassEX.cbSize		= sizeof(WNDCLASSEX);
		windowClassEX.style         = CS_HREDRAW | CS_VREDRAW;
		windowClassEX.lpfnWndProc	= &WindowMessageManager::StaticWndProc;
		windowClassEX.cbClsExtra    = 0;
		windowClassEX.cbWndExtra    = 0;
		windowClassEX.hInstance     = m_hInstance;
		windowClassEX.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		windowClassEX.hCursor		= LoadCursor(NULL, IDC_ARROW);
		windowClassEX.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
		windowClassEX.lpszMenuName  = NULL;
		windowClassEX.lpszClassName	= "VistasD3D9Window";
		windowClassEX.hIconSm		= NULL;

		if(!RegisterClassEx(&windowClassEX))
			throw new MesonException("Failed registering window class!");
	}

	/**	Create new window for render target and save
		the render target pointer in the window structure's
		user data section.
	 */
	dword dwStyle = WS_OVERLAPPEDWINDOW;

	if (!(m_hWnd = CreateWindow("VistasD3D9Window", strWindowTitle, dwStyle, m_nLeft, m_nTop, m_uiWidth, m_uiHeight, NULL, NULL, m_hInstance, NULL)))
		throw new MesonException("Failed to create window!");

	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (__int3264)(LONG_PTR)this);
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);

	/**	Attach window to message manager. This way, the window will be notified
		of window events and messages.
	 */
	VistasEngine::GetInstance()->GetWindowMessageManager()->AttachWindow(this);

	/** Initialise Direct3D resources such as presentation parameters
		and create the Direct3D9 device if it has not been created yet.
	 */
	InitialiseDirect3D();

	/** Switch the window to full screen mode, if required.
	 */
	SetFullScreen(p_bFullScreen);
}

//----------------------------------------------------------------------------------------------
/// Destroys the render target.
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::Destroy(void) 
{
	/** If this is the primary window, force shutdown on 
		Direct3D resources. 
	 */
	if (m_bIsPrimaryWindow)
		ShutdownDirect3D();

	/** Stop this render target from receiving any more
		window messages. Detach from window message manager.
	 */
	VistasEngine::GetInstance()->GetWindowMessageManager()->DetachWindow(this);

	/** Physically destroy this window, setting its handle
		to NULL. Set the active flag to false.
	 */
	DestroyWindow(m_hWnd);

	m_hWnd = NULL;
	m_bActive = false;
}

//----------------------------------------------------------------------------------------------
/// Initialises the Direct3D9 Device.
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::InitialiseDirect3D(void)
{
	Meson::Common::Logging::Channel& channelOut = VistasEngine::GetInstance()->Logger().Out;

	/** If specified settings are not supported, fall back to a 
		windowed 640 x 480 display.
	 */
	if (!m_pAdapterDevice->IsModeSupported(m_uiWidth, m_uiHeight, m_uiColourBufferSize, m_uiDepthBufferSize, m_uiStencilBufferSize, 60, !m_bIsFullScreen, &m_presentParameters))
		channelOut << "WARNING: Mode not supported by selected adapter. Switching to compatibility mode.";
	else
		channelOut << "Setting adapter mode to " << m_uiWidth << " by " << m_uiHeight << ", C=" << m_uiColourBufferSize << 
					  ", Z=" << m_uiDepthBufferSize << ", S=" << m_uiStencilBufferSize << ", FullScreen=" << m_bIsFullScreen << "\n";

	m_presentParameters.hDeviceWindow = m_hWnd;

	/** We want a single Direct3D Device per application - instead of creating a new device
		per window, we create a new swap chain bound to the one and only device.
	 */
	if (m_pAdapterDevice->GetDirect3DDevice() == NULL)
	{
		LPDIRECT3DDEVICE9 pDirect3DDevice9 = NULL;
		
		/** Create the device with the following behaviour flags: 
		 */
		dword dwBehaviourFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE;

		/** If the NVIDIA performance hud is enabled, create the device using the 
			reference rasteriser, otherwise use the hardware abstraction layer.
		 */
		if (m_bIsPerformanceHudEnabled)
		{
			if (D3D_OK != m_pAdapterDevice->GetDirect3D()->CreateDevice( m_pAdapterDevice->GetAdapterID(), D3DDEVTYPE_REF, m_hWnd, dwBehaviourFlags, &m_presentParameters, &pDirect3DDevice9))
				throw new MesonException("Failed to create Direct3D9 Device.", __FILE__, __LINE__ );
		}
		else
		{
			if (D3D_OK != m_pAdapterDevice->GetDirect3D()->CreateDevice( m_pAdapterDevice->GetAdapterID(), D3DDEVTYPE_HAL, m_hWnd, dwBehaviourFlags, &m_presentParameters, &pDirect3DDevice9))
			throw new MesonException("Failed to create Direct3D9 Device.", __FILE__, __LINE__ );
		}

		// Get default swap chain
		pDirect3DDevice9->GetSwapChain(0, &m_pSwapChain);

		// Get Render Target and Depth-Stencil surface
		pDirect3DDevice9->GetRenderTarget(0, &m_pRenderTargetSurface);
		pDirect3DDevice9->GetDepthStencilSurface(&m_pDepthStencilSurface);

		// Set the device-adapter handle to a valid 
		m_pAdapterDevice->SetDirect3DDevice(pDirect3DDevice9);
		
		// This is the primary window
		m_bIsPrimaryWindow = true;
	}
	else
	{
		// Create an additional swap chain for secondary windows
		if (D3D_OK != m_pAdapterDevice->GetDirect3DDevice()->CreateAdditionalSwapChain(&m_presentParameters, &m_pSwapChain))
			throw new MesonException("Failed to create swap chain.", __FILE__, __LINE__ );

		// Get Render Target
		m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pRenderTargetSurface);

		// TODO: Create Depth-Stencil surface
		// HINT: Additional surfaces will not work as render targets unless this is implemented.
		throw new MesonException("Multiple window chains not yet implemented!", __FILE__, __LINE__ );

		m_bIsPrimaryWindow = false;
	}
}

//----------------------------------------------------------------------------------------------
/// If render target is the primary window, this call terminates the Direct3D9 Device.
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::ShutdownDirect3D(void)
{
	// Release Target Surface
	SAFE_RELEASE(m_pRenderTargetSurface);

	// Release Depth/Stencil Surface
	SAFE_RELEASE(m_pDepthStencilSurface);

	// Release Swap Chain
	SAFE_RELEASE(m_pSwapChain);
}

//----------------------------------------------------------------------------------------------
/// Swaps the back buffer with the front buffer.
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::Present(void)
{
	DX9RenderDevice* pDX9RenderDevice = NULL;

	if (m_pSwapChain != NULL)
	{
		HRESULT hResult = m_pSwapChain->Present(NULL, NULL, NULL, NULL, 0);
		
		switch(hResult)
		{
			case D3D_OK:
				return;

			case D3DERR_DEVICELOST:
				pDX9RenderDevice = (DX9RenderDevice*)VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();
				pDX9RenderDevice->OnDeviceLost();
				return;

			case D3DERR_DEVICENOTRESET:
			{
				ResetDevice();

				pDX9RenderDevice = (DX9RenderDevice*)VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();
				pDX9RenderDevice->OnDeviceReset();
				return;
			}

			default:
				throw new MesonException("Present operation failed.", __FILE__, __LINE__);
		}
	}
}

//----------------------------------------------------------------------------------------------
///	Resets the Direct3D9 device.
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::ResetDevice(void)
{
	LPDIRECT3DDEVICE9 pDirect3DDevice9 = m_pAdapterDevice->GetDirect3DDevice();

	SAFE_RELEASE(m_pRenderTargetSurface);
	SAFE_RELEASE(m_pDepthStencilSurface);
	
	pDirect3DDevice9->Reset(&m_presentParameters);

	pDirect3DDevice9->GetRenderTarget(0, &m_pRenderTargetSurface);
	pDirect3DDevice9->GetDepthStencilSurface(&m_pDepthStencilSurface);
}

//----------------------------------------------------------------------------------------------
/// Toggles between windowed and full screen mode.
/// \param p_bEnable If true, full screen mode is enabled.
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::SetFullScreen(bool p_bEnable)
{
	if (p_bEnable)
	{
		m_presentParameters.BackBufferWidth = m_uiWidth;
		m_presentParameters.BackBufferHeight = m_uiHeight;
		m_presentParameters.Windowed = false;

		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, m_uiWidth, m_uiHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		m_presentParameters.BackBufferWidth = m_uiWidth;
		m_presentParameters.BackBufferHeight = m_uiHeight;
		m_presentParameters.Windowed = true;

		int nScreenWidth	= GetSystemMetrics(SM_CXSCREEN),
			nScreenHeight	= GetSystemMetrics(SM_CYSCREEN);

		int nLeft = (nScreenWidth - m_uiWidth) >> 2,
			nTop = (nScreenHeight - m_uiHeight) >> 2;

		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hWnd, HWND_TOP, nLeft, nTop, m_uiWidth, m_uiHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	DX9RenderDevice *pDX9RenderDevice = static_cast<DX9RenderDevice*>(VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive());
	pDX9RenderDevice->OnDeviceLost();

	ResetDevice();

	pDX9RenderDevice->OnDeviceReset();
}

//----------------------------------------------------------------------------------------------
/// Event triggered every time the window changes position.
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::OnWindowMoved(void) 
{
	RECT rcWindow;

	GetWindowRect(m_hWnd, &rcWindow );
	
	m_nLeft = rcWindow.left;
	m_nTop = rcWindow.top;
}

//----------------------------------------------------------------------------------------------
/// Event triggered every time the window is resized.
//----------------------------------------------------------------------------------------------
void DX9WindowRenderTarget::OnWindowResized(void)
{
	RECT rcWindow;

	GetWindowRect(m_hWnd, &rcWindow );

	m_uiWidth = rcWindow.right - rcWindow.left;
	m_uiHeight = rcWindow.bottom - rcWindow.top;
}
//----------------------------------------------------------------------------------------------
LPDIRECT3DSURFACE9 DX9WindowRenderTarget::GetRenderTargetSurface(void) const
{
	return m_pRenderTargetSurface;
}
//----------------------------------------------------------------------------------------------
LPDIRECT3DSURFACE9 DX9WindowRenderTarget::GetDepthStencilSurface(void) const
{
	return m_pDepthStencilSurface;
}

Meson_Vistas_END