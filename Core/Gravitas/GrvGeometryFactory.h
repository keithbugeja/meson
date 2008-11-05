#pragma once

#include "String8.h"

#include "GrvNamespaces.h"
#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \class IGeometryFactory
/// Abstract factory interface for geometry instances. Implementations of this interface
/// generate specific geometry implementations and are normally used through the engine's plugin
/// mechanism.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API IGeometryFactory
{
public:
	virtual ~IGeometryFactory(void) {};

	//----------------------------------------------------------------------------------------------
	/// Returns the name of the geometry factory.
	/// \returns the name of the geometry factory.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetName(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Creates and returns a new geometry instance.
	/// \returns a new geometry instance.
	//----------------------------------------------------------------------------------------------
	virtual GeometryPtr CreateGeometry(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Creates and returns a new geometry instance with the given ID.
	/// \param p_strId ID to assign to the geometry instance.
	/// \returns a new geometry instance.
	//----------------------------------------------------------------------------------------------
	virtual GeometryPtr CreateGeometry(const Meson::Common::Text::String& p_strId) = 0;
};

Meson_Gravitas_Geometry_END
