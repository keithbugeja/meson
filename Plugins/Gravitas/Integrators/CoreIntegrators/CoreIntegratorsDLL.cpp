//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreIntegratorsDLL
//	Dynamic Link Library interface for Gravitas core integrators plugin.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		30/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"

#include "GrvNamespaces.h"
#include "CoreIntegratorsPlugIn.h"

Meson_Gravitas_BEGIN

using namespace Meson::Gravitas;
CoreIntegratorsPlugIn* g_pCoreIntegratorsPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pCoreIntegratorsPlugIn == NULL)
			g_pCoreIntegratorsPlugIn = new CoreIntegratorsPlugIn();

		return g_pCoreIntegratorsPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pCoreIntegratorsPlugIn;
		g_pCoreIntegratorsPlugIn = NULL;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pCoreIntegratorsPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pCoreIntegratorsPlugIn;
	}
}

Meson_Gravitas_END

