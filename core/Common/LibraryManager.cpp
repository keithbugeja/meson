//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::LibraryManager
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "LibraryManager.h"

Meson_Common_PlugIn_BEGIN

//----------------------------------------------------------------------------------------------
/// Factory for concrete implementations of ILibrary.
/// \returns Pointer to the newly created instance.
//----------------------------------------------------------------------------------------------
ILibrary* LibraryManager::CreateLibrary()
{
	#ifdef PLATFORM_WIN32
		return new WinLibrary();
	#else			
		throw new MesonException("No Library System defined for platform!", __FILE__, __LINE__);
	#endif
}

//----------------------------------------------------------------------------------------------
///	Loads the specified dynamic library.
/// \param p_strName Dynamic library file
/// \return <b>true</b> if module has been successfully loaded, <b>false</b> otherwise
//----------------------------------------------------------------------------------------------
bool LibraryManager::Load(const String& p_strName)
{
	ILibrary* pLibrary = CreateLibrary();
	
	if (!pLibrary->Load(p_strName))
		return false;

	m_libraryMap.Insert(p_strName, pLibrary);
	return true;
}

//----------------------------------------------------------------------------------------------
/// Unloads the specified dynamic library
/// \param p_strName Dynamic library file
//----------------------------------------------------------------------------------------------
void LibraryManager::Unload(const String& p_strName)
{
	if (m_libraryMap.ContainsKey(p_strName))
	{
		ILibrary* pLibrary = m_libraryMap[p_strName];
		m_libraryMap.RemoveKey(p_strName);
		delete pLibrary;
	}
}

//----------------------------------------------------------------------------------------------
/// Returns the <b>singleton</b> instance to the PlugIn loaded by the specified 
///	dynamic library. 
///	\param p_strName Dynamic library file
/// \return Pointer to PlugIn instance
//----------------------------------------------------------------------------------------------
IPlugIn* LibraryManager::GetPlugInSingleton(const String& p_strName)
{
	if (!m_libraryMap.ContainsKey(p_strName))
		return NULL;

	ILibrary* pLibrary = m_libraryMap[p_strName];

	SYMBOL_SINGLE_GET GetPlugInSingleton = (SYMBOL_SINGLE_GET)pLibrary->GetSymbol("GetPlugInSingleton");
	MESON_ASSERT(GetPlugInSingleton != NULL, "GetPlugInSingleton() is NULL!");
	
	IPlugIn* plugIn = GetPlugInSingleton();
	return plugIn;
}

//----------------------------------------------------------------------------------------------
///	Disposes of the PlugIn singleton instance within the specified dynamic library.
/// \param p_strName Dynamic library file
//----------------------------------------------------------------------------------------------
void LibraryManager::DisposePlugInSingleton(const String& p_strName)
{
	if (m_libraryMap.ContainsKey(p_strName))
	{
		ILibrary* pLibrary = m_libraryMap[p_strName];

		SYMBOL_SINGLE_DISPOSE DisposePlugInSingleton = (SYMBOL_SINGLE_DISPOSE)pLibrary->GetSymbol("DisposePlugInSingleton");
		MESON_ASSERT(DisposePlugInSingleton != NULL, "DisposePlugInSingleton() is NULL!");
		
		DisposePlugInSingleton();
	}
}

//----------------------------------------------------------------------------------------------
/// Creates a new PlugIn instance.
/// \param p_strName Dynamic library file
/// \return Handle to newly created instance
//----------------------------------------------------------------------------------------------
HPlugInInstance LibraryManager::CreatePlugInInstance(const String& p_strName)
{
	if (!m_libraryMap.ContainsKey(p_strName))
		return NULL;

	ILibrary* pLibrary = m_libraryMap[p_strName];
	
	SYMBOL_MULTI_CREATE CreatePlugInInstance = (SYMBOL_MULTI_CREATE)pLibrary->GetSymbol("CreatePlugInInstance");
	MESON_ASSERT(CreatePlugInInstance != NULL, "CreatePlugInInstance() is NULL!");
	
	return CreatePlugInInstance();
}

//----------------------------------------------------------------------------------------------
/// Retrieves a PlugIn instance by the respective handle.
/// \param p_hInstance Handle to instance
/// \param p_strName Dynamic library file
/// \return Pointer to PlugIn object
//----------------------------------------------------------------------------------------------
IPlugIn* LibraryManager::GetPlugInInstance(const String& p_strName, HPlugInInstance p_hInstance)
{
	if (!m_libraryMap.ContainsKey(p_strName))
		return NULL;

	ILibrary* pLibrary = m_libraryMap[p_strName];

	SYMBOL_MULTI_GET GetPlugInInstance = (SYMBOL_MULTI_GET)pLibrary->GetSymbol("GetPlugInInstance");
	MESON_ASSERT(GetPlugInInstance != NULL, "GetPlugInInstance() is NULL!");
	
	return GetPlugInInstance( p_hInstance );
}

//----------------------------------------------------------------------------------------------
/// Disposes of a given PlugIn instance through the respective handle.
/// \param p_strName Dynamic library file
/// \param p_hInstance Handle to instance
//----------------------------------------------------------------------------------------------
void LibraryManager::DisposePlugInInstance(const String& p_strName, HPlugInInstance p_hInstance)
{
	if (m_libraryMap.ContainsKey(p_strName))
	{
		ILibrary* pLibrary = m_libraryMap[p_strName];
	
		SYMBOL_MULTI_DISPOSE DisposePlugInInstance = (SYMBOL_MULTI_DISPOSE)pLibrary->GetSymbol("DisposePlugInInstance");
		MESON_ASSERT(DisposePlugInInstance != NULL, "DisposePlugInInstance() is NULL!");
		
		DisposePlugInInstance( p_hInstance );
	}
}

//----------------------------------------------------------------------------------------------
/// Disposes of all PlugIn instances.
/// \param p_strName Dynamic library file
//----------------------------------------------------------------------------------------------
void LibraryManager::DisposePlugInInstances(const String& p_strName)
{
	if (m_libraryMap.ContainsKey(p_strName))
	{
		ILibrary* pLibrary = m_libraryMap[p_strName];
	
		SYMBOL_MULTI_DISPOSEALL DisposePlugInInstances = (SYMBOL_MULTI_DISPOSEALL)pLibrary->GetSymbol("DisposePlugInInstances");
		MESON_ASSERT(DisposePlugInInstances != NULL, "DisposePlugInInstances() is NULL!");
		
		DisposePlugInInstances();
	}
}

Meson_Common_PlugIn_END
