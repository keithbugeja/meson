#include "KDTreeSpace.h"

#include "Sorting.h"
#include "MesonException.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::System;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Space;

KDNode* KDTreeSpace::PartitionBodies(uint p_unLevel, BodyList& p_listBodies)
{
	KDNode* pKDNode = new KDNode();

	if (p_listBodies.Size() == 0)
		return pKDNode;

	// do not partition further if any limit parameter satisfied
	if (p_unLevel >= m_unMaximumDepth || p_listBodies.Size() <= m_unMinimumBodiesPerNode)
	{
		pKDNode->Bodies = p_listBodies;
		return pKDNode;
	}

	// determine largest span
	Real rLargestSpan = (Real) 0.0, rBestMin = (Real) 0.0, rBestMax = (Real) 0.0;
	TInterval<Real> itvProjection;
	for (int nAxisIndex = 0; nAxisIndex < 3; nAxisIndex++)
	{
		Real rMin = TMaths<Real>::Maximum;
		Real rMax = -TMaths<Real>::Maximum;
		for (size_t unBodyIndex = 0; unBodyIndex < p_listBodies.Size(); unBodyIndex++)
		{
			BodyPtr pBody = p_listBodies[unBodyIndex];
			BoundingVolumePtr pBoundingVolume = pBody->GetBoundingVolume();
			if (pBoundingVolume == NULL) continue;

			pBoundingVolume->ProjectToInterval(KDAxes[nAxisIndex], itvProjection);
			rMin = TMaths<Real>::Min(rMin, itvProjection.Min);
			rMax = TMaths<Real>::Max(rMax, itvProjection.Max);
		}

		Real rSpan(rMax - rMin);
		if (rLargestSpan < rSpan)
		{
			pKDNode->Type = (KDNode::KDNodeType) nAxisIndex;
			rLargestSpan = rSpan;
			rBestMin = rMin;
			rBestMax = rMax;
		}
	}

	// set node partition type
	pKDNode->Partition = (rBestMin + rBestMax) * (Real) 0.5;

	BodyArrayList listBodiesPositive, listBodiesNegative;

	// partition bodies
	const TVector3<Real>& vecAxis = KDAxes[pKDNode->Type];
	for (size_t unBodyIndex = 0; unBodyIndex < p_listBodies.Size(); unBodyIndex++)
	{
		BodyPtr pBody = p_listBodies[unBodyIndex];
		BoundingVolumePtr pBoundingVolume = pBody->GetBoundingVolume();
		if (pBoundingVolume == NULL)
		{
			pKDNode->Bodies.Add(pBody);
			continue;
		}

		switch (pKDNode->GetPartitionStatus(*pBoundingVolume))
		{
		case KDNode::Positive:
			listBodiesPositive.Add(pBody);
			break;
		case KDNode::Negative:
			listBodiesNegative.Add(pBody);
			break;
		default: // straddling
			pKDNode->Bodies.Add(pBody);
			break;
		}
	}

	// recusrively partition bodies
	if (listBodiesPositive.Size() > 0)
	{
		pKDNode->Child[1] = PartitionBodies(p_unLevel + 1, listBodiesPositive);
		pKDNode->Child[1]->Parent = pKDNode;
	}

	if (listBodiesNegative.Size() > 0)
	{
		pKDNode->Child[0] = PartitionBodies(p_unLevel + 1, listBodiesNegative);
		pKDNode->Child[0]->Parent = pKDNode;
	}

	return pKDNode;
}

bool KDTreeSpace::RemoveBodyFromNode(BodyPtr p_pBody,
	const IBoundingVolume& p_boundingVolume, KDNode* p_pKDNode)
{
	// check if one of bodies in current node
	int nIndex = p_pKDNode->Bodies.IndexOf(p_pBody);
	if (nIndex >= 0)
	{
		p_pKDNode->Bodies.RemoveAt(nIndex);
		return true;
	}

	// if node is leaf and not found yet, cannot search further
	if (p_pKDNode->IsLeaf())
		return false;

	// do sytematic search using indicative partition first
	// (may be in other partition or straddling as body may have moved)

	if (p_pKDNode->GetPartitionStatus(p_boundingVolume) == KDNode::Positive)
	{
		// go approprirate partition first
		if (RemoveBodyFromNode(p_pBody, p_boundingVolume, p_pKDNode->Child[1]))
			return true;
		// if not removed, try other partition
		return RemoveBodyFromNode(p_pBody, p_boundingVolume, p_pKDNode->Child[0]);
	}
	else // negative (or straddling)
	{
		// go approprirate partition first
		if (RemoveBodyFromNode(p_pBody, p_boundingVolume, p_pKDNode->Child[0]))
			return true;
		// if not removed, try other partition
		return RemoveBodyFromNode(p_pBody, p_boundingVolume, p_pKDNode->Child[1]);
	}
}

KDTreeSpace::KDTreeSpace(void)
	: m_listBodies()
	, m_stopWatchRebuild()
	, m_unMaximumDepth(10)
	, m_unMinimumBodiesPerNode(2)
	, m_rRebuildInterval((Real) 1.0)
{
	m_pKDNodeRoot = new KDNode();
}

KDTreeSpace::KDTreeSpace(const String& p_strId)
	: ISpace(p_strId)
	, m_listBodies()
	, m_stopWatchRebuild()
	, m_unMaximumDepth(10)
	, m_unMinimumBodiesPerNode(2)
	, m_rRebuildInterval((Real) 1.0)
{
	m_pKDNodeRoot = new KDNode();
}

KDTreeSpace::~KDTreeSpace(void)
{
	delete m_pKDNodeRoot;
}

void KDTreeSpace::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	Real rMin = (Real) -20.0;
	Real rMax = (Real)  20.0;
	BoundingAxisAlignedBox boundingAxisAlignedBoxRoot(TVector3<Real>(rMin, rMin, rMin), TVector3<Real>(rMax, rMax, rMax));
	m_pKDNodeRoot->RenderInstrumentation(p_pInstrumentationDevice, boundingAxisAlignedBoxRoot);
}

TEnumerator<BodyPtr>& KDTreeSpace::GetBodies(void)
{
	return m_listBodies.GetEnumerator();
}

BodyPtr KDTreeSpace::GetBody(const String& p_strId)
{
	for (TEnumerator<BodyPtr>& enumBodies = m_listBodies.GetEnumerator();
		enumBodies.HasMoreElements();)
	{
		BodyPtr pBody = enumBodies.NextElement();
		if (pBody->GetId() == p_strId)
			return pBody;
	}

	throw new MesonException(
		"No body with ID '" + p_strId + "' is contained in the space.",
		__FILE__, __LINE__);
}

void KDTreeSpace::AddBody(BodyPtr p_pBody)
{
	if (m_listBodies.IndexOf(p_pBody) >= 0)
		return;
	m_listBodies.Add(p_pBody);

	// add to root node for eventual update
	m_pKDNodeRoot->Bodies.Add(p_pBody);
}

void KDTreeSpace::RemoveBody(BodyPtr p_pBody)
{
	int nIndex = m_listBodies.IndexOf(p_pBody);
	if (nIndex < 0)
		return;

	m_listBodies.RemoveAt(nIndex);
	if (p_pBody->GetBoundingVolume() != NULL)
		RemoveBodyFromNode(p_pBody, *p_pBody->GetBoundingVolume(), m_pKDNodeRoot);
}

void KDTreeSpace::Rebuild(void)
{
	delete m_pKDNodeRoot;
	m_pKDNodeRoot = PartitionBodies(0, m_listBodies);
}

void KDTreeSpace::Update(void)
{
	if (m_stopWatchRebuild.GetElapsedTime().TotalSeconds() > m_rRebuildInterval)
	{
		m_stopWatchRebuild.Reset();
		delete m_pKDNodeRoot;
		m_pKDNodeRoot = PartitionBodies(0, m_listBodies);
	}
	else
		m_pKDNodeRoot->UpdateNode();
}

void KDTreeSpace::ComputeCandidateCollisions(
	BodyPairList& p_listCandidateCollisions)
{
	p_listCandidateCollisions.Clear();

	if (m_listBodies.Size() == 0) return;

	m_pKDNodeRoot->ComputeCandidateCollisions(p_listCandidateCollisions);
}

void KDTreeSpace::IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
	BodyRayIntersectionList& p_listBodyRayIntersections) const
{
	p_listBodyRayIntersections.Clear();

	m_pKDNodeRoot->IntersectRay(p_ray, p_listBodyRayIntersections);

	// assign original ray to all intersections
	for (size_t unIndex = 0; unIndex < p_listBodyRayIntersections.Size(); unIndex++)
		p_listBodyRayIntersections[unIndex].Ray = &p_ray;

	// do final sorting
	TSorting<BodyRayIntersection>::QuickSort(p_listBodyRayIntersections);
}
