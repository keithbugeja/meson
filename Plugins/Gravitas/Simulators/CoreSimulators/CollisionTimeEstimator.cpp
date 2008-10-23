#include "CollisionTimeEstimator.h"

#include "Sorting.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Simulation;

CollisionTimeEstimator::CollisionTimeEstimator(void)
{
}

CollisionTimeEstimator::~CollisionTimeEstimator(void)
{
}

void CollisionTimeEstimator::EstimateCollisions(
	Real p_rMaxTime,
	const TList<BodyPair> &p_listCandidateCollisions,
	TList<CollisionEstimate> &p_listCollisionEstimates)
{
	CollisionManager *pCollisionManager
		= GravitasEngine::GetInstance()->GetCollisionManager();

	p_listCollisionEstimates.Clear();

	for (size_t unIndex = 0; unIndex < p_listCandidateCollisions.Size(); unIndex++)
	{
		BodyPair& bodyPair = p_listCandidateCollisions(unIndex);
		BodyPtr pBody1 = bodyPair.Element1;
		BodyPtr pBody2 = bodyPair.Element2;

		BoundingVolumePtr pBoundingVolume1 = pBody1->GetBoundingVolume();
		BoundingVolumePtr pBoundingVolume2 = pBody2->GetBoundingVolume();

		// compute relative kinetic state
		KineticProperties& kineticProperties1 = pBody1->GetKineticProperties();
		KineticProperties& kineticProperties2 = pBody2->GetKineticProperties();
		KineticProperties kineticPropertiesRelative = kineticProperties2 - kineticProperties1;

		// if both bounding volumes available, do bounding test
		if (pBoundingVolume1 != NULL && pBoundingVolume2 != NULL)
		{
			// compute sweep from relative velocity x max time
			TVector3<Real> vecSweep = kineticPropertiesRelative.LinearVelocity * p_rMaxTime;
			if (!pBoundingVolume1->IntersectsDynamic(*pBoundingVolume2, vecSweep))
				continue; 
		}

		// do accurate geometry test
		GeometryPtr pGeometry1 = pBody1->GetGeometry();
		GeometryPtr pGeometry2 = pBody2->GetGeometry();

		// don't test if either geometry not set
		if (pGeometry1 == NULL) continue;
		if (pGeometry2 == NULL) continue;

		// don't test if corresponding handler not registered
		if (!pCollisionManager->HasCollisionDetector(pGeometry1, pGeometry2))
			continue;

		// get collision handler
		ICollisionDetector *pCollisionDetector
			= pCollisionManager->GetCollisionDetector(pGeometry1, pGeometry2);

		// compute relative placement and movement
		Transform trnPlacement1(kineticProperties1.Orientation, kineticProperties1.Position.ToVector());
		Transform trnPlacement2(kineticProperties2.Orientation, kineticProperties2.Position.ToVector());
		Transform trnRelativePlacement = trnPlacement2.ChangeBasisCopy(trnPlacement1);

		// estimate impact within max time, and ignore if no impact
		Real rImpactTime;
		if (!pCollisionDetector->EstimateImpact(*pGeometry1, *pGeometry2,
			trnRelativePlacement,
			kineticPropertiesRelative.LinearVelocity,
			kineticPropertiesRelative.AngularVelocity,
			p_rMaxTime, rImpactTime))
			continue;

		// add new estimate
		p_listCollisionEstimates.Add(CollisionEstimate(pBody1, pBody2, rImpactTime));
	}

	// sort estimates by most immediate first
	TSorting<CollisionEstimate>::QuickSort(p_listCollisionEstimates);
}
