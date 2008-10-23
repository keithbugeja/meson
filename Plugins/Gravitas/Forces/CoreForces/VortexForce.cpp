#include "VortexForce.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;

VortexForce::VortexForce(void)
	: m_ptEyePosition(TPoint3<Real>::Origin)
	, m_rAxialAttraction((Real) 10.0)
	, m_rVerticalAttraction((Real) 40.0)
{
}

VortexForce::VortexForce(const String& p_strId)
	: IForce(p_strId)
	, m_ptEyePosition(TPoint3<Real>::Origin)
	, m_rAxialAttraction((Real) 10.0)
	, m_rVerticalAttraction((Real) 40.0)
{
}

VortexForce::~VortexForce(void)
{
}

void VortexForce::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties.Insert("EyePosition", PropertyDescriptor("EyePosition", PropertyType::Point, false));
	p_mapProperties.Insert("AxialAttraction", PropertyDescriptor("AxialAttraction", PropertyType::Real, false));
	p_mapProperties.Insert("VerticalAttraction", PropertyDescriptor("VerticalAttraction", PropertyType::Real, false));
}

void VortexForce::GetProperty(const String& p_strName, Real& p_rValue) const
{
	if (p_strName == "AxialAttraction")
		p_rValue = m_rAxialAttraction;
	else if (p_strName == "VerticalAttraction")
		p_rValue = m_rVerticalAttraction;
	else
		throw new MesonException("VortexForce: Invalid property specified.", __FILE__, __LINE__);
}

void VortexForce::GetProperty(const String& p_strName, TPoint3<Real>& p_ptValue) const
{
	if (p_strName == "EyePosition")
		p_ptValue = m_ptEyePosition;
	else
		throw new MesonException("VortexForce: Invalid property specified.", __FILE__, __LINE__);
}

void VortexForce::SetProperty(const String& p_strName, Real p_rValue)
{
	if (p_strName == "AxialAttraction")
		m_rAxialAttraction = TMaths<Real>::Max((Real) 0.0, p_rValue);
	else if (p_strName == "VerticalAttraction")
		m_rVerticalAttraction = TMaths<Real>::Max((Real) 0.0, p_rValue);
	else
		throw new MesonException("VortexForce: Invalid property specified.", __FILE__, __LINE__);
}

void VortexForce::SetProperty(const String& p_strName, const TPoint3<Real>& p_ptValue)
{
	if (p_strName == "EyePosition")
		m_ptEyePosition = p_ptValue;
	else
		throw new MesonException("VortexForce: Invalid property specified.", __FILE__, __LINE__);
}

TVector3<Real> VortexForce::GetForceValue(
	MassProperties* p_pMassProperties,
	KineticProperties* p_pKineticProperties,
	GeometryPtr p_pGeometry,
	Real p_rTime)
{
	TVector3<Real> vecForceValue(m_ptEyePosition - p_pKineticProperties->Position);

	Real rAxialDistanceSqr = vecForceValue.X * vecForceValue.X + vecForceValue.Z * vecForceValue.Z;

	Real rAxialDistanceSqrClamped = TMaths<Real>::Max((Real) 1.0, rAxialDistanceSqr);

	Real rDeltaYFixed = vecForceValue.Y > (Real) 0.0
		? TMaths<Real>::Max((Real) 1.0, vecForceValue.Y)
		: TMaths<Real>::Min((Real) -1.0, vecForceValue.Y);

	Real rDistanceY = TMaths<Real>::Abs(rDeltaYFixed);

	vecForceValue.X *= m_rAxialAttraction / rDistanceY;
	vecForceValue.Z *= m_rAxialAttraction / rDistanceY;

	vecForceValue.Y = m_rVerticalAttraction / (rAxialDistanceSqrClamped * rDistanceY);

	TVector3<Real> vecSide = vecForceValue ^ TVector3<Real>::Up;
	vecForceValue += vecSide / (vecForceValue.Length() + (Real) 1.0);

	return vecForceValue * p_pMassProperties->Mass;
}

TPoint3<Real> VortexForce::GetApplicationPoint(
	MassProperties* p_pMassProperties,
	KineticProperties* p_pKineticProperties,
	GeometryPtr p_pGeometry,
	Real p_rTime)
{
	return TPoint3<Real>::Origin;
}

bool VortexForce::IsInstantaneous(void)
{
	return false;
}
