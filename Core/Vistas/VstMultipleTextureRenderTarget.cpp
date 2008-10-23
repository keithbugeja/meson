//----------------------------------------------------------------------------------------------
//	VstMultipleTextureRenderTarget.cpp
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstMultipleTextureRenderTarget.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
void MultipleTextureRenderTarget::Create(const Meson::Common::Text::String& p_strName)
{
	m_strName = p_strName;
}
//----------------------------------------------------------------------------------------------
void MultipleTextureRenderTarget::Destroy(void)
{
	m_textureRenderTargetList.Clear();
}
//----------------------------------------------------------------------------------------------
void MultipleTextureRenderTarget::AttachTextureRenderTarget(TextureRenderTarget* p_pTextureRenderTarget)
{
	m_textureRenderTargetList.Add(p_pTextureRenderTarget);
}
//----------------------------------------------------------------------------------------------
void MultipleTextureRenderTarget::DetachTextureRenderTarget(TextureRenderTarget* p_pTextureRenderTarget)
{
	m_textureRenderTargetList.Remove(p_pTextureRenderTarget);
}
//----------------------------------------------------------------------------------------------
void MultipleTextureRenderTarget::DetachAllTextureRenderTargets(void)
{
	m_textureRenderTargetList.Clear();
}
//----------------------------------------------------------------------------------------------
uint MultipleTextureRenderTarget::Size(void) const
{
	return (uint)m_textureRenderTargetList.Size();
}
//----------------------------------------------------------------------------------------------
TextureRenderTarget* MultipleTextureRenderTarget::GetElement(uint p_uiIndex) const
{
	MESON_ASSERT(p_uiIndex < m_textureRenderTargetList.Size(), "Index out of range!");
	return m_textureRenderTargetList[p_uiIndex];
}
//----------------------------------------------------------------------------------------------
void MultipleTextureRenderTarget::SetElement(uint p_uiIndex, TextureRenderTarget* p_pTextureRenderTarget)
{
	MESON_ASSERT(p_uiIndex < m_textureRenderTargetList.Size(), "Index out of range!");
	m_textureRenderTargetList[p_uiIndex] = p_pTextureRenderTarget;
}
//----------------------------------------------------------------------------------------------
TextureRenderTarget*& MultipleTextureRenderTarget::operator[](uint p_uiIndex)
{
	return m_textureRenderTargetList[p_uiIndex];
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
