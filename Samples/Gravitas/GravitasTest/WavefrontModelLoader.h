#pragma once

#include "Point3.h"
#include "String8.h"
#include "List.h"

#include "GrvTypes.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;

struct WavefrontFace
{
	int VertexIndex[3];
	int NormalIndex[3];
	int TextureCoordinateIndex[3];

	WavefrontFace(void);
	bool operator==(const WavefrontFace& p_wavefrontFace) const;
};

typedef TList<WavefrontFace> WavefrontFaceList;
typedef TArrayList<WavefrontFace> WavefrontFaceArrayList;

class WavefrontModelLoader
{
public:
	WavefrontModelLoader(void);
	~WavefrontModelLoader(void);

	void LoadModel(
		const String& p_strFilename,
		PointList& p_listVertices,
		VectorList& p_listNormals,
		TList< TVector2<Real> >& p_listTextureCoordinates,
		WavefrontFaceList& p_listFaces);
};
