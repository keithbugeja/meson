
#include "GrvKineticProperties.h"

using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Kinetics;

KineticProperties::KineticProperties(void)
	: Active(true)
	, CanSleep(true)
	, LastActive((Real) 0.0)
	, Position(TVector3<Real>::Zero)
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
	kineticStateRelative.Position = p_kineticProperties.OrientationConjugate.TransformCopy(
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
	const TVector3<Real>& p_vecWorldPosition, 
	TVector3<Real>& p_vecLocalPosition) const
{
	// if   X' = Q X Q* + V
	// then X = Q* X' Q - (Q* V Q)

	p_vecLocalPosition
		= (OrientationConjugate * p_vecWorldPosition * Orientation).Vector
			- (OrientationConjugate * Position * Orientation).Vector;
}

void KineticProperties::TransformPointToWorld(
	const TVector3<Real>& p_vecLocalPosition, 
	TVector3<Real>& p_vecWorldPosition) const
{
	p_vecWorldPosition = Position + (Orientation * p_vecLocalPosition * OrientationConjugate).Vector;
}

void KineticProperties::ComputeLocalPointVelocity(
	const TVector3<Real>& p_vecLocalPosition,
	TVector3<Real>& p_vecVelocity) const
{
	p_vecVelocity = LinearVelocity + (AngularVelocity ^ p_vecLocalPosition);
}

void KineticProperties::ComputeWorldPointVelocity(
	const Meson::Common::Maths::TVector3<Real>& p_vecWorldPosition,
	Meson::Common::Maths::TVector3<Real>& p_vecVelocity) const
{
	p_vecVelocity = LinearVelocity + (AngularVelocity ^ (p_vecWorldPosition - Position));
}
