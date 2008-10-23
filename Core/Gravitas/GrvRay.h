#pragma once

#include "Point3.h"
#include "Vector3.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvTransform.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct Ray
/// Ray representation defined by a source point and direction vector.
//----------------------------------------------------------------------------------------------
struct Ray
{
	// variables

	//----------------------------------------------------------------------------------------------
	/// A point defining the source of origin for the ray.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> Source;

	//----------------------------------------------------------------------------------------------
	/// Unit vector representing the direction of the ray.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Direction;

	// methods

	//----------------------------------------------------------------------------------------------
	/// Constructs a ray with the source at the origin and oriented along the positive X axis.
	//----------------------------------------------------------------------------------------------
	Ray(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a ray from the given source ray.
	/// \param p_ray Source ray to copy.
	//----------------------------------------------------------------------------------------------
	Ray(const Ray& p_ray);

	//----------------------------------------------------------------------------------------------
	/// Constructs a ray using the given source point and direction vector.
	/// \param p_ptSource Source of origin for the ray.
	/// \param p_vecDirection Direction vector for the ray.
	//----------------------------------------------------------------------------------------------
	Ray(const Meson::Common::Maths::TPoint3<Real>& p_ptSource,
		const Meson::Common::Maths::TVector3<Real>& p_vecDirection);

	//----------------------------------------------------------------------------------------------
	/// Constructs a ray using the given source point and oriented to point to the given
	/// pass-through point.
	/// \param p_ptSource Source of origin for the ray.
	/// \param p_ptPassThrough Point through which the ray must pass.
	//----------------------------------------------------------------------------------------------
	Ray(const Meson::Common::Maths::TPoint3<Real>& p_ptSource,
		const Meson::Common::Maths::TPoint3<Real>& p_ptPassThrough);

	//----------------------------------------------------------------------------------------------
	/// Destroys the ray.
	//----------------------------------------------------------------------------------------------
	~Ray(void);

	//----------------------------------------------------------------------------------------------
	/// Orients the ray such that it passes through the given point.
	/// \param p_ptPassThrough Point through which the ray must pass.
	//----------------------------------------------------------------------------------------------
	void OrientTo(const Meson::Common::Maths::TPoint3<Real>& p_ptPassThrough);

	//----------------------------------------------------------------------------------------------
	/// Transforms the ray using the given 3x3 matrix.
	/// \param p_matrix Transformation matrix to apply to the ray.
	//----------------------------------------------------------------------------------------------
	void Transform(const Meson::Common::Maths::TMatrix3<Real>& p_matrix);

	//----------------------------------------------------------------------------------------------
	/// Transforms the ray using the given transform object.
	/// \param p_transform Transform object to apply to the ray.
	//----------------------------------------------------------------------------------------------
	void Transform(const Meson::Gravitas::Geometry::Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Returns a transformed copy of the ray using the given 3x3 matrix.
	/// \param p_matrix Transformation matrix to apply to the ray.
	/// \returns a transformed copy of the ray.
	//----------------------------------------------------------------------------------------------
	Ray TransformCopy(const Meson::Common::Maths::TMatrix3<Real>& p_matrix);

	//----------------------------------------------------------------------------------------------
	/// Returns a transformed copy of the ray using the given transform object.
	/// \param p_transform Transform object to apply to the ray.
	/// \returns a transformed copy of the ray.
	//----------------------------------------------------------------------------------------------
	Ray TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the closest point on the ray to the given point.
	/// \param p_ptPoint Point to test.
	/// \returns the closest point on the ray to the given point.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> ClosestPointTo(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the shortest distance from the ray to the given point.
	/// \param p_ptPoint Point to test.
	/// \returns the shortest distance from the ray to the given point.
	//----------------------------------------------------------------------------------------------
	Real DistanceTo(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the shortest squared distance from the ray to the given point.
	/// \param p_ptPoint Point to test.
	/// \returns the shortest squared distance from the ray to the given point.
	//----------------------------------------------------------------------------------------------
	Real DistanceSquaredTo(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns a point on the ray for the given distance from the source.
	/// \param p_rDistance Distance of the required point of the ray from the source.
	/// \returns a point on the ray for the given distance from the source.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> ComputePosition(Real p_rDistance) const;
};

Meson_Gravitas_Geometry_END
