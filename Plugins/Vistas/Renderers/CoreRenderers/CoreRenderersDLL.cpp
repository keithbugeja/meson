//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"
#include "VstNamespaces.h"
#include "CoreRenderersPlugIn.h"

Meson_Vistas_BEGIN

CoreRenderersPlugIn* g_pCoreRenderersPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pCoreRenderersPlugIn == NULL)
			g_pCoreRenderersPlugIn = new CoreRenderersPlugIn();

		return g_pCoreRenderersPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pCoreRenderersPlugIn;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pCoreRenderersPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pCoreRenderersPlugIn;
	}
}

Meson_Vistas_END

