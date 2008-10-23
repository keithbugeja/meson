//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "String.h"
#include "VstTypes.h"

#include "VstRendererFactory.h"
#include "BasicRenderer.h"

Meson_Vistas_BEGIN

class BasicRendererFactory : public IRendererFactory
{
public:
	BasicRendererFactory(void);
	~BasicRendererFactory(void);

	const Meson::Common::Text::String& GetName(void) const;
	
	RendererPtr CreateRenderer(void);

protected:
	Meson::Common::Text::String m_strName;
};

Meson_Vistas_END