#include "GravityForce.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;

GravityForce::GravityForce(void)
	: m_vecAcceleration((Real) 0.0, (Real) -5.0, (Real) 0.0)
{
}

GravityForce::GravityForce(const String& p_strId)
	: IForce(p_strId)
	, m_vecAcceleration((Real) 0.0, (Real) -5.0, (Real) 0.0)
{
}

GravityForce::~GravityForce(void)
{
}

void GravityForce::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties.Insert("Acceleration", PropertyDescriptor("Acceleration", PropertyType::Vector, false));
}

void GravityForce::GetProperty(const String& p_strName, TVector3<Real>& p_vecValue) const
{
	if (p_strName == "Acceleration")
		p_vecValue = m_vecAcceleration;
	else
		throw new MesonException("GravityForce: Invalid property specified.", __FILE__, __LINE__);
}

void GravityForce::SetProperty(const String& p_strName, const TVector3<Real>& p_vecValue)
{
	if (p_strName == "Acceleration")
		m_vecAcceleration = p_vecValue;
	else
		throw new MesonException("GravityForce: Invalid property specified.", __FILE__, __LINE__);
}

TVector3<Real> GravityForce::GetForceValue(
	MassProperties* p_pMassProperties,
	KineticProperties* p_pKineticProperties,
	GeometryPtr p_pGeometry,
	Real p_rTime)
{
	return m_vecAcceleration * p_pMassProperties->Mass;
}

TVector3<Real> GravityForce::GetApplicationPoint(
	MassProperties* p_pMassProperties,
	KineticProperties* p_pKineticProperties,
	GeometryPtr p_pGeometry,
	Real p_rTime)
{
	return TVector3<Real>::Zero;
}

bool GravityForce::IsInstantaneous(void)
{
	return false;
}
