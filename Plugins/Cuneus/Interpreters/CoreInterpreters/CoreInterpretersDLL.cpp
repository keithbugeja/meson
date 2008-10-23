// CuneusInterpreterDLL.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>

#ifndef DLLExport
#define DLLExport __declspec( dllexport )
#endif

#include "LibrarySymbols.h"
#include "CnsNamespaces.h"
#include "InterpreterPlugIn.h"

Meson_Cuneus_BEGIN

InterpreterPlugIn *g_pInterpreterPlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pInterpreterPlugIn == NULL)
			g_pInterpreterPlugIn = new InterpreterPlugIn();

		return g_pInterpreterPlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pInterpreterPlugIn;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn *GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pInterpreterPlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pInterpreterPlugIn;
	}
}

Meson_Cuneus_END

