//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Geometry::BoundingOrientedBox
//	Oriented bounding box implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		16/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Quaternion.h"
#include "Vector3.h"
#include "Interval.h"

#include "GrvBoundingVolume.h"
#include "GrvLineSegment.h"

#include "GrvBoundingSphere.h"
#include "GrvBoundingAxisAlignedBox.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \class BoundingOrientedBox
/// Oriented bounding box implementation of a bounding volume.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API BoundingOrientedBox
	: public Meson::Gravitas::Geometry::IBoundingVolume
{
public: // public variables
	//----------------------------------------------------------------------------------------------
	/// Centroid of the oriented bounding box.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Centre;

	//----------------------------------------------------------------------------------------------
	/// Box axes in world coordinates.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Axis[3];

	//----------------------------------------------------------------------------------------------
	/// Vector of half-extents along the box axes.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Extent;

private: // private methods
	void QuickSweep(
		const Meson::Common::Maths::TVector3<Real> &p_vecSweep,
		BoundingOrientedBox &p_boundingOrientedBox) const;
	void NormaliseAxes(void);

public: // public methods

	// constructors 

	//----------------------------------------------------------------------------------------------
	/// Constructs an oriented bounding box. The box is axis-aligned, and unit-size with extents of
	/// 0.5 along eeach component.
	//----------------------------------------------------------------------------------------------
	BoundingOrientedBox(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs an oriented bounding box with the given source oriented box.
	/// \param p_boundingOrientedBox Source box to copy.
	//----------------------------------------------------------------------------------------------
	BoundingOrientedBox(const BoundingOrientedBox &p_boundingOrientedBox);

	//----------------------------------------------------------------------------------------------
	/// Constructs an oriented bounding box with the given source axis-aligned box. The oriented box
	/// is aligend with the coordinate axis to match the source box.
	/// \param p_boundingAxisAlignedBox Source axis-aligned box to copy.
	//----------------------------------------------------------------------------------------------
	BoundingOrientedBox(const BoundingAxisAlignedBox &p_boundingAxisAlignedBox);

	//----------------------------------------------------------------------------------------------
	/// Constructs an oriented bounding box using the given centre, orientation quoternion and
	/// extents.
	/// \param p_vecCentre Centroid for the box.
	/// \param p_qtrOrientation A unit-quaternion representing the box orientation.
	/// \param p_vecExtent A vector of half-extents along the local box coordinates.
	//----------------------------------------------------------------------------------------------
	BoundingOrientedBox(
		const Meson::Common::Maths::TVector3<Real> &p_vecCentre,
		const Meson::Common::Maths::TQuaternion<Real> &p_qtrOrientation,
		const Meson::Common::Maths::TVector3<Real> &p_vecExtent);

	//----------------------------------------------------------------------------------------------
	/// Constructs an oriented bounding box using the given centre and extents. The box is oriented
	/// with the coordinate axes.
	/// \param p_vecCentre Centroid for the box.
	/// \param p_vecExtent A vector of half-extents along the local box coordinates.
	//----------------------------------------------------------------------------------------------
	BoundingOrientedBox(
		const Meson::Common::Maths::TVector3<Real> &p_vecCentre,
		const Meson::Common::Maths::TVector3<Real> &p_vecExtent);

	//----------------------------------------------------------------------------------------------
	/// Destroys the oriented bounding box.
	//----------------------------------------------------------------------------------------------
	~BoundingOrientedBox(void);

	// bounding volume methods
	BoundingVolumeType::BoundingVolumeType GetType(void) const;

	bool Contains(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const;

	bool Intersects(const Ray& p_ray) const;
	bool Intersects(const Ray& p_ray,
		Meson::Common::Maths::TVector3<Real>& p_vecIntersectionPoint) const;

	bool Intersects(const Triangle& p_triangle) const;

	bool Intersects(const IBoundingVolume &p_boundingVolume) const;
	bool IntersectsDynamic(const IBoundingVolume &p_boundingVolume,
		const Meson::Common::Maths::TVector3<Real> &p_vecSweep) const;

	BoundingVolumePtr Duplicate(void);
	void AssignFrom(BoundingVolumePtr p_pBoundingVolume);
	void Transform(
		const Meson::Gravitas::Geometry::Transform& p_transform);
	void ClosestPointTo(
		const Meson::Common::Maths::TVector3<Real> &p_vecPoint,
		Meson::Common::Maths::TVector3<Real> &p_vecClosestPoint) const;
	void ProjectToInterval(
		const Meson::Common::Maths::TVector3<Real> &p_vecAxis,
		Meson::Common::Maths::TInterval<Real> &p_interval) const;

	// custom methods

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given oriented bounding box is equal to this box.
	/// \param p_boundingOrientedBox oriented bounding box to compare.
	/// \returns true if the given oriented bounding box is equal to this box.
	//----------------------------------------------------------------------------------------------
	bool operator==(const BoundingOrientedBox &p_boundingOrientedBox) const;

	//----------------------------------------------------------------------------------------------
	/// Assigns the properties of the given oriented bounding box to this box.
	/// \param p_boundingOrientedBox Source oriented bounding box to copy.
	/// \returns a reference to this oriented bounding box.
	//----------------------------------------------------------------------------------------------
	BoundingOrientedBox& operator=(
		const BoundingOrientedBox &p_boundingOrientedBox);

	//----------------------------------------------------------------------------------------------
	/// Assigns the properties of the given axis-aligned bounding box to this box. The orientation
	/// of this box is aligened with the coordinate axes in line with the source axis-aligned box.
	/// \param p_boundingAxisAlignedBox Source axis-aligned bounding box to copy.
	/// \returns a reference to this oriented bounding box.
	//----------------------------------------------------------------------------------------------
	BoundingOrientedBox& operator=(
		const BoundingAxisAlignedBox &p_boundingAxisAlignedBox);

	//----------------------------------------------------------------------------------------------
	/// Returns the orientation of the oriented bounding box as a 3x3 rotation matrix.
	/// \returns the orientation of the oriented bounding box as a 3x3 rotation matrix.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TMatrix3<Real> Orientation(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given bounding sphere intersects this oriented bounding box.
	/// \param p_boundingSphere Bounding sphere to test.
	/// \returns true if the given bounding sphere intersects this oriented bounding box.
	//----------------------------------------------------------------------------------------------
	bool Intersects(const BoundingSphere &p_boundingSphere) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given axis-aligned bounding box intersects this box.
	/// \param p_boundingAxisAlignedBox Axis-aligned bounding box to test.
	/// \returns true if the given axis-aligned bounding box intersects this box.
	//----------------------------------------------------------------------------------------------
	bool Intersects(const BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given oriented bounding box intersects this box.
	/// \param p_boundingOrientedBox Oriented bounding box to test.
	/// \returns true if the given oriented bounding box intersects this box.
	//----------------------------------------------------------------------------------------------
	bool Intersects(const BoundingOrientedBox &p_boundingOrientedBox) const;

	//----------------------------------------------------------------------------------------------
	/// Populates the given list with the vertices of the oriented bounding box.
	/// \param p_listVertices Vertex list to populate.
	//----------------------------------------------------------------------------------------------
	void EnumerateFeatures(
		Meson::Gravitas::VectorList& p_listVertices) const;

	//----------------------------------------------------------------------------------------------
	/// Populates the given list with the edges of the oriented bounding box.
	/// \param p_listEdges List of line segments to populate with edges.
	//----------------------------------------------------------------------------------------------
	void EnumerateFeatures(
		Meson::Gravitas::Geometry::LineSegmentList& p_listEdges) const;

	//----------------------------------------------------------------------------------------------
	/// Populates the given lists with the vertices and edges of the oriented bounding box.
	/// \param p_listVertices Vertex list to populate.
	/// \param p_listEdges List of line segments to populate with edges.
	//----------------------------------------------------------------------------------------------
	void EnumerateFeatures(
		Meson::Gravitas::VectorList& p_listVertices,
		Meson::Gravitas::Geometry::LineSegmentList& p_listEdges) const;

	//----------------------------------------------------------------------------------------------
	/// Computes a list of maximal (support) vertices along the given direction. The list usually
	/// contains only one vertex, but in some cases multiple supporting vertices may be found.
	/// \param p_vecDirection A direction vector (not necessarily unit length).
	/// \param p_listVertices Destination list of maximal vertices.
	//----------------------------------------------------------------------------------------------
	void EnumerateMaximalVertices(
		const Meson::Common::Maths::TVector3<Real>& p_vecDirection,
		Meson::Gravitas::VectorList& p_listVertices) const;
};

Meson_Gravitas_Geometry_END
