#pragma once

#include "Point3.h"
#include "Vector3.h"
#include "Interval.h"
#include "ArrayList.h"
#include "LinkedList.h"

#include "GrvNamespaces.h"
#include "GrvRay.h"
#include "GrvTypes.h"
#include "GrvTransform.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct Triangle
/// Representation of a triangle. Provides a number of services for geometrical queries,
/// distance queries, containment tests and transformations.
//----------------------------------------------------------------------------------------------
struct Triangle
{
	// variables
	Meson::Common::Maths::TPoint3<Real> Vertices[3];

	// methods

	//----------------------------------------------------------------------------------------------
	/// Constructs a collapsed triangle with its vertices at the origin.
	//----------------------------------------------------------------------------------------------
	Triangle(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a triangle from the given source.
	/// \param p_triangle Source triangle to copy.
	//----------------------------------------------------------------------------------------------
	Triangle(const Triangle& p_triangle);

	//----------------------------------------------------------------------------------------------
	/// Constructs a triangle using the given vertices.
	/// \param p_ptVertex0 First vertex for the triangle.
	/// \param p_ptVertex1 Second vertex for the triangle.
	/// \param p_ptVertex2 Third vertex for the triangle.
	//----------------------------------------------------------------------------------------------
	Triangle(
		const Meson::Common::Maths::TPoint3<Real>& p_ptVertex0,
		const Meson::Common::Maths::TPoint3<Real>& p_ptVertex1,
		const Meson::Common::Maths::TPoint3<Real>& p_ptVertex2);

	//----------------------------------------------------------------------------------------------
	/// Returns the area of the triangle.
	/// \returns the area of the triangle.
	//----------------------------------------------------------------------------------------------
	Real Area(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the trangle's normal vector. The normal is computed using a right-handed coordinate
	/// system.
	/// \returns the trangle's normal vector.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Normal(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the centroid of the triangle.
	/// \returns the centroid of the triangle.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> Centroid(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns a vector offset representing an edge with the given index. The edge offset is
	/// computed by subtracting the (i+1)th vertex from the (i)th vertex where (i) is the index and
	/// (i + 1) is computed modulus 3.
	/// \param p_unIndex Edge index to compute.
	/// \returns a vector offset representing an edge with the given index
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Edge(size_t p_unIndex) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the distance between the closest point on the triangle and the given point.
	/// \param p_ptPoint Point to test.
	/// \returns the distance between the closest point on the triangle and the given point.
	//----------------------------------------------------------------------------------------------
	Real DistanceFromPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the distance between the closest point on the triangle and the given point. The
	/// closest point is stored in the given parameter.
	/// \param p_ptPoint Point to test.
	/// \param p_ptClosestPoint Computed closest point on the triangle.
	/// \returns the distance between the closest point on the triangle and the given point.
	//----------------------------------------------------------------------------------------------
	Real DistanceFromPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint,
		Meson::Common::Maths::TPoint3<Real>& p_ptClosestPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the signed distance between the plane occupied by the triangle and the given point.
	/// The computed distance is positive if the test point is above the plane as given by the
	/// triangle normal, zero if coplanar, or negative if the point is below the plane.
	/// \param p_ptPoint Point to test.
	/// \returns the signed distance between the plane occupied by the triangle and the given point.
	//----------------------------------------------------------------------------------------------
	Real SignedDistanceFromPlane(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the closest point on the triangle to the given test point.
	/// \param p_ptPoint Point to test.
	/// \returns the closest point on the triangle to the given test point.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> ClosestPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given point is contained in the triangle.
	/// \param p_ptPoint Point to test.
	/// \returns true if the given point is contained in the triangle.
	//----------------------------------------------------------------------------------------------
	bool Contains(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given ray intersects the triangle.
	/// \param p_ray Ray to test for intersection.
	/// \returns true if the given ray intersects the triangle.
	//----------------------------------------------------------------------------------------------
	bool Intersects(const Ray& p_ray) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given ray intersects the triangle.
	/// \param p_ray Ray to test for intersection.
	/// \param p_ptIntersectionPoint Computed point of intersection on the triangle.
	/// \returns true if the given ray intersects the triangle.
	//----------------------------------------------------------------------------------------------
	bool Intersects(const Ray& p_ray,
		const Meson::Common::Maths::TPoint3<Real>& p_ptIntersectionPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given triangle intersects this triangle.
	/// \param p_trisngle Triangle to test for intersection.
	/// \returns true if the given triangle intersects this triangle.
	//----------------------------------------------------------------------------------------------
	bool Intersects(const Triangle& p_trisngle) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given point lies on the plane containing the triangle. This is
	/// equivalent to testing if the signed plane distance is zero.
	/// \param p_ptPoint Point to test.
	/// \returns true if the given point lies on the plane containing the triangle.
	//----------------------------------------------------------------------------------------------
	bool PointOnPlane(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given point lies above the plane containing the triangle. This is
	/// equivalent to testing if the signed plane distance is positive.
	/// \param p_ptPoint Point to test.
	/// \returns true if the given point lies above the plane containing the triangle.
	//----------------------------------------------------------------------------------------------
	bool PointAbovePlane(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given point lies below the plane containing the triangle. This is
	/// equivalent to testing if the signed plane distance is negative.
	/// \param p_ptPoint Point to test.
	/// \returns true if the given point lies below the plane containing the triangle.
	//----------------------------------------------------------------------------------------------
	bool PointBelowPlane(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true of the plane projection of the given point lies within the triangle.
	/// \param p_ptPoint Point to test.
	/// \returns true of the plane projection of the given point lies within the triangle.
	//----------------------------------------------------------------------------------------------
	bool ContainsPointProjection(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Projects the triangle on to the given interval using the given axis.
	/// \param p_vecAxis Axis of projection given as a vector.
	/// \param p_interval Interval of projection.
	//----------------------------------------------------------------------------------------------
	void ProjectToInterval(Meson::Common::Maths::TVector3<Real> p_vecAxis,
		Meson::Common::Maths::TInterval<Real>& p_interval) const;

	//----------------------------------------------------------------------------------------------
	/// Enumerates the maximal vertices along the given direction and stores them in the given
	/// vertex list. The list may contain two vertices in case of a maximal edge perpendicular to
	/// the direction vector, or in case of the triangle normal is parallel to the direction vector.
	/// \param p_vecDirection Direction vector to use for finding the maximal vertices.
	/// \param p_listVertices List of vertices in which to store the maximal vertices.
	//----------------------------------------------------------------------------------------------
	void EnumerateMaximalVertices(
		const Meson::Common::Maths::TVector3<Real>& p_vecDirection,
		Meson::Gravitas::PointList& p_listVertices) const;

	//----------------------------------------------------------------------------------------------
	/// Inverts the vertex winding of the triangle.
	//----------------------------------------------------------------------------------------------
	void Invert(void);

	//----------------------------------------------------------------------------------------------
	/// Transforms the triangle using the given transform object.
	/// \param p_transform Transform object to apply to the triangle.
	//----------------------------------------------------------------------------------------------
	void Transform(const Meson::Gravitas::Geometry::Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Returns a transformed copy of the triangle using the given transform object.
	/// \param p_transform Transform object to apply to the triangle copy.
	/// \returns a transformed copy of the triangle using the given transform object.
	//----------------------------------------------------------------------------------------------
	Triangle TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform) const;

	//----------------------------------------------------------------------------------------------
	/// Assigns the given triangle to this triangle.
	/// \param p_triangle Source triangle to copy.
	/// \returns a reference to this triangle.
	//----------------------------------------------------------------------------------------------
	Triangle& operator=(const Triangle& p_triangle);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given triangle matches this triangle. The vertices of both triangles
	/// must be given in the same order.
	/// \param p_triangle Triangle to compare.
	/// \returns true if the given triangle matches this triangle.
	//----------------------------------------------------------------------------------------------
	bool operator==(const Triangle& p_triangle) const;
};

//----------------------------------------------------------------------------------------------
/// Abstract list of triangles.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList<Triangle> TriangleList;

//----------------------------------------------------------------------------------------------
/// Array list of triangles.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList<Triangle> TriangleArrayList;

//----------------------------------------------------------------------------------------------
/// Linked list of triangles.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList<Triangle> TriangleLinkedList;

Meson_Gravitas_Geometry_END
