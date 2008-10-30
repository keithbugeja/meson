#pragma once

#include "String8.h"
#include "List.h"

#include "GrvTypes.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;

Meson_Gravitas_Geometry_BEGIN

struct GeometryFace
{
	TArrayList<int> VertexIndices;

	GeometryFace(void);

	GeometryFace& operator=(const GeometryFace& p_geometryFace);
	bool operator==(const GeometryFace& p_geometryFace) const;
};

typedef TList<GeometryFace> GeometryFaceList;
typedef TArrayList<GeometryFace> GeometryFaceArrayList;

class GeometryLoader
{
public:
	GeometryLoader(void);
	~GeometryLoader(void);

	void LoadWavefrontGeometry(
		const String& p_strFilename,
		bool p_bTriangulateFaces,
		VectorList& p_listVertices,
		GeometryFaceList& p_listGeometryFaces);
};

Meson_Gravitas_Geometry_END
