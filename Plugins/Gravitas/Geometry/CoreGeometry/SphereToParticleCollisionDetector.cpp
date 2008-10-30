#include "SphereToParticleCollisionDetector.h"

#include "DebugSupport.h"
#include "Maths.h"
#include "PolynomialSolver.h"

#include "Sphere.h"
#include "Particle.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Maths::Solvers;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Collision;

SphereToParticleCollisionDetector::SphereToParticleCollisionDetector(void)
	: m_strGeometryType1("Sphere")
	, m_strGeometryType2("Particle")
{
}

SphereToParticleCollisionDetector::~SphereToParticleCollisionDetector(void)
{
}

const String& SphereToParticleCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& SphereToParticleCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool SphereToParticleCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1, const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Sphere", "The first volume must be a sphere.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Particle", "The second volume must be a particle.")

	Sphere* pSphere = (Sphere *) &p_geometry1;

	return p_trnRelativePlacement.Translation.LengthSquared()
		<= pSphere->RadiusSquared;
}

bool SphereToParticleCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1, const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Sphere", "The first volume must be a sphere.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Particle", "The second volume must be a particle.")

	TVector3<Real> vecOffset = p_trnRelativePlacement.Translation;
	TVector3<Real> vecVelocity = p_vecRelativeLinearVelocity;
	Sphere* pSphere = (Sphere *) &p_geometry1;

	Real rRadiiSumSquared = pSphere->RadiusSquared + TMaths<Real>::Epsilon;

	Real rOffsetSquared = vecOffset.LengthSquared();

	// simple intersection test for relatively static spheres
	if (vecVelocity.IsZero())
	{
		p_rTime = 0.0;
		return rOffsetSquared < rRadiiSumSquared;
	}

	// calculate coefficients of quadratic solution in time
	Real rA = vecVelocity.LengthSquared();
	Real rB = 2.0 * vecOffset * vecVelocity;
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

void SphereToParticleCollisionDetector::ComputeContactManifold(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	ContactManifold &p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Sphere", "The first volume must be a sphere.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Particle", "The second volume must be a particle.")

	Sphere* pSphere = (Sphere *) &p_geometry1;

	p_contactManifold.ContactPoints.Clear();

	// no contacts if too far away
	if (pSphere->RadiusSquared + TMaths<Real>::Epsilon
		< p_trnRelativePlacement.Translation.LengthSquared())
		return;

	if (p_trnRelativePlacement.Translation.LengthSquared()
		> pSphere->RadiusSquared + TMaths<Real>::Epsilon)
		return;

	TVector3<Real> vecNormal = p_trnRelativePlacement.Translation.NormaliseCopy();

	TVector3<Real> vecContact = vecNormal * pSphere->Radius;

	Real rPenetration = pSphere->Radius
		- p_trnRelativePlacement.Translation.Length();

	p_contactManifold.ContactPoints.Add(
		ContactPoint(vecContact, vecNormal, rPenetration));
}
