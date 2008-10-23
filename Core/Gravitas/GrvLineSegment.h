#pragma once

#include "Point3.h"
#include "Vector3.h"
#include "List.h"
#include "ArrayList.h"
#include "LinkedList.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvTransform.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct LineSegment
/// Representation of a line segment. Provides a number of services for determining length,
/// direction, midpoint and supports a number of distance queries.
//----------------------------------------------------------------------------------------------
struct LineSegment
{
	//----------------------------------------------------------------------------------------------
	/// Starting point of the line segment.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> Start;

	//----------------------------------------------------------------------------------------------
	/// Ending point of the line segment.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> End;

	//----------------------------------------------------------------------------------------------
	/// Constructs a zero-length line segment positioned at the origin.
	//----------------------------------------------------------------------------------------------
	LineSegment(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a line segment using the given source line segment.
	/// \param p_lineSegment Line segment to copy.
	//----------------------------------------------------------------------------------------------
	LineSegment(const LineSegment& p_lineSegment);

	//----------------------------------------------------------------------------------------------
	/// Constructs a line segment using the given start and end points.
	/// \param p_ptStart Starting point for the line segment.
	/// \param p_ptEnd Ending point for the line segment.
	//----------------------------------------------------------------------------------------------
	LineSegment(
		const Meson::Common::Maths::TPoint3<Real>& p_ptStart,
		const Meson::Common::Maths::TPoint3<Real>& p_ptEnd);

	//----------------------------------------------------------------------------------------------
	/// Returns the length of the line segment.
	/// \returns the length of the line segment.
	//----------------------------------------------------------------------------------------------
	Real Length(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the squared length of the line segment.
	/// \returns the squared length of the line segment.
	//----------------------------------------------------------------------------------------------
	Real LengthSquared(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the vector difference between the ending and starting point of the line segment.
	/// \returns the vector difference between the ending and starting point of the line segment.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Offset(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns a unit vector giving the direction of the line segment.
	/// \returns a unit vector giving the direction of the line segment.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Direction(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the midpoint of the line segment.
	/// \returns the midpoint of the line segment.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> Midpoint(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the distance of the given point to the line segment.
	/// \param p_ptPoint Point to test.
	/// \returns the distance of the given point to the line segment.
	//----------------------------------------------------------------------------------------------
	Real DistanceFromPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the distance of the given point to the line segment and computes the closest point
	/// on the segment.
	/// \param p_ptPoint Point to test.
	/// \param p_ptClosestPoint Computed point on the line segment, closest to the test point.
	/// \returns the distance of the given point to the line segment.
	//----------------------------------------------------------------------------------------------
	Real DistanceFromPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint,
		Meson::Common::Maths::TPoint3<Real>& p_ptClosestPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the closest point on the line segment to the given point.
	/// \param p_ptPoint Point to test.
	/// \returns the closest point on the line segment to the given point.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> ClosestPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Computes the closest point on this line segment to the given segment, and conversely, the
	/// closest point on the given segment to this segment.
	/// \param p_lineSegment Line segment to 
	/// \param p_ptPoint1 Computed point on this segment, closest to the given segment.
	/// \param p_ptPoint2 Computed point on the given segment, closest to this segment.
	//----------------------------------------------------------------------------------------------
	void ClosestPointsToSegment(
		const LineSegment& p_lineSegment,
		Meson::Common::Maths::TPoint3<Real>& p_ptPoint1, 
		Meson::Common::Maths::TPoint3<Real>& p_ptPoint2);

	//----------------------------------------------------------------------------------------------
	/// Swaps the values of the starting and ending point.
	//----------------------------------------------------------------------------------------------
	void Invert(void);

	//----------------------------------------------------------------------------------------------
	/// Transforms the line segment using the given transform object.
	/// \param p_transform Transform object used to apply the transformation.
	//----------------------------------------------------------------------------------------------
	void Transform(const Meson::Gravitas::Geometry::Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Returns a transformed copy of the line segment using the given transform object.
	/// \param p_transform Transform object used to apply the transformation.
	/// \returns a transformed copy of the line segment using the given transform object.
	//----------------------------------------------------------------------------------------------
	LineSegment TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Assigns the given line segment to this segment.
	/// \param p_lineSegment Source line segment to copy.
	/// \returns a reference to this line segment.
	//----------------------------------------------------------------------------------------------
	LineSegment& operator=(const LineSegment& p_lineSegment);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given line segment is equal to this segment.
	/// \param p_lineSegment Line segment to compare.
	/// \returns true if the given line segment is equal to this segment.
	//----------------------------------------------------------------------------------------------
	bool operator==(const LineSegment& p_lineSegment) const;
};

//----------------------------------------------------------------------------------------------
/// Abstract list of line segments.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList<LineSegment> LineSegmentList;

//----------------------------------------------------------------------------------------------
/// Array list of line segments.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList<LineSegment> LineSegmentArrayList;

//----------------------------------------------------------------------------------------------
/// Linked list of line segments.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList<LineSegment> LineSegmentLinkedList;

Meson_Gravitas_Geometry_END
