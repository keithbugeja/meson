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
#include "CoreConstraintsPlugIn.h"

Meson_Gravitas_BEGIN

using namespace Meson::Gravitas;
CoreConstraintsPlugIn* g_pCoreConstraintsPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pCoreConstraintsPlugIn == NULL)
			g_pCoreConstraintsPlugIn = new CoreConstraintsPlugIn();

		return g_pCoreConstraintsPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pCoreConstraintsPlugIn;
		g_pCoreConstraintsPlugIn = NULL;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pCoreConstraintsPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pCoreConstraintsPlugIn;
	}
}

Meson_Gravitas_END

