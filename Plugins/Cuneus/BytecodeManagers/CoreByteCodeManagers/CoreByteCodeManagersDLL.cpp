// BasicByteCodeManager.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"
#include "CnsNamespaces.h"
#include "CoreByteCodeManagersPlugIn.h"

Meson_Cuneus_BEGIN

CoreByteCodeManagersPlugIn *g_pByteCodeMgrPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pByteCodeMgrPlugIn == NULL)
			g_pByteCodeMgrPlugIn = new CoreByteCodeManagersPlugIn();

		return g_pByteCodeMgrPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pByteCodeMgrPlugIn;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn *GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pByteCodeMgrPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pByteCodeMgrPlugIn;
	}
}

Meson_Cuneus_END

