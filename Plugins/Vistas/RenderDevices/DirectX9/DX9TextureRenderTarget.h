//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <d3d9.h>

#include "VstTextureRenderTarget.h"
#include "DX9AdapterDevice.h"

Meson_Vistas_BEGIN

class DX9TextureRenderTarget : public TextureRenderTarget
{
public:
	DX9TextureRenderTarget(DX9AdapterDevice *p_pAdapterDevice);
	~DX9TextureRenderTarget(void);

	virtual void Create(const Meson::Common::Text::String& p_strName, TextureBuffer* p_pTextureBuffer, bool p_bCreateDepthStencilSurface);
	virtual void Destroy(void);

	virtual void CreateDepthStencilSurface(void);
	virtual void DestroyDepthStencilSurface(void);

	/* TODO: Might consider wrapping up surfaces into a resource such as
	   a PixelBuffer object and exposing it to the higher levels of the engine.
	 */
	LPDIRECT3DSURFACE9 GetRenderTargetSurface(void);
	LPDIRECT3DSURFACE9 GetDepthStencilSurface(void);

protected:
	LPDIRECT3DSURFACE9 m_pRenderTargetSurface,
					   m_pDepthStencilSurface;

	DX9AdapterDevice* m_pAdapterDevice;
};

Meson_Vistas_END
