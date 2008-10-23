#pragma once

#include "Vector3.h"
#include "Point3.h"
#include "List.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvLineSegment.h"

Meson_Gravitas_Geometry_BEGIN

class PolyhedronFace
{
public: // public variables
	Meson::Common::Collections::TList<Meson::Common::Maths::TPoint3<Real>>
		*VertexList;
	size_t VertexIndex[3];
	size_t FaceIndex[3];
	Meson::Common::Maths::TVector3<Real> Normal;

public: // public methods
	PolyhedronFace(void);
	PolyhedronFace(const PolyhedronFace &p_polyhedronFace);
	PolyhedronFace(
		const Meson::Common::Collections::TList<Meson::Common::Maths::TPoint3<Real>> *p_plistVertices,
		size_t p_unVertexIndex0, size_t p_unVertexIndex1, size_t p_unVertexIndex2);
	PolyhedronFace(
		const Meson::Common::Collections::TList<Meson::Common::Maths::TPoint3<Real>> *p_plistVertices,
		size_t p_unVertexIndex0, size_t p_unVertexIndex1, size_t p_unVertexIndex2,
		size_t p_unFaceIndex0, size_t p_unFaceIndex1, size_t p_unFaceIndex2);
	PolyhedronFace(
		const Meson::Common::Collections::TList<Meson::Common::Maths::TPoint3<Real>> *p_plistVertices,
		const size_t *p_punVertexIndices);

	bool HasVertexIndex(size_t p_unIndex) const;
	Real SignedDistanceFromPlane(const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;
	bool IsOutside(const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;
	bool IsInside(const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;
	bool IsInPlane(const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;

	bool IntersectsQuick(const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;
	bool Intersects(const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;
	bool Intersects(const Meson::Gravitas::Geometry::LineSegment &p_lineSegment) const;
	bool Intersects(const Meson::Gravitas::Geometry::LineSegment &p_lineSegment,
		Meson::Common::Maths::TPoint3<Real> &p_ptIntersectionPoint) const;

	Meson::Common::Maths::TPoint3<Real> ClosestPointTo(
		const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;

	void UpdateNormal(void);
	void Invert(void);

	PolyhedronFace &operator=(const PolyhedronFace &p_polyhedronFace);
	bool operator==(const PolyhedronFace &p_polyhedronFace) const;
	bool operator<(const PolyhedronFace &p_polyhedronFace) const;
};

Meson_Gravitas_Geometry_END
