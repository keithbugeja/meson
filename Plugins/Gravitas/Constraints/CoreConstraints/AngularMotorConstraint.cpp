#include "AngularMotorConstraint.h"

#include "MesonException.h"
#include "Quaternion.h"

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

String AngularMotorConstraint::s_strType("AngularMotor");

void AngularMotorConstraint::UpdateDerivativeVariables(void)
{
	// compute offset perp to axis
	TVector3<Real> vecBody1AxisOffset(m_ptBody1Anchor2 - m_ptBody1Anchor1);
	TVector3<Real> vecBody2AxisOffset(m_ptBody2Anchor2 - m_ptBody2Anchor1);

	// compute relative velocity of a rotation point in body1 space
	m_vecBody1DeltaVelocity = vecBody1AxisOffset ^ m_vecBody1TorqueOffset;
	if (!m_vecBody1DeltaVelocity.IsZero())
		m_vecBody1DeltaVelocity.Normalise();
	m_vecBody1DeltaVelocity *= m_vecBody1TorqueOffset.Length() * m_rAngularSpeed;

	// compute relative velocity of a rotation point in body2 space
	m_vecBody2DeltaVelocity = vecBody2AxisOffset ^ m_vecBody2TorqueOffset;
	if (!m_vecBody2DeltaVelocity.IsZero())
		m_vecBody2DeltaVelocity.Normalise();
	m_vecBody2DeltaVelocity *= m_vecBody2TorqueOffset.Length() * m_rAngularSpeed;
}

TPoint3<Real> AngularMotorConstraint::GetCurrentAxisPosition1(void) const
{
	TPoint3<Real> ptWorldBody1Anchor1, ptWorldBody2Anchor1;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_ptBody1Anchor1, ptWorldBody1Anchor1);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_ptBody2Anchor1, ptWorldBody2Anchor1);
	return ptWorldBody1Anchor1 + (ptWorldBody2Anchor1 - ptWorldBody1Anchor1) * (Real) 0.5;
}

TPoint3<Real> AngularMotorConstraint::GetCurrentAxisPosition2(void) const
{
	TPoint3<Real> ptWorldBody1Anchor2, ptWorldBody2Anchor2;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_ptBody1Anchor2, ptWorldBody1Anchor2);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_ptBody2Anchor2, ptWorldBody2Anchor2);
	return ptWorldBody1Anchor2 + (ptWorldBody2Anchor2 - ptWorldBody1Anchor2) * (Real) 0.5;
}

AngularMotorConstraint::AngularMotorConstraint(void)
	: m_pBody1(NULL)
	, m_pBody2(NULL)
	, m_bBroken(true)
	, m_rBreakingThreshold(TMaths<Real>::Maximum)
	, m_ptBindAxisPoint1()
	, m_ptBindAxisPoint2()
	, m_ptBody1Anchor1()
	, m_ptBody1Anchor2()
	, m_ptBody2Anchor1()
	, m_ptBody2Anchor2()
	, m_rAngularSpeed((Real) 0.0)
	, m_vecBody1TorqueOffset(true)
	, m_vecBody2TorqueOffset(true)
	, m_vecBody1DeltaVelocity(true)
	, m_vecBody2DeltaVelocity(true)
	, m_listConstraintDiscrepancies()
{
}

AngularMotorConstraint::AngularMotorConstraint(const String& p_strId)
	: IConstraint(p_strId)
	, m_pBody1(NULL)
	, m_pBody2(NULL)
	, m_bBroken(true)
	, m_rBreakingThreshold(TMaths<Real>::Maximum)
	, m_ptBindAxisPoint1()
	, m_ptBindAxisPoint2()
	, m_ptBody1Anchor1()
	, m_ptBody1Anchor2()
	, m_ptBody2Anchor1()
	, m_ptBody2Anchor2()
	, m_rAngularSpeed((Real) 0.0)
	, m_vecBody1TorqueOffset(true)
	, m_vecBody2TorqueOffset(true)
	, m_vecBody1DeltaVelocity(true)
	, m_vecBody2DeltaVelocity(true)
	, m_listConstraintDiscrepancies()
{
}

AngularMotorConstraint::~AngularMotorConstraint(void)
{
}

void AngularMotorConstraint::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();

	p_mapProperties["BindAxisPoint1"]
		= PropertyDescriptor("BindAxisPoint1", PropertyType::Point, false);
	p_mapProperties["BindAxisPoint2"]
		= PropertyDescriptor("BindAxisPoint2", PropertyType::Point, false);
	p_mapProperties["AngularSpeed"]
		= PropertyDescriptor("AngularSpeed", PropertyType::Real, false);
	p_mapProperties["CurrentAxisPosition1"]
		= PropertyDescriptor("CurrentAxisPosition1", PropertyType::Point, true);
	p_mapProperties["CurrentAxisPosition2"]
		= PropertyDescriptor("CurrentAxisPosition2", PropertyType::Point, true);
}

void AngularMotorConstraint::GetProperty(const String &p_strName, Real &p_rValue) const
{
	if (p_strName == "AngularSpeed")
		p_rValue = m_rAngularSpeed;
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for AngularMotor constraint.",
			__FILE__, __LINE__);
}

void AngularMotorConstraint::SetProperty(const String &p_strName, Real p_rValue)
{
	if (p_strName == "AngularSpeed")
	{
		m_rAngularSpeed = p_rValue;
		UpdateDerivativeVariables();
	}
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for AngularMotor constraint.",
			__FILE__, __LINE__);
}

void AngularMotorConstraint::GetProperty(const String &p_strName, TPoint3<Real> &p_ptValue) const
{
	if (p_strName == "BindAxisPoint1")
		p_ptValue = m_ptBindAxisPoint1;
	else if (p_strName == "BindAxisPoint2")
		p_ptValue = m_ptBindAxisPoint2;
	else if (p_strName == "CurrentAxisPosition1")
		p_ptValue = GetCurrentAxisPosition1();
	else if (p_strName == "CurrentAxisPosition2")
		p_ptValue = GetCurrentAxisPosition2();
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for AngularMotor constraint.",
			__FILE__, __LINE__);
			
}

void AngularMotorConstraint::SetProperty(const String &p_strName, const TPoint3<Real> &p_ptValue)
{
	if (p_strName == "BindAxisPoint1")
		m_ptBindAxisPoint1 = p_ptValue;
	else if (p_strName == "BindAxisPoint2")
		m_ptBindAxisPoint2 = p_ptValue;
	else if (p_strName == "CurrentAxisPosition1")
		throw new MesonException(
			"Property 'CurrentAxisPosition1' is read-only for AngularMotor constraint.",
			__FILE__, __LINE__);
	else if (p_strName == "CurrentAxisPosition2")
		throw new MesonException(
			"Property 'CurrentAxisPosition2' is read-only for AngularMotor constraint.",
			__FILE__, __LINE__);
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for AngularMotor constraint.",
			__FILE__, __LINE__);
}

void AngularMotorConstraint::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	if (!m_bBroken)
		p_pInstrumentationDevice->DrawLine(
			GetCurrentAxisPosition1(), GetCurrentAxisPosition2());
}

const String& AngularMotorConstraint::GetType(void) const
{
	return s_strType;
}

BodyPtr AngularMotorConstraint::GetBody1(void)
{
	return m_pBody1;
}

void AngularMotorConstraint::SetBody1(BodyPtr p_pBody1)
{
	m_pBody1 = p_pBody1;
}

BodyPtr AngularMotorConstraint::GetBody2(void)
{
	return m_pBody2;
}

void AngularMotorConstraint::SetBody2(BodyPtr p_pBody2)
{
	m_pBody2 = p_pBody2;
}

void AngularMotorConstraint::Bind(void)
{
	if (m_pBody1 == NULL) return;
	if (m_pBody2 == NULL) return;

	// compute axis anchor points for local body coords

	// body 1 anchors
	m_pBody1->GetKineticProperties().TransformPointToLocal(
		m_ptBindAxisPoint1, m_ptBody1Anchor1);
	m_pBody1->GetKineticProperties().TransformPointToLocal(
		m_ptBindAxisPoint2, m_ptBody1Anchor2);

	// body 2 anchors
	m_pBody2->GetKineticProperties().TransformPointToLocal(
		m_ptBindAxisPoint1, m_ptBody2Anchor1);
	m_pBody2->GetKineticProperties().TransformPointToLocal(
		m_ptBindAxisPoint2, m_ptBody2Anchor2);

	// compute offset perp to axis
	TVector3<Real> vecBody1AxisOffset(m_ptBody1Anchor2 - m_ptBody1Anchor1);
	m_vecBody1TorqueOffset = vecBody1AxisOffset ^ TVector3<Real>::Up;
	if (m_vecBody1TorqueOffset.IsZero())
		m_vecBody1TorqueOffset = vecBody1AxisOffset ^ TVector3<Real>::Right;
	Real rAxisLength1 = vecBody1AxisOffset.Length();
	m_vecBody1TorqueOffset.Normalise();
	m_vecBody1TorqueOffset *= rAxisLength1 * (Real) 0.5;

	TVector3<Real> vecWorldTorqueOffset
		= (m_pBody1->GetKineticProperties().Orientation * m_vecBody1TorqueOffset * m_pBody1->GetKineticProperties().Orientation.ConjugateCopy()).Vector;
	m_vecBody2TorqueOffset
		= (m_pBody2->GetKineticProperties().Orientation.ConjugateCopy() * m_vecBody1TorqueOffset * m_pBody2->GetKineticProperties().Orientation).Vector;

	UpdateDerivativeVariables();

	m_bBroken = false;
}

void AngularMotorConstraint::Unbind(void)
{
	m_bBroken = true;
}

bool AngularMotorConstraint::IsInstantaneous(void) const
{
	return false;
}

bool AngularMotorConstraint::IsBroken(void) const
{
	return m_bBroken;
}

void AngularMotorConstraint::SetBroken(bool p_bBroken)
{
	m_bBroken = p_bBroken;
}

Real AngularMotorConstraint::GetBreakingThreshold(void) const
{
	return m_rBreakingThreshold;
}

void AngularMotorConstraint::SetBreakingThreshold(Real p_rValue)
{
	m_rBreakingThreshold = p_rValue;
}

bool AngularMotorConstraint::CanRecomputeDiscrepancies(void)
{
	return true;
}

void AngularMotorConstraint::ComputeDiscrepancies(void)
{
	m_listConstraintDiscrepancies.Clear();

	// -- connection point

	// compute local anchor points in world space

	// axis anchor 1
	TPoint3<Real> ptWorldBody1Anchor1, ptWorldBody2Anchor1;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_ptBody1Anchor1, ptWorldBody1Anchor1);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_ptBody2Anchor1, ptWorldBody2Anchor1);

	// add connection discrepancy (may be nil, but still
	// enforces velocity constraint with zero restitution)
	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			ptWorldBody1Anchor1, ptWorldBody2Anchor1));

	// axis anchor 2
	TPoint3<Real> ptWorldBody1Anchor2, ptWorldBody2Anchor2;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_ptBody1Anchor2, ptWorldBody1Anchor2);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_ptBody2Anchor2, ptWorldBody2Anchor2);

	// add connection discrepancy (may be nil, but still
	// enforces velocity constraint with zero restitution)
	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			ptWorldBody1Anchor2, ptWorldBody2Anchor2));

	// -- induce rotation

	// compute torque offset in world space
	TQuaternion<Real>& qtnRotation1 = m_pBody1->GetKineticProperties().Orientation;
	TVector3<Real> vecWorld1TorqueOffset(
		(qtnRotation1 * TQuaternion<Real>(m_vecBody1TorqueOffset) * qtnRotation1.ConjugateCopy()).Vector );
	TQuaternion<Real>& qtnRotation2 = m_pBody2->GetKineticProperties().Orientation;
	TVector3<Real> vecWorld2TorqueOffset(
		(qtnRotation2 * TQuaternion<Real>(m_vecBody2TorqueOffset) * qtnRotation2.ConjugateCopy()).Vector );
	TVector3<Real> vecWorldTorqueOffset((vecWorld1TorqueOffset + vecWorld2TorqueOffset) * (Real) 0.5);

	// compute required delta velocity of points in world space
	TVector3<Real> vecWorld1DeltaVelocity(
		(qtnRotation1 * TQuaternion<Real>(m_vecBody1DeltaVelocity) * qtnRotation1.ConjugateCopy()).Vector );
	TVector3<Real> vecWorld2DeltaVelocity(
		(qtnRotation2 * TQuaternion<Real>(m_vecBody2DeltaVelocity) * qtnRotation2.ConjugateCopy()).Vector );
	TVector3<Real> vecWorldDeltaVelocity((vecWorld1DeltaVelocity + vecWorld2DeltaVelocity) * (Real) 0.5);

	// compute average axis anchor points
	TPoint3<Real> ptWorldBodyAnchor1(ptWorldBody1Anchor1);
	ptWorldBodyAnchor1 += ptWorldBody2Anchor1.ToVector();
	ptWorldBodyAnchor1.ToVector() *= (Real) 0.5;

	TPoint3<Real> ptWorldBodyAnchor2(ptWorldBody1Anchor2);
	ptWorldBodyAnchor2 += ptWorldBody2Anchor2.ToVector();
	ptWorldBodyAnchor2.ToVector() *= (Real) 0.5;

	// generate velocity discrepancies to induce rotation
	// around axis anchor points
	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			ptWorldBodyAnchor1 + vecWorldTorqueOffset,
			ptWorldBodyAnchor1 + vecWorldTorqueOffset,
			vecWorldDeltaVelocity, (Real) 0.4));

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			ptWorldBodyAnchor1 - vecWorldTorqueOffset,
			ptWorldBodyAnchor1 - vecWorldTorqueOffset,
			-vecWorldDeltaVelocity, (Real) 0.4));

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			ptWorldBodyAnchor2 + vecWorldTorqueOffset,
			ptWorldBodyAnchor2 + vecWorldTorqueOffset,
			vecWorldDeltaVelocity, (Real) 0.4));

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
			ptWorldBodyAnchor2 - vecWorldTorqueOffset,
			ptWorldBodyAnchor2 - vecWorldTorqueOffset,
			-vecWorldDeltaVelocity, (Real) 0.4));
}

const ConstraintDiscrepancyList& AngularMotorConstraint::GetDiscrepancies(void)
{
	return m_listConstraintDiscrepancies;
}
