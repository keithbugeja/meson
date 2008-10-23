#pragma once

#include "Point3.h"
#include "Vector3.h"
#include "ArrayList.h"
#include "LinkedList.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvTransform.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct Tetrahedron
/// Representation of a tetrahedron. Provides a number of services for geometrical queries,
/// distance queries, containment tests and transformations.
//----------------------------------------------------------------------------------------------
struct Tetrahedron
{
	// variables
	//----------------------------------------------------------------------------------------------
	/// Vertices of the tetrahedron.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> Vertices[4];

	// methods

	//----------------------------------------------------------------------------------------------
	/// Constructs a tetrahedron with all vertices at the origin.
	//----------------------------------------------------------------------------------------------
	Tetrahedron(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a tetrahedron using the given source tetrahedron.
	/// \param p_tetrahedron Source tetrahedron to copy.
	//----------------------------------------------------------------------------------------------
	Tetrahedron(const Tetrahedron& p_tetrahedron);

	//----------------------------------------------------------------------------------------------
	/// Constructs a tetrahedron using the given vertices.
	/// \param p_ptVertex0 First vertex for the tetrahedron.
	/// \param p_ptVertex1 Second vertex for the tetrahedron.
	/// \param p_ptVertex2 Third vertex for the tetrahedron.
	/// \param p_ptVertex3 Fourth vertex for the tetrahedron.
	//----------------------------------------------------------------------------------------------
	Tetrahedron(
		const Meson::Common::Maths::TPoint3<Real>& p_ptVertex0,
		const Meson::Common::Maths::TPoint3<Real>& p_ptVertex1,
		const Meson::Common::Maths::TPoint3<Real>& p_ptVertex2,
		const Meson::Common::Maths::TPoint3<Real>& p_ptVertex3);

	//----------------------------------------------------------------------------------------------
	/// Returns the volume of the tetrahedron.
	/// \returns the volume of the tetrahedron.
	//----------------------------------------------------------------------------------------------
	Real Volume(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the tetrahedron centroid.
	/// \returns the tetrahedron centroid.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> Centroid(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the distance of the given point to the tetrahedron.
	/// \param p_ptPoint Point to test.
	/// \returns the distance of the given point to the tetrahedron.
	//----------------------------------------------------------------------------------------------
	Real DistanceFromPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the distance of the given point to the tetrahedron and computes the closest point
	/// on the tetrahedron.
	/// \param p_ptPoint Point to test.
	/// \param p_ptClosestPoint Computed point on the tetrahedron, closest to the test point.
	/// \returns the distance of the given point to the tetrahedron.
	//----------------------------------------------------------------------------------------------
	Real DistanceFromPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint,
		Meson::Common::Maths::TPoint3<Real>& p_ptClosestPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns the closest point on the tetrahedron to the given point.
	/// \param p_ptPoint Point to test.
	/// \returns the closest point on the tetrahedron to the given point.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TPoint3<Real> ClosestPoint(
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given point is contained in the tetrahedron.
	/// \param p_ptPoint Point to test for containment.
	/// \returns true if the given point is contained in the tetrahedron.
	//----------------------------------------------------------------------------------------------
	bool Contains(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Transforms the tetrahedron using the given transform object.
	/// \param p_transform Transform object used to apply the transformation.
	//----------------------------------------------------------------------------------------------
	void Transform(const Meson::Gravitas::Geometry::Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Returns a transformed copy of the tetrahedron using the given transform object.
	/// \param p_transform Transform object used to apply the transformation.
	/// \returns a transformed copy of the tetrahedron using the given transform object.
	//----------------------------------------------------------------------------------------------
	Tetrahedron TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform) const;

	//----------------------------------------------------------------------------------------------
	/// Assigns the given tetrahedron to this tetrahedron.
	/// \param p_tetrahedron Source tetrahedron to copy.
	/// \returns a reference to this tetrahedron.
	//----------------------------------------------------------------------------------------------
	Tetrahedron& operator=(const Tetrahedron& p_tetrahedron);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given tetrahedron is equal to this tetrahedron.
	/// \param p_tetrahedron The other tetrahedron to compare.
	/// \returns true if the given tetrahedron is equal to this tetrahedron.
	//----------------------------------------------------------------------------------------------
	bool operator==(const Tetrahedron& p_tetrahedron) const;
};

//----------------------------------------------------------------------------------------------
/// Abstract list of tetrahedrons.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList<Tetrahedron> TetrahedronList;

//----------------------------------------------------------------------------------------------
/// Array list of tetrahedrons.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList<Tetrahedron> TetrahedronArrayList;

//----------------------------------------------------------------------------------------------
/// Linked list of tetrahedrons.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList<Tetrahedron> TetrahedronLinkedList;

Meson_Gravitas_Geometry_END
