#include "BasicForce.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;

BasicForce::BasicForce(void)
	: m_vecForceValue()
	, m_ptApplicationPoint()
	, m_bInstantaneous(false)
{
}

BasicForce::BasicForce(const String& p_strId)
	: IForce(p_strId)
	, m_vecForceValue()
	, m_ptApplicationPoint()
	, m_bInstantaneous(false)
{
}

BasicForce::~BasicForce(void)
{
}

void BasicForce::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties.Insert("ForceValue", PropertyDescriptor("ForceValue", PropertyType::Vector, false));
	p_mapProperties.Insert("ApplicationPoint", PropertyDescriptor("ApplicationPoint", PropertyType::Point, false));
	p_mapProperties.Insert("Instantaneous", PropertyDescriptor("Instantaneous", PropertyType::Boolean, false));
}

void BasicForce::GetProperty(const String& p_strName, bool& p_bValue) const
{
	if (p_strName == "Instantaneous")
		p_bValue = m_bInstantaneous;
	else
		throw new MesonException("BasicForce: Invalid property specified.", __FILE__, __LINE__);
}

void BasicForce::GetProperty(const String& p_strName, TVector3<Real>& p_vecValue) const
{
	if (p_strName == "ForceValue")
		p_vecValue = m_vecForceValue;
	else
		throw new MesonException("BasicForce: Invalid property specified.", __FILE__, __LINE__);
}

void BasicForce::GetProperty(const String& p_strName, TPoint3<Real>& p_ptValue) const
{
	if (p_strName == "ApplicationPoint")
		p_ptValue = m_ptApplicationPoint;
	else
		throw new MesonException("BasicForce: Invalid property specified.", __FILE__, __LINE__);
}

void BasicForce::SetProperty(const String& p_strName, bool p_bValue)
{
	if (p_strName == "Instantaneous")
		m_bInstantaneous = p_bValue;
	else
		throw new MesonException("BasicForce: Invalid property specified.", __FILE__, __LINE__);
}

void BasicForce::SetProperty(const String& p_strName, const TVector3<Real>& p_vecValue)
{
	if (p_strName == "ForceValue")
		m_vecForceValue = p_vecValue;
	else
		throw new MesonException("BasicForce: Invalid property specified.", __FILE__, __LINE__);
}

void BasicForce::SetProperty(const String& p_strName, const TPoint3<Real>& p_ptValue)
{
	if (p_strName == "ApplicationPoint")
		m_ptApplicationPoint = p_ptValue;
	else
		throw new MesonException("BasicForce: Invalid property specified.", __FILE__, __LINE__);
}

TVector3<Real> BasicForce::GetForceValue(
	MassProperties* p_pMassProperties,
	KineticProperties* p_pKineticProperties,
	GeometryPtr p_pGeometry,
	Real p_rTime)
{
	return m_vecForceValue;
}

TPoint3<Real> BasicForce::GetApplicationPoint(
	MassProperties* p_pMassProperties,
	KineticProperties* p_pKineticProperties,
	GeometryPtr p_pGeometry,
	Real p_rTime)
{
	return m_ptApplicationPoint;
}

bool BasicForce::IsInstantaneous(void)
{
	return m_bInstantaneous;
}
