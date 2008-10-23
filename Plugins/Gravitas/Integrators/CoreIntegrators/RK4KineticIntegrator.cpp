#include "RK4KineticIntegrator.h"

#include "Definitions.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Kinetics;

const String g_strRK4IntegratorName = "RK4";

RK4KineticIntegrator::RK4KineticIntegrator(void)
{
}

RK4KineticIntegrator::~RK4KineticIntegrator(void)
{
}

const String& RK4KineticIntegrator::GetName(void) const
{
	return g_strRK4IntegratorName;
}

void RK4KineticIntegrator::IntegrateVelocity(
	Real p_rCurrentTime, Real p_rDeltaTime,
	IVectorTimeFunction& p_linearAcceleration,
	IVectorTimeFunction& p_angularAcceleration,
	KineticProperties& p_kineticProperties)
{
	const Real FOUR = (Real) 4.0;
	Real rDeltaTimeHalf = p_rDeltaTime * (Real) 0.5;
	Real rDeltaTimeSixth = p_rDeltaTime / (Real) 6.0;

	TVector3<Real> vecK1, vecK2, vecK3;
	TVector3<Real> vecLinearAccelerationHalfTime
		= p_linearAcceleration(p_rCurrentTime + rDeltaTimeHalf);
	TVector3<Real> vecLinearAccelerationFullTime
		= p_linearAcceleration(p_rCurrentTime + p_rDeltaTime);
	TVector3<Real> vecAngularAccelerationHalfTime
		= p_angularAcceleration(p_rCurrentTime + rDeltaTimeHalf);
	TVector3<Real> vecAngularAccelerationFullTime
		= p_angularAcceleration(p_rCurrentTime + p_rDeltaTime);

	// linear velocity
	vecK1 = p_linearAcceleration(p_rCurrentTime);
	vecK2 = vecLinearAccelerationHalfTime;
	vecK3 = vecLinearAccelerationFullTime;
	p_kineticProperties.LinearVelocity
		+= rDeltaTimeSixth * (vecK1 + vecK2 * FOUR + vecK3);

	// angular velocity
	vecK1 = p_angularAcceleration(p_rCurrentTime);
	vecK2 = vecAngularAccelerationHalfTime;
	vecK3 = vecAngularAccelerationFullTime;
	p_kineticProperties.AngularVelocity
		+= rDeltaTimeSixth * (vecK1 + vecK2 * FOUR + vecK3);

	// compute damping factor
	Real rDampingFactor(TMaths<Real>::Exp(-p_kineticProperties.DampingCoefficient * p_rDeltaTime));
	// apply damping
	p_kineticProperties.LinearVelocity *= rDampingFactor;
	p_kineticProperties.AngularVelocity *= rDampingFactor;
}


void RK4KineticIntegrator::IntegratePosition(
	Real p_rCurrentTime, Real p_rDeltaTime,
	IVectorTimeFunction& p_linearAcceleration,
	IVectorTimeFunction& p_angularAcceleration,
	KineticProperties& p_kineticProperties)
{
	const Real FOUR = (Real) 4.0;
	Real rDeltaTimeHalf = p_rDeltaTime * (Real) 0.5;
	Real rDeltaTimeSixth = p_rDeltaTime / (Real) 6.0;

	TVector3<Real> vecK1, vecK2, vecK3;
	TVector3<Real> vecLinearAccelerationHalfTime
		= p_linearAcceleration(p_rCurrentTime + rDeltaTimeHalf);
	TVector3<Real> vecLinearAccelerationFullTime
		= p_linearAcceleration(p_rCurrentTime + p_rDeltaTime);
	TVector3<Real> vecAngularAccelerationHalfTime
		= p_angularAcceleration(p_rCurrentTime + rDeltaTimeHalf);
	TVector3<Real> vecAngularAccelerationFullTime
		= p_angularAcceleration(p_rCurrentTime + p_rDeltaTime);

	// position
	vecK1 = p_kineticProperties.LinearVelocity;
	vecK2 = p_kineticProperties.LinearVelocity
		+ vecLinearAccelerationHalfTime * rDeltaTimeHalf;
	vecK3 = p_kineticProperties.LinearVelocity
		+ vecLinearAccelerationFullTime * p_rDeltaTime;
	p_kineticProperties.Position
		+= rDeltaTimeSixth * (vecK1 + vecK2 * FOUR + vecK3);

	// orientation 
	vecK1 = p_kineticProperties.AngularVelocity;
	vecK2 = p_kineticProperties.AngularVelocity
		+ vecAngularAccelerationHalfTime * rDeltaTimeHalf;
	vecK3 = p_kineticProperties.AngularVelocity
		+ vecAngularAccelerationFullTime * p_rDeltaTime;
	TVector3<Real> vecOrientationDelta
		= rDeltaTimeSixth * (vecK1 + vecK2 * FOUR + vecK3);
	// d(q) = 0.5 w q
	TQuaternion<Real> qtnOrientationDelta
		= TQuaternion<Real>(vecOrientationDelta) * p_kineticProperties.Orientation * (Real) 0.5;
	p_kineticProperties.Orientation += qtnOrientationDelta;
	p_kineticProperties.Orientation.Normalise();
	p_kineticProperties.OrientationConjugate = p_kineticProperties.Orientation.ConjugateCopy();
}
