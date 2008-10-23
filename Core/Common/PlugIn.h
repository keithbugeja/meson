//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::IPlugIn
//	Interface class for extension plug-ins
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
//	Notes:
//	
//	The Meson Plug-In system interfaces to dynamic libraries through a set of functions which
//	allow the creation and disposal of external Plug-In objects. Meson supports both Singleton
//	and Multiple-Instance Plug-Ins. The PlugInManager automatically loads and registers 
//	Singleton Plug-Ins; using Multi-Instance Plug-Ins requires the user to manually Load, 
//	and Instantiate external Plug-Ins using the LibraryManager before registering them with
//	the PlugInManager.
//
//	Libraries implementing the Singleton interface must define and export the following functions:
//
//	IPlugIn* GetPlugInSingleton(void)	
//		returns the singleton Plug-In instance
//
//	void DisposePlugInSingleton(void)   
//		disposes of the singleton Plug-In instance
//
//	Plug-Ins implementing the Multi-Instance interface must define and export the following functions:
//
//	HPlugInInstance CreatePlugInInstance(void)	
//		creates a new Plug-In instance and returns an associated handle
//
//	IPlugIn* GetPlugInInstance(HPlugInInstance p_hInstance)
//		returns the Plug-In instance associated with the given handle
//
//	void DisposePlugInInstance(HPlugInInstance p_hInstance)
//		disposes of the Plug-In instance associated with the given handle
//
//	void DisposePlugInInstances()
//		disposes of all the instances
//
//	Note that statically linked Plug-Ins may also be registered to extend Meson.
//----------------------------------------------------------------------------------------------
//	To do:
//
//	Under the current plug-in system, Meson common and engine code is linked twice, once to the
//	application and another to plug-ins. This is both a bottleneck and a potential compatibility
//	hazard. Should investigate.
//----------------------------------------------------------------------------------------------

#pragma once

#include "Types.h"
#include "Namespaces.h"
#include "MesonException.h"
#include "DebugSupport.h"

#include "String8.h"
#include "Version.h"

Meson_Common_PlugIn_BEGIN

class SharedObjectMap;

class IPlugIn
{
public:
	// Returns the plug-in name
	virtual const Meson::Common::Text::String& GetName(void) const = 0;

	// Returns the plug-in version
	virtual const Version& GetVersion(void) const = 0;

	// Registers the plug-in with its respective engine
	virtual void Register(SharedObjectMap* p_pSharedObjectMap) = 0;
	
	// Initialises the plug-in
	virtual void Initialise(void) = 0;

	// Shuts down the plug-in
	virtual void Shutdown(void) = 0;

	// Unregisters the plug-in from its respective engine
	virtual void Unregister(void) = 0;
};

Meson_Common_PlugIn_END

