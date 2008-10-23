//----------------------------------------------------------------------------------------------
#include "VstTexture.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
Texture::Texture(const Meson::Common::Text::String& p_strName, TextureBufferPtr p_pTextureBuffer)
	: m_strName(p_strName),
	  m_pTextureBuffer(p_pTextureBuffer)
{
}
//----------------------------------------------------------------------------------------------
Texture::~Texture(void)
{
	m_pTextureBuffer = NULL;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& Texture::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
TextureBufferPtr Texture::GetTextureBuffer(void)
{
	return m_pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
TextureBuffer::TextureType Texture::GetType(void) const
{
	return m_pTextureBuffer->GetTextureType();
}
//----------------------------------------------------------------------------------------------
TextureBuffer::TextureFormat Texture::GetFormat(void) const
{
	return m_pTextureBuffer->GetTextureFormat();
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
