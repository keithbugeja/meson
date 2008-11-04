//----------------------------------------------------------------------------------------------
//	VstTextureRenderTarget.h
//	Interface class for texture render targets.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Configuration.h"

#include "VstTypes.h"
#include "VstRenderTarget.h"
#include "VstTextureBuffer.h"

Meson_Vistas_BEGIN

class VISTAS_API TextureRenderTarget : public RenderTarget
{
public:
	const RenderTargetType GetType(void) const { return RTT_TEXTURE; }

	virtual void Create(const Meson::Common::Text::String& p_strName, TextureBuffer* p_pTextureBuffer, bool p_bCreateDepthStencilSurface);
	virtual void Destroy(void);

	virtual void CreateDepthStencilSurface(void) = 0;
	virtual void DestroyDepthStencilSurface(void) = 0;

	virtual TextureBuffer* GetTextureBuffer(void);
	virtual void SetTextureBuffer(TextureBuffer* p_pTextureBufferPtr);

	virtual bool HasDepthStencilSurface(void) const;

protected:
	bool m_bHasDepthStencilSurface;
	TextureBuffer* m_pTextureBuffer;
};

Meson_Vistas_END
