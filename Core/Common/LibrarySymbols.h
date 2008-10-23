#pragma once

#include "Types.h"
#include "PlugIn.h"

Meson_Common_PlugIn_BEGIN

typedef dword HPlugInInstance;

typedef IPlugIn*(*SYMBOL_SINGLE_GET)(void);
typedef void(*SYMBOL_SINGLE_DISPOSE)(void);

typedef HPlugInInstance(*SYMBOL_MULTI_CREATE)(void);
typedef IPlugIn*(*SYMBOL_MULTI_GET)(HPlugInInstance);
typedef void(*SYMBOL_MULTI_DISPOSE)(HPlugInInstance);
typedef void(*SYMBOL_MULTI_DISPOSEALL)(void);

Meson_Common_PlugIn_END
