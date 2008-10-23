#include "VstTextureState.h"

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
TextureState::TextureState(TexturePtr& p_pTexture)
	: m_pTexture(p_pTexture),
	  m_pTextureBuffer(NULL),
	  m_strAlias(),
	  m_strAliasCatalogue(),
	  m_eTextureType(TT_COMPLEX)
{
}
//----------------------------------------------------------------------------------------------
TextureState::TextureState(TextureBufferPtr p_pTextureBuffer)
	: m_pTexture(NULL),
	  m_pTextureBuffer(p_pTextureBuffer),
	  m_strAlias(),
	  m_strAliasCatalogue(),
	  m_eTextureType(TT_SIMPLE)
{
}
//----------------------------------------------------------------------------------------------
TextureState::TextureState(const Meson::Common::Text::String& p_strTextureCatalogue, const Meson::Common::Text::String& p_strTextureName)
	: m_pTexture(NULL),
	  m_pTextureBuffer(NULL),
	  m_strAlias(p_strTextureCatalogue),
	  m_strAliasCatalogue(p_strTextureName),
	  m_eTextureType(TT_COMPLEX_ALIAS)
{
}
//----------------------------------------------------------------------------------------------
TextureState::TextureState(const TextureState& p_textureState)
	: m_pTexture(p_textureState.m_pTexture),
	  m_pTextureBuffer(p_textureState.m_pTextureBuffer),
	  m_strAlias(p_textureState.m_strAliasCatalogue),
	  m_strAliasCatalogue(p_textureState.m_strAliasCatalogue),
	  m_eTextureType(p_textureState.m_eTextureType)
{
}
//----------------------------------------------------------------------------------------------
TextureState::~TextureState(void)
{
	m_pTexture = NULL;
	m_pTextureBuffer = NULL;
}
//----------------------------------------------------------------------------------------------
TextureState::TextureType TextureState::GetType(void) const
{
	return m_eTextureType;
}
//----------------------------------------------------------------------------------------------
void TextureState::SetTexture(TexturePtr p_pTexture)
{
	m_eTextureType = TT_COMPLEX;
	m_pTexture = p_pTexture;
}
//----------------------------------------------------------------------------------------------
TexturePtr TextureState::GetTexture(void)
{
	return m_pTexture;
}
//----------------------------------------------------------------------------------------------
void TextureState::SetTextureBuffer(TextureBufferPtr p_pTextureBuffer)
{
	m_eTextureType = TT_SIMPLE;
	m_pTextureBuffer = p_pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
TextureBufferPtr TextureState::GetTextureBuffer(void)
{
	if (m_eTextureType == TT_SIMPLE)
		return m_pTextureBuffer;

	MESON_ASSERT(m_pTexture != NULL, "Texture object reference is set to NULL!");
	return m_pTexture->GetTextureBuffer();
}
//----------------------------------------------------------------------------------------------
void TextureState::SetAlias(const Meson::Common::Text::String& p_strTextureCatalogue, const Meson::Common::Text::String& p_strTextureName)
{
	m_eTextureType = TT_COMPLEX_ALIAS;
	m_strAlias = p_strTextureName;
	m_strAliasCatalogue = p_strTextureCatalogue;
}
//----------------------------------------------------------------------------------------------
bool TextureState::GetAlias(Meson::Common::Text::String& p_strTextureCatalogue, Meson::Common::Text::String& p_strTextureName) const
{
	if (m_eTextureType == TT_COMPLEX_ALIAS)
	{
		p_strTextureCatalogue = m_strAliasCatalogue;
		p_strTextureName = m_strAlias;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------------------
const TextureState& TextureState::operator= (const TextureState& p_textureState)
{
	m_eTextureType = p_textureState.m_eTextureType;
	m_pTexture = p_textureState.m_pTexture;
	m_pTextureBuffer = p_textureState.m_pTextureBuffer;
	m_strAlias = p_textureState.m_strAlias;
	m_strAliasCatalogue = p_textureState.m_strAliasCatalogue;

	return *this;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
