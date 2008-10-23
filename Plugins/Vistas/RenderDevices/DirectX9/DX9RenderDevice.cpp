//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "DX9RenderDevice.h"
#include "VstGeometry.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Config;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
//	Direct3D9 object reference
//----------------------------------------------------------------------------------------------
LPDIRECT3D9 DX9RenderDevice::m_pD3D9 = NULL;
//----------------------------------------------------------------------------------------------
/** Direct3D uses the same comparison flags for Alpha, Depth and Stencil
    Todo: Consider using a single comparison array.
 */
//----------------------------------------------------------------------------------------------
//  Depth buffer comparison functions
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dDepthCompareFunction[DepthRenderState::DRSCMP_COUNT] = 
{
	D3DCMP_NEVER,           
    D3DCMP_LESS,            
    D3DCMP_EQUAL,           
    D3DCMP_LESSEQUAL,       
	D3DCMP_GREATER,         
    D3DCMP_NOTEQUAL,        
    D3DCMP_GREATEREQUAL,    
    D3DCMP_ALWAYS
};
//----------------------------------------------------------------------------------------------
//  Alpha buffer comparison functions
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dAlphaCompareFunction[AlphaRenderState::ARSCMP_COUNT] = 
{
	D3DCMP_NEVER,           
    D3DCMP_LESS,            
    D3DCMP_EQUAL,           
    D3DCMP_LESSEQUAL,       
	D3DCMP_GREATER,         
    D3DCMP_NOTEQUAL,        
    D3DCMP_GREATEREQUAL,    
    D3DCMP_ALWAYS
};
//----------------------------------------------------------------------------------------------
//  Alpha blending functions
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dAlphaBlend[AlphaRenderState::ARSB_COUNT] = 
{
	D3DBLEND_ZERO,
	D3DBLEND_ONE,
	D3DBLEND_SRCCOLOR,
	D3DBLEND_INVSRCCOLOR,
	D3DBLEND_SRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_DESTCOLOR,
	D3DBLEND_INVDESTCOLOR,
	D3DBLEND_DESTALPHA,
	D3DBLEND_INVDESTALPHA,
	D3DBLEND_BLENDFACTOR,
	D3DBLEND_INVBLENDFACTOR,
	D3DBLEND_BLENDFACTOR,
	D3DBLEND_INVBLENDFACTOR,
	D3DBLEND_BLENDFACTOR,
	D3DBLEND_INVBLENDFACTOR
};
//----------------------------------------------------------------------------------------------
//  Stencil buffer comparison functions
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dStencilCompareFunction[StencilRenderState::SRSCMP_COUNT] =
{
	D3DCMP_NEVER,           
    D3DCMP_LESS,            
    D3DCMP_EQUAL,           
    D3DCMP_LESSEQUAL,       
	D3DCMP_GREATER,         
    D3DCMP_NOTEQUAL,        
    D3DCMP_GREATEREQUAL,    
    D3DCMP_ALWAYS
};
//----------------------------------------------------------------------------------------------
//  Stencil operations
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dStencilOperation[StencilRenderState::SRSOP_COUNT] = 
{
	D3DSTENCILOP_KEEP,
	D3DSTENCILOP_ZERO,
	D3DSTENCILOP_REPLACE,
	D3DSTENCILOP_INCR,
	D3DSTENCILOP_DECR,
	D3DSTENCILOP_INCRSAT,
	D3DSTENCILOP_DECRSAT,
	D3DSTENCILOP_INVERT
};
//----------------------------------------------------------------------------------------------
//	Culling modes
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dCullMode[CullRenderState::CRSFW_COUNT] = 
{
	D3DCULL_CCW,
	D3DCULL_CW
};
//----------------------------------------------------------------------------------------------
//	Primitive types
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dPrimitiveType[PT_COUNT] = 
{
	D3DPT_POINTLIST,
	D3DPT_LINELIST,
	D3DPT_LINESTRIP,
	D3DPT_LINESTRIP,
	D3DPT_TRIANGLELIST,
	D3DPT_TRIANGLESTRIP,
	D3DPT_TRIANGLEFAN
};
//----------------------------------------------------------------------------------------------
//	Filter types
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dFilterType[SamplerState::FT_COUNT] =
{
	D3DTEXF_NONE,
	D3DTEXF_POINT,
	D3DTEXF_LINEAR,
	D3DTEXF_ANISOTROPIC
};
//----------------------------------------------------------------------------------------------
//	Texture wrapping types
//----------------------------------------------------------------------------------------------
DWORD DX9RenderDevice::m_d3dWrapType[SamplerState::WT_COUNT] =
{
	D3DTADDRESS_WRAP,
	D3DTADDRESS_MIRROR,
	D3DTADDRESS_CLAMP,
	D3DTADDRESS_BORDER,
	D3DTADDRESS_MIRRORONCE
};
//----------------------------------------------------------------------------------------------
DX9RenderDevice::DX9RenderDevice() 
	: RenderDeviceAdapter(),
	  m_strName("DirectX9"),
	  m_pHardwareResourceFactory(NULL),
	  m_pHLSLProgramFactory(NULL),
	  m_pAdapterManager(NULL),
	  m_pAdapterDevice(NULL),	  
	  m_pPrimaryWindow(NULL)
{
	// Set default configuration options
	SetDefaultConfiguration();

	// Get DLL Instance Handle
	m_hInstance = (HINSTANCE)GetModuleHandle("DirectX9Renderer.dll");
}
//----------------------------------------------------------------------------------------------
DX9RenderDevice::~DX9RenderDevice(){}
//----------------------------------------------------------------------------------------------
const String& DX9RenderDevice::GetName(void) const { return m_strName; }
//----------------------------------------------------------------------------------------------
/** Starts the Render Device, initialising it in the process and creating a default render
	target if required. The following steps are carried out during start up:

	1. Direct3D9 object created
	2. AdapterManager object created
	3. AdapterDevice object is initialised:
		* Associated with requested adapter id
		* Respective setup carried out, if performance HUD is enabled
	4. Initialises the device capability structures
	5. Creates the default window render target, if requested
 */
//----------------------------------------------------------------------------------------------
WindowRenderTarget* DX9RenderDevice::Startup(void) 
{
	// Base startup
	RenderDeviceAdapter::Startup();

	// Create D3D9 object
	if ( m_pD3D9 == NULL && (m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION )) == NULL )
		throw new MesonException("Failed to startup render device : cannot create Direct3D9 object.", __FILE__, __LINE__);

	// Create adapter manager
	m_pAdapterManager = new DX9AdapterManager(m_pD3D9);

	// Device Adapter
	uint uiAdapterID = m_configuration.GetLong("Adapter.ID");
	bool bUsePeformanceHud = m_configuration.GetBool("Adapter.PerformanceHud.Enabled");

	if(bUsePeformanceHud)
		m_pAdapterDevice = m_pAdapterManager->GetAdapterByDeviceID(m_pAdapterManager->GetAdapterCount() - 1);
	else
		m_pAdapterDevice = m_pAdapterManager->GetAdapterByDeviceID(uiAdapterID);

	/** Now that the adapter device has been initialised with the Direct3D9
		object, the RenderDevice may populate its capability structures for
		the given adapter
	 */
	SetDeviceCapabilities();

	/** Note that for the time being, the primary window must always be
		created, otherwise the D3D Device will not be initialised!

		Todo: Allow for a window handle to be passed and initialise D3D on
			  given handle.
	 */

	if (m_configuration.GetBool("Initialise.CreateWindow.Enabled"))
	{
		Configuration windowConfiguration;

		uint uiWidth  = m_configuration.GetLong("Initialise.CreateWindow.Width"),
			 uiHeight = m_configuration.GetLong("Initialise.CreateWindow.Height");

		bool bFullScreen = m_configuration.GetBool("Initialise.CreateWindow.FullScreen");

		windowConfiguration.Set("Window.Title", *m_configuration.Get("Initialise.CreateWindow.Title")->GetString());

		windowConfiguration.SetLong("Window.Left", m_configuration.GetLong("Initialise.CreateWindow.Left"));
		windowConfiguration.SetLong("Window.Top", m_configuration.GetLong("Initialise.CreateWindow.Top"));

		windowConfiguration.SetBool("Device.ColourBuffer.Enabled", m_configuration.GetBool("Initialise.CreateWindow.ColourBuffer.Enabled"));
		windowConfiguration.SetLong("Device.ColourBuffer.Size", m_configuration.GetLong("Initialise.CreateWindow.ColourBuffer.Size"));
		windowConfiguration.SetBool("Device.DepthBuffer.Enabled", m_configuration.GetBool("Initialise.CreateWindow.DepthBuffer.Enabled"));
		windowConfiguration.SetLong("Device.DepthBuffer.Size", m_configuration.GetLong("Initialise.CreateWindow.DepthBuffer.Size"));
		windowConfiguration.SetBool("Device.StencilBuffer.Enabled", m_configuration.GetBool("Initialise.CreateWindow.StencilBuffer.Enabled"));
		windowConfiguration.SetLong("Device.SrencilBuffer.Size", m_configuration.GetLong("Initialise.CreateWindow.StencilBuffer.Size"));
		windowConfiguration.SetBool("Device.PerformanceHud.Enabled", bUsePeformanceHud );

		WindowRenderTarget* pWindow = CreateWindowRenderTarget("DefaultWindowRenderTarget", uiWidth, uiHeight, bFullScreen, &windowConfiguration);
		
		// Register window render target as current within current device
		SetRenderTarget("DefaultWindowRenderTarget");

		return pWindow;
	}
	else
		throw new MesonException("Failed to startup render device : window-less initialisation is not supported yet.", __FILE__, __LINE__ );

	return NULL;
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::Shutdown(void)
{
	// Have any windows been created?
	if (m_pPrimaryWindow != NULL)
	{
		// Destroy secondary windows
		TEnumerator<DX9WindowRenderTarget*>& windowEnumerator = m_secondaryWindowList.GetEnumerator();

		while(windowEnumerator.HasMoreElements())
			windowEnumerator.NextElement()->Destroy();

		m_secondaryWindowList.Clear();

		// Destroy primary window
		m_pPrimaryWindow->Destroy();
		m_pPrimaryWindow = NULL;
	}

	// Clear active adapter device
	m_pAdapterDevice = NULL;

	// Delete sampler array
	SAFE_DELETE_ARRAY(m_pSamplerStates);

	// Delete lights array
	SAFE_DELETE(m_pLights);

	// Delete shader program manager
	SAFE_DELETE(m_pShaderProgramManager);

	// Delete HLSL Program factory
	SAFE_DELETE(m_pHLSLProgramFactory);

	// Delete hardware resource manager
	SAFE_DELETE(m_pHardwareResourceManager);

	// Delete hardware resource factory implementation
	SAFE_DELETE(m_pHardwareResourceFactory);

	// Delete Adapter Manager, releasing D3DDevice9 objects attached to AdapterDevices
	SAFE_DELETE(m_pAdapterManager);

	// Release Direct3D9 object
	SAFE_RELEASE(m_pD3D9);

	// Base shutdown
	RenderDeviceAdapter::Shutdown();
}
//----------------------------------------------------------------------------------------------
WindowRenderTarget* DX9RenderDevice::CreateWindowRenderTarget(const String& p_strName, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, Meson::Common::Config::Configuration* p_pConfiguration)
{
	if (m_renderTargetMap.ContainsKey(p_strName))
		throw new MesonException("Cannot create window render target : requested render target name is not unique.", __FILE__, __LINE__ );

	// New WindowRenderTarget instance
	DX9WindowRenderTarget* pWindow = new DX9WindowRenderTarget(m_hInstance, m_pAdapterDevice);
	
	// Create window
	pWindow->Create(p_strName, p_uiWidth, p_uiHeight, p_bFullScreen, p_pConfiguration);

	// Set as primary, if first created window
	if (m_pPrimaryWindow == NULL)
	{
		m_pPrimaryWindow = pWindow;
		
		// Hardware resource manager + factory
		m_pHardwareResourceFactory = new DX9HardwareResourceFactory(m_pAdapterDevice->GetDirect3DDevice());
		m_pHardwareResourceManager = new HardwareResourceManager(m_pHardwareResourceFactory);

		// Shader manager + HLSL program factory
		m_pShaderProgramManager = new ShaderProgramManager();
		m_pHLSLProgramFactory = new DX9HLSLProgramFactory(m_pAdapterDevice->GetDirect3DDevice());
		m_pShaderProgramManager->RegisterShaderProgramFactory(m_pHLSLProgramFactory);
		m_pShaderProgramManager->SetShaderProgramFactory(m_pHLSLProgramFactory->GetName());

		// Lights
		{
			uint uiMaxLights = GetDeviceCapabilities(DCI_MAXIMUM_LIGHTS);
			Light* pLights = m_pLights = (Light*)(new char[sizeof(Light) * uiMaxLights]);
			
			for (uint uiIndex = 0; uiIndex < uiMaxLights; uiIndex++, pLights++)
				pLights = new( pLights ) Light("InternalLight");
		}

		// Sampler States
		{
			uint uiMaxSamplers = GetDeviceCapabilities(DCI_MAXIMUM_SIMULATANEOUS_TEXTURES);
			m_pSamplerStates = new SamplerState[uiMaxSamplers];
		}
	}
	else
	{
		m_secondaryWindowList.Add(pWindow);
	}

	// Attach to render target list
	AttachRenderTarget(pWindow);

	// Return render target
	return pWindow;
}
//----------------------------------------------------------------------------------------------
TextureRenderTarget* DX9RenderDevice::CreateTextureRenderTarget(const Meson::Common::Text::String& p_strName, TextureBuffer* p_pTextureBuffer, bool p_bCreateDepthStencilSurface)
{
	if (m_renderTargetMap.ContainsKey(p_strName))
		throw new MesonException("Cannot create texture render target : requested render target name is not unique.", __FILE__, __LINE__ );

	// Create render target
	DX9TextureRenderTarget* pTextureRenderTarget = new DX9TextureRenderTarget(m_pAdapterDevice);
	pTextureRenderTarget->Create(p_strName, p_pTextureBuffer, p_bCreateDepthStencilSurface);
	
	// Attach to render target list
	AttachRenderTarget(pTextureRenderTarget);

	// Return render target
	return pTextureRenderTarget;
}
//----------------------------------------------------------------------------------------------
MultipleTextureRenderTarget* DX9RenderDevice::CreateMultipleTextureRenderTarget(const Meson::Common::Text::String& p_strName)
{
	if (m_renderTargetMap.ContainsKey(p_strName))
		throw new MesonException("Cannot create texture render target : requested render target name is not unique.", __FILE__, __LINE__ );

	MultipleTextureRenderTarget* pMultiRenderTarget = new MultipleTextureRenderTarget();
	pMultiRenderTarget->Create(p_strName);

	AttachRenderTarget(pMultiRenderTarget);

	return pMultiRenderTarget;
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::DestroyRenderTarget(const Meson::Common::Text::String& p_strName)
{
	RenderTarget* pRenderTarget = NULL;

	if (!m_renderTargetMap.ContainsKey(p_strName, pRenderTarget))
		throw new MesonException("Cannot destroy render target : requested render target not found.", __FILE__, __LINE__ );

	switch (pRenderTarget->GetType())
	{
		case RenderTarget::RTT_TEXTURE:
		{
			// Remove render target from list
			m_renderTargetMap.RemoveKey(p_strName);

			// Destroy texture render target
			TextureRenderTarget* pTextureRenderTarget = (TextureRenderTarget*)pRenderTarget;
			pTextureRenderTarget->Destroy();
			break;
		}

		case RenderTarget::RTT_MULTI:
		{
			// Remove render target from list
			m_renderTargetMap.RemoveKey(p_strName);
			
			// Destroy texture render target
			MultipleTextureRenderTarget* pMultiRenderTarget = (MultipleTextureRenderTarget*)pRenderTarget;
			pMultiRenderTarget->Destroy();
			break;
		}

		default:
			throw new MesonException("Cannot destroy render target type : method not yet implemented.", __FILE__, __LINE__ );
	}
}
//----------------------------------------------------------------------------------------------
bool DX9RenderDevice::BeginScene(void)
{
	if (IsDeviceLost())
	{
		OnDeviceLost();
		return false;
	}

	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->BeginScene()))
		throw new MesonException("BeginScene failed.", __FILE__, __LINE__ );
		
	return true;
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::EndScene(void)
{
	if (IsDeviceLost())
		OnDeviceLost();
	else
		if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->EndScene()))
			throw new MesonException("EndScene failed.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::ClearDepthBuffer(float p_fDepthValue)
{
	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, p_fDepthValue, 0)))
		throw new MesonException("Failed clearing depth buffer : make sure that render target has a depth buffer.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::ClearStencilBuffer(uint p_uiStencilValue)
{
	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, p_uiStencilValue)))
		throw new MesonException("Failed clearing stencil buffer : make sure that render target has a stencil buffer.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::ClearColourBuffer(const Colour& p_colour)
{
	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, p_colour.ToARGB(), 0, 0)))
		throw new MesonException("Failed clearing colour buffer.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::ClearBuffers(const Colour& p_colour, float p_fDepthValue, uint p_uiStencilValue)
{
	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, p_colour.ToARGB(), p_fDepthValue, p_uiStencilValue)))
		throw new MesonException("Failed clearing buffers : make sure that render target has both depth and stencil buffers.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetColourMask(IRenderDevice::ColourMask p_eColourMask)
{
	RenderDeviceAdapter::SetColourMask(p_eColourMask);
	SetD3DRenderState(D3DRS_COLORWRITEENABLE, ((DWORD)p_eColourMask) & 0xf);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetBiasRenderState(BiasRenderState* p_pBiasRenderState)
{
	RenderDeviceAdapter::SetBiasRenderState(p_pBiasRenderState);

	float fScaleBias = p_pBiasRenderState->GetScaleBias(),
		  fDepthBias = p_pBiasRenderState->GetDepthBias();

	SetD3DRenderState(D3DRS_SLOPESCALEDEPTHBIAS, reinterpret_cast<DWORD&>(fScaleBias));
	SetD3DRenderState(D3DRS_DEPTHBIAS, reinterpret_cast<DWORD&>(fDepthBias));
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetTargetRenderState(TargetRenderState* p_pTargetRenderState)
{
	RenderDeviceAdapter::SetTargetRenderState(p_pTargetRenderState);

	SetColourMask((IRenderDevice::ColourMask)p_pTargetRenderState->GetColourMask());
	SetRenderTarget(p_pTargetRenderState->GetRenderTarget());
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetMaterialRenderState(MaterialRenderState* p_pMaterialRenderState)
{
	RenderDeviceAdapter::SetMaterialRenderState(p_pMaterialRenderState);

	SetD3DRenderState(D3DRS_FILLMODE, p_pMaterialRenderState->IsWireframeEnabled() ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

#ifndef DX9RD_FFP_OFF
	D3DMATERIAL9 d3dMaterial;

	d3dMaterial.Ambient = GetD3DColourValue(p_pMaterialRenderState->GetAmbient());
	d3dMaterial.Diffuse	= GetD3DColourValue(p_pMaterialRenderState->GetDiffuse());
	d3dMaterial.Emissive = GetD3DColourValue(p_pMaterialRenderState->GetEmissive());
	d3dMaterial.Specular = GetD3DColourValue(p_pMaterialRenderState->GetSpecular());
	d3dMaterial.Power = p_pMaterialRenderState->GetShininess();

	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetMaterial(&d3dMaterial)))
		throw new MesonException("Failed setting material.", __FILE__, __LINE__ );
#endif
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetStencilRenderState(StencilRenderState *p_pStencilRenderState)
{
	RenderDeviceAdapter::SetStencilRenderState(p_pStencilRenderState);

	SetD3DRenderState(D3DRS_STENCILENABLE, p_pStencilRenderState->IsStencilTestEnabled());

	SetD3DRenderState(D3DRS_STENCILPASS, m_d3dStencilOperation[p_pStencilRenderState->GetPassOperation()]);
	SetD3DRenderState(D3DRS_STENCILFAIL, m_d3dStencilOperation[p_pStencilRenderState->GetFailOperation()]);
	SetD3DRenderState(D3DRS_STENCILZFAIL, m_d3dStencilOperation[p_pStencilRenderState->GetDepthFailOperation()]);

	SetD3DRenderState(D3DRS_STENCILREF, p_pStencilRenderState->GetStencilReference());
	SetD3DRenderState(D3DRS_STENCILMASK, p_pStencilRenderState->GetStencilMask());
	SetD3DRenderState(D3DRS_STENCILWRITEMASK, p_pStencilRenderState->GetStencilWriteMask());
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetDepthRenderState(DepthRenderState *p_pDepthRenderState)
{
	RenderDeviceAdapter::SetDepthRenderState(p_pDepthRenderState);

	SetD3DRenderState(D3DRS_ZENABLE, p_pDepthRenderState->IsDepthTestEnabled());
	SetD3DRenderState(D3DRS_ZWRITEENABLE, p_pDepthRenderState->IsDepthWriteEnabled());
	SetD3DRenderState(D3DRS_ZFUNC, m_d3dDepthCompareFunction[p_pDepthRenderState->GetCompareType()] );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetAlphaRenderState(AlphaRenderState *p_pAlphaRenderState)
{
	RenderDeviceAdapter::SetAlphaRenderState(p_pAlphaRenderState);

	SetD3DRenderState(D3DRS_ALPHATESTENABLE, p_pAlphaRenderState->IsAlphaTestEnabled());
	SetD3DRenderState(D3DRS_ALPHAREF, p_pAlphaRenderState->GetAlphaReference());
	SetD3DRenderState(D3DRS_ALPHAFUNC, m_d3dAlphaCompareFunction[p_pAlphaRenderState->GetCompareType()] );

	SetD3DRenderState(D3DRS_ALPHABLENDENABLE, p_pAlphaRenderState->IsAlphaBlendEnabled());
	SetD3DRenderState(D3DRS_SRCBLEND, m_d3dAlphaBlend[p_pAlphaRenderState->GetSourceBlendMode()]);
	SetD3DRenderState(D3DRS_DESTBLEND, m_d3dAlphaBlend[p_pAlphaRenderState->GetDestinationBlendMode()]);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetCullRenderState(CullRenderState *p_pCullRenderState)
{
	RenderDeviceAdapter::SetCullRenderState(p_pCullRenderState);

	// Is culling enabled?
	if (p_pCullRenderState->IsCullingEnabled())
	{
		/** Face culling mode is inverted : Direct3D works with a left handed coordinate frame,
			while Vistas uses a right handed system; the conversion process from rhs to lhs
			invalidates vertex windings and therefore, face culling has been flipped to retain
			a correct working order across all drivers.
		 */ 
		// Is front or back face culling enabled? 
		if (p_pCullRenderState->GetFaceCullingMode() == CullRenderState::CRSFC_FRONT)
			SetD3DRenderState(D3DRS_CULLMODE, m_d3dCullMode[p_pCullRenderState->GetFaceWindingMode()]);
		else
		{
			// Back face culling; reverse winding orders
			if (p_pCullRenderState->GetFaceWindingMode() == CullRenderState::CRSFW_CW)
				SetD3DRenderState(D3DRS_CULLMODE, m_d3dCullMode[CullRenderState::CRSFW_CCW]);
			else
				SetD3DRenderState(D3DRS_CULLMODE, m_d3dCullMode[CullRenderState::CRSFW_CW]);
		}
	}
	else
	{
		// Culling disabled
		SetD3DRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetLightRenderState(LightRenderState* p_pLightRenderState)
{
	RenderDeviceAdapter::SetLightRenderState(p_pLightRenderState);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetSamplerRenderState(SamplerRenderState* p_pSamplerRenderState)
{
	RenderDeviceAdapter::SetSamplerRenderState(p_pSamplerRenderState);

	uint uiSamplerIndex = -1,
		 uiMaximumSamplers = GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_SIMULATANEOUS_TEXTURES);

	SamplerUnitEnumerator samplerUnitEnumerator = p_pSamplerRenderState->GetSamplerUnitEnumerator();
	while (samplerUnitEnumerator.HasMoreElements())
	{
		SamplerUnitPtr& pSamplerUnit = samplerUnitEnumerator.NextElement();
		
		if ((uiSamplerIndex = pSamplerUnit->GetSamplerIndex()) < uiMaximumSamplers)
		{
			if (pSamplerUnit->GetTextureState() != NULL)
				SetTexture(uiSamplerIndex, pSamplerUnit->GetTextureState()->GetTextureBuffer());

			if (pSamplerUnit->GetSamplerState() != NULL)
				SetSamplerState(uiSamplerIndex, pSamplerUnit->GetSamplerState());
		}
	}
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetStreamSource(uint p_uiStream, VertexBuffer* p_pVertexBuffer)
{
	m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetStreamSource(p_uiStream,(LPDIRECT3DVERTEXBUFFER9)(p_pVertexBuffer->GetResourceHandle()), 0, p_pVertexBuffer->GetVertexSize() );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetVertexDeclaration(VertexDeclaration* p_vertexDeclaration)
{
	DX9VertexDeclaration* pVertexDeclaration = static_cast<DX9VertexDeclaration*>(p_vertexDeclaration);
	m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetVertexDeclaration(pVertexDeclaration->GetVertexDeclaration(false));
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetVertexChannel(VertexChannel* p_pVertexChannel)
{
	DX9VertexDeclaration* pVertexDeclaration = static_cast<DX9VertexDeclaration*>((VertexDeclaration*)p_pVertexChannel->GetVertexDeclaration());
	
	// Bind streams
	StreamVertexBufferPairEnumerator vertexBufferEnumerator = p_pVertexChannel->GetStreamVertexBufferPairEnumerator();
	StreamVertexBufferPair keyValuePair;

	while(vertexBufferEnumerator.HasMoreElements())
	{
		keyValuePair = vertexBufferEnumerator.NextElement();
		m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetStreamSource(keyValuePair.Key,(LPDIRECT3DVERTEXBUFFER9)((keyValuePair.Value)->GetResourceHandle()), 0, keyValuePair.Value->GetVertexSize() );
	}

	// Bind vertex declaration
	m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetVertexDeclaration(pVertexDeclaration->GetVertexDeclaration(false));
}

//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetIndexBuffer(IndexBuffer* p_pIndexBuffer)
{
	m_pAdapterDevice->GetDirect3DDevice()->SetIndices((LPDIRECT3DINDEXBUFFER9)p_pIndexBuffer->GetResourceHandle());
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::DrawPrimitive(PrimitiveType p_ePrimitiveType, uint p_uiStartVertex, uint p_uiCount)
{
	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->DrawPrimitive((D3DPRIMITIVETYPE)m_d3dPrimitiveType[p_ePrimitiveType], p_uiStartVertex, p_uiCount)))
		throw new MesonException("DrawPrimitive failed.", __FILE__, __LINE__ );
}

//----------------------------------------------------------------------------------------------
void DX9RenderDevice::DrawIndexedPrimitive(PrimitiveType p_ePrimitiveType, uint p_uiStartVertex, uint p_uiStartIndex, uint p_uiIndexLowerBound, uint p_uiIndexUpperBound, uint p_uiPrimitiveCount )
{
	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_d3dPrimitiveType[p_ePrimitiveType], 
																							p_uiStartVertex, p_uiIndexLowerBound, p_uiIndexUpperBound - p_uiIndexLowerBound, 
																							p_uiStartIndex, p_uiPrimitiveCount)))
		throw new MesonException("DrawIndexedPrimitive failed.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetProjectionMatrix(const Meson::Common::Maths::Matrix4f& p_projectionMatrix)
{
	D3DMATRIX d3dProjection = GetD3DMatrix(p_projectionMatrix);

	RenderDeviceAdapter::SetProjectionMatrix(p_projectionMatrix);
	m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetTransform(D3DTS_PROJECTION, &d3dProjection);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetViewMatrix(const Meson::Common::Maths::Matrix4f& p_viewMatrix)
{
	// The negation of the z vector and the perspective divisor (w) is necessary
	// for converting from Vistas' right-handed coordinate system to direct3D's
	// left handed system. The base SetViewMatrix is not called to avoid redundancy.

	m_viewMatrix._11 = -p_viewMatrix._11;
	m_viewMatrix._12 = -p_viewMatrix._12;
	m_viewMatrix._13 = -p_viewMatrix._13;
	m_viewMatrix._14 = -p_viewMatrix._14;

	m_viewMatrix._21 =  p_viewMatrix._21;
	m_viewMatrix._22 =  p_viewMatrix._22;
	m_viewMatrix._23 =  p_viewMatrix._23;
	m_viewMatrix._24 =  p_viewMatrix._24;

	m_viewMatrix._31 = -p_viewMatrix._31;
	m_viewMatrix._32 = -p_viewMatrix._32;
	m_viewMatrix._33 = -p_viewMatrix._33;
	m_viewMatrix._34 = -p_viewMatrix._34;

	m_viewMatrix._41 =  p_viewMatrix._41;
	m_viewMatrix._42 =  p_viewMatrix._42;
	m_viewMatrix._43 =  p_viewMatrix._43;
	m_viewMatrix._44 =  p_viewMatrix._44;

	//RenderDeviceAdapter::SetViewMatrix(p_viewMatrix);

	D3DMATRIX d3dView = GetD3DMatrix(m_viewMatrix);
	m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetTransform(D3DTS_VIEW, &d3dView);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetWorldMatrix(const Meson::Common::Maths::Matrix4f& p_worldMatrix)
{
	D3DMATRIX d3dWorld = GetD3DMatrix(p_worldMatrix);

	RenderDeviceAdapter::SetWorldMatrix(p_worldMatrix);
	m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetTransform(D3DTS_WORLD, &d3dWorld);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetSamplerState(uint p_uiSamplerIndex, const SamplerState* p_pSamplerState)
{
	MESON_ASSERT(p_pSamplerState != NULL, "Unable to set Sampler State : Sampler Sate is null");

	RenderDeviceAdapter::SetSamplerState(p_uiSamplerIndex, p_pSamplerState);
	SetD3DSampler(p_uiSamplerIndex, &m_pSamplerStates[p_uiSamplerIndex]);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetTexture(uint p_uiTextureIndex, TextureBuffer* p_pTextureBuffer)
{
	LPDIRECT3DBASETEXTURE9 pTexture = (LPDIRECT3DBASETEXTURE9)p_pTextureBuffer->GetResourceHandle();

	if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetTexture(p_uiTextureIndex, pTexture)))
		throw new MesonException("SetTexture failed.", __FILE__, __LINE__ );
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetLight(uint p_uiLightIndex, const Light* p_pLight)
{
	RenderDeviceAdapter::SetLight(p_uiLightIndex, p_pLight);
	SetD3DLight(p_uiLightIndex, &m_pLights[p_uiLightIndex]);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetViewport(Viewport* p_pViewport)
{
	if (p_pViewport != m_pViewport)
	{
		RenderDeviceAdapter::SetViewport(p_pViewport);

		D3DVIEWPORT9 d3dViewport;

		d3dViewport.X = (DWORD)p_pViewport->GetMappedFrameLeft();
		d3dViewport.Y = (DWORD)p_pViewport->GetMappedFrameTop();
		d3dViewport.Width = (DWORD)p_pViewport->GetMappedFrameWidth();
		d3dViewport.Height = (DWORD)p_pViewport->GetMappedFrameHeight();
		d3dViewport.MinZ = p_pViewport->GetNearDepth();
		d3dViewport.MaxZ = p_pViewport->GetFarDepth();

		if (D3D_OK != (m_hResult = m_pAdapterDevice->GetDirect3DDevice()->SetViewport(&d3dViewport)))
			throw new MesonException("SetViewport failed.", __FILE__, __LINE__ );
	}
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetRenderTarget(const Meson::Common::Text::String& p_strName)
{
	// TODO:
	// Store driver results and throw exceptions if operations fail.
	RenderDeviceAdapter::SetRenderTarget(p_strName);

	LPDIRECT3DDEVICE9 pDirect3DDevice9 = m_pAdapterDevice->GetDirect3DDevice();

	if (m_pRenderTarget != NULL)
	{
		switch(m_pRenderTarget->GetType())
		{
			case RenderTarget::RTT_WINDOW:
			{
				DX9WindowRenderTarget* pWindowRenderTarget = (DX9WindowRenderTarget*)m_pRenderTarget;
				pDirect3DDevice9->SetRenderTarget(0, pWindowRenderTarget->GetRenderTargetSurface());
				pDirect3DDevice9->SetDepthStencilSurface(pWindowRenderTarget->GetDepthStencilSurface());
				break;
			}

			case RenderTarget::RTT_TEXTURE:
			{
				DX9TextureRenderTarget* pTextureRenderTarget = (DX9TextureRenderTarget*)m_pRenderTarget;
				pDirect3DDevice9->SetRenderTarget(0, pTextureRenderTarget->GetRenderTargetSurface());
				
				// Clear other render targets
				pDirect3DDevice9->SetRenderTarget(1, NULL);
				pDirect3DDevice9->SetRenderTarget(2, NULL);
				pDirect3DDevice9->SetRenderTarget(3, NULL);
				
				if (pTextureRenderTarget->HasDepthStencilSurface())
					pDirect3DDevice9->SetDepthStencilSurface(pTextureRenderTarget->GetDepthStencilSurface());

				break;
			}

			case RenderTarget::RTT_MULTI:
			{
				MultipleTextureRenderTarget* pMultiRenderTarget = (MultipleTextureRenderTarget*)m_pRenderTarget;
				
				for (uint uiIndex = 0, uiTargetCount = pMultiRenderTarget->Size(); 
					uiIndex < uiTargetCount; uiIndex++)
				{
					DX9TextureRenderTarget* pTextureRenderTarget = (DX9TextureRenderTarget*)pMultiRenderTarget->GetElement(uiIndex);
					pDirect3DDevice9->SetRenderTarget(uiIndex, pTextureRenderTarget->GetRenderTargetSurface());
					
					if (pTextureRenderTarget->HasDepthStencilSurface())
						pDirect3DDevice9->SetDepthStencilSurface(pTextureRenderTarget->GetDepthStencilSurface());
				}

				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::MakePerspectiveProjection(float p_fLeft, float p_fRight, float p_fTop, float p_fBottom, float p_fNear, float p_fFar, Meson::Common::Maths::Matrix4f* p_pMatrixOut )
{
	p_pMatrixOut->_12 = p_pMatrixOut->_14 = 
		p_pMatrixOut->_21 = p_pMatrixOut->_24 = 
		p_pMatrixOut->_31 = p_pMatrixOut->_32 = 
		p_pMatrixOut->_41 = p_pMatrixOut->_42 = p_pMatrixOut->_44 = 0.0f;

	float fRightMinLeft = p_fRight - p_fLeft,
		  fTopMinBottom = p_fTop - p_fBottom,
		  fNearMinFar	= p_fNear - p_fFar;

	p_pMatrixOut->_11 = 2.0f * p_fNear / fRightMinLeft;
	p_pMatrixOut->_13 = (p_fLeft + p_fRight) / fRightMinLeft;

	p_pMatrixOut->_22 = 2.0f * p_fNear / fTopMinBottom;
	p_pMatrixOut->_23 = (p_fTop + p_fBottom) / fTopMinBottom;
	
	p_pMatrixOut->_33 = p_fFar / fNearMinFar;
	p_pMatrixOut->_34 = p_fNear * p_pMatrixOut->_33;
	
	p_pMatrixOut->_43 = -1.0f;
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::MakeOrthographicProjection( float p_fLeft, float p_fRight, float p_fTop, float p_fBottom, float p_fNear, float p_fFar, Meson::Common::Maths::Matrix4f* p_pMatrixOut )
{
	p_pMatrixOut->_12 = p_pMatrixOut->_13 = 
		p_pMatrixOut->_21 = p_pMatrixOut->_23 = 
		p_pMatrixOut->_31 = p_pMatrixOut->_32 = 
		p_pMatrixOut->_41 = p_pMatrixOut->_42 = p_pMatrixOut->_43 = 0.0f;

	p_pMatrixOut->_11 = 2.0f / (p_fRight - p_fLeft);
	p_pMatrixOut->_22 = 2.0f / (p_fTop - p_fBottom);
	p_pMatrixOut->_33 = 1.0f / (p_fNear - p_fFar);
	p_pMatrixOut->_14 = (p_fLeft + p_fRight) / (p_fLeft - p_fRight);
	p_pMatrixOut->_24 = (p_fTop + p_fBottom) / (p_fBottom - p_fTop);
	p_pMatrixOut->_34 = p_fNear * p_pMatrixOut->_33;
	p_pMatrixOut->_44 = 1.0f;
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetDefaultConfiguration(void)
{
	m_configuration.SetLong("Adapter.ID", 0);
	m_configuration.SetBool("Adapter.PerformanceHud.Enabled", false);

	m_configuration.SetBool("Initialise.CreateWindow.Enabled", true);
	m_configuration.SetBool("Initialise.CreateWindow.FullScreen", false);
	m_configuration.SetLong("Initialise.CreateWindow.Left", 64);
	m_configuration.SetLong("Initialise.CreateWindow.Top", 64);
	m_configuration.SetLong("Initialise.CreateWindow.Width", 640);
	m_configuration.SetLong("Initialise.CreateWindow.Height", 480);
	m_configuration.SetBool("Initialise.CreateWindow.ColourBuffer.Enabled", true);
	m_configuration.SetLong("Initialise.CreateWindow.ColourBuffer.Size", 32);
	m_configuration.SetBool("Initialise.CreateWindow.DepthBuffer.Enabled", true);
	m_configuration.SetLong("Initialise.CreateWindow.DepthBuffer.Size", 24);
	m_configuration.SetBool("Initialise.CreateWindow.StencilBuffer.Enabled", false);
	m_configuration.SetLong("Initialise.CreateWindow.StencilBuffer.Size", 8);

	m_configuration.Set("Initialise.CreateWindow.Title", "Vistas Direct3D9 Renderer Application");
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetDeviceCapabilities(void)
{
	D3DCAPS9 d3dCaps;
	
	m_pAdapterDevice->GetDirect3D()->GetDeviceCaps(m_pAdapterDevice->GetAdapterID(), D3DDEVTYPE_HAL, &d3dCaps);

	m_dwDeviceCapabilities[IRenderDevice::DCI_MAXIMUM_ANISOTROPY] = d3dCaps.MaxAnisotropy;
	m_dwDeviceCapabilities[IRenderDevice::DCI_MAXIMUM_LIGHTS] = d3dCaps.MaxActiveLights;
	m_dwDeviceCapabilities[IRenderDevice::DCI_MAXIMUM_SIMULATANEOUS_TEXTURES] = d3dCaps.MaxSimultaneousTextures;
	m_dwDeviceCapabilities[IRenderDevice::DCI_MAXIMUM_USER_CLIPPLANES] = d3dCaps.MaxUserClipPlanes;
	m_dwDeviceCapabilities[IRenderDevice::DCI_MAXIMUM_STREAMS] = d3dCaps.MaxStreams;

	m_fDeviceCapabilities[IRenderDevice::DCF_MAXIMUM_POINTSIZE] = d3dCaps.MaxPointSize;
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetD3DRenderState(D3DRENDERSTATETYPE p_d3drsType, DWORD p_dwValue)
{
	DWORD dwValue;

	LPDIRECT3DDEVICE9 pD3DDevice9 = m_pAdapterDevice->GetDirect3DDevice();
	m_hResult = pD3DDevice9->GetRenderState(p_d3drsType, &dwValue);
	
	if (dwValue != p_dwValue)
		m_hResult = pD3DDevice9->SetRenderState(p_d3drsType, p_dwValue);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetD3DSamplerState(DWORD p_dwSampler, D3DSAMPLERSTATETYPE p_d3dssType, DWORD p_dwValue)
{
	DWORD dwValue;

	LPDIRECT3DDEVICE9 pD3DDevice9 = m_pAdapterDevice->GetDirect3DDevice();
	m_hResult = pD3DDevice9->GetSamplerState(p_dwSampler, p_d3dssType, &dwValue);

	if (dwValue != p_dwValue)
		m_hResult = pD3DDevice9->SetSamplerState(p_dwSampler, p_d3dssType, p_dwValue);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetD3DSampler(uint p_uiSamplerIndex, const SamplerState* p_pSamplerState)
{
	SetD3DSamplerState(p_uiSamplerIndex, D3DSAMP_ADDRESSU, m_d3dWrapType[p_pSamplerState->GetUWrapType()]);
	SetD3DSamplerState(p_uiSamplerIndex, D3DSAMP_ADDRESSV, m_d3dWrapType[p_pSamplerState->GetVWrapType()]);
	SetD3DSamplerState(p_uiSamplerIndex, D3DSAMP_ADDRESSW, m_d3dWrapType[p_pSamplerState->GetWWrapType()]);
	SetD3DSamplerState(p_uiSamplerIndex, D3DSAMP_BORDERCOLOR, p_pSamplerState->GetBorderColour().ToARGB());
	SetD3DSamplerState(p_uiSamplerIndex, D3DSAMP_MINFILTER, m_d3dFilterType[p_pSamplerState->GetMinificationFilter()]);
	SetD3DSamplerState(p_uiSamplerIndex, D3DSAMP_MAGFILTER, m_d3dFilterType[p_pSamplerState->GetMagnificationFilter()]);
	SetD3DSamplerState(p_uiSamplerIndex, D3DSAMP_MIPFILTER, m_d3dFilterType[p_pSamplerState->GetMipmapFilter()]);
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::SetD3DLight(uint p_uiLightIndex, const Light* p_pLight)
{
	D3DLIGHT9 d3dLight;
	LPDIRECT3DDEVICE9 pD3DDevice = m_pAdapterDevice->GetDirect3DDevice();

	if (p_pLight != NULL && p_pLight->IsLightEnabled())
	{
		ZeroMemory(&d3dLight, sizeof(D3DLIGHT9));

		switch( p_pLight->GetType() )
		{
			case Light::LT_DIRECTIONAL:
				d3dLight.Type = D3DLIGHT_DIRECTIONAL;
				d3dLight.Direction = GetD3DVector((Vector3f)p_pLight->GetDirection());
				break;
			
			case Light::LT_POINT:
				d3dLight.Type = D3DLIGHT_POINT;
				d3dLight.Position = GetD3DVector((Vector3f)p_pLight->GetPosition());
				d3dLight.Attenuation0 = p_pLight->GetConstantAttenuation();
				d3dLight.Attenuation1 = p_pLight->GetLinearAttenuation();
				d3dLight.Attenuation2 = p_pLight->GetQuadraticAttenuation();
				d3dLight.Range = p_pLight->GetRange();
				break;
			
			case Light::LT_SPOT:
				d3dLight.Type = D3DLIGHT_SPOT;
				d3dLight.Direction = GetD3DVector((Vector3f)p_pLight->GetDirection());
				d3dLight.Position = GetD3DVector((Vector3f)p_pLight->GetPosition());
				d3dLight.Attenuation0 = p_pLight->GetConstantAttenuation();
				d3dLight.Attenuation1 = p_pLight->GetLinearAttenuation();
				d3dLight.Attenuation2 = p_pLight->GetQuadraticAttenuation();
				d3dLight.Range = p_pLight->GetRange();
				break;
		}

		// Common to all lights
		d3dLight.Ambient = GetD3DColourValue((Colour)p_pLight->GetAmbient());
		d3dLight.Diffuse = GetD3DColourValue((Colour)p_pLight->GetDiffuse());
		d3dLight.Specular = GetD3DColourValue((Colour)p_pLight->GetSpecular());

		pD3DDevice->SetLight(p_uiLightIndex, &d3dLight);
		pD3DDevice->LightEnable(p_uiLightIndex, true);
	}
	else
		pD3DDevice->LightEnable(p_uiLightIndex, false);
}
//----------------------------------------------------------------------------------------------
D3DMATRIX DX9RenderDevice::GetD3DMatrix(const Matrix4f& p_matrix)
{
	D3DMATRIX d3dMatrix = { p_matrix._11, p_matrix._21, p_matrix._31, p_matrix._41,
							p_matrix._12, p_matrix._22, p_matrix._32, p_matrix._42,
							p_matrix._13, p_matrix._23, p_matrix._33, p_matrix._43,
							p_matrix._14, p_matrix._24, p_matrix._34, p_matrix._44 };

	return d3dMatrix;
}
//----------------------------------------------------------------------------------------------
D3DMATRIX DX9RenderDevice::GetD3DMatrix(float* p_pfMatrix)
{
	D3DMATRIX d3dMatrix = { p_pfMatrix[0], p_pfMatrix[4], p_pfMatrix[8], p_pfMatrix[12],
							p_pfMatrix[1], p_pfMatrix[5], p_pfMatrix[9], p_pfMatrix[13],
							p_pfMatrix[2], p_pfMatrix[6], p_pfMatrix[10], p_pfMatrix[14],
							p_pfMatrix[3], p_pfMatrix[7], p_pfMatrix[11], p_pfMatrix[15] };

	return d3dMatrix;
}
//----------------------------------------------------------------------------------------------
D3DVECTOR DX9RenderDevice::GetD3DVector(const Meson::Common::Maths::Vector3f& p_vector)
{
	D3DVECTOR d3dVector = { p_vector.X, p_vector.Y, p_vector.Z };
	
	return d3dVector;
}
//----------------------------------------------------------------------------------------------
D3DVECTOR DX9RenderDevice::GetD3DVector(float* p_pfVector)
{
	D3DVECTOR d3dVector = { p_pfVector[0], p_pfVector[1], p_pfVector[2] };

	return d3dVector;
}
//----------------------------------------------------------------------------------------------
D3DCOLOR DX9RenderDevice::GetD3DColour(const Colour& p_colour)
{
	return (D3DCOLOR)p_colour.ToARGB();
}
//----------------------------------------------------------------------------------------------
D3DCOLORVALUE DX9RenderDevice::GetD3DColourValue(const Colour& p_colour)
{
	D3DCOLORVALUE d3dColourValue;

	d3dColourValue.a = p_colour.A;
	d3dColourValue.b = p_colour.B;
	d3dColourValue.g = p_colour.G;
	d3dColourValue.r = p_colour.R;

	return d3dColourValue;
}
//----------------------------------------------------------------------------------------------
bool DX9RenderDevice::IsDeviceLost(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice9 = m_pAdapterDevice->GetDirect3DDevice();

	m_hResult = pD3DDevice9->TestCooperativeLevel();

	switch( m_hResult )
	{
		case D3DERR_DEVICELOST:
		{
			Sleep(20);
			return true;
		}

		case D3DERR_DEVICENOTRESET:
		{		
			OnDeviceLost();
			ResetDevice();
			OnDeviceReset();
			return false;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::OnDeviceLost(void)
{
	// Device Lost
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::OnDeviceReset(void)
{
	// Device reset
}
//----------------------------------------------------------------------------------------------
void DX9RenderDevice::ResetDevice(void)
{
	if (m_pPrimaryWindow != NULL)
		m_pPrimaryWindow->ResetDevice();
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
