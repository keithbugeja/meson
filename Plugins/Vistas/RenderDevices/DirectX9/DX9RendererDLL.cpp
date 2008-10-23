//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9RendererDLL
//	Dynamic Link Library interface for Vistas Direct3D9 renderer plugin.
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
#include "DX9RendererPlugIn.h"

Meson_Vistas_BEGIN

DX9RendererPlugIn* g_pDX9PlugIn = NULL;

extern "C" 
{
	// Singleton
	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInSingleton(void)
	{
		if (g_pDX9PlugIn == NULL)
			g_pDX9PlugIn = new DX9RendererPlugIn();

		return g_pDX9PlugIn;
	}

	DLLExport void DisposePlugInSingleton(void)
	{
		delete g_pDX9PlugIn;
	}

	// Multi-instance
	DLLExport Meson::Common::PlugIn::HPlugInInstance CreatePlugInInstance(void)
	{
		return (Meson::Common::PlugIn::HPlugInInstance)NULL;
	}

	DLLExport Meson::Common::PlugIn::IPlugIn* GetPlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
		return g_pDX9PlugIn;
	}

	void DLLExport DisposePlugInInstance(Meson::Common::PlugIn::HPlugInInstance p_hInstance)
	{
	}

	void DLLExport DisposePlugInInstances()
	{
		delete g_pDX9PlugIn;
	}
}

Meson_Vistas_END

