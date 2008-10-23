#pragma once

//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Geometry::IBoundingVolume
//	Bounding sphere implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		15/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "Pointer.h"
#include "Interval.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvTransform.h"
#include "GrvRay.h"
#include "GrvTriangle.h"

Meson_Gravitas_Geometry_BEGIN

namespace BoundingVolumeType
{
	//----------------------------------------------------------------------------------------------
	/// Enumeration of bounding volume types.
	//----------------------------------------------------------------------------------------------
	enum BoundingVolumeType
	{
		Sphere,
		AxisAlignedBox,
		OrientedBox
	};
}

//----------------------------------------------------------------------------------------------
/// \class IBoundingVolume
/// An abstract interface for bounding volumes. Currently supported bounding volume types are
/// spheres, axis-aligned bounding boxes and arbitrarily oriented boxes. Bounding volumes serve
/// as geometric primitives in space representation implementations (broad phase collision
/// detection) and are also employed in quick body-body tests (mid-phase collision detection)
/// prior to performing accurate body geometry tests. They are also used in other geometry-
/// related operations throughout the engine.
//----------------------------------------------------------------------------------------------
class IBoundingVolume
	: public Meson::Common::TReferenceCounter<IBoundingVolume>
{
public:
	virtual ~IBoundingVolume(void) {};

	//----------------------------------------------------------------------------------------------
	/// Returns the bounding volume type. This method provides a simple form of runtine type
	/// identification allowing for downcasting to the actual type if required.
	/// \returns the bounding volume type.
	//----------------------------------------------------------------------------------------------
	virtual BoundingVolumeType::BoundingVolumeType GetType(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given point is contained in the volume.
	/// \returns true if the given point is contained in the volume.
	//----------------------------------------------------------------------------------------------
	virtual bool Contains(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given ray intersects the volume.
	/// \param p_ray Ray to test against the volume.
	/// \returns true if the given ray intersects the volume.
	//----------------------------------------------------------------------------------------------
	virtual bool Intersects(const Ray& p_ray) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given ray intersects the volume. If the test is successful, the
	/// given intersection point reference is also populated.
	/// \param p_ray Ray to test against the volume.
	/// \param p_ptIntersectionPoint Intersection point of the ray with the volume.
	/// \returns true if the given ray intersects the volume.
	//----------------------------------------------------------------------------------------------
	virtual bool Intersects(const Ray& p_ray,
		Meson::Common::Maths::TPoint3<Real>& p_ptIntersectionPoint) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given triangle intersects the volume.
	/// \returns true if the given triangle intersects the volume.
	//----------------------------------------------------------------------------------------------
	virtual bool Intersects(const Triangle& p_triangle) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given bounding volume intersects this volume.
	/// \param p_boundingVolume bounding volume to test against this volume.
	/// \returns true if the given bounding volume intersects this volume.
	//----------------------------------------------------------------------------------------------
	virtual bool Intersects(const IBoundingVolume& p_boundingVolume) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given bounding volume potentially intersects this volume over the
	/// given translational sweep. The implementation of this predicate method should be cheap and
	/// conservative in nature, returning only potential positives or definite negatives.
	/// \param p_boundingVolume bounding volume to test against this volume.
	/// \param p_vecSweep Translational sweep applied to the tested volume.
	/// \returns true if the given bounding volume potentially intersects this volume over the
	/// given translational sweep.
	//----------------------------------------------------------------------------------------------
	virtual bool IntersectsDynamic(const IBoundingVolume& p_boundingVolume,
		const Meson::Common::Maths::TVector3<Real>& p_vecSweep) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a managed pointer to a duplicated instance of this bounding volume. The instance
	/// should be of the same type as the underlying implementation of this volume.
	/// \returns a managed pointer to a duplicated instance of this bounding volume.
	//----------------------------------------------------------------------------------------------
	virtual Meson::Common::TPointer<IBoundingVolume> Duplicate(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Assigns the properties of the given bounding volume to this volume. Both implementations
	/// must match or an exception is thrown.
	/// \param p_pBoundingVolume Source bounding volume to copy.
	//----------------------------------------------------------------------------------------------
	virtual void AssignFrom(Meson::Common::TPointer<IBoundingVolume> p_pBoundingVolume) = 0;

	//----------------------------------------------------------------------------------------------
	/// Transforms this bounding volume using the given transform instance.
	/// \param p_transform Transform instance to apply.
	//----------------------------------------------------------------------------------------------
	virtual void Transform(
		const Meson::Gravitas::Geometry::Transform& p_transform) = 0;

	//----------------------------------------------------------------------------------------------
	/// Computes the closest point, within the bounding volume, to the given point. If the given
	/// point is within the volume, it should coincide with the computed point.
	/// \param p_ptPoint Source point to operate on.
	/// \param p_ptClosestPoint Computed closest point within the volume.
	//----------------------------------------------------------------------------------------------
	void ClosestPointTo(
		const Meson::Common::Maths::TPoint3<Real> &p_ptPoint,
		Meson::Common::Maths::TPoint3<Real> &p_ptClosestPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Projects the bounding volume onto the given axis, storing the extents in the given interval.
	/// The given axis vector need not be of unit length, however, the interval span will scale
	/// accordingly. For any given axis vector, its negation should return the same interval.
	/// \param p_vecAxis Axis vector on which to project the volume.
	/// \param p_interval Interval used to store projection extents.
	//----------------------------------------------------------------------------------------------
	virtual void ProjectToInterval(
		const Meson::Common::Maths::TVector3<Real> &p_vecAxis,
		Meson::Common::Maths::TInterval<Real> &p_interval) const = 0;
};

//----------------------------------------------------------------------------------------------
/// Managed pointer to a bounding volume.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IBoundingVolume> BoundingVolumePtr;

Meson_Gravitas_Geometry_END
