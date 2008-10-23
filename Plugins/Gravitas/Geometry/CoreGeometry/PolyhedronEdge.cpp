#include "PolyhedronEdge.h"

Meson_Gravitas_Geometry_BEGIN

PolyhedronEdge::PolyhedronEdge(void)
{
}

PolyhedronEdge::PolyhedronEdge(const PolyhedronEdge& p_polyhedronEdge)
{
	VertexIndex[0] = p_polyhedronEdge.VertexIndex[0];
	VertexIndex[1] = p_polyhedronEdge.VertexIndex[1];
}

PolyhedronEdge::PolyhedronEdge(size_t p_unVertexIndex0, size_t p_unVertexIndex1)
{
	VertexIndex[0] = p_unVertexIndex0;
	VertexIndex[1] = p_unVertexIndex1;
}

PolyhedronEdge& PolyhedronEdge::operator=(const PolyhedronEdge& p_polyhedronEdge)
{
	VertexIndex[0] = p_polyhedronEdge.VertexIndex[0];
	VertexIndex[1] = p_polyhedronEdge.VertexIndex[1];
	return *this;
}

bool PolyhedronEdge::operator==(const PolyhedronEdge& p_polyhedronEdge)
{
	if (VertexIndex[0] != p_polyhedronEdge.VertexIndex[0]) return false;
	if (VertexIndex[1] != p_polyhedronEdge.VertexIndex[1]) return false;
	return true;
}

Meson_Gravitas_Geometry_END
