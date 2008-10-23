//----------------------------------------------------------------------------------------------
//	VstTextureRenderTarget.cpp
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstTextureRenderTarget.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
void TextureRenderTarget::Create(const Meson::Common::Text::String& p_strName, TextureBuffer* p_pTextureBuffer, bool p_bCreateDepthStencilSurface)
{
	m_strName = p_strName;
	m_pTextureBuffer = p_pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
void TextureRenderTarget::Destroy(void)
{
	m_pTextureBuffer = NULL;
}
//----------------------------------------------------------------------------------------------
TextureBuffer* TextureRenderTarget::GetTextureBuffer(void)
{
	return m_pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
void TextureRenderTarget::SetTextureBuffer(TextureBuffer* p_pTextureBufferPtr)
{
	m_pTextureBuffer = p_pTextureBufferPtr;
}
//----------------------------------------------------------------------------------------------
bool TextureRenderTarget::HasDepthStencilSurface(void) const
{
	return m_bHasDepthStencilSurface;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
