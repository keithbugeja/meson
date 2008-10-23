#include "RigidJointConstraint.h"

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

String RigidJointConstraint::s_strType("RigidJoint");

RigidJointConstraint::RigidJointConstraint(void)
	: m_pBody1(NULL)
	, m_pBody2(NULL)
	, m_bBroken(true)
	, m_rBreakingThreshold(TMaths<Real>::Maximum)
	, m_ptBody1Anchor2()
	, m_ptBody2Anchor1()
	, m_ptBody1Anchor3()
	, m_ptBody2Anchor3()
	, m_listConstraintDiscrepancies()
{
}

RigidJointConstraint::RigidJointConstraint(const String& p_strId)
	: IConstraint(p_strId)
	, m_pBody1(NULL)
	, m_pBody2(NULL)
	, m_bBroken(true)
	, m_rBreakingThreshold(TMaths<Real>::Maximum)
	, m_ptBody1Anchor2()
	, m_ptBody2Anchor1()
	, m_ptBody1Anchor3()
	, m_ptBody2Anchor3()
	, m_listConstraintDiscrepancies()
{
}

RigidJointConstraint::~RigidJointConstraint(void)
{
}

void RigidJointConstraint::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	TPoint3<Real>& ptBodyPosition1 = m_pBody1->GetKineticProperties().Position;
	TPoint3<Real>& ptBodyPosition2 = m_pBody2->GetKineticProperties().Position;

	p_pInstrumentationDevice->DrawLine(ptBodyPosition1, ptBodyPosition2);
}

const String& RigidJointConstraint::GetType(void) const
{
	return s_strType;
}

BodyPtr RigidJointConstraint::GetBody1(void)
{
	return m_pBody1;
}

void RigidJointConstraint::SetBody1(BodyPtr p_pBody1)
{
	m_pBody1 = p_pBody1;
}

BodyPtr RigidJointConstraint::GetBody2(void)
{
	return m_pBody2;
}

void RigidJointConstraint::SetBody2(BodyPtr p_pBody2)
{
	m_pBody2 = p_pBody2;
}

void RigidJointConstraint::Bind(void)
{
	if (m_pBody1 == NULL) return;
	if (m_pBody2 == NULL) return;

	// compute axis anchor points for local body coords

	TPoint3<Real>& ptPosition1
		= m_pBody1->GetKineticProperties().Position;
	TPoint3<Real>& ptPosition2
		= m_pBody2->GetKineticProperties().Position;

	// body 1 and 2 distance anchors
	m_pBody1->GetKineticProperties().TransformPointToLocal(
		ptPosition2, m_ptBody1Anchor2);
	m_pBody2->GetKineticProperties().TransformPointToLocal(
		ptPosition1, m_ptBody2Anchor1);

	// body rotation anchor
	TVector3<Real> vecOffset(ptPosition2 - ptPosition1);
	Real rHalfDistance; 
	TVector3<Real> vecNormal1, vecNormal2;
	if (vecOffset.IsZero())
	{
		vecNormal1 = TVector3<Real>::Right;
		rHalfDistance = (Real) 1.0;
	}
	else
	{
		vecOffset.ComputeOrthonormals(vecNormal1, vecNormal2);
		rHalfDistance = vecOffset.Length() * (Real) 0.5;
	}
	TPoint3<Real> ptRotationAnchorWorld
		= ptPosition1 + vecOffset * (Real) 0.5 + vecNormal1 * rHalfDistance;
	m_pBody1->GetKineticProperties().TransformPointToLocal(
		ptRotationAnchorWorld, m_ptBody1Anchor3);
	m_pBody2->GetKineticProperties().TransformPointToLocal(
		ptRotationAnchorWorld, m_ptBody2Anchor3);

	m_bBroken = false;
}

void RigidJointConstraint::Unbind(void)
{
	m_bBroken = true;
}

bool RigidJointConstraint::IsInstantaneous(void) const
{
	return false;
}

bool RigidJointConstraint::IsBroken(void) const
{
	return m_bBroken;
}

void RigidJointConstraint::SetBroken(bool p_bBroken)
{
	m_bBroken = p_bBroken;
}

Real RigidJointConstraint::GetBreakingThreshold(void) const
{
	return m_rBreakingThreshold;
}

void RigidJointConstraint::SetBreakingThreshold(Real p_rValue)
{
	m_rBreakingThreshold = p_rValue;
}

bool RigidJointConstraint::CanRecomputeDiscrepancies(void)
{
	return true;
}

void RigidJointConstraint::ComputeDiscrepancies(void)
{
	m_listConstraintDiscrepancies.Clear();

	// -- connection point

	// compute local anchor points in world space

	// axis anchor 1
	TPoint3<Real> ptWorldBody2Anchor1;
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_ptBody2Anchor1, ptWorldBody2Anchor1);

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(m_pBody1->GetKineticProperties().Position,
		ptWorldBody2Anchor1));

	// axis anchor 2
	TPoint3<Real> ptWorldBody1Anchor2;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_ptBody1Anchor2, ptWorldBody1Anchor2);

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(ptWorldBody1Anchor2,
		m_pBody2->GetKineticProperties().Position));

	// axis anchor 3
	TPoint3<Real> ptWorldBody1Anchor3, ptWorldBody2Anchor3;
	m_pBody1->GetKineticProperties().TransformPointToWorld(
		m_ptBody1Anchor3, ptWorldBody1Anchor3);
	m_pBody2->GetKineticProperties().TransformPointToWorld(
		m_ptBody2Anchor3, ptWorldBody2Anchor3);

	m_listConstraintDiscrepancies.Add(
		ConstraintDiscrepancy(
		ptWorldBody1Anchor3, ptWorldBody2Anchor3, TVector3<Real>::Zero, (Real) 0.2));
}

const ConstraintDiscrepancyList& RigidJointConstraint::GetDiscrepancies(void)
{
	return m_listConstraintDiscrepancies;
}
