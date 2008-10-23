//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

#include "VstGeometry.h"
#include "VstPropertySet.h"

Meson_Vistas_BEGIN

class IGeometryFactory
{
public:
	virtual ~IGeometryFactory(void) {};
	virtual const Meson::Common::Text::String& GetName(void) const = 0;

	virtual IPropertySet* GetPropertySet(void) = 0;

	virtual GeometryPtr CreateGeometry(void) = 0;
	virtual GeometryPtr CreateGeometry(const Meson::Common::Text::String& p_strId) = 0;
};

Meson_Vistas_END