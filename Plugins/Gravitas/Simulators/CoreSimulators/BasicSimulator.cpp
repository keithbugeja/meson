#include "BasicSimulator.h"

#include "GrvGravitasEngine.h"
#include "GrvContactConstraint.h"

using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Simulation;
using namespace Meson::Gravitas::Space;

void BasicSimulator::IntegrateVelocity(Real p_rSubDeltaTime)
{
	KineticIntegratorPtr pKineticIntegrator
		= m_pEnvironment->KineticIntegrator;

	TEnumerator<BodyPtr>& enumBodies = m_pEnvironment->Space->GetBodies();
	while (enumBodies.HasMoreElements())
	{
		BodyPtr pBody = enumBodies.NextElement();

		if (pBody->GetType() != BodyType::Rigid)
			continue;

		KineticProperties& kineticProperties = pBody->GetKineticProperties();
		if (!kineticProperties.Active)
			continue;

		ForceAccumulator& forceAccumulator
			= pBody->GetForceAccumulator();
		IVectorTimeFunction& linearAcceleration
			= forceAccumulator.GetLinearAcceleration();
		IVectorTimeFunction& angularAcceleration
			= forceAccumulator.GetAngularAcceleration();

		pKineticIntegrator->IntegrateVelocity(m_rCurrentTime, p_rSubDeltaTime,
			linearAcceleration, angularAcceleration, kineticProperties);

		// clear contact and other instantaneous force generators
		forceAccumulator.ClearInstantaneousForces();

		// sleep objects if static
		m_activityManager.SuspendActivity(pBody);
	}
}

void BasicSimulator::IntegratePosition(Real p_rSubDeltaTime)
{
	KineticIntegratorPtr pKineticIntegrator
		= m_pEnvironment->KineticIntegrator;

	TEnumerator<BodyPtr>& enumBodies = m_pEnvironment->Space->GetBodies();
	while (enumBodies.HasMoreElements())
	{
		BodyPtr pBody = enumBodies.NextElement();

		if (pBody->GetType() != BodyType::Rigid)
			continue;

		KineticProperties& kineticProperties = pBody->GetKineticProperties();
		if (!kineticProperties.Active)
			continue;

		ForceAccumulator& forceAccumulator
			= pBody->GetForceAccumulator();
		IVectorTimeFunction& linearAcceleration
			= forceAccumulator.GetLinearAcceleration();
		IVectorTimeFunction& angularAcceleration
			= forceAccumulator.GetAngularAcceleration();

		pKineticIntegrator->IntegratePosition(m_rCurrentTime, p_rSubDeltaTime,
			linearAcceleration, angularAcceleration, kineticProperties);

		// update inertia inverse in world space
		TMatrix3<Real>& matInertiaTensorWorldInverse
			= pBody->GetMassProperties().InertiaTensorWorldInverse;
		if (!matInertiaTensorWorldInverse.IsZero())
		{
			TMatrix3<Real> matOrientation;
			pBody->GetKineticProperties().Orientation.ExtractMatrix(matOrientation);
			TMatrix3<Real> matOrientationInverse = matOrientation.TransposeCopy();
			matInertiaTensorWorldInverse
				= matOrientation * pBody->GetMassProperties().InertiaTensorInverse * matOrientationInverse;
		}

		// clear contact and other instantaneous force generators
		//forceAccumulator.ClearInstantaneousForceGenerators();
	}

	// keep space updated
	m_pEnvironment->Space->Update();
}

void BasicSimulator::DetectCollisions(void)
{
	// detect and resolve collisions
	CollisionManager* pCollisionManager
		= GravitasEngine::GetInstance()->GetCollisionManager();

	for (size_t unIndex = 0; unIndex < m_listCandidateCollisions.Size(); unIndex++)
	{
		BodyPair& bodyPair = m_listCandidateCollisions(unIndex);
		BodyPtr pBody1 = bodyPair.Element1;
		BodyPtr pBody2 = bodyPair.Element2;

		// don't test if either geometry not set
		if (pBody1->GetGeometry() == NULL) continue;
		if (pBody2->GetGeometry() == NULL) continue;

		if (!pCollisionManager->HasCollisionDetector(pBody1->GetGeometry(), pBody2->GetGeometry()))
		{
			// try swapping bodies
			BodyPtr pBodyTemp = pBody1;
			pBody1 = pBody2;
			pBody2 = pBodyTemp;

			// don't test if no suitable detector found
			if (!pCollisionManager->HasCollisionDetector(pBody1->GetGeometry(), pBody2->GetGeometry()))
				continue;
		}

		GeometryPtr pGeometry1 = pBody1->GetGeometry();
		GeometryPtr pGeometry2 = pBody2->GetGeometry();

		BoundingVolumePtr pBoundingVolume1 = pBody1->GetBoundingVolume();
		BoundingVolumePtr pBoundingVolume2 = pBody2->GetBoundingVolume();

		// compute relative kinetic state
		KineticProperties& kineticProperties1 = pBody1->GetKineticProperties();
		KineticProperties& kineticProperties2 = pBody2->GetKineticProperties();
		KineticProperties kineticPropertiesRelative = kineticProperties2 - kineticProperties1;

		// if both bounding volumes available, do bounding test
		if (pBoundingVolume1 != NULL && pBoundingVolume2 != NULL)
		{
			if (!pBoundingVolume1->Intersects(*pBoundingVolume2))
				continue; 
		}

		// do accurate geometry test
		// get collision handler
		ICollisionDetector *pCollisionDetector
			= pCollisionManager->GetCollisionDetector(pGeometry1, pGeometry2);

		// compute relative placement and movement
		Transform trnPlacement1(kineticProperties1.Orientation, kineticProperties1.Position);
		Transform trnPlacement2(kineticProperties2.Orientation, kineticProperties2.Position);
		Transform trnRelativePlacement = trnPlacement2.ChangeBasisCopy(trnPlacement1);

		// test for collision
		ContactManifold contactManifold;
		if (!pCollisionDetector->TestIntersection(
			*pGeometry1, *pGeometry2, trnRelativePlacement))
			continue;

		// compute contacts
		pCollisionDetector->ComputeContactManifold(*pGeometry1, *pGeometry2,
			trnRelativePlacement, contactManifold);

		// do nothing if no contacts detected
		if (contactManifold.ContactPoints.Size() == 0)
			continue;

		// awaken objects if either is asleep
		//kineticProperties1.PropagateActivity(kineticProperties2, m_rCurrentTime);
		m_activityManager.PropagateActivity(pBody1, pBody2);

		// transform contact set to world space
		contactManifold.Transform(trnPlacement1);

		// fill in rest of contact set
		contactManifold.Body1 = pBody1;
		contactManifold.Body2 = pBody2;

		contactManifold.UpdateContactVelocities();

		// perturb contact normals if applicable
		contactManifold.PerturbContactNormals();

		// create non-penetration constraint and add to list
		ConstraintPtr pConstraintContact(new ContactConstraint(contactManifold));
		m_listConstraints.Add(pConstraintContact);

		// report collision
		m_collisionTracker.ReportCollision(contactManifold);
	}
}

void BasicSimulator::SolveConstraints(void)
{
	// get constraint solver - do nothing if not set
	IConstraintSolver* pConstraintSolver
		= m_pEnvironment->ConstraintSolver;
	if (pConstraintSolver == NULL) return;

	size_t unCount = m_listConstraintBatches.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
		pConstraintSolver->Solve(m_listConstraintBatches[unIndex]);
}

void BasicSimulator::SubStep(Real p_rSubDeltaTime)
{
	// get space - do nothing if not set
	ISpace *pSpace = m_pEnvironment->Space;
	if (pSpace == NULL) return;

	// get integrator - do nothing if not set
	KineticIntegratorPtr pKineticIntegrator
		= m_pEnvironment->KineticIntegrator;
	if (pKineticIntegrator == NULL) return;

	// prepare list of potential collisions from space
	pSpace->ComputeCandidateCollisions(m_listCandidateCollisions);
	/*
	GravitasEngine::GetInstance()->Logger().Out
		<< "All combs: " << (int) (m_pEnvironment->Bodies->Size() * (m_pEnvironment->Bodies->Size() - 1) / 2)
		<< " Filtered: " << (int) m_listCandidateCollisions.Size() << "\n";
		*/

	// apply filter if available
	CollisionFilter* pCollisionFilter = m_pEnvironment->CollisionFilter;
	if (pCollisionFilter != NULL)
		pCollisionFilter->FilterList(m_listCandidateCollisions);

	// integrate position and velocity
	IntegratePosition(p_rSubDeltaTime);
	IntegrateVelocity(p_rSubDeltaTime);

	// clear constraints for this step
	m_listConstraints.Clear();

	// add explicit constraints
	m_listConstraints.AddList(*m_pEnvironment->Constraints);

	// detect collisions and add non-penetration constraints
	DetectCollisions();

	// aggregate constraints into batches
	m_constraintBatchAggregator.GenerateBatches(
		m_listConstraints, m_rCurrentTime, m_listConstraintBatches);

	// solve constraints
	SolveConstraints();
} 

BasicSimulator::BasicSimulator(void)
	: ISimulator()
	, m_pEnvironment(NULL)
	, m_activityManager()
	, m_constraintBatchAggregator()
	, m_collisionTracker()
	, m_rCurrentTime((Real) 0.0)
	, m_listCandidateCollisions()
	, m_listConstraints()
	, m_listConstraintBatches()
{
}

BasicSimulator::BasicSimulator(const String& p_strId)
	: ISimulator(p_strId)
	, m_pEnvironment(NULL)
	, m_activityManager()
	, m_constraintBatchAggregator()
	, m_collisionTracker()
	, m_rCurrentTime((Real) 0.0)
	, m_listCandidateCollisions()
	, m_listConstraints()
	, m_listConstraintBatches()
{
}

BasicSimulator::~BasicSimulator(void)
{
}

void BasicSimulator::Initialise(EnvironmentPtr p_pEnvironment)
{
	m_pEnvironment = p_pEnvironment;
}

void BasicSimulator::Terminate(void)
{
	m_pEnvironment = NULL;
}

void BasicSimulator::Step(Real p_rCurrentTime, Real p_rDeltaTime)
{
	m_activityManager.SetCurrentTime(p_rCurrentTime);
	m_rCurrentTime = p_rCurrentTime;
	Real p_rSubDeltaTime = (Real) 0.01;
	while (p_rDeltaTime > (Real) 0.0)
	{
		p_rSubDeltaTime = TMaths<Real>::Min(p_rSubDeltaTime, p_rDeltaTime);
		SubStep(p_rSubDeltaTime);
		m_rCurrentTime += p_rSubDeltaTime;
		m_activityManager.SetCurrentTime(m_rCurrentTime);
		p_rDeltaTime -= p_rSubDeltaTime;
	}

	// generate collision events
	m_collisionTracker.GenerateCollisionEvents();
} 
