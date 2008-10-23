//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::PlugInManager
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Singleton.h"
#include "Types.h"

#include "MesonException.h"
#include "DebugSupport.h"

#include "String8.h"
#include "Version.h"
#include "ArrayList.h"

#include "LibraryManager.h"
#include "SharedObjectMap.h"

Meson_Common_PlugIn_BEGIN

class PlugInManager : public TSingleton<PlugInManager>
{
	friend TSingleton<PlugInManager>;

private:
	Meson::Common::Collections::TArrayList<IPlugIn*> m_plugInArray;
	SharedObjectMap m_sharedObjectMap;

protected:
	~PlugInManager() { LibraryManager::Delete(); }

public:
	// Dynamic library loading/unloading mechanism
	IPlugIn *Load(const String& p_strLibrary);
	void Unload(const String& p_strLibrary);

	// Static library loading/unloading mechanism
	void Register(IPlugIn *p_pPlugIn);
	void Unregister(IPlugIn *p_pPlugIn);

	// Initalisation
	void Initialise(IPlugIn *p_pPlugIn);
	void Shutdown(IPlugIn *p_pPlugIn);

	// Object sharing across link libraries
	void RegisterSharedObject(void* p_pSharedObject, String p_strSharedObjectName);
	void UnregisterSharedObject(String p_strSharedObjectName);
};

Meson_Common_PlugIn_END


