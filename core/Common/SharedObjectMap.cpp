//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::SharedObjectMap
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "SharedObjectMap.h"

using namespace Meson::Common::Text;

Meson_Common_PlugIn_BEGIN

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void SharedObjectMap::RegisterObject(void* p_pObject, String p_strObjectName)
{
	m_sharedObjectMap[p_strObjectName] = p_pObject;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void* SharedObjectMap::GetRegisteredObject(String p_strObjectName)
{
	if (m_sharedObjectMap.ContainsKey(p_strObjectName))
		return m_sharedObjectMap[p_strObjectName];

	return NULL;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void SharedObjectMap::UnregisterObject(String p_strObjectName)
{
	m_sharedObjectMap.RemoveKey(p_strObjectName);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void SharedObjectMap::UnregisterObject(void* p_pObject)
{
	m_sharedObjectMap.RemoveValue(p_pObject);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void SharedObjectMap::UnregisterAllObjects(void)
{
	m_sharedObjectMap.Clear();
}

Meson_Common_PlugIn_END
