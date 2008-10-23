
#include "GrvKineticProperties.h"

using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Kinetics;

KineticProperties::KineticProperties(void)
	: Active(true)
	, CanSleep(true)
	, LastActive((Real) 0.0)
	, Position(TPoint3<Real>::Origin)
	, Orientation(TQuaternion<Real>::Identity)
	, OrientationConjugate(TQuaternion<Real>::Identity)
	, LinearVelocity(TVector3<Real>::Zero)
	, AngularVelocity(TVector3<Real>::Zero)
	, DampingCoefficient((Real) 0.1)
{
}

KineticProperties::KineticProperties(const KineticProperties& p_kineticProperties)
	: Active(true)
	, CanSleep(p_kineticProperties.CanSleep)
	, LastActive((Real) 0.0)
	, Position(p_kineticProperties.Position)
	, Orientation(p_kineticProperties.Orientation)
	, OrientationConjugate(p_kineticProperties.Orientation.ConjugateCopy())
	, LinearVelocity(p_kineticProperties.LinearVelocity)
	, AngularVelocity(p_kineticProperties.AngularVelocity)
	, DampingCoefficient(p_kineticProperties.DampingCoefficient)
{
}

KineticProperties::~KineticProperties(void)
{
}

KineticProperties& KineticProperties::operator=(const KineticProperties &p_kineticProperties)
{
	Active = p_kineticProperties.Active;
	CanSleep = p_kineticProperties.CanSleep;
	LastActive = p_kineticProperties.LastActive;
	Position = p_kineticProperties.Position;
	Orientation = p_kineticProperties.Orientation;
	OrientationConjugate = p_kineticProperties.OrientationConjugate;
	LinearVelocity = p_kineticProperties.LinearVelocity;
	AngularVelocity = p_kineticProperties.AngularVelocity;
	DampingCoefficient = p_kineticProperties.DampingCoefficient;
	return *this;
}

KineticProperties KineticProperties::operator-(const KineticProperties &p_kineticProperties)
{
	KineticProperties kineticStateRelative;
	kineticStateRelative.Position = TPoint3<Real>::Origin
		+ p_kineticProperties.OrientationConjugate.TransformCopy(
			Position - p_kineticProperties.Position);
	kineticStateRelative.Orientation = p_kineticProperties.OrientationConjugate * Orientation;
	kineticStateRelative.OrientationConjugate = kineticStateRelative.Orientation.ConjugateCopy();
	kineticStateRelative.LinearVelocity = LinearVelocity - p_kineticProperties.LinearVelocity;
	kineticStateRelative.AngularVelocity = AngularVelocity - p_kineticProperties.AngularVelocity;
	return kineticStateRelative;
}

void KineticProperties::ComputeDerivativeProperties(void)
{
	OrientationConjugate = Orientation.ConjugateCopy();
}

Real KineticProperties::GetActivity(void)
{
	return LinearVelocity.LengthSquared() + AngularVelocity.LengthSquared();
}

void KineticProperties::TransformPointToLocal(
	const TPoint3<Real>& p_ptWorldPosition, 
	TPoint3<Real>& p_ptLocalPosition) const
{
	// if   X' = Q X Q* + V
	// then X = Q* X' Q - (Q* V Q)

	p_ptLocalPosition
		= TPoint3<Real>::Origin
			+ (OrientationConjugate * p_ptWorldPosition.ToVector() * Orientation).Vector
			- (OrientationConjugate * Position.ToVector() * Orientation).Vector;
}

void KineticProperties::TransformPointToWorld(
	const TPoint3<Real>& p_ptLocalPosition, 
	TPoint3<Real>& p_ptWorldPosition) const
{
	p_ptWorldPosition = Position + (Orientation * p_ptLocalPosition.ToVector() * OrientationConjugate).Vector;
}

void KineticProperties::ComputeLocalPointVelocity(
	const Meson::Common::Maths::TPoint3<Real>& p_ptLocalPosition,
	Meson::Common::Maths::TVector3<Real>& p_vecVelocity) const
{
	p_vecVelocity = LinearVelocity + (AngularVelocity ^ (p_ptLocalPosition.ToVector()));
}

void KineticProperties::ComputeWorldPointVelocity(
	const Meson::Common::Maths::TPoint3<Real>& p_ptWorldPosition,
	Meson::Common::Maths::TVector3<Real>& p_vecVelocity) const
{
	p_vecVelocity = LinearVelocity + (AngularVelocity ^ (p_ptWorldPosition - Position));
}
