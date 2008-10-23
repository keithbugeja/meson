#pragma once

#include "Vector3.h"

#include "GrvNamespaces.h"
#include "GrvGravitasEntity.h"
#include "GrvGeometry.h"
#include "GrvTransform.h"
#include "GrvContactManifold.h"

Meson_Gravitas_Collision_BEGIN

//----------------------------------------------------------------------------------------------
/// \class ICollisionDetector
/// An abstract interface of a collision detection algorithm for two specific geometry
/// implementations. The collision manager maintains a mapping of geometry type pairs against
/// specific collision detector implementations, allowing for specialised algorithms for each
/// given type pair. Typical examples of specialised algorithms are sphere-sphere tests and
/// box-cylinder tests. The interface supports interference detection tests (overlaps),
/// collision time estimation tests and contact manifold computation. The collision algorithm
/// implemented by the interface is assumed to operate on geometry implementations given in
/// one explicit order as defined by the GetGeometryType1() and GetGeometryType2() methods.
/// For any two geometry implementations, only one collision detector implementation is required
/// as the collision manager is able to automatically swap geometry to match the algorithm.
//----------------------------------------------------------------------------------------------
class ICollisionDetector
	: public Meson::Gravitas::IGravitasEntity
{
public:
	virtual ~ICollisionDetector(void) {};

	//----------------------------------------------------------------------------------------------
	/// Returns the type name of the first geometry implementation assumed by the collision
	/// detector.
	/// \returns the type name of the first geometry implementation assumed by the collision
	/// detector.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetGeometryType1(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns the type name of the second geometry implementation assumed by the collision
	/// detector.
	/// \returns the type name of the second geometry implementation assumed by the collision
	/// detector.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetGeometryType2(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given geometry instances overlap, assuming the given relative
	/// placement of the first geometry with respect to the second geometry.
	/// \param p_geometry1 First geometry instance sujected to the test.
	/// \param p_geometry2 Second geometry instance sujected to the test.
	/// \param p_trnRelativePlacement A transform defining the relative position and orientation of
	///        the second geometry with respect to the first geometry.
	/// \returns true if the given geometry instances overlap.
	//----------------------------------------------------------------------------------------------
	virtual bool TestIntersection(
		const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
		const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
		const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement) = 0;

	//----------------------------------------------------------------------------------------------
	/// Performs a collision estimation test for the given geometry instances, assuming the given
	/// relative placement and the relative linear and angular velocity. The test succeeds if
	/// a collision is estimated within the given time upperbound, in which case, the given time
	/// reference is updated with the estimated time of collision.
	/// \param p_geometry1 First geometry instance sujected to the test.
	/// \param p_geometry2 Second geometry instance sujected to the test.
	/// \param p_trnRelativePlacement A transform defining the relative position and orientation of
	///        the second geometry with respect to the first geometry.
	/// \param p_vecRelativeLinearVelocity relative linear velocity of the second geometry instance
	///        with respect to the first instance.
	/// \param p_vecRelativeAngularVelocity relative angular velocity of the second geometry
	///        instance with respect to the first instance.
	/// \param p_rMaxTime relative time upperbound for the test.
	/// \param p_rTime relative time of estimated collision if test is successful.
	/// \returns true if the geometry instances are estimated to collide within the given time
	///          upperbound, or false otherwise.
	//----------------------------------------------------------------------------------------------
	virtual bool EstimateImpact(
		const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
		const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
		const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
		const Meson::Common::Maths::TVector3<Real>& p_vecRelativeLinearVelocity,
		const Meson::Common::Maths::TVector3<Real>& p_vecRelativeAngularVelocity,
		Real p_rMaxTime,
		Real& p_rTime) = 0;

	//----------------------------------------------------------------------------------------------
	/// Computes a contact manifold for the given geometry instances, assuming the given relative
	/// placement of the second geometry instance with respect to the first. The contact manifold
	/// consists of representative contact points, each characterised by a point of contact, a
	/// collision normal and a penetration depth. The method is responsible for resetting the
	/// manifold structure prior to population due to potential reuse within the simulator.
	/// \param p_geometry1 First geometry instance sujected to the test.
	/// \param p_geometry2 Second geometry instance sujected to the test.
	/// \param p_trnRelativePlacement A transform defining the relative position and orientation of
	///        the second geometry with respect to the first geometry.
	/// \param p_contactManifold Contact manifold structure to be populated by the manifold
	///        computation method.
	//----------------------------------------------------------------------------------------------
	virtual void ComputeContactManifold(
		const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
		const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
		const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
		ContactManifold& p_contactManifold) = 0;
};

Meson_Gravitas_Collision_END
