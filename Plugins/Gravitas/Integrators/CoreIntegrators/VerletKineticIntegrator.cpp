#include "VerletKineticIntegrator.h"

#include "Definitions.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Kinetics;

const String g_strVerletIntegratorName = "Verlet";

VerletKineticIntegrator::VerletKineticIntegrator(void)
	: m_unSubdivisions(1)
	, m_rSubdivisionsInverse(1.0)
{
}

VerletKineticIntegrator::~VerletKineticIntegrator(void)
{
}

void VerletKineticIntegrator::EnumerateProperties(
	TMap<String, PropertyDescriptor>&
		p_mapProperties)
{
	p_mapProperties.Clear();
	p_mapProperties["Subdivisions"]
		= PropertyDescriptor("Subdivisions", PropertyType::Integer, false);
}

void VerletKineticIntegrator::GetProperty(const String& p_strName, int &p_nValue)
{
	if (p_strName == "Subdivisions")
		p_nValue = (int) m_unSubdivisions;
	else
		MESON_ASSERT(false, "Verlet Kinetic Integrator: Invalid property specified.")
}

void VerletKineticIntegrator::SetProperty(const String& p_strName, int p_nValue)
{
	if (p_strName == "Subdivisions")
	{
		m_unSubdivisions = (size_t) MAX(1, p_nValue);
		m_rSubdivisionsInverse = (Real) 1.0 / (Real) m_unSubdivisions;
	}
	else
		MESON_ASSERT(false, "Verlet Kinetic Integrator: Invalid property specified.")
}

const String& VerletKineticIntegrator::GetName(void) const
{
	return g_strVerletIntegratorName;
}

void VerletKineticIntegrator::IntegrateVelocity(
	Real p_rCurrentTime, Real p_rDeltaTime,
	IVectorTimeFunction& p_linearAcceleration,
	IVectorTimeFunction& p_angularAcceleration,
	KineticProperties& p_kineticProperties)
{
	Real rDeltaSubtime = p_rDeltaTime * m_rSubdivisionsInverse;
	const Real HALF = (Real) 0.5;

	// compute damping factor
	Real rDampingFactor(TMaths<Real>::Exp(-p_kineticProperties.DampingCoefficient * rDeltaSubtime));

	// integrate across subdivisions
	for (size_t unIndex = 0; unIndex < m_unSubdivisions; unIndex++)
	{
		// keep copies of accelerations
		TVector3<Real> vecLinearAcceleration = p_linearAcceleration(p_rCurrentTime);
		TVector3<Real> vecAngularAcceleration = p_angularAcceleration(p_rCurrentTime);

		// linear velocity
		p_kineticProperties.LinearVelocity += vecLinearAcceleration * rDeltaSubtime;

		// angular velocity
		p_kineticProperties.AngularVelocity += vecAngularAcceleration * rDeltaSubtime;

		// apply damping
		p_kineticProperties.LinearVelocity *= rDampingFactor;
		p_kineticProperties.AngularVelocity *= rDampingFactor;

		// track time per subdivision
		p_rCurrentTime += rDeltaSubtime;
	}
}


void VerletKineticIntegrator::IntegratePosition(
	Real p_rCurrentTime, Real p_rDeltaTime,
	IVectorTimeFunction& p_linearAcceleration,
	IVectorTimeFunction& p_angularAcceleration,
	KineticProperties& p_kineticProperties)
{
	Real rDeltaSubtime = p_rDeltaTime * m_rSubdivisionsInverse;
	Real rDeltaSubtimeSquared = rDeltaSubtime* rDeltaSubtime;
	const Real HALF = (Real) 0.5;

	// integrate across subdivisions
	for (size_t unIndex = 0; unIndex < m_unSubdivisions; unIndex++)
	{
		// keep copies of accelerations
		TVector3<Real> vecLinearAcceleration = p_linearAcceleration(p_rCurrentTime);
		TVector3<Real> vecAngularAcceleration = p_angularAcceleration(p_rCurrentTime);

		// position
		p_kineticProperties.Position += p_kineticProperties.LinearVelocity * rDeltaSubtime
			+ vecLinearAcceleration * rDeltaSubtimeSquared * HALF;

		// orientation

		// d(q) = 0.5 w q
		TQuaternion<Real> qtnAngularVelocity
			= TQuaternion<Real>(p_kineticProperties.AngularVelocity);
		TQuaternion<Real> qtnOrientationDerivative
			= qtnAngularVelocity * p_kineticProperties.Orientation * HALF;

		// d(d(q)) = 0.5 (a q + w d(q))
		TQuaternion<Real> qtnOrientationDerivative2
			= (TQuaternion<Real>(vecAngularAcceleration) * p_kineticProperties.Orientation
				+ qtnAngularVelocity * qtnOrientationDerivative) * HALF;

		p_kineticProperties.Orientation += qtnOrientationDerivative * rDeltaSubtime
			+ qtnOrientationDerivative2 * rDeltaSubtimeSquared * HALF;
		p_kineticProperties.Orientation.Normalise();
		p_kineticProperties.OrientationConjugate = p_kineticProperties.Orientation.ConjugateCopy();

		// track time per subdivision
		p_rCurrentTime += rDeltaSubtime;
	}
}
