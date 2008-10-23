//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreSimulatorsDLL
//	Dynamic Link Library interface for Gravitas core simulators plugin.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		28/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"

#include "GrvNamespaces.h"
#include "CoreSimulatorsPlugIn.h"

Meson_Gravitas_BEGIN

using namespace Meson::Gravitas;
CoreSimulatorsPlugIn* g_pCoreSimulatorsPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pCoreSimulatorsPlugIn == NULL)
			g_pCoreSimulatorsPlugIn = new CoreSimulatorsPlugIn();

		return g_pCoreSimulatorsPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pCoreSimulatorsPlugIn;
		g_pCoreSimulatorsPlugIn = NULL;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pCoreSimulatorsPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pCoreSimulatorsPlugIn;
	}
}

Meson_Gravitas_END

