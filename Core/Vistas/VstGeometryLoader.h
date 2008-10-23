//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

#include "VstGeometry.h"
#include "VstAbstractMesh.h"

Meson_Vistas_BEGIN
	//----------------------------------------------------------------------------------------------
	class IGeometryLoader : public Meson::Common::TReferenceCounter<IGeometryLoader>
	{
	public:
		virtual ~IGeometryLoader() { };

		virtual const Meson::Common::Text::String& GetType(void) const = 0;

		virtual GeometryListPtr Load(const Meson::Common::Text::String& p_strFilename) = 0;
		virtual GeometryPtr LoadSingle(const Meson::Common::Text::String& p_strFilename) = 0;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END