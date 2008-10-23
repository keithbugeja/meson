//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreSpacesDLL
//	Dynamic Link Library interface for Gravitas core spaces plugin.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		20/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"

#include "GrvNamespaces.h"
#include "CoreSpacesPlugIn.h"

Meson_Gravitas_BEGIN

using namespace Meson::Gravitas;
CoreSpacesPlugIn* g_pCoreSpacesPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pCoreSpacesPlugIn == NULL)
			g_pCoreSpacesPlugIn = new CoreSpacesPlugIn();

		return g_pCoreSpacesPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pCoreSpacesPlugIn;
		g_pCoreSpacesPlugIn = NULL;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pCoreSpacesPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pCoreSpacesPlugIn;
	}
}

Meson_Gravitas_END

