//----------------------------------------------------------------------------------------------
//	Meson::Common::PlugIn::ILibrary
//  Interface class for a system's dynamic library implementation
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Definitions.h"
#include "Types.h"

#include "MesonException.h"
#include "DebugSupport.h"

#include "String8.h"
#include "Version.h"
#include "Platform.h"

Meson_Common_PlugIn_BEGIN

class ILibrary
{
public:

	// Load a dynamic library
	virtual bool Load(Meson::Common::Text::String p_strPlugIn) = 0;
	
	// Return a symbol from dynamic library
	virtual void* GetSymbol(Meson::Common::Text::String p_strSymbolName) = 0;
	
	// Unload dynamic library
	virtual void Unload(void) = 0;
};

Meson_Common_PlugIn_END
