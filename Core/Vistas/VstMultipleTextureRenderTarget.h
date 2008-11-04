//----------------------------------------------------------------------------------------------
//	VstMultipleRenderTarget.h
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstTextureRenderTarget.h"

Meson_Vistas_BEGIN

class VISTAS_API MultipleTextureRenderTarget : public RenderTarget
{
public:
	const RenderTargetType GetType(void) const { return RTT_MULTI; }

	virtual void Create(const Meson::Common::Text::String& p_strName);
	virtual void Destroy(void);

	virtual void AttachTextureRenderTarget(TextureRenderTarget* p_pTextureRenderTarget);
	virtual void DetachTextureRenderTarget(TextureRenderTarget* p_pTextureRenderTarget);
	virtual void DetachAllTextureRenderTargets(void);

	virtual uint Size(void) const;

	virtual TextureRenderTarget* GetElement(uint p_uiIndex) const;
	virtual void SetElement(uint p_uiIndex, TextureRenderTarget* p_pTextureRenderTarget);

	virtual TextureRenderTarget*& operator[](uint p_uiIndex);

protected:
	Meson::Common::Collections::TArrayList<TextureRenderTarget*> m_textureRenderTargetList;
};

Meson_Vistas_END
