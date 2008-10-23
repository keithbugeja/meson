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

#include "VstGeometryFactory.h"

Meson_Vistas_BEGIN

class PlaneFactory : public IGeometryFactory
{
public:
	PlaneFactory(void);
	~PlaneFactory(void);
	const Meson::Common::Text::String& GetName(void) const;

	IPropertySet* GetPropertySet(void);

	GeometryPtr CreateGeometry(void);
	GeometryPtr CreateGeometry(const Meson::Common::Text::String& p_strId);

protected:
	PropertySet m_propertySet;
	Meson::Common::Text::String m_strName;
};

Meson_Vistas_END