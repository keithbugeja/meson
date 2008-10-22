//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::WinLibrary
//	Concrete Library class implementation for Windows Dynamic Link Libraries (DLLs).
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
//	
//	Notes:
//
//	PlugIn Dynamic Link Library must provide a number of entry points through which the Manager
//	instantiates and registers PlugIns. 
//
//	#ifndef DLLExport
//	#define DLLExport __declspec( dllexport )
//	#endif
//
//	#include "LibrarySymbols.h"
//
//  extern "C" 
//  {
//		SamplePlugIn* pSamplePlugIn = NULL;
//
//		// Singleton
//		DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void);
//		DLLExport void DisposePlugInSingleton(void);
//
//		// Multi-instance
//		DLLExport HPlugInInstance CreatePlugInInstance(void);
//		DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(HPlugInInstance p_hInstance);
//		void DLLExport DisposePlugInInstance(HPlugInInstance p_hInstance);
//		void DLLExport DisposePlugInInstances();
//	}
//----------------------------------------------------------------------------------------------
#pragma once

#ifdef PLATFORM_WIN32

#include "Types.h"
#include "Namespaces.h"
#include "Definitions.h"
#include "MesonException.h"
#include "DebugSupport.h"

#include "String8.h"
#include "Version.h"
#include "Platform.h"
#include "Library.h"

#include <windows.h>

Meson_Common_PlugIn_BEGIN

class WinLibrary : public ILibrary
{
protected:
	HMODULE m_hModule;

public:
	bool Load(Meson::Common::Text::String p_strName)
	{
		return ((m_hModule = LoadLibraryA(p_strName)) != NULL);
	}

	void* GetSymbol(Meson::Common::Text::String p_strSymbol)
	{
		return (void*)GetProcAddress(m_hModule, p_strSymbol);
	}

	void Unload(void)
	{
		FreeLibrary(m_hModule);
	}
};

Meson_Common_PlugIn_END

#endif