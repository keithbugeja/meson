#pragma once

#include "ArrayList.h"
#include "Point3.h"

#include "GrvGeometry.h"
#include "GrvRay.h"

Meson_Gravitas_Geometry_BEGIN

class ConvexPolyhedronFace
{
public:
	const PointList* Vertices;
	Meson::Common::Collections::TArrayList<ushort> VertexIndices;
	Meson::Common::Maths::TVector3<Real> Normal;

public:
	ConvexPolyhedronFace(void);
	ConvexPolyhedronFace(const ConvexPolyhedronFace& p_convexPolyhedronFace);
	ConvexPolyhedronFace(PointList* p_pVertices);
	~ConvexPolyhedronFace(void);

	ConvexPolyhedronFace& operator=(const ConvexPolyhedronFace& p_convexPolyhedronFace);
	bool operator==(const ConvexPolyhedronFace& p_convexPolyhedronFace) const;

	Real ComputeArea(void) const;
	Real ComputeSignedVolume(const Meson::Common::Maths::TPoint3<Real>& p_ptOrigin) const;

	void UpdateDerivativeData(void);

	bool HasVertexIndex(ushort p_usVertexIndex) const;

	Real GetSignedDistanceFromPlane(const Meson::Common::Maths::TPoint3<Real>& ptPoint) const;
	bool IsPointOnPlane(const Meson::Common::Maths::TPoint3<Real>& ptPoint) const;
	bool IsPointAbovePlane(const Meson::Common::Maths::TPoint3<Real>& ptPoint) const;
	bool IsPointBelowPlane(const Meson::Common::Maths::TPoint3<Real>& ptPoint) const;
	bool IsPointProjectableOnFace(const Meson::Common::Maths::TPoint3<Real>& ptPoint) const;
	bool IntersectsRay(const Ray& p_ray) const;
	bool IntersectsRay(const Ray& p_ray,
		Meson::Common::Maths::TPoint3<Real>& p_ptIntersectionPoint) const;
};

Meson_Gravitas_Geometry_END
