#include "ParticleCollisionDetector.h"

#include "DebugSupport.h"
#include "Maths.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Collision;

ParticleCollisionDetector::ParticleCollisionDetector(void)
	: m_strGeometryType("Particle")
{
}

ParticleCollisionDetector::~ParticleCollisionDetector(void)
{
}

const String &ParticleCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType;
}

const String &ParticleCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType;
}

bool ParticleCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Particle", "Both geometries must be particles.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Particle", "Both geometries must be particles.")

	return p_trnRelativePlacement.Translation.LengthSquared() <= TMaths<Real>::Epsilon;
}

bool ParticleCollisionDetector::EstimateImpact(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime, Real &p_rTime)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Particle", "Both geometries must be particles.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Particle", "Both geometries must be particles.")

	const TVector3<Real>& vecOffset = p_trnRelativePlacement.Translation;

	// if moving away, no intercept (includes test for negative time)
	Real rDotPV = vecOffset * p_vecRelativeLinearVelocity;
	if (rDotPV > (Real) 0.0) return false;

	// compute time of closest encounter
	// (line eqn parameter with velocity as direction => parameter as time)
	Real rTime = -rDotPV / p_vecRelativeLinearVelocity.LengthSquared();

	// if closest encounter is beyond max time parameter, collision
	// is too far in the future
	if (rTime > p_rMaxTime) return false;

	// otherwise, closest point is within max time, but may be too distant

	// compute closest point and distance
	TVector3<Real> vecClosestPoint = vecOffset + p_vecRelativeLinearVelocity * rTime;
	Real rClosestDistance = vecClosestPoint.Length();

	// if too distant, no intercept
	if (rClosestDistance > TMaths<Real>::Epsilon)
		return false;

	// otherwise, intercept
	p_rTime = rTime;
	return true;
}

void ParticleCollisionDetector::ComputeContactManifold(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	ContactManifold &p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Particle", "Both geometries must be particles.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Particle", "Both geometries must be particles.")

	p_contactManifold.ContactPoints.Clear();

	const TVector3<Real>& vecOffset = p_trnRelativePlacement.Translation;
	TVector3<Real> vecContact = vecOffset * (Real) 0.5;
	TVector3<Real> vecNormal = vecOffset.NormaliseCopy();

	p_contactManifold.ContactPoints.Add(
		ContactPoint(vecContact, vecNormal, (Real) 0.0));
}
