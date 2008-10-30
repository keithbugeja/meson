#include "SequentialConstraintSolver.h"

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

void SequentialConstraintSolver::ResolveConstraintDiscrepancy(
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

	// -- resolve velocity --

	// get mass properties
	MassProperties& massProperties1 = pRigidBody1->GetMassProperties();
	MassProperties& massProperties2 = pRigidBody2->GetMassProperties();

	// get kinetic state for both bodies
	KineticProperties& kineticProperties1 = pRigidBody1->GetKineticProperties();
	KineticProperties& kineticProperties2 = pRigidBody2->GetKineticProperties();

	// compute impulse application points in world space

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

	// add artificial velocity component to induce rotational position correction
	vecDeltaVelocity += (p_constraintDiscrepancy.Point1 - p_constraintDiscrepancy.Point2)
		* p_constraintDiscrepancy.CorrectionFactor;

	// compute direction for required delta velocity
	TVector3<Real> vecDeltaNormal(vecDeltaVelocity);
	if (vecDeltaNormal.IsZero())
		vecDeltaNormal = TVector3<Real>::Right;
	else
		vecDeltaNormal.Normalise();

	// check if breaking limit reached
	Real rRelativeSpeedNormal(vecRelativeVelocity.Length());
	if (TMaths<Real>::Abs(rRelativeSpeedNormal) > p_pConstraint->GetBreakingThreshold())
	{
		p_pConstraint->SetBroken(true);
		EventManager* pEventManager = GravitasEngine::GetInstance()->GetEventManager();
		if (pEventManager->IsEnabled(EventFlags::ConstraintBroken))
			pEventManager->Notify(
				ConstraintEventPtr(
					new ConstraintEvent(p_pConstraint, ConstraintEventType::Broken)));
		return;
	}

	// compute impulse numerator
	Real rImpulseNumerator(vecDeltaVelocity.Length());

	// compute linear impulse denominator components
	Real rImpulseDenominatorLinear(massProperties1.MassInverse + massProperties2.MassInverse);

	// compute angular impulse denominator components
	TVector3<Real> vecOffset1(vecDiscrepancyPoint1 - kineticProperties1.Position);
	TVector3<Real> vecOffset2(vecDiscrepancyPoint2 - kineticProperties2.Position);

	Real rImpulseDenominatorAngular
		= (((massProperties1.InertiaTensorWorldInverse * (vecOffset1 ^ vecDeltaNormal)) ^ vecOffset1)
		+  ((massProperties2.InertiaTensorWorldInverse * (vecOffset2 ^ vecDeltaNormal)) ^ vecOffset2))
		* vecDeltaNormal;

	// compute normal impulse denominator
	Real rImpulseDenominator(rImpulseDenominatorLinear + rImpulseDenominatorAngular);

	// compute impulse magnitude
	Real rImpulse(rImpulseNumerator / rImpulseDenominator);

	// compute impulse vector
	TVector3<Real> vecImpulse(rImpulse * vecDeltaNormal
		* p_constraintDiscrepancy.CorrectionFactor);

	// update linear velocities
	kineticProperties1.LinearVelocity -= vecImpulse * massProperties1.MassInverse;
	kineticProperties2.LinearVelocity += vecImpulse * massProperties2.MassInverse;

	// compute impulsive torque w.r.t to each body
	TVector3<Real> vecImpulsiveTorque1(vecOffset1 ^ -vecImpulse);
	TVector3<Real> vecImpulsiveTorque2(vecOffset2 ^  vecImpulse);

	// update angular velocities
	kineticProperties1.AngularVelocity += massProperties1.InertiaTensorWorldInverse * vecImpulsiveTorque1;
	kineticProperties2.AngularVelocity += massProperties2.InertiaTensorWorldInverse * vecImpulsiveTorque2;

	// -- resolve position --
	Real rMassRatio(massProperties1.GetMassRatio(massProperties2));
	TVector3<Real> vecCorrectedOffset(vecDiscrepancyOffset
		* p_constraintDiscrepancy.CorrectionFactor);

	// reduce correction for non-recomputable constraints
	if (!p_pConstraint->CanRecomputeDiscrepancies())
		vecCorrectedOffset *= m_rIterationInverse;

	// apply linear translation
	kineticProperties1.Position += vecCorrectedOffset * ((Real) 1.0 - rMassRatio);
	kineticProperties2.Position -= vecCorrectedOffset * rMassRatio;
}

void SequentialConstraintSolver::ResolveConstraint(ConstraintPtr p_pConstraint, Real p_rCurrentTime)
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

SequentialConstraintSolver::SequentialConstraintSolver(void)
	: m_strName("Sequential")
	, m_activityManager()
	, m_unIterations(20)
	, m_rIterationInverse((Real) 0.05)
	, m_rActivityThreshold((Real) 0.1)
	, m_rActivityThresholdSquared((Real) 0.01)
{
}

SequentialConstraintSolver::~SequentialConstraintSolver(void)
{
}

void SequentialConstraintSolver::EnumerateProperties(
	TMap<Meson::Common::Text::String, PropertyDescriptor> &p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties["Iterations"]
		= PropertyDescriptor("Iterations", PropertyType::Integer, false);
	p_mapProperties["IterationInverse"]
		= PropertyDescriptor("IterationInverse", PropertyType::Real, true);
}

void SequentialConstraintSolver::GetProperty(const String& p_strName, int& p_nValue) const
{
	if (p_strName == "Iterations")
		p_nValue = m_unIterations;
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for sequential constraint solver.",
			__FILE__, __LINE__);
}

void SequentialConstraintSolver::SetProperty(const String& p_strName, int p_nValue)
{
	if (p_strName == "Iterations")
	{
		m_unIterations = p_nValue > 0 ? p_nValue : 1;
		m_rIterationInverse = (Real) 1.0 / (Real) m_unIterations;
	}
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for sequential constraint solver.",
			__FILE__, __LINE__);
}

void SequentialConstraintSolver::GetProperty(const String& p_strName, Real& p_rValue) const
{
	if (p_strName == "IterationInverse")
		p_rValue = m_rIterationInverse;
	else
		throw new MesonException(
			"Unknown property '" + p_strName + "' for sequential constraint solver.",
			__FILE__, __LINE__);
}

void SequentialConstraintSolver::SetProperty(const String& p_strName, Real p_rValue)
{
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
}

const String& SequentialConstraintSolver::GetName(void) const
{
	return m_strName;
}

void SequentialConstraintSolver::Solve(ConstraintBatch& p_constraintBatch)
{
	for (size_t unIterationIndex = 0; unIterationIndex < m_unIterations;
		unIterationIndex++)
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
}

