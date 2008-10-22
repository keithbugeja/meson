//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::PlugInManager
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#ifdef PLATFORM_WIN32

// Core Includes
#include "Types.h"
#include "Namespaces.h"
#include "Definitions.h"
#include "MesonException.h"
#include "DebugSupport.h"

#include "String8.h"
#include "Version.h"
#include "Platform.h"
#include "PlugInLoader.h"

#include <windows.h>

// namespace Meson.Common.PlugIn
Meson_Common_PlugIn_BEGIN

class WinPlugInLoader : public PlugInLoader
{
public:
	PlugIn* Load(String p_strPlugIn)
	{
		HMODULE hModule = LoadLibrary( p_strPlugIn );
		
		return NULL;
	}

	void Unload(String p_strPlugIn)
	{
	}
};

Meson_Common_PlugIn_END

#endif