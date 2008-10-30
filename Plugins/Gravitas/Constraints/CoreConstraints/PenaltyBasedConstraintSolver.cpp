#include "PenaltyBasedConstraintSolver.h"

#include "GrvRigidBody.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Event;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;

void PenaltyBasedConstraintSolver::ResolveConstraintDiscrepancy(
	ConstraintPtr p_pConstraint, 
	const ConstraintDiscrepancy& p_constraintDiscrepancy)
{
	// check if constraint already broken
	if (p_pConstraint->IsBroken())
		return;

	// get discrepancy points in world space
	const TVector3<Real>& vecDiscrepancyPoint1 = p_constraintDiscrepancy.Point1;
	const TVector3<Real>& vecDiscrepancyPoint2 = p_constraintDiscrepancy.Point2;

	// compute offset between discrepancy points
	TVector3<Real> vecDiscrepancyOffset(vecDiscrepancyPoint2 - vecDiscrepancyPoint1);

	// cast bodies to rigid bodies
	RigidBody* pRigidBody1 = (RigidBody*) (IBody*)p_pConstraint->GetBody1();
	RigidBody* pRigidBody2 = (RigidBody*) (IBody*)p_pConstraint->GetBody2();

	// check if breaking limit reached
	Real rDiscrepancyLength(vecDiscrepancyOffset.Length());
	if (rDiscrepancyLength > p_pConstraint->GetBreakingThreshold())
	{
		p_pConstraint->SetBroken(true);

		// propagate awake state
		m_activityManager.PropagateActivity(p_pConstraint->GetBody1(), p_pConstraint->GetBody2());

		return;
	}

	// -- compute penalty forces --

	// get kinetic state for both bodies
	KineticProperties& kineticProperties1 = pRigidBody1->GetKineticProperties();
	KineticProperties& kineticProperties2 = pRigidBody2->GetKineticProperties();

	// compute relative velocity of discrepancy points
	TVector3<Real> vecVelocity1, vecVelocity2;
	kineticProperties1.ComputeWorldPointVelocity(vecDiscrepancyPoint1, vecVelocity1);
	kineticProperties2.ComputeWorldPointVelocity(vecDiscrepancyPoint2, vecVelocity2);
	TVector3<Real> vecRelativeVelocity(vecVelocity2 - vecVelocity1);

	// propagate awake state
	if (vecRelativeVelocity.LengthSquared() > m_rActivityThresholdSquared)
		m_activityManager.PropagateActivity(p_pConstraint->GetBody1(), p_pConstraint->GetBody2());

	// compute required change in velocity
	TVector3<Real> vecDeltaVelocity(p_constraintDiscrepancy.RelativeVelocity - vecRelativeVelocity);

	// -- positional penalty force --

	// compute direction of discrepancy
	TVector3<Real> vecDiscrepancyDirection(vecDiscrepancyOffset);
	if (vecDiscrepancyDirection.IsZero())
		vecDiscrepancyDirection = TVector3<Real>::Right;
	else
		vecDiscrepancyDirection.Normalise();

	// compute relative velocity along discrepancy
	Real rVelocityAlongDiscrepancy = vecRelativeVelocity * vecDiscrepancyDirection;

	// compute positional penalty force value with spring and damper components
	Real rPositionalPenaltyForce
		= rDiscrepancyLength * m_rPositionalSpringCoefficient
			- rVelocityAlongDiscrepancy * m_rPositionalDamperCoefficient;

	// compute positional penalty force vector
	TVector3<Real> vecPositionalPenaltyForce = vecDiscrepancyDirection * -rPositionalPenaltyForce;

	// -- delta velocity penalty force --

	// compute direction of delta velocity
	TVector3<Real> vecDeltaVelocityDirection(vecDeltaVelocity);
	if (vecDeltaVelocityDirection.IsZero())
		vecDeltaVelocityDirection = TVector3<Real>::Right;
	else
		vecDeltaVelocityDirection.Normalise();

	// compute relative velocity along delta velocity direction
	Real rVelocityAlongDeltaVelocity = vecRelativeVelocity * vecDeltaVelocityDirection;

	Real rDeltaVelocity = vecDeltaVelocity.Length();

	// compute delta velocity penalty force value with spring and damper components
	Real rVelocityPenaltyForce
		= rDeltaVelocity * m_rVelocitySpringCoefficient
			- rVelocityAlongDeltaVelocity * m_rVelocityDamperCoefficient;

	// compute velocity penalty force vector
	TVector3<Real> vecVelocityPenaltyForce = vecDeltaVelocityDirection * rVelocityPenaltyForce;

	// -- total penalty force --
	TVector3<Real> vecPenaltyForce(vecPositionalPenaltyForce + vecVelocityPenaltyForce);

	// get mass properties
	MassProperties& massProperties1 = pRigidBody1->GetMassProperties();
	MassProperties& massProperties2 = pRigidBody2->GetMassProperties();

	// compute mass coefficient
	Real rMassCoefficient = (Real) 1.0 / (massProperties1.MassInverse + massProperties2.MassInverse);

	// modulate penalty force with mass coefficient
	vecPenaltyForce *= rMassCoefficient;

	// create instantenaeous penalty force generators
	TVector3<Real> vecOffset1 = vecDiscrepancyPoint1 - kineticProperties1.Position;
	TVector3<Real> vecOffset2 = vecDiscrepancyPoint2 - kineticProperties2.Position;
	
	// NOTE: add custom penalty force implementation to avoid external dependency

	/*
	BasicForce* basicForcePenalty1 = new BasicForce(-vecPenaltyForce, ptOffset1, true);
	BasicForce* basicForcePenalty2 = new BasicForce(vecPenaltyForce, ptOffset2, true);

	pRigidBody1->GetForceAccumulator().AddForce(ForcePtr(basicForcePenalty1));
	pRigidBody2->GetForceAccumulator().AddForce(ForcePtr(basicForcePenalty2));
	*/
}

void PenaltyBasedConstraintSolver::ResolveConstraint(ConstraintPtr p_pConstraint, Real p_rCurrentTime)
{
	BodyPtr pBody1(p_pConstraint->GetBody1());
	BodyPtr pBody2(p_pConstraint->GetBody2());

	// only rigid bodies supported for now
	if (pBody1->GetType() != BodyType::Rigid) return;
	if (pBody2->GetType() != BodyType::Rigid) return;

	// do nothing if both bodies inactive
	if (!pBody1->GetKineticProperties().Active
		&& !pBody2->GetKineticProperties().Active)
		return;

	// do nothing if both bodies have infinite mass
	if (pBody1->GetMassProperties().HasInfiniteMass()
		&& pBody2->GetMassProperties().HasInfiniteMass())
		return;

	// compute discrepancies
	p_pConstraint->ComputeDiscrepancies();
	const ConstraintDiscrepancyList& listConstraintDiscrepancies
		= p_pConstraint->GetDiscrepancies();

	// solve discrepancies sequentially
	size_t unCount = listConstraintDiscrepancies.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		ResolveConstraintDiscrepancy(p_pConstraint,
			listConstraintDiscrepancies[unIndex]);

		// check if constraint already broken
		if (p_pConstraint->IsBroken())
			return;
	}
}

PenaltyBasedConstraintSolver::PenaltyBasedConstraintSolver(void)
	: m_strName("PenaltyBased")
	, m_activityManager()
	, m_rPositionalSpringCoefficient((Real) 1000.0)
	, m_rPositionalDamperCoefficient((Real) 0.0)
	, m_rVelocitySpringCoefficient((Real) 0.0)
	, m_rVelocityDamperCoefficient((Real) 0.0)
	, m_rActivityThreshold((Real) 0.1)
	, m_rActivityThresholdSquared((Real) 0.01)
{
}

PenaltyBasedConstraintSolver::~PenaltyBasedConstraintSolver(void)
{
}

void PenaltyBasedConstraintSolver::EnumerateProperties(
	TMap<Meson::Common::Text::String, PropertyDescriptor> &p_mapProperties) const
{
	p_mapProperties.Clear();
	/*
	p_mapProperties["Iterations"]
		= PropertyDescriptor("Iterations", PropertyType::Integer, false);
	p_mapProperties["IterationInverse"]
		= PropertyDescriptor("IterationInverse", PropertyType::Real, true);
		*/
}

void PenaltyBasedConstraintSolver::GetProperty(const String& p_strName, Real& p_rValue) const
{
	/*
	if (p_strName == "IterationInverse")
		p_rValue = m_rIterationInverse;
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for sequential constraint solver.",
			__FILE__, __LINE__);
			*/
}

void PenaltyBasedConstraintSolver::SetProperty(const String& p_strName, Real p_rValue)
{
	/*
	if (p_strName == "IterationInverse")
	{
		throw new MesonException(
			"Property 'IterationInverse' is read-only.",
			__FILE__, __LINE__);
	}
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for sequential constraint solver.",
			__FILE__, __LINE__);
	*/
}

const String& PenaltyBasedConstraintSolver::GetName(void) const
{
	return m_strName;
}

void PenaltyBasedConstraintSolver::Solve(ConstraintBatch& p_constraintBatch)
{
	size_t unConstraintCount = p_constraintBatch.Constraints.Size();
	for (size_t unConstraintIndex = 0; unConstraintIndex < unConstraintCount;)
	{
		ConstraintPtr pConstraint = p_constraintBatch.Constraints[unConstraintIndex];
		if (pConstraint->IsBroken())
		{
			// remove broken constraint
			p_constraintBatch.Constraints.RemoveAt(unConstraintIndex);
			--unConstraintCount;
		}
		else
		{
			// process this constraint and proceed
			ResolveConstraint(pConstraint, p_constraintBatch.Time);
			++unConstraintIndex;
		}
	}
}

