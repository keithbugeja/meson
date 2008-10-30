#include "SphereCollisionDetector.h"

#include "DebugSupport.h"
#include "Maths.h"
#include "PolynomialSolver.h"

#include "Sphere.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Maths::Solvers;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Collision;

SphereCollisionDetector::SphereCollisionDetector(void)
	: m_strGeometryType("Sphere")
{
}

SphereCollisionDetector::~SphereCollisionDetector(void)
{
}

const String &SphereCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType;
}

const String &SphereCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType;
}

bool SphereCollisionDetector::TestIntersection(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Sphere", "Both geometries must be spheres.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Both geometries must be spheres.")

	const Sphere &sphere1 = (Sphere &) p_geometry1;
	const Sphere &sphere2 = (Sphere &) p_geometry2;

	Real rRadiiSum = sphere1.Radius + sphere2.Radius;

	return p_trnRelativePlacement.Translation.LengthSquared() <= rRadiiSum * rRadiiSum;
}

bool SphereCollisionDetector::EstimateImpact(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real &p_rTime)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Sphere", "Both geometries must be spheres.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Both geometries must be spheres.")

	const Sphere &sphere1 = (Sphere &) p_geometry1;
	const Sphere &sphere2 = (Sphere &) p_geometry2;

	Real rRadiiSum = sphere1.Radius + sphere2.Radius;
	Real rRadiiSumSquared = rRadiiSum * rRadiiSum;

	const TVector3<Real>& vecOffset = p_trnRelativePlacement.Translation;
	Real rOffsetSquared = vecOffset.LengthSquared();

	// simple intersection test for relatively static spheres
	if (p_vecRelativeLinearVelocity.IsZero())
	{
		p_rTime = 0.0;
		return rOffsetSquared < rRadiiSumSquared;
	}

	// calculate coefficients of quadratic solution in time
	Real rA = p_vecRelativeLinearVelocity.LengthSquared();
	Real rB = 2.0 * vecOffset * p_vecRelativeLinearVelocity;
	Real rC = rOffsetSquared - rRadiiSumSquared;

	Real rTime2 = 0.0;

	// if polynomial has no solution, no intersection possible at any time
	if (!TPolynomialSolver<Real>::SolveQuadratic(rA, rB, rC, p_rTime, rTime2))
		return false;

	if (p_rTime < 0.0)
	{
		// time entry is in the past
		if (rTime2 >= 0)
		{
			// past time of entry but time of exit is now or in future
			p_rTime = 0.0;
			return true; // in collision now
		}
		else
			// both entry and exit in the past
			return false; // no collision
	}

	if (p_rTime > p_rMaxTime)
		return false; // collision too far in the future

	// otherwise collision imminent
	return true;	
}

void SphereCollisionDetector::ComputeContactManifold(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	ContactManifold &p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Sphere", "Both geometries must be spheres.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Both geometries must be spheres.")

	Sphere &sphere1 = (Sphere &) p_geometry1;
	Sphere &sphere2 = (Sphere &) p_geometry2;

	p_contactManifold.ContactPoints.Clear();

	Real rDistance = p_trnRelativePlacement.Translation.Length();
	Real rRadiiSum = sphere1.Radius + sphere2.Radius;

	// do nothing if too far away
	if (rRadiiSum < rDistance) return;

	Real rOffset1 = TMaths<Real>::Equals(rDistance, (Real) 0.0)
		? (Real) 0.0 : sphere1.Radius / rDistance;

	Real rOffset2 = TMaths<Real>::Equals(rDistance, (Real) 0.0)
		? (Real) 1.0 : (rDistance - sphere2.Radius) / rDistance;

	Real rOffsetMean = (rOffset1 + rOffset2) * (Real) 0.5;

	const TVector3<Real>& vecOffset = p_trnRelativePlacement.Translation;
	TVector3<Real> vecContact = vecOffset * rOffsetMean;

	// handle degenerate case of coinciding sphers
	TVector3<Real> vecNormal
		= vecOffset.IsZero()
			? TVector3<Real>::Up
			: vecOffset.NormaliseCopy();

	Real rPenetration = rRadiiSum - rDistance; 

	p_contactManifold.ContactPoints.Add(
		ContactPoint(vecContact, vecNormal, rPenetration));
}
