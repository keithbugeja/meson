#pragma once

#include "Vector3.h"
#include "List.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvLineSegment.h"

Meson_Gravitas_Geometry_BEGIN

class PolyhedronFace
{
public: // public variables
	VectorList* VertexList;
	size_t VertexIndex[3];
	size_t FaceIndex[3];
	Meson::Common::Maths::TVector3<Real> Normal;

public: // public methods
	PolyhedronFace(void);
	PolyhedronFace(const PolyhedronFace &p_polyhedronFace);
	PolyhedronFace(
		const VectorList* p_plistVertices,
		size_t p_unVertexIndex0, size_t p_unVertexIndex1, size_t p_unVertexIndex2);
	PolyhedronFace(
		const VectorList* p_plistVertices,
		size_t p_unVertexIndex0, size_t p_unVertexIndex1, size_t p_unVertexIndex2,
		size_t p_unFaceIndex0, size_t p_unFaceIndex1, size_t p_unFaceIndex2);
	PolyhedronFace(
		const VectorList* p_plistVertices,
		const size_t* p_punVertexIndices);

	bool HasVertexIndex(size_t p_unIndex) const;
	Real SignedDistanceFromPlane(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool IsOutside(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool IsInside(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool IsInPlane(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;

	bool IntersectsQuick(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool Intersects(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool Intersects(const Meson::Gravitas::Geometry::LineSegment& p_lineSegment) const;
	bool Intersects(const Meson::Gravitas::Geometry::LineSegment& p_lineSegment,
		Meson::Common::Maths::TVector3<Real> &p_vecIntersectionPoint) const;

	Meson::Common::Maths::TVector3<Real> ClosestPointTo(
		const Meson::Common::Maths::TVector3<Real> &p_vecPoint) const;

	void UpdateNormal(void);
	void Invert(void);

	PolyhedronFace& operator=(const PolyhedronFace& p_polyhedronFace);
	bool operator==(const PolyhedronFace& p_polyhedronFace) const;
	bool operator<(const PolyhedronFace& p_polyhedronFace) const;
};

Meson_Gravitas_Geometry_END
