#include "GrvContactPoint.h"

#include "Maths.h"
#include "RandomGeneric.h"

using namespace Meson::Common::Maths;
using namespace Meson::Common::Random;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;

ContactPoint::ContactPoint(void)
	: Position(TPoint3<Real>::Origin)
	, Normal(TVector3<Real>::Right)
	, Penetration((Real) 0.0)
	, RelativeVelocity(TVector3<Real>::Zero)
{
}

ContactPoint::ContactPoint(const ContactPoint& p_contactPoint)
	: Position(p_contactPoint.Position)
	, Normal(p_contactPoint.Normal)
	, Penetration(p_contactPoint.Penetration)
	, RelativeVelocity(p_contactPoint.RelativeVelocity)
{
}

ContactPoint::ContactPoint(
	const TPoint3<Real>& p_ptPosition,
	const TVector3<Real>& p_vecNormal,
	Real p_rPenetration)
	: Position(p_ptPosition)
	, Normal(p_vecNormal)
	, Penetration(p_rPenetration)
	, RelativeVelocity(TVector3<Real>::Zero)
{
}

ContactPoint::~ContactPoint(void)
{
}

bool ContactPoint::IsColliding(void) const
{
	return RelativeVelocity * Normal < -TMaths<Real>::Epsilon;
}

bool ContactPoint::IsInContact(void) const
{
	return TMaths<Real>::Equals(RelativeVelocity * Normal, (Real) 0.0);
}

bool ContactPoint::IsSeparating(void) const
{
	return RelativeVelocity * Normal > TMaths<Real>::Epsilon;
}

bool ContactPoint::IsResting(void) const
{
	return RelativeVelocity.IsZero();
}

bool ContactPoint::IsSliding(void) const
{
	TVector3<Real> vecRelativeTangentVelocity(
		RelativeVelocity - (RelativeVelocity * Normal) * Normal);
	return !vecRelativeTangentVelocity.IsZero();
}

void ContactPoint::Transform(const Meson::Gravitas::Geometry::Transform& p_transform)
{
	p_transform.Apply(Position);
	p_transform.Rotation.Transform(Normal);
	p_transform.Rotation.Transform(RelativeVelocity);
}

void ContactPoint::PerturbNormal(Real p_rCoefficient)
{
	static RandomGeneric s_randomGeneric;
	Real rHalfCoefficient = p_rCoefficient * (Real) 0.5;
	Normal.X += s_randomGeneric.NextDouble() * p_rCoefficient - rHalfCoefficient;
	Normal.Y += s_randomGeneric.NextDouble() * p_rCoefficient - rHalfCoefficient;
	Normal.Z += s_randomGeneric.NextDouble() * p_rCoefficient - rHalfCoefficient;
	Normal.Normalise();
}


ContactPoint& ContactPoint::operator=(const ContactPoint& p_contactPoint)
{
	Position = p_contactPoint.Position;
	Normal = p_contactPoint.Normal;
	Penetration = p_contactPoint.Penetration;
	RelativeVelocity = p_contactPoint.RelativeVelocity;
	return *this;
}

bool ContactPoint::operator==(const ContactPoint& p_contactPoint) const
{
	if (Position != p_contactPoint.Position) return false;
	if (Normal != p_contactPoint.Normal) return false;
	if (!TMaths<Real>::Equals(Penetration, p_contactPoint.Penetration))
		return false;
	return true;
}

bool ContactPoint::operator!=(const ContactPoint& p_contactPoint) const
{
	if (Position != p_contactPoint.Position) return true;
	if (Normal != p_contactPoint.Normal) return true;
	if (!TMaths<Real>::Equals(Penetration, p_contactPoint.Penetration))
		return true;
	return false;
}
