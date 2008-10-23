#pragma once

#include "GrvGeometryFactory.h"

Meson_Gravitas_Geometry_BEGIN

class SphereFactory
	: public IGeometryFactory
{
private: // private variables
	Meson::Common::Text::String m_strName;

public: // public methods
	SphereFactory(void);
	~SphereFactory(void);
	const Meson::Common::Text::String& GetName(void);
	GeometryPtr CreateGeometry(void);
	GeometryPtr CreateGeometry(const Meson::Common::Text::String& p_strId);
};

Meson_Gravitas_Geometry_END
