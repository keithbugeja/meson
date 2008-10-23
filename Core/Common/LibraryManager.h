//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::LibraryManager
//	Header file for the Library Manager
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Definitions.h"
#include "Singleton.h"
#include "Types.h"

#include "MesonException.h"
#include "DebugSupport.h"

#include "String8.h"
#include "Version.h"
#include "Platform.h"

#include "PlugIn.h"
#include "Library.h"
#include "LibrarySymbols.h"
#include "HashMap.h"

using namespace Meson::Common::Text;

//----------------------------------------------------------------------------------------------
// Library implementation (per platform)
//----------------------------------------------------------------------------------------------
#ifdef PLATFORM_WIN32
	#include "WinLibrary.h"
#elif defined PLATFORM_OSX
	#include "OSXLibrary.h"
#elif defined PLATFORM_LINUX
	#include "LinuxLibrary.h"
#endif

Meson_Common_PlugIn_BEGIN

//----------------------------------------------------------------------------------------------
//	Library Manager
//----------------------------------------------------------------------------------------------
class LibraryManager : public TSingleton<LibraryManager>
{
private:
	Meson::Common::Collections::THashMap<String, ILibrary*> m_libraryMap;

	ILibrary *CreateLibrary();

public:
	bool Load(const String& p_strName);
	void Unload(const String& p_strName);

	// Used for singleton plugins
	IPlugIn* GetPlugInSingleton(const String& p_strName);
	void DisposePlugInSingleton(const String& p_strName);

	// Used for multi-instance plugins
	HPlugInInstance CreatePlugInInstance(const String& p_strName);
	IPlugIn* GetPlugInInstance(const String& p_strName, HPlugInInstance p_hInstance);
	void DisposePlugInInstance(const String& p_strName, HPlugInInstance p_hInstance);
	void DisposePlugInInstances(const String& p_strName);
};

Meson_Common_PlugIn_END


