#include "EulerKineticIntegrator.h"

#include "Definitions.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Kinetics;

const String g_strEulerIntegratorName = "Euler";

EulerKineticIntegrator::EulerKineticIntegrator(void)
	: m_unSubdivisions(1)
	, m_rSubdivisionsInverse((Real) 1.0)
{
}

EulerKineticIntegrator::~EulerKineticIntegrator(void)
{
}

void EulerKineticIntegrator::EnumerateProperties(
	TMap<String, PropertyDescriptor>&
		p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties["Subdivisions"]
		= PropertyDescriptor("Subdivisions", PropertyType::Integer, false);
}

void EulerKineticIntegrator::GetProperty(
	const String& p_strName, int &p_nValue) const
{
	if (p_strName == "Subdivisions")
		p_nValue = (int) m_unSubdivisions;
	else
		MESON_ASSERT(false, "Euler Kinetic Integrator: Invalid property specified.")
}

void EulerKineticIntegrator::SetProperty(const String& p_strName, int p_nValue)
{
	if (p_strName == "Subdivisions")
	{
		m_unSubdivisions = (size_t) MAX(1, p_nValue);
		m_rSubdivisionsInverse = (Real) 1.0 / (Real) m_unSubdivisions;
	}
	else
		MESON_ASSERT(false, "Euler Kinetic Integrator: Invalid property specified.")
}

const String& EulerKineticIntegrator::GetName(void) const
{
	return g_strEulerIntegratorName;
}

void EulerKineticIntegrator::IntegrateVelocity(
	Real p_rCurrentTime, Real p_rDeltaTime,
	IVectorTimeFunction& p_linearAcceleration,
	IVectorTimeFunction& p_angularAcceleration,
	KineticProperties& p_kineticProperties)
{
	Real rDeltaSubtime = p_rDeltaTime * m_rSubdivisionsInverse;

	// compute damping factor
	Real rDampingFactor(TMaths<Real>::Exp(-p_kineticProperties.DampingCoefficient * rDeltaSubtime));

	// integrate across subdivisions
	for (size_t unIndex = 0; unIndex < m_unSubdivisions; unIndex++)
	{
		// linear velocity
		p_kineticProperties.LinearVelocity += p_linearAcceleration(p_rCurrentTime) * rDeltaSubtime;

		// angular velocity
		p_kineticProperties.AngularVelocity += p_angularAcceleration(p_rCurrentTime) * rDeltaSubtime;

		// apply damping
		p_kineticProperties.LinearVelocity *= rDampingFactor;
		p_kineticProperties.AngularVelocity *= rDampingFactor;

		// track time per subdivision
		p_rCurrentTime += rDeltaSubtime;
	}
}


void EulerKineticIntegrator::IntegratePosition(
	Real p_rCurrentTime, Real p_rDeltaTime,
	IVectorTimeFunction& p_linearAcceleration,
	IVectorTimeFunction& p_angularAcceleration,
	KineticProperties& p_kineticProperties)
{
	Real rDeltaSubtime = p_rDeltaTime * m_rSubdivisionsInverse;

	// integrate across subdivisions
	for (size_t unIndex = 0; unIndex < m_unSubdivisions; unIndex++)
	{
		// position
		p_kineticProperties.Position += p_kineticProperties.LinearVelocity * rDeltaSubtime;

		// orientation 
		// d(q) = 0.5 w q
		TQuaternion<Real> qtnOrientationDerivative
			= TQuaternion<Real>(p_kineticProperties.AngularVelocity) * p_kineticProperties.Orientation * (Real) 0.5;
		p_kineticProperties.Orientation += qtnOrientationDerivative * rDeltaSubtime;
		p_kineticProperties.Orientation.Normalise();
		p_kineticProperties.OrientationConjugate = p_kineticProperties.Orientation.ConjugateCopy();

		// track time per subdivision
		p_rCurrentTime += rDeltaSubtime;
	}
}
