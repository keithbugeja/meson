//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreForcesLibrary
//	Dynamic Link Library interface for Gravitas core forces plugin.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		30/07/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"

#include "GrvNamespaces.h"
#include "CoreForcesPlugIn.h"

Meson_Gravitas_BEGIN

CoreForcesPlugIn* g_pCoreForcesPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pCoreForcesPlugIn == NULL)
			g_pCoreForcesPlugIn = new CoreForcesPlugIn();

		return g_pCoreForcesPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pCoreForcesPlugIn;
		g_pCoreForcesPlugIn = NULL;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pCoreForcesPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pCoreForcesPlugIn;
	}
}

Meson_Gravitas_END

