//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

#include "VstRenderer.h"

Meson_Vistas_BEGIN

class IRendererFactory
{
public:
	virtual ~IRendererFactory(void) {};
	virtual const Meson::Common::Text::String& GetName(void) const = 0;
	virtual RendererPtr CreateRenderer(void) = 0;
};

Meson_Vistas_END