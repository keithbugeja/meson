#pragma once

#include "GrvNamespaces.h"

Meson_Gravitas_Geometry_BEGIN

class PolyhedronEdge
{
public: // public variables
	size_t VertexIndex[2];

public: // public methods
	PolyhedronEdge(void);
	PolyhedronEdge(const PolyhedronEdge &p_polyhedronEdge);
	PolyhedronEdge(size_t p_unVertexIndex0, size_t p_unVertexIndex1);
	PolyhedronEdge& operator=(const PolyhedronEdge &p_polyhedronEdge);
	bool operator==(const PolyhedronEdge &p_polyhedronEdge);
};

Meson_Gravitas_Geometry_END
