//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTextureBuffer.h"

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
TextureBuffer::TextureBuffer(TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureType p_eTextureType, 
							 uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipMapLevels, dword p_dwPool, dword p_dwUsage)
	: HardwareResource(p_dwPool, p_dwUsage)
	, m_uiWidth(p_uiWidth)
	, m_uiHeight(p_uiHeight)
	, m_uiDepth(p_uiDepth)
	, m_uiMipMapLevels(p_uiMipMapLevels)
	, m_eTextureFormat(p_eTextureFormat)
	, m_eTextureType(p_eTextureType)
{
}
//----------------------------------------------------------------------------------------------
uint TextureBuffer::GetWidth(void) const
{
	return m_uiWidth;
}
//----------------------------------------------------------------------------------------------
uint TextureBuffer::GetHeight(void) const
{
	return m_uiHeight;
}
//----------------------------------------------------------------------------------------------
uint TextureBuffer::GetDepth(void) const
{
	return m_uiDepth;
}
//----------------------------------------------------------------------------------------------
uint TextureBuffer::GetMipMapLevels(void) const
{
	return m_uiMipMapLevels;
}
//----------------------------------------------------------------------------------------------
TextureBuffer::TextureFormat TextureBuffer::GetTextureFormat(void) const
{
	return m_eTextureFormat;
}
//----------------------------------------------------------------------------------------------
TextureBuffer::TextureType TextureBuffer::GetTextureType(void) const
{
	return m_eTextureType;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END