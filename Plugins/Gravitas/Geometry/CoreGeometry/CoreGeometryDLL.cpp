//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreGeometryDLL
//	Dynamic Link Library interface for Gravitas core geometry plugin.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"

#include "GrvNamespaces.h"
#include "CoreGeometryPlugIn.h"

Meson_Gravitas_BEGIN

CoreGeometryPlugIn* g_pCoreGeometryPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pCoreGeometryPlugIn == NULL)
			g_pCoreGeometryPlugIn = new CoreGeometryPlugIn();

		return g_pCoreGeometryPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pCoreGeometryPlugIn;
		g_pCoreGeometryPlugIn = NULL;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pCoreGeometryPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pCoreGeometryPlugIn;
	}
}

Meson_Gravitas_END

