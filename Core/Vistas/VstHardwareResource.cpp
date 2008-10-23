//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstHardwareResource.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
HardwareResource::HardwareResource(dword p_dwPool, dword p_dwUsage)
	: Resource()
	, m_bIsLocked(false)
	, m_dwMemoryPool(p_dwPool)
	, m_dwMemoryUsage(p_dwUsage)
{
}
//----------------------------------------------------------------------------------------------
HardwareResource::HardwareResource(const Meson::Common::Text::String& p_strId, dword p_dwPool, dword p_dwUsage)
	: Resource(p_strId)
	, m_bIsLocked(false)
	, m_dwMemoryPool(p_dwPool)
	, m_dwMemoryUsage(p_dwUsage)
{
}
//----------------------------------------------------------------------------------------------
HardwareResource::~HardwareResource(void)
{
}
//----------------------------------------------------------------------------------------------
bool HardwareResource::IsLocked(void) 
{ 
	return m_bIsLocked; 
}
//----------------------------------------------------------------------------------------------
dword HardwareResource::GetMemoryPool(void) 
{ 
	return m_dwMemoryPool; 
}
//----------------------------------------------------------------------------------------------
dword HardwareResource::GetMemoryUsage(void)
{ 
	return m_dwMemoryUsage; 
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END