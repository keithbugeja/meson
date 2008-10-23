// Scavenger.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"
#include "CnsNamespaces.h"
#include "ScavengerPlugIn.h"

Meson_Cuneus_BEGIN

ScavengerPlugIn *g_pGarbageCollectorPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pGarbageCollectorPlugIn == NULL)
			g_pGarbageCollectorPlugIn = new ScavengerPlugIn();

		return g_pGarbageCollectorPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pGarbageCollectorPlugIn;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn *GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pGarbageCollectorPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pGarbageCollectorPlugIn;
	}
}

Meson_Cuneus_END


