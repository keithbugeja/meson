#include "RevoluteJointConstraint.h"

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

String RevoluteJointConstraint::s_strType("RevoluteJoint");

void RevoluteJointConstraint::UpdateAngleLimitDerivatives(void)
{
	m_rAngleLimitCosine = TMaths<Real>::Cos(m_rAngleLimit);

	TVector3<Real> vecLocalAxisMidpint1(
		(m_vecBody1Anchor1 + m_vecBody1Anchor2) * (Real) 0.5);
	TVector3<Real> vecLocalAxisMidpint2(
		(m_vecBody2Anchor1 + m_vecBody2Anchor2) * (Real) 0.5);

	Real rA = vecLocalAxisMidpint1.Length();
	Real rB = vecLocalAxisMidpint2.Length();
	Real rC2 = rA * rA + rB * rB + (Real) 2.0 * rA * rB * m_rAngleLimitCosine;
	m_rMinimumCentroidDistance = TMaths<Real>::Sqrt(rC2);
}

TVector3<Real> RevoluteJointConstraint::GetCurrentAxisPosition1(void) const
{
	TVector3<Real> vecWorldBody1Anchor1, vecWorldBody2Anchor1;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_vecBody1Anchor1, vecWorldBody1Anchor1);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_vecBody2Anchor1, vecWorldBody2Anchor1);
	return (vecWorldBody1Anchor1 + vecWorldBody2Anchor1) * (Real) 0.5;
}

TVector3<Real> RevoluteJointConstraint::GetCurrentAxisPosition2(void) const
{
	TVector3<Real> vecWorldBody1Anchor2, vecWorldBody2Anchor2;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_vecBody1Anchor2, vecWorldBody1Anchor2);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_vecBody2Anchor2, vecWorldBody2Anchor2);
	return (vecWorldBody1Anchor2 + vecWorldBody2Anchor2) * (Real) 0.5;
}

RevoluteJointConstraint::RevoluteJointConstraint(void)
	: m_pBody1(NULL)
	, m_pBody2(NULL)
	, m_bBroken(true)
	, m_rBreakingThreshold(TMaths<Real>::Maximum)
	, m_vecBindAxisPoint1()
	, m_vecBindAxisPoint2()
	, m_vecBody1Anchor1()
	, m_vecBody1Anchor2()
	, m_vecBody2Anchor1()
	, m_vecBody2Anchor2()
	, m_rAngleLimit(TMaths<Real>::Pi)
	, m_rJointRestitution((Real) 0.0)
	, m_rAngleLimitCosine((Real) -1.0)
	, m_rMinimumCentroidDistance((Real) 0.0)
	, m_listConstraintDiscrepancies()
{
}

RevoluteJointConstraint::RevoluteJointConstraint(const String& p_strId)
	: IConstraint(p_strId)
	, m_pBody1(NULL)
	, m_pBody2(NULL)
	, m_bBroken(true)
	, m_rBreakingThreshold(TMaths<Real>::Maximum)
	, m_vecBindAxisPoint1()
	, m_vecBindAxisPoint2()
	, m_vecBody1Anchor1()
	, m_vecBody1Anchor2()
	, m_vecBody2Anchor1()
	, m_vecBody2Anchor2()
	, m_rAngleLimit(TMaths<Real>::Pi)
	, m_rJointRestitution((Real) 0.0)
	, m_rAngleLimitCosine((Real) -1.0)
	, m_rMinimumCentroidDistance((Real) 0.0)
	, m_listConstraintDiscrepancies()
{
}

RevoluteJointConstraint::~RevoluteJointConstraint(void)
{
}

void RevoluteJointConstraint::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();

	p_mapProperties["BindAxisPoint1"]
		= PropertyDescriptor("BindAxisPoint1", PropertyType::Vector, false);
	p_mapProperties["BindAxisPoint2"]
		= PropertyDescriptor("BindAxisPoint2", PropertyType::Vector, false);
	p_mapProperties["AngleLimit"]
		= PropertyDescriptor("AngleLimit", PropertyType::Real, false);
	p_mapProperties["JointRestitution"]
		= PropertyDescriptor("JointRestitution", PropertyType::Real, false);
	p_mapProperties["CurrentAxisPosition1"]
		= PropertyDescriptor("CurrentAxisPosition1", PropertyType::Vector, true);
	p_mapProperties["CurrentAxisPosition2"]
		= PropertyDescriptor("CurrentAxisPosition2", PropertyType::Vector, true);
}

void RevoluteJointConstraint::GetProperty(const String &p_strName, Real &p_rValue) const
{
	if (p_strName == "AngleLimit")
		p_rValue = m_rAngleLimit;
	else if (p_strName == "JointRestitution")
		p_rValue = m_rJointRestitution;
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for RevoluteJoint constraint.",
			__FILE__, __LINE__);
}

void RevoluteJointConstraint::SetProperty(const String &p_strName, Real p_rValue)
{
	if (p_strName == "AngleLimit")
	{
		m_rAngleLimit = TMaths<Real>::Clamp(p_rValue, (Real) 0.0, TMaths<Real>::Pi);
		UpdateAngleLimitDerivatives();
	}
	else if (p_strName == "JointRestitution")
	{
		m_rJointRestitution = TMaths<Real>::Clamp(p_rValue, -1, 1);
		UpdateAngleLimitDerivatives();
	}
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for RevoluteJoint constraint.",
			__FILE__, __LINE__);
}

void RevoluteJointConstraint::GetProperty(const String &p_strName, TVector3<Real> &p_vecValue) const
{
	if (p_strName == "BindAxisPoint1")
		p_vecValue = m_vecBindAxisPoint1;
	else if (p_strName == "BindAxisPoint2")
		p_vecValue = m_vecBindAxisPoint2;
	else if (p_strName == "CurrentAxisPosition1")
		p_vecValue = GetCurrentAxisPosition1();
	else if (p_strName == "CurrentAxisPosition2")
		p_vecValue = GetCurrentAxisPosition2();
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for RevoluteJoint constraint.",
			__FILE__, __LINE__);
			
}

void RevoluteJointConstraint::SetProperty(const String &p_strName, const TVector3<Real> &p_vecValue)
{
	if (p_strName == "BindAxisPoint1")
		m_vecBindAxisPoint1 = p_vecValue;
	else if (p_strName == "BindAxisPoint2")
		m_vecBindAxisPoint2 = p_vecValue;
	else if (p_strName == "CurrentAxisPosition1")
		throw new MesonException(
			"Property 'CurrentAxisPosition1' is read-only for RevoluteJoint constraint.",
			__FILE__, __LINE__);
	else if (p_strName == "CurrentAxisPosition2")
		throw new MesonException(
			"Property 'CurrentAxisPosition2' is read-only for RevoluteJoint constraint.",
			__FILE__, __LINE__);
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for RevoluteJoint constraint.",
			__FILE__, __LINE__);
}

void RevoluteJointConstraint::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	if (m_bBroken)
		return;

	TVector3<Real>& vecBodyPosition1 = m_pBody1->GetKineticProperties().Position;
	TVector3<Real>& vecBodyPosition2 = m_pBody2->GetKineticProperties().Position;
	TVector3<Real> vecAxisAnchor1 = GetCurrentAxisPosition1();
	TVector3<Real> vecAxisAnchor2 = GetCurrentAxisPosition2();

	p_pInstrumentationDevice->DrawLine(vecBodyPosition1, vecAxisAnchor1);
	p_pInstrumentationDevice->DrawLine(vecBodyPosition1, vecAxisAnchor2);
	p_pInstrumentationDevice->DrawLine(vecBodyPosition2, vecAxisAnchor1);
	p_pInstrumentationDevice->DrawLine(vecBodyPosition2, vecAxisAnchor2);
}

const String& RevoluteJointConstraint::GetType(void) const
{
	return s_strType;
}

BodyPtr RevoluteJointConstraint::GetBody1(void)
{
	return m_pBody1;
}

void RevoluteJointConstraint::SetBody1(BodyPtr p_pBody1)
{
	m_pBody1 = p_pBody1;
}

BodyPtr RevoluteJointConstraint::GetBody2(void)
{
	return m_pBody2;
}

void RevoluteJointConstraint::SetBody2(BodyPtr p_pBody2)
{
	m_pBody2 = p_pBody2;
}

void RevoluteJointConstraint::Bind(void)
{
	if (m_pBody1 == NULL) return;
	if (m_pBody2 == NULL) return;

	// compute axis anchor points for local body coords

	// body 1 anchors
	m_pBody1->GetKineticProperties().TransformPointToLocal(
		m_vecBindAxisPoint1, m_vecBody1Anchor1);
	m_pBody1->GetKineticProperties().TransformPointToLocal(
		m_vecBindAxisPoint2, m_vecBody1Anchor2);

	// body 2 anchors
	m_pBody2->GetKineticProperties().TransformPointToLocal(
		m_vecBindAxisPoint1, m_vecBody2Anchor1);
	m_pBody2->GetKineticProperties().TransformPointToLocal(
		m_vecBindAxisPoint2, m_vecBody2Anchor2);

	m_bBroken = false;

	UpdateAngleLimitDerivatives();
}

void RevoluteJointConstraint::Unbind(void)
{
	m_bBroken = true;
}

bool RevoluteJointConstraint::IsInstantaneous(void) const
{
	return false;
}

bool RevoluteJointConstraint::IsBroken(void) const
{
	return m_bBroken;
}

void RevoluteJointConstraint::SetBroken(bool p_bBroken)
{
	m_bBroken = p_bBroken;
}

Real RevoluteJointConstraint::GetBreakingThreshold(void) const
{
	return m_rBreakingThreshold;
}

void RevoluteJointConstraint::SetBreakingThreshold(Real p_rValue)
{
	m_rBreakingThreshold = p_rValue;
}

bool RevoluteJointConstraint::CanRecomputeDiscrepancies(void)
{
	return true;
}

void RevoluteJointConstraint::ComputeDiscrepancies(void)
{
	m_listConstraintDiscrepancies.Clear();

	// -- connection point

	// compute local anchor points in world space

	// axis anchor 1
	TVector3<Real> vecWorldBody1Anchor1, vecWorldBody2Anchor1;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_vecBody1Anchor1, vecWorldBody1Anchor1);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_vecBody2Anchor1, vecWorldBody2Anchor1);

	// add connection discrepancy (may be nil, but still
	// enforces velocity constraint with zero restitution)
	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(vecWorldBody1Anchor1, vecWorldBody2Anchor1));

	// axis anchor 2
	TVector3<Real> vecWorldBody1Anchor2, vecWorldBody2Anchor2;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_vecBody1Anchor2, vecWorldBody1Anchor2);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_vecBody2Anchor2, vecWorldBody2Anchor2);

	// add connection discrepancy (may be nil, but still
	// enforces velocity constraint with zero restitution)
	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(vecWorldBody1Anchor2, vecWorldBody2Anchor2));

	// -- angle limit

	if (m_rAngleLimit >= TMaths<Real>::Pi)
		return;

	// compute average world anchors
	TVector3<Real> vecWorldAnchor1((vecWorldBody1Anchor1 + vecWorldBody2Anchor1) * (Real) 0.5);
	TVector3<Real> vecWorldAnchor2((vecWorldBody1Anchor2 + vecWorldBody2Anchor2) * (Real) 0.5);

	// compute axis midpoint
	TVector3<Real> vecAxisMidpoint((vecWorldAnchor1 + vecWorldAnchor2) * (Real) 0.5);

	// compute oriented directions from cm's to bind point
	TVector3<Real> vecOrientedDirection1(vecAxisMidpoint
		- m_pBody1->GetKineticProperties().Position);
	TVector3<Real> vecOrientedDirection2(vecAxisMidpoint
		- m_pBody2->GetKineticProperties().Position);

	vecOrientedDirection1.Normalise();
	vecOrientedDirection2.Normalise();

	// compute angle
	Real rCurrentCosine(TMaths<Real>::Clamp(
		vecOrientedDirection1 * -vecOrientedDirection2, (Real) -1.0, (Real) 1.0));

	// do nothing else if angle limit not exceeded
	if (rCurrentCosine > m_rAngleLimitCosine)
		return;

	// otherwise, compute corrective impulses to limit angle

	TVector3<Real> vecCentroidOffset(m_pBody2->GetKineticProperties().Position
		- m_pBody1->GetKineticProperties().Position);
	TVector3<Real> vecCentroidDirection(vecCentroidOffset);
	if (vecCentroidDirection.IsZero())
		vecCentroidDirection = TVector3<Real>::Right;
	else
		vecCentroidDirection.Normalise();

	Real rCurrentDist((m_pBody2->GetKineticProperties().Position - m_pBody1->GetKineticProperties().Position).Length());

	Real rDiscrepancy((m_rMinimumCentroidDistance - rCurrentDist) * (Real) 0.5);
	TVector3<Real> vecDiscrepancy(rDiscrepancy * vecCentroidDirection);

	TVector3<Real> vecMidPoint(m_pBody1->GetKineticProperties().Position + vecCentroidOffset * (Real) 0.5);

	TVector3<Real> vecHalfAxisOffset(vecWorldAnchor2 - vecWorldAnchor1);

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			vecMidPoint + vecHalfAxisOffset + vecDiscrepancy,
			vecMidPoint + vecHalfAxisOffset - vecDiscrepancy,
			m_rJointRestitution));			

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			vecMidPoint - vecHalfAxisOffset + vecDiscrepancy,
			vecMidPoint - vecHalfAxisOffset - vecDiscrepancy,
			m_rJointRestitution));			
}

const ConstraintDiscrepancyList& RevoluteJointConstraint::GetDiscrepancies(void)
{
	return m_listConstraintDiscrepancies;
}
