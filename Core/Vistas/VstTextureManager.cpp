//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"
#include "VstTextureManager.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
TextureManager::TextureManager(void)
{
}
//----------------------------------------------------------------------------------------------
TextureManager::~TextureManager(void)
{
	TextureEnumerator textureEnumerator = m_textureBufferMap.GetValueEnumerator();

	while(textureEnumerator.HasMoreElements())
		textureEnumerator.NextElement() = NULL;

	m_textureBufferMap.Clear();
}
//----------------------------------------------------------------------------------------------
TexturePtr TextureManager::CreateTexture1D(const Meson::Common::Text::String& p_strName, TextureBuffer::TextureFormat p_eTextureFormat, 
										   TextureBuffer::TextureUsage p_eTextureUsage, uint p_uiSize)
{
	if (ContainsTexture(p_strName))
		throw new MesonException("Another texture already exists with the given name", __FILE__, __LINE__);

	TextureBufferPtr pTextureBuffer = 
		VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateTextureBuffer2D(p_eTextureFormat, 
			p_uiSize, 0x01, 0x00, TextureBuffer::MP_DEFAULT, p_eTextureUsage);

	TexturePtr pTexture(new Texture(p_strName, pTextureBuffer));
	m_textureBufferMap.Insert(p_strName, pTexture);

	return pTexture;
}
//----------------------------------------------------------------------------------------------
TexturePtr TextureManager::CreateTexture2D(const Meson::Common::Text::String& p_strName, TextureBuffer::TextureFormat p_eTextureFormat, 
										   TextureBuffer::TextureUsage p_eTextureUsage, uint p_uiWidth, uint p_uiHeight)
{
	if (ContainsTexture(p_strName))
		throw new MesonException("Another texture already exists with the given name", __FILE__, __LINE__);

	TextureBufferPtr pTextureBuffer = 
		VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateTextureBuffer2D(p_eTextureFormat, 
			p_uiWidth, p_uiHeight, 0x00, TextureBuffer::MP_DEFAULT, p_eTextureUsage);

	TexturePtr pTexture(new Texture(p_strName, pTextureBuffer));
	m_textureBufferMap.Insert(p_strName, pTexture);

	return pTexture;
}
//----------------------------------------------------------------------------------------------
TexturePtr TextureManager::CreateTexture3D(const Meson::Common::Text::String& p_strName, TextureBuffer::TextureFormat p_eTextureFormat, 
										   TextureBuffer::TextureUsage p_eTextureUsage, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth)
{
	if (ContainsTexture(p_strName))
		throw new MesonException("Another texture already exists with the given name", __FILE__, __LINE__);

	TextureBufferPtr pTextureBuffer = 
		VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateTextureBuffer3D(p_eTextureFormat, 
			p_uiWidth, p_uiHeight, p_uiDepth, 0x00, TextureBuffer::MP_DEFAULT, p_eTextureUsage);

	TexturePtr pTexture(new Texture(p_strName, pTextureBuffer));
	m_textureBufferMap.Insert(p_strName, pTexture);

	return pTexture;
}
//----------------------------------------------------------------------------------------------
TexturePtr TextureManager::CreateTextureCube(const Meson::Common::Text::String& p_strName, TextureBuffer::TextureFormat p_eTextureFormat, 
											 TextureBuffer::TextureUsage p_eTextureUsage, uint p_uiEdgeLength)
{
	if (ContainsTexture(p_strName))
		throw new MesonException("Another texture already exists with the given name", __FILE__, __LINE__);

	TextureBufferPtr pTextureBuffer = 
		VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateTextureBufferCube(p_eTextureFormat, 
			p_uiEdgeLength, 0x00, TextureBuffer::MP_DEFAULT, p_eTextureUsage);

	TexturePtr pTexture(new Texture(p_strName, pTextureBuffer));
	m_textureBufferMap.Insert(p_strName, pTexture);

	return pTexture;
}
//----------------------------------------------------------------------------------------------
TexturePtr TextureManager::CreateTextureFromTextureBuffer(const Meson::Common::Text::String& p_strName, TextureBufferPtr p_pTextureBuffer)
{
	if (ContainsTexture(p_strName))
		throw new MesonException("Another texture already exists with the given name", __FILE__, __LINE__);

	TexturePtr pTexture(new Texture(p_strName, p_pTextureBuffer));
	m_textureBufferMap.Insert(p_strName, pTexture);

	return pTexture;
}
//----------------------------------------------------------------------------------------------
TexturePtr TextureManager::CreateTextureFromFile(const Meson::Common::Text::String& p_strName, const Meson::Common::Text::String& p_strFilename, TextureBuffer::TextureType p_eTextureType)
{
	if (ContainsTexture(p_strName))
		throw new MesonException("Another texture already exists with the given name", __FILE__, __LINE__);

	TextureBufferPtr pTextureBuffer = 
		VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateTextureBufferFromFile(p_strFilename, p_eTextureType);

	TexturePtr pTexture(new Texture(p_strName, pTextureBuffer));
	m_textureBufferMap.Insert(p_strName, pTexture);

	return pTexture;
}
//----------------------------------------------------------------------------------------------
bool TextureManager::ContainsTexture(const Meson::Common::Text::String& p_strName)
{
	return m_textureBufferMap.ContainsKey(p_strName);
}
//----------------------------------------------------------------------------------------------
bool TextureManager::ContainsTexture(const Meson::Common::Text::String& p_strName, TexturePtr& p_pTexture)
{
	return m_textureBufferMap.ContainsKey(p_strName, p_pTexture);
}
//----------------------------------------------------------------------------------------------
TexturePtr TextureManager::GetTexture(const Meson::Common::Text::String& p_strName)
{
	return m_textureBufferMap[p_strName];
}
//----------------------------------------------------------------------------------------------
void TextureManager::ReleaseTexture(TexturePtr p_pTexture)
{
	if (ContainsTexture(p_pTexture->GetName()))
		m_textureBufferMap.RemoveKey(p_pTexture->GetName());
}
//----------------------------------------------------------------------------------------------
void TextureManager::ReleaseTexture(const Meson::Common::Text::String& p_strName)
{
	if (ContainsTexture(p_strName))
		m_textureBufferMap.RemoveKey(p_strName);
}
//----------------------------------------------------------------------------------------------
void TextureManager::ReleaseAllTextures(void)
{
	m_textureBufferMap.Clear();
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END