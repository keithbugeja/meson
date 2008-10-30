#include "ConvexPolyhedronEdge.h"

#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

ConvexPolyhedronEdge::ConvexPolyhedronEdge(void)
	: Vertices(NULL)
	, Displacement(false)
{
}

ConvexPolyhedronEdge::ConvexPolyhedronEdge(const ConvexPolyhedronEdge& p_convexPolyhedronEdge)
	: Vertices(p_convexPolyhedronEdge.Vertices)
	, Displacement(p_convexPolyhedronEdge.Displacement)
{
	VertexIndices[0] = p_convexPolyhedronEdge.VertexIndices[0];
	VertexIndices[1] = p_convexPolyhedronEdge.VertexIndices[1];
}

ConvexPolyhedronEdge::ConvexPolyhedronEdge(
	VectorList* p_pVertices,
	ushort p_nVertexIndex0, ushort p_nVertexIndex1)
	: Vertices(p_pVertices)
{
	VertexIndices[0] = p_nVertexIndex0;
	VertexIndices[1] = p_nVertexIndex1;
}

ConvexPolyhedronEdge& ConvexPolyhedronEdge::operator=(const ConvexPolyhedronEdge& p_convexPolyhedronEdge)
{
	Vertices = p_convexPolyhedronEdge.Vertices;
	VertexIndices[0] = p_convexPolyhedronEdge.VertexIndices[0];
	VertexIndices[1] = p_convexPolyhedronEdge.VertexIndices[1];
	Displacement = p_convexPolyhedronEdge.Displacement;
	return *this;
}

bool ConvexPolyhedronEdge::operator==(const ConvexPolyhedronEdge& p_convexPolyhedronEdge) const
{
	return Vertices == p_convexPolyhedronEdge.Vertices
		&& VertexIndices[0] == p_convexPolyhedronEdge.VertexIndices[0]
		&& VertexIndices[1] == p_convexPolyhedronEdge.VertexIndices[1];
}

void ConvexPolyhedronEdge::UpdateDerivativeData(void)
{
	if (Vertices == NULL)
		throw new MesonException("Vertices reference not set.",
			__FILE__, __LINE__);

	// update displacement
	if (VertexIndices[0] >= Vertices->Size())
		throw new MesonException("Invalid vertex index 0.",
			__FILE__, __LINE__);

	if (VertexIndices[1] >= Vertices->Size())
		throw new MesonException("Invalid vertex index 1.",
			__FILE__, __LINE__);

	Displacement = (*Vertices)[VertexIndices[1]] - (*Vertices)[VertexIndices[0]];
}
