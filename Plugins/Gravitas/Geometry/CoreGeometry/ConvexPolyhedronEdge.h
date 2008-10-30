#pragma once

#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

struct ConvexPolyhedronEdge
{
	const VectorList* Vertices;
	ushort VertexIndices[2];
	Meson::Common::Maths::TVector3<Real> Displacement;

	ConvexPolyhedronEdge(void);
	ConvexPolyhedronEdge(const ConvexPolyhedronEdge& p_convexPolyhedronEdge);
	ConvexPolyhedronEdge(
		VectorList* p_pVertices,
		ushort p_nVertexIndex0, ushort p_nVertexIndex1);

	ConvexPolyhedronEdge& operator=(const ConvexPolyhedronEdge& p_convexPolyhedronEdge);
	bool operator==(const ConvexPolyhedronEdge& p_convexPolyhedronEdge) const;

	void UpdateDerivativeData(void);
};

Meson_Gravitas_Geometry_END
