//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"

#include "DX9TextureRenderTarget.h"
#include "DX9TextureBuffer.h"

#include "MesonException.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
DX9TextureRenderTarget::DX9TextureRenderTarget(DX9AdapterDevice *p_pAdapterDevice)
	: m_pAdapterDevice(p_pAdapterDevice),
	  m_pRenderTargetSurface(NULL),
	  m_pDepthStencilSurface(NULL)
{
}
//----------------------------------------------------------------------------------------------
DX9TextureRenderTarget::~DX9TextureRenderTarget(void)
{
}
//----------------------------------------------------------------------------------------------
void DX9TextureRenderTarget::Create(const Meson::Common::Text::String& p_strName, TextureBuffer* p_pTextureBuffer, bool p_bCreateDepthStencilSurface)
{
	// Call base
	TextureRenderTarget::Create(p_strName, p_pTextureBuffer, p_bCreateDepthStencilSurface);
	
	// Create depth-stencil buffer if required
	if (!p_bCreateDepthStencilSurface) 
	{
		m_pDepthStencilSurface = NULL;
		m_bHasDepthStencilSurface = false;
	}
	else
		CreateDepthStencilSurface();
}
//----------------------------------------------------------------------------------------------
void DX9TextureRenderTarget::Destroy(void)
{
	SAFE_RELEASE(m_pRenderTargetSurface);
	SAFE_RELEASE(m_pDepthStencilSurface);
}
//----------------------------------------------------------------------------------------------
void DX9TextureRenderTarget::CreateDepthStencilSurface(void)
{
	if (m_pTextureBuffer == NULL)
		throw new MesonException("Cannot create depth-stencil : provided texture buffer is invalid", __FILE__, __LINE__);

	if (m_pAdapterDevice->GetDirect3DDevice()->CreateDepthStencilSurface(m_pTextureBuffer->GetWidth(), m_pTextureBuffer->GetHeight(), D3DFMT_D24S8, 
																		 D3DMULTISAMPLE_NONE, 0, 
																		 FALSE,
																		 &m_pDepthStencilSurface, 
																		 NULL) != D3D_OK)
		throw new MesonException("Unable to create depth-stencil surface", __FILE__, __LINE__);

	m_bHasDepthStencilSurface = true;
}
//----------------------------------------------------------------------------------------------
void DX9TextureRenderTarget::DestroyDepthStencilSurface(void)
{
	SAFE_RELEASE(m_pDepthStencilSurface);
	m_bHasDepthStencilSurface = false;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
/* TODO: Might consider wrapping up surfaces into a resource such as
   a PixelBuffer object and exposing it to the higher levels of the engine.
 */
//----------------------------------------------------------------------------------------------
LPDIRECT3DSURFACE9 DX9TextureRenderTarget::GetRenderTargetSurface(void)
{
	// Get surface if it is null
	if (m_pRenderTargetSurface == NULL)
	{
		if (m_pTextureBuffer->GetTextureType() == TextureBuffer::TT_2D)
		{
			LPDIRECT3DTEXTURE9 pDX9Texture = ((DX9TextureBuffer*)m_pTextureBuffer)->GetTexture2D();
			pDX9Texture->GetSurfaceLevel(0, &m_pRenderTargetSurface);
		}
	}

	return m_pRenderTargetSurface;
}
//----------------------------------------------------------------------------------------------
LPDIRECT3DSURFACE9 DX9TextureRenderTarget::GetDepthStencilSurface(void)
{
	return m_pDepthStencilSurface;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
