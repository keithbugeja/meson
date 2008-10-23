// WeightedRoundRobin.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"
#include "CnsNamespaces.h"
#include "SchedulerPlugIn.h"

Meson_Cuneus_BEGIN

SchedulerPlugIn *g_pSchedulerPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pSchedulerPlugIn == NULL)
			g_pSchedulerPlugIn = new SchedulerPlugIn();

		return g_pSchedulerPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pSchedulerPlugIn;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn *GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pSchedulerPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pSchedulerPlugIn;
	}
}

Meson_Cuneus_END

