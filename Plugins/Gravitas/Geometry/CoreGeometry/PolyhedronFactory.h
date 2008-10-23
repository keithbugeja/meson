#pragma once

#include "GrvGeometryFactory.h"

Meson_Gravitas_Geometry_BEGIN

class PolyhedronFactory
	: public IGeometryFactory
{
private: // private variables
	Meson::Common::Text::String m_strName;

public: // public methods
	PolyhedronFactory(void);
	~PolyhedronFactory(void);
	const Meson::Common::Text::String& GetName(void);
	GeometryPtr CreateGeometry(void);
};

Meson_Gravitas_Geometry_END
