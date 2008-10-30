#pragma once

#include "ArrayList.h"

#include "GrvGeometry.h"
#include "GrvRay.h"

Meson_Gravitas_Geometry_BEGIN

class ConvexPolyhedronFace
{
public:
	const VectorList* Vertices;
	Meson::Common::Collections::TArrayList<ushort> VertexIndices;
	Meson::Common::Maths::TVector3<Real> Normal;

public:
	ConvexPolyhedronFace(void);
	ConvexPolyhedronFace(const ConvexPolyhedronFace& p_convexPolyhedronFace);
	ConvexPolyhedronFace(VectorList* p_pVertices);
	~ConvexPolyhedronFace(void);

	ConvexPolyhedronFace& operator=(const ConvexPolyhedronFace& p_convexPolyhedronFace);
	bool operator==(const ConvexPolyhedronFace& p_convexPolyhedronFace) const;

	Real ComputeArea(void) const;
	Real ComputeSignedVolume(const Meson::Common::Maths::TVector3<Real>& p_vecOrigin) const;

	void UpdateDerivativeData(void);

	bool HasVertexIndex(ushort p_usVertexIndex) const;

	Real GetSignedDistanceFromPlane(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool IsPointOnPlane(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool IsPointAbovePlane(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool IsPointBelowPlane(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool IsPointProjectableOnFace(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;
	bool IntersectsRay(const Ray& p_ray) const;
	bool IntersectsRay(const Ray& p_ray,
		Meson::Common::Maths::TVector3<Real>& p_vecIntersectionPoint) const;
};

Meson_Gravitas_Geometry_END
