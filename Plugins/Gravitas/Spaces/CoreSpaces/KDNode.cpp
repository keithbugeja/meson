#include "KDNode.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Memory;
using namespace Meson::Common::System;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Space;

MemoryManager* Meson::Gravitas::Space::GetKDTreeNodeMemoryManager(void)
{
	MemoryManager* pMemoryManagerSpaces = GetSpaceMemoryManager();
	static MemoryManager s_memoryManagerKDTreeNodes
		= MemoryManager("KD-TRee Nodes", pMemoryManagerSpaces, sizeof(KDNode), 1024);
	return &s_memoryManagerKDTreeNodes;
}

KDNode::KDNode(void)
	: Parent(NULL)
	, Type(KDNode::X)
	, Partition((Real) 0.0)
	, Bodies()
{
	Child[0] = Child[1] = NULL;
}

KDNode::~KDNode(void)
{
	if (Child[0] != NULL)
		delete Child[0];

	if (Child[1] != NULL)
		delete Child[1];
}

void* KDNode::operator new(size_t p_unSize)
{
	return GetKDTreeNodeMemoryManager()->AllocatePooled(p_unSize);
}

void KDNode::operator delete(void* p_pObject, size_t p_unSize)
{
	::operator delete(p_pObject);
}							

bool KDNode::IsLeaf(void) const
{
	return Child[0] == NULL && Child[1] == NULL;
}

KDNode::PartitionStatus KDNode::GetPartitionStatus(
	const IBoundingVolume& p_boundingVolume) const
{
	const TVector3<Real>& vecAxis = KDAxes[Type];
	static TInterval<Real> itvProjection;
	p_boundingVolume.ProjectToInterval(vecAxis, itvProjection);

	if (itvProjection.Min > Partition)
		return KDNode::Positive;
	else if (itvProjection.Max < Partition)
		return KDNode::Negative;
	else
		return KDNode::Straddling;
}


void KDNode::UpdateNode(void)
{
	for (size_t unIndex = 0; unIndex < Bodies.Size();)
	{
		BodyPtr pBody = Bodies[unIndex];
		BoundingVolumePtr pBoundingVolume = pBody->GetBoundingVolume();
		if (pBoundingVolume == NULL)
		{
			unIndex++;
			continue;
		}

		// check if parent node is straddled, and move up if needed
		if (Parent != NULL)
		{
			PartitionStatus partitionStatusComputed
				= Parent->GetPartitionStatus(*pBoundingVolume);

			PartitionStatus partitionStatusActual
				= (Parent->Child[0] == this) ? KDNode::Negative : KDNode::Positive;

			if (partitionStatusComputed != partitionStatusActual)
			{
				Bodies.RemoveAt(unIndex);
				Parent->Bodies.Add(pBody);
				continue;
			}
		}

		if (!IsLeaf())
		{
			PartitionStatus partitionStatus	= GetPartitionStatus(*pBoundingVolume);

			// check if current body can be pushed down to a child
			if (partitionStatus == KDNode::Positive && Child[1] != NULL)
			{
				// can push down body to positive child if available
				Bodies.RemoveAt(unIndex);
				Child[1]->Bodies.Add(pBody);
				continue;
			}
			else if (partitionStatus == KDNode::Negative && Child[0] != NULL)
			{
				// can push down body to negative child if available
				Bodies.RemoveAt(unIndex);
				Child[0]->Bodies.Add(pBody);
				continue;
			}
		}

		unIndex++;
	}

	// recursively update child nodes if available
	if (Child[0] != NULL)
		Child[0]->UpdateNode();
	if (Child[1] != NULL)
		Child[1]->UpdateNode();
}

void KDNode::ComputeCandidateCollisions(
	BodyPtr p_pBodyExternal,
	BodyPairList& p_listCandidateCollisions)
{
	// do collision candidates with owned bodies
	bool bInfiniteMassExternal = p_pBodyExternal->GetMassProperties().HasInfiniteMass();
	for (size_t unIndex = 0; unIndex < Bodies.Size(); unIndex++)
	{
		BodyPtr pBody = Bodies[unIndex];

		// ignore potential collision between bodies both of infinite mass
		if (bInfiniteMassExternal && pBody->GetMassProperties().HasInfiniteMass())
			continue;

		if (p_pBodyExternal->GetKineticProperties().Active
			|| pBody->GetKineticProperties().Active)
			p_listCandidateCollisions.Add(BodyPair(p_pBodyExternal, pBody));
	}

	// add children's collision candidates
	if (Child[0] != NULL)
		Child[0]->ComputeCandidateCollisions(p_pBodyExternal, p_listCandidateCollisions);
	if (Child[1] != NULL)
		Child[1]->ComputeCandidateCollisions(p_pBodyExternal, p_listCandidateCollisions);
}

void KDNode::ComputeCandidateCollisions(
	BodyPairList& p_listCandidateCollisions)
{
	// do collision candidates with owned bodies
	for (size_t unIndex1 = 0; unIndex1 < Bodies.Size(); unIndex1++)
	{
		BodyPtr pBody1 = Bodies[unIndex1];
		bool bInfiniteMass1 = pBody1->GetMassProperties().HasInfiniteMass();

		for (size_t unIndex2 = unIndex1 + 1; unIndex2 < Bodies.Size(); unIndex2++)
		{
			BodyPtr pBody2 = Bodies[unIndex2];

			// ignore potential collision between bodies both of infinite mass
			if (bInfiniteMass1 && pBody2->GetMassProperties().HasInfiniteMass())
				continue;

			if (pBody1->GetKineticProperties().Active
				|| pBody2->GetKineticProperties().Active)
				p_listCandidateCollisions.Add(BodyPair(pBody1, pBody2));
		}

		// also compute candidates of straddling bodies with child nodes
		if (Child[0] != NULL)
			Child[0]->ComputeCandidateCollisions(pBody1, p_listCandidateCollisions);
		if (Child[1] != NULL)
			Child[1]->ComputeCandidateCollisions(pBody1, p_listCandidateCollisions);
	}

	// add children's collision candidates
	if (Child[0] != NULL)
		Child[0]->ComputeCandidateCollisions(p_listCandidateCollisions);
	if (Child[1] != NULL)
		Child[1]->ComputeCandidateCollisions(p_listCandidateCollisions);
}

void KDNode::IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
	BodyRayIntersectionList& p_listBodyRayIntersections) const
{
	TPoint3<Real> ptIntersection;
	Transform trnWorld, trnLocal;
	size_t unBodyCount = Bodies.Size();
	for (size_t unBodyIndex = 0; unBodyIndex < unBodyCount; unBodyIndex++)
	{
		BodyPtr pBody = Bodies[unBodyIndex];

		// if bounding volume available, do quick cull test
		BoundingVolumePtr pBoundingVolume = pBody->GetBoundingVolume();
		if (pBoundingVolume != NULL && !pBoundingVolume->Intersects(p_ray))
			continue;

		// prepare world and local transforms
		KineticProperties& kineticProperties = pBody->GetKineticProperties();
		trnWorld.Translation = kineticProperties.Position.ToVector();
		trnWorld.Rotation = kineticProperties.Orientation;
		trnLocal = trnWorld.InvertCopy();

		// transform ray into local geometry space
		Ray rayLocal(p_ray);
		rayLocal.Transform(trnLocal);

		// do local ray cast but skip on failure
		GeometryPtr pGeometry(pBody->GetGeometry());
		if (!pGeometry->IntersectsRay(rayLocal, ptIntersection))
			continue;
		
		// transform local intersection point to world coords
		trnWorld.Apply(ptIntersection);

		// insert in sorted order
		BodyRayIntersection bodyRayIntersection(p_ray, pBody, ptIntersection);
		size_t unListCount = p_listBodyRayIntersections.Size();
		int nInsertIndex = 0;
		for (size_t unListIndex = 0; unListIndex < unListCount; unListIndex++)
		{
			if (p_listBodyRayIntersections[unListIndex] >= bodyRayIntersection)
			{
				nInsertIndex = (int) unListIndex;
				break;
			}
		}
		if (nInsertIndex >= 0)
			p_listBodyRayIntersections.Insert(nInsertIndex,
				bodyRayIntersection);
		else
			p_listBodyRayIntersections.Add(
				bodyRayIntersection);
	}

	// if leaf node, do nothing else
	if (IsLeaf())
		return;

	// determine which partition contains ray source
	Real rSourceProjection = p_ray.Source.ToVector() * KDAxes[Type];
	Real rDirectionProjection = p_ray.Direction * KDAxes[Type];

	if (rSourceProjection < Partition)
	{
		// ray source is in negative partition
		if (Child[0] != NULL)
			Child[0]->IntersectRay(p_ray, p_listBodyRayIntersections);

		// test other partition if ray pointing towards it
		if (rDirectionProjection > (Real) 0.0 && Child[1] != NULL)
		{
			// project ray into partition
			Real rDistance = (Partition - rSourceProjection) / rDirectionProjection;
			Ray rayProjected;
			rayProjected.Source = p_ray.ComputePosition(rDistance);
			rayProjected.Direction = p_ray.Direction;

			// apply projected ray to node
			Child[1]->IntersectRay(rayProjected, p_listBodyRayIntersections);
		}
	}
	else
	{
		// ray source is in positive partition
		if (Child[1] != NULL)
			Child[1]->IntersectRay(p_ray, p_listBodyRayIntersections);

		// test other partition if ray pointing towards it
		if (rDirectionProjection < (Real) 0.0 && Child[0] != NULL)
		{
			// project ray into partition
			Real rDistance = (Partition - rSourceProjection) / rDirectionProjection;
			Ray rayProjected;
			rayProjected.Source = p_ray.ComputePosition(rDistance);
			rayProjected.Direction = p_ray.Direction;

			// apply projected ray to node
			Child[0]->IntersectRay(rayProjected, p_listBodyRayIntersections);
		}
	}
}

void KDNode::RenderInstrumentation(IInstrumentationDevice* p_pInstrumentationDevice,
	const BoundingAxisAlignedBox& p_boundingAxisAlignedBox)
{
	p_pInstrumentationDevice->DrawBox(p_boundingAxisAlignedBox.Centre(), p_boundingAxisAlignedBox.Extent());

	if (Child[0] == NULL || Child[1] == NULL)
		return;

	BoundingAxisAlignedBox boundingAxisAlignedBoxChild0(p_boundingAxisAlignedBox);
	BoundingAxisAlignedBox boundingAxisAlignedBoxChild1(p_boundingAxisAlignedBox);
	switch (Type)
	{
	case KDNode::X:
		boundingAxisAlignedBoxChild0.Max.X = boundingAxisAlignedBoxChild1.Min.X = Partition;
		break;
	case KDNode::Y:
		boundingAxisAlignedBoxChild0.Max.Y = boundingAxisAlignedBoxChild1.Min.Y = Partition;
		break;
	default:
		boundingAxisAlignedBoxChild0.Max.Z = boundingAxisAlignedBoxChild1.Min.Z = Partition;
		break;
	}

	Child[0]->RenderInstrumentation(p_pInstrumentationDevice, boundingAxisAlignedBoxChild0);
	Child[1]->RenderInstrumentation(p_pInstrumentationDevice, boundingAxisAlignedBoxChild1);
}
