//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Geometry::BoundingSphere
//	Bounding sphere implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		15/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Point3.h"

#include "GrvBoundingVolume.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \class BoundingSphere
/// Sphere implementation of a bounding volume.
//----------------------------------------------------------------------------------------------
class BoundingSphere
	: public Meson::Gravitas::Geometry::IBoundingVolume
{
public: // public variables

	//----------------------------------------------------------------------------------------------
	/// Bounding sphere centre.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> Centre;

	//----------------------------------------------------------------------------------------------
	/// Radius of the bounding sphere.
	//----------------------------------------------------------------------------------------------
	Real Radius;

	//----------------------------------------------------------------------------------------------
	/// Squared radius of the bounding sphere. This property is used to speedup particular
	/// geometrical computations.
	//----------------------------------------------------------------------------------------------
	Real RadiusSquared;

public:
	//----------------------------------------------------------------------------------------------/ 
	/// Constructs a bounding sphere with a unit radius.
	//----------------------------------------------------------------------------------------------
	BoundingSphere(void);

	//----------------------------------------------------------------------------------------------/ 
	/// Constructs a bounding sphere using the given source sphere.
	/// \param p_boundingSphere Source bounding sphere to copy.
	//----------------------------------------------------------------------------------------------
	BoundingSphere(const BoundingSphere &p_boundingSphere);

	//----------------------------------------------------------------------------------------------
	/// Constructs a bounding sphere from the given centre and radius. The squared radius is
	/// automatically computed from the given radius.
	/// \param p_ptCentre Bounding sphere centre.
	/// \param p_rRadius Bounding sphere radius.
	//----------------------------------------------------------------------------------------------
	BoundingSphere(const Meson::Common::Maths::TPoint3<Real>& p_ptCentre, Real p_rRadius);

	//----------------------------------------------------------------------------------------------
	/// Destroys the bounding sphere.
	//----------------------------------------------------------------------------------------------
	~BoundingSphere(void);

	// bounding volume methods
	BoundingVolumeType::BoundingVolumeType GetType(void) const;

	bool Contains(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	bool Intersects(const Ray& p_ray) const;
	bool Intersects(const Ray& p_ray,
		Meson::Common::Maths::TPoint3<Real>& p_ptIntersectionPoint) const;

	bool Intersects(const Triangle& p_triangle) const;

	bool Intersects(const IBoundingVolume& p_boundingVolume) const;
	bool IntersectsDynamic(const IBoundingVolume& p_boundingVolume,
		const Meson::Common::Maths::TVector3<Real>& p_vecSweep) const;

	BoundingVolumePtr Duplicate(void);
	void AssignFrom(BoundingVolumePtr p_pBoundingVolume);
	void Transform(
		const Meson::Gravitas::Geometry::Transform& p_transform);
	void ClosestPointTo(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint,
		Meson::Common::Maths::TPoint3<Real>& p_ptClosestPoint) const;
	void ProjectToInterval(
		const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
		Meson::Common::Maths::TInterval<Real>& p_interval) const;

	// custom methods
	//----------------------------------------------------------------------------------------------
	/// Assigns the given bounding sphere to this sphere.
	/// \param p_boundingSphere Source bounding sphere to copy.
	/// \returns a reference to this bounding sphere.
	//----------------------------------------------------------------------------------------------
	BoundingSphere& operator=(const BoundingSphere& p_boundingSphere);
};

Meson_Gravitas_Geometry_END
