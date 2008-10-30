//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Geometry::BoundingAxisAlignedBox
//	Axis-aligned bounding box implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		15/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Interval.h"

#include "GrvBoundingVolume.h"

#include "GrvBoundingSphere.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \class BoundingAxisAlignedBox
/// Axis-aligned bounding box implementation of a bounding volume.
//----------------------------------------------------------------------------------------------
class BoundingAxisAlignedBox
	: public Meson::Gravitas::Geometry::IBoundingVolume
{
public: // public variables

	//----------------------------------------------------------------------------------------------
	/// Lowerbound coordinates for the axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Min;

	//----------------------------------------------------------------------------------------------
	/// Upperbound coordinates for the axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Max;

private: // private methods
	void QuickSweep(
		const Meson::Common::Maths::TVector3<Real> &p_vecSweep,
		BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const;

public: // public methods
	//----------------------------------------------------------------------------------------------
	/// Constructs a unit-size, axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	BoundingAxisAlignedBox(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs an axis-aligned bounding box from the given source box.
	/// \param p_boundingAxisAlignedBox Source axis-aligned box to copy.
	//----------------------------------------------------------------------------------------------
	BoundingAxisAlignedBox(const BoundingAxisAlignedBox &p_boundingAxisAlignedBox);

	//----------------------------------------------------------------------------------------------
	/// Constructs an axis-aligned bounding box from the given lowerbound and upperbound points.
	/// \param p_vecMin Lowerbound coordinates for the box.
	/// \param p_vecMax Upperbound coordinates for the box.
	//----------------------------------------------------------------------------------------------
	BoundingAxisAlignedBox(
		const Meson::Common::Maths::TVector3<Real> &p_vecMin,
		const Meson::Common::Maths::TVector3<Real> &p_vecMax);

	//----------------------------------------------------------------------------------------------
	/// Destorys the axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	~BoundingAxisAlignedBox(void);

	// bounding volume methods
	BoundingVolumeType::BoundingVolumeType GetType(void) const;

	bool Contains(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;

	bool Intersects(const Ray& p_ray) const;
	bool Intersects(const Ray& p_ray,
		Meson::Common::Maths::TVector3<Real>& p_vecIntersectionPoint) const;

	bool Intersects(const Triangle& p_triangle) const;

	bool Intersects(const IBoundingVolume& p_boundingVolume) const;
	bool IntersectsDynamic(const IBoundingVolume& p_boundingVolume,
		const Meson::Common::Maths::TVector3<Real>& p_vecSweep) const;

	BoundingVolumePtr Duplicate(void);
	void AssignFrom(BoundingVolumePtr p_pBoundingVolume);
	void Transform(
		const Meson::Gravitas::Geometry::Transform& p_transform);
	void ClosestPointTo(
		const Meson::Common::Maths::TVector3<Real>& p_vecPoint,
		Meson::Common::Maths::TVector3<Real>& p_vecClosestPoint) const;
	void ProjectToInterval(
		const Meson::Common::Maths::TVector3<Real> &p_vecAxis,
		Meson::Common::Maths::TInterval<Real>& p_interval) const;

	// custom methods

	//----------------------------------------------------------------------------------------------
	/// Returns the centroid of the axis-aligned bounding box.
	/// \returns the centroid of the axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Centre(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the extents (half-width, half-height and half-depth) of the axis-aligned bounding
	/// box.
	/// \returns the extents of the axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Extent(void) const;

	//----------------------------------------------------------------------------------------------
	/// Assigns the given axis-aligned bounding box to this box.
	/// \param p_boundingAxisAlignedBox Source axis-aligned bounding box to copy.
	/// \returns a reference to this axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	BoundingAxisAlignedBox& operator=(
		const BoundingAxisAlignedBox& p_boundingAxisAlignedBox);

	//----------------------------------------------------------------------------------------------
	/// Extends the axis-aligned bounding box to contain the given point.
	/// \param p_vecPoint A point to be contained in the axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	void ExtendToPoint(const Meson::Common::Maths::TVector3<Real>& p_vecPoint);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given axis-aligned bounding box is equal to this box.
	/// \param p_boundingAxisAlignedBox Axis-aligned bounding box to compare.
	/// \returns true if the given axis-aligned bounding box is equal to this box.
	//----------------------------------------------------------------------------------------------
	const bool operator==(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given axis-aligned bounding box is not equal to this box.
	/// \param p_boundingAxisAlignedBox Axis-aligned bounding box to compare.
	/// \returns true if the given axis-aligned bounding box is not equal to this box.
	//----------------------------------------------------------------------------------------------
	const bool operator!=(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given axis-aligned bounding box intersects this box.
	/// \param p_boundingAxisAlignedBox Axis-aligned bounding box to test.
	/// \returns true if the given axis-aligned bounding box intersects this box.
	//----------------------------------------------------------------------------------------------
	bool Intersects(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given bounding sphere intersects this axis-aligned bounding box.
	/// \param p_boundingSphere Bounding sphere to test.
	/// \returns true if the given bounding sphere intersects this axis-aligned bounding box.
	//----------------------------------------------------------------------------------------------
	bool Intersects(const BoundingSphere& p_boundingSphere) const;

	//----------------------------------------------------------------------------------------------
	/// Computes a list of maximal (support) vertices along the given direction. The list usually
	/// contains only one vertex, but in some cases multiple supporting vertices may be found.
	/// \param p_vecDirection A direction vector (not necessarily unit length).
	/// \param p_listVertices Destination list of maximal vertices.
	//----------------------------------------------------------------------------------------------
	void EnumerateMaximalVertices(
		const Meson::Common::Maths::TVector3<Real>& p_vecDirection,
		Meson::Gravitas::VectorList& p_vectorList) const;
};

Meson_Gravitas_Geometry_END
