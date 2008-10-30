#include "SphericalJointConstraint.h"

#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;

String SphericalJointConstraint::s_strType("SphericalJoint");

void SphericalJointConstraint::UpdateAngleLimitDerivatives(void)
{
	m_rAngleLimitCosine = TMaths<Real>::Cos(m_rAngleLimit);
	Real rA = m_vecLocalConnectionPoint1.Length();
	Real rB = m_vecLocalConnectionPoint2.Length();
	Real rC2 = rA * rA + rB * rB + (Real) 2.0 * rA * rB * m_rAngleLimitCosine;
	m_rMinimumCentroidDistance = TMaths<Real>::Sqrt(rC2);
}

TVector3<Real> SphericalJointConstraint::GetCurrentJointPosition(void) const
{
	// compute current joint position
	TVector3<Real> vecWorldConnectionPoint1, vecWorldConnectionPoint2;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_vecLocalConnectionPoint1, vecWorldConnectionPoint1);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_vecLocalConnectionPoint2, vecWorldConnectionPoint2);
	return (vecWorldConnectionPoint1 + vecWorldConnectionPoint2) * (Real) 0.5;
}

SphericalJointConstraint::SphericalJointConstraint(void)
	: m_pBody1(NULL)
	, m_pBody2(NULL)
	, m_bBroken(true)
	, m_rBreakingThreshold(TMaths<Real>::Maximum)
	, m_vecBindPoint()
	, m_vecLocalConnectionPoint1()
	, m_vecLocalConnectionPoint2()
	, m_rAngleLimit(TMaths<Real>::Pi)
	, m_rJointRestitution((Real) 0.0)
	, m_rAngleLimitCosine((Real) -1.0)
	, m_rMinimumCentroidDistance((Real) 0.0)
	, m_listConstraintDiscrepancies()
{
}

SphericalJointConstraint::SphericalJointConstraint(const String& p_strId)
	: IConstraint(p_strId)
	, m_pBody1(NULL)
	, m_pBody2(NULL)
	, m_bBroken(true)
	, m_rBreakingThreshold(TMaths<Real>::Maximum)
	, m_vecBindPoint()
	, m_vecLocalConnectionPoint1()
	, m_vecLocalConnectionPoint2()
	, m_rAngleLimit(TMaths<Real>::Pi)
	, m_rJointRestitution((Real) 0.0)
	, m_rAngleLimitCosine((Real) -1.0)
	, m_rMinimumCentroidDistance((Real) 0.0)
	, m_listConstraintDiscrepancies()
{
}

SphericalJointConstraint::~SphericalJointConstraint(void)
{
}

void SphericalJointConstraint::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties["BindPoint"]
		= PropertyDescriptor("BindPoint", PropertyType::Vector, false);
	p_mapProperties["AngleLimit"]
		= PropertyDescriptor("AngleLimit", PropertyType::Real, false);
	p_mapProperties["JointRestitution"]
		= PropertyDescriptor("JointRestitution", PropertyType::Real, false);
	p_mapProperties["CurrentJointPosition"]
		= PropertyDescriptor("CurrentJointPosition", PropertyType::Vector, true);
}

void SphericalJointConstraint::GetProperty(const String& p_strName, Real& p_rValue) const
{
	if (p_strName == "AngleLimit")
		p_rValue = m_rAngleLimit;
	else if (p_strName == "JointRestitution")
		p_rValue = m_rJointRestitution;
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for SphericalJoint constraint.",
			__FILE__, __LINE__);
}

void SphericalJointConstraint::SetProperty(const String& p_strName, Real p_rValue)
{
	if (p_strName == "AngleLimit")
	{
		m_rAngleLimit = TMaths<Real>::Clamp(p_rValue, (Real) 0.0, TMaths<Real>::Pi);
		UpdateAngleLimitDerivatives();
	}
	else if (p_strName == "JointRestitution")
	{
		m_rJointRestitution = TMaths<Real>::Clamp(p_rValue);
		UpdateAngleLimitDerivatives();
	}
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for SphericalJoint constraint.",
			__FILE__, __LINE__);
}

void SphericalJointConstraint::GetProperty(const String& p_strName, TVector3<Real>& p_vecValue) const
{
	if (p_strName == "BindPoint")
		p_vecValue = m_vecBindPoint;
	else if (p_strName == "CurrentJointPosition")
	{
		p_vecValue = GetCurrentJointPosition();
	}
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for SphericalJoint constraint.",
			__FILE__, __LINE__);
}

void SphericalJointConstraint::SetProperty(const String &p_strName, const TVector3<Real> &p_vecValue)
{
	if (p_strName == "BindPoint")
		m_vecBindPoint = p_vecValue;
	else if (p_strName == "CurrentJointPosition")
		throw new MesonException(
			"Property 'CurrentJointPosition' is read-only for SphericalJoint constraint.",
			__FILE__, __LINE__);
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for SphericalJoint constraint.",
			__FILE__, __LINE__);
}

void SphericalJointConstraint::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	if (m_bBroken)
		return;

	// compute current joint position
	TVector3<Real> vecWorldConnectionPoint = GetCurrentJointPosition();

	p_pInstrumentationDevice->DrawLine(
		m_pBody1->GetKineticProperties().Position,
		vecWorldConnectionPoint);

	p_pInstrumentationDevice->DrawLine(
		vecWorldConnectionPoint,
		m_pBody2->GetKineticProperties().Position);
}

const String& SphericalJointConstraint::GetType(void) const
{
	return s_strType;
}

BodyPtr SphericalJointConstraint::GetBody1(void)
{
	return m_pBody1;
}

void SphericalJointConstraint::SetBody1(BodyPtr p_pBody1)
{
	m_pBody1 = p_pBody1;
}

BodyPtr SphericalJointConstraint::GetBody2(void)
{
	return m_pBody2;
}

void SphericalJointConstraint::SetBody2(BodyPtr p_pBody2)
{
	m_pBody2 = p_pBody2;
}

void SphericalJointConstraint::Bind(void)
{
	if (m_pBody1 == NULL) return;
	if (m_pBody2 == NULL) return;

	// compute bind point in local body coordinates
	m_pBody1->GetKineticProperties().TransformPointToLocal(
		m_vecBindPoint, m_vecLocalConnectionPoint1);
	m_pBody2->GetKineticProperties().TransformPointToLocal(
		m_vecBindPoint, m_vecLocalConnectionPoint2);

	m_bBroken = false;

	UpdateAngleLimitDerivatives();
}

void SphericalJointConstraint::Unbind(void)
{
	m_bBroken = true;
}

bool SphericalJointConstraint::IsInstantaneous(void) const
{
	return false;
}

bool SphericalJointConstraint::IsBroken(void) const
{
	return m_bBroken;
}

void SphericalJointConstraint::SetBroken(bool p_bBroken)
{
	m_bBroken = p_bBroken;
}

Real SphericalJointConstraint::GetBreakingThreshold(void) const
{
	return m_rBreakingThreshold;
}

void SphericalJointConstraint::SetBreakingThreshold(Real p_rValue)
{
	m_rBreakingThreshold = p_rValue;
}

bool SphericalJointConstraint::CanRecomputeDiscrepancies(void)
{
	return true;
}

void SphericalJointConstraint::ComputeDiscrepancies(void)
{
	m_listConstraintDiscrepancies.Clear();

	// -- connection point

	// compute local connection points in world space
	TVector3<Real> vecWorldConnectionPoint1, vecWorldConnectionPoint2;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_vecLocalConnectionPoint1, vecWorldConnectionPoint1);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_vecLocalConnectionPoint2, vecWorldConnectionPoint2);

	// add connection discrepancy (may be nil, but still
	// enforces velocity constraint with zero restitution)
	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(vecWorldConnectionPoint1, vecWorldConnectionPoint2));

	// -- angle limit

	if (m_rAngleLimit >= TMaths<Real>::Pi)
		return;

	// compute oriented directions from cm's to bind point
	TVector3<Real> vecOrientedDirection1(vecWorldConnectionPoint1
		- m_pBody1->GetKineticProperties().Position);
	TVector3<Real> vecOrientedDirection2(vecWorldConnectionPoint2
		- m_pBody2->GetKineticProperties().Position);

	vecOrientedDirection1.Normalise();
	vecOrientedDirection2.Normalise();

	// compute angle
	Real rCurrentCosine(TMaths<Real>::Clamp(
		vecOrientedDirection1 * -vecOrientedDirection2, (Real) -1.0, (Real) 1.0));

	// do nothing else if angle limit not exceeded
	if (rCurrentCosine > m_rAngleLimitCosine)
		return;

	// otherwise, compute corrective impulse to limit angle

	TVector3<Real> vecCentroidOffset = m_pBody2->GetKineticProperties().Position - m_pBody1->GetKineticProperties().Position;
	TVector3<Real> vecCentroidDirection(vecCentroidOffset);
	if (vecCentroidDirection.IsZero())
		vecCentroidDirection = TVector3<Real>::Right;
	else
		vecCentroidDirection.Normalise();

	Real rCurrentDist((m_pBody2->GetKineticProperties().Position
		- m_pBody1->GetKineticProperties().Position).Length());

	Real rDiscrepancy((m_rMinimumCentroidDistance - rCurrentDist) * (Real) 0.5);
	TVector3<Real> vecDiscrepancy(rDiscrepancy * vecCentroidDirection);

	TVector3<Real> ptMidPoint(m_pBody1->GetKineticProperties().Position + vecCentroidOffset * (Real) 0.5);

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			ptMidPoint + vecDiscrepancy,
			ptMidPoint - vecDiscrepancy,
			m_rJointRestitution));
}

const ConstraintDiscrepancyList& SphericalJointConstraint::GetDiscrepancies(void)
{
	return m_listConstraintDiscrepancies;
}
