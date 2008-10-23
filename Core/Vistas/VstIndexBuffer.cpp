//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstIndexBuffer.h"

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
IndexBuffer::IndexBuffer(IndexType p_eIndexType, uint p_uiIndexCount, dword p_dwPool, dword p_dwUsage)
	: HardwareResource(p_dwPool, p_dwUsage),
	  m_eIndexType(p_eIndexType), 
	  m_uiIndexCount(p_uiIndexCount)
{
}
//----------------------------------------------------------------------------------------------
IndexBuffer::IndexType IndexBuffer::GetIndexType(void) const
{
	return m_eIndexType;
}
//----------------------------------------------------------------------------------------------
uint IndexBuffer::GetIndexCount(void) const 
{ 
	return m_uiIndexCount; 
}

Meson_Vistas_END