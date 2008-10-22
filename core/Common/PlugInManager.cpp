//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::PlugInManager
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "PlugInManager.h"

Meson_Common_PlugIn_BEGIN

//----------------------------------------------------------------------------------------------
/// Loads a plug-in contained in a dynamic system library.
///	\param p_strLibrary Name of dynamic library file
/// \return Pointer to plug-in instance
//----------------------------------------------------------------------------------------------
IPlugIn* PlugInManager::Load(const String& p_strLibrary)
{
	IPlugIn* pPlugIn = NULL;

	if (NULL == LibraryManager::GetInstance()->Load(p_strLibrary) ||
		NULL == (pPlugIn = LibraryManager::GetInstance()->GetPlugInSingleton(p_strLibrary)))
		return NULL;

	Register(pPlugIn);
	return pPlugIn;
}

//----------------------------------------------------------------------------------------------
/// Unloads a loaded plug-in.
///	\param p_strLibrary Name of dynamic library file
//----------------------------------------------------------------------------------------------
void PlugInManager::Unload(const String& p_strLibrary)
{
	IPlugIn* pPlugIn = LibraryManager::GetInstance()->GetPlugInSingleton(p_strLibrary);
	
	if (pPlugIn != NULL)
	{
		Unregister(pPlugIn);
		LibraryManager::GetInstance()->DisposePlugInSingleton(p_strLibrary);
		LibraryManager::GetInstance()->Unload(p_strLibrary);
	}
}

//----------------------------------------------------------------------------------------------
/// Registers a static plug-in with the plug-in manager.
///	\param p_pPlugIn Pointer to plug-in instance
//----------------------------------------------------------------------------------------------
void PlugInManager::Register(IPlugIn* p_pPlugIn)
{
	if (p_pPlugIn != NULL)
	{
		m_plugInArray.Add(p_pPlugIn);
		p_pPlugIn->Register(&m_sharedObjectMap);
	}
}

//----------------------------------------------------------------------------------------------
///	Unregisters a plug-in from the plug-in manager.
/// \param p_strName Name of plug-in to unregister
//----------------------------------------------------------------------------------------------
void PlugInManager::Unregister(IPlugIn* p_pPlugIn)
{
	if (p_pPlugIn != NULL)
	{
		m_plugInArray.Remove(p_pPlugIn);
		p_pPlugIn->Unregister();
	}
}

//----------------------------------------------------------------------------------------------
///	Performs initialisation of specified plug-in.
///	\param p_strName Name of plug-in to initialise
//----------------------------------------------------------------------------------------------
void PlugInManager::Initialise(IPlugIn* p_pPlugIn)
{
	if (p_pPlugIn != NULL)
		p_pPlugIn->Initialise();
}

//----------------------------------------------------------------------------------------------
///	Shutdown the specified plug-in.
///	\param p_strName Name of plug-in to shutdown
//----------------------------------------------------------------------------------------------
void PlugInManager::Shutdown(IPlugIn* p_pPlugIn)
{
	if (p_pPlugIn != NULL)
		p_pPlugIn->Shutdown();
}

//----------------------------------------------------------------------------------------------
/// Register a shared object to expose within dynamic libraries
///	\param p_pSharedObject Pointer to shared object
/// \param p_strSharedObjectName Name accessor for shared object
//----------------------------------------------------------------------------------------------
void PlugInManager::RegisterSharedObject(void* p_pSharedObject, String p_strSharedObjectName)
{
	m_sharedObjectMap.RegisterObject(p_pSharedObject, p_strSharedObjectName);
}

//----------------------------------------------------------------------------------------------
/// Unregister shared object
/// \param p_strSharedObjectName Name of object to unregister
//----------------------------------------------------------------------------------------------
void PlugInManager::UnregisterSharedObject(String p_strSharedObjectName)
{
	m_sharedObjectMap.UnregisterObject(p_strSharedObjectName);
}

Meson_Common_PlugIn_END

