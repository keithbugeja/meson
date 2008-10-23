#include "BasicSpace.h"

#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Space;

BasicSpace::BasicSpace(void)
	: m_listBodies()
{
}

BasicSpace::BasicSpace(const String& p_strId)
	: ISpace(p_strId)
	, m_listBodies()
{
}

BasicSpace::~BasicSpace(void)
{
}

TEnumerator<BodyPtr>& BasicSpace::GetBodies(void)
{
	return m_listBodies.GetEnumerator();
}

BodyPtr BasicSpace::GetBody(const String& p_strId)
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

void BasicSpace::AddBody(BodyPtr p_pBody)
{
	if (m_listBodies.IndexOf(p_pBody) >= 0)
		return;
	m_listBodies.Add(p_pBody);
}

void BasicSpace::RemoveBody(BodyPtr p_pBody)
{
	m_listBodies.Remove(p_pBody);
}

void BasicSpace::Rebuild(void)
{
}

void BasicSpace::Update(void)
{
}

void BasicSpace::ComputeCandidateCollisions(
	BodyPairList& p_listCandidateCollisions)
{
	p_listCandidateCollisions.Clear();

	size_t unCount = m_listBodies.Size();
	if (unCount == 0) return;

	for (size_t unIndex1 = 0; unIndex1 < unCount; unIndex1++)
	{
		BodyPtr pBody1 = m_listBodies[unIndex1];
		bool bInfiniteMass1 = pBody1->GetMassProperties().HasInfiniteMass();

		for (size_t unIndex2 = unIndex1 + 1; unIndex2 < unCount; unIndex2++)
		{
			BodyPtr pBody2 = m_listBodies[unIndex2];

			// ignore potential collision between bodies both of infinite mass
			if (bInfiniteMass1 && pBody2->GetMassProperties().HasInfiniteMass())
				continue;

			if (pBody1->GetKineticProperties().Active
				|| pBody2->GetKineticProperties().Active)
				p_listCandidateCollisions.Add(BodyPair(pBody1, pBody2));
		}
	}
}

void BasicSpace::IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
	BodyRayIntersectionList& p_listBodyRayIntersections) const
{
	p_listBodyRayIntersections.Clear();

	TPoint3<Real> ptIntersection;
	Transform trnWorld, trnLocal;
	size_t unBodyCount = m_listBodies.Size();
	for (size_t unBodyIndex = 0; unBodyIndex < unBodyCount; unBodyIndex++)
	{
		BodyPtr pBody = m_listBodies[unBodyIndex];

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
			if (p_listBodyRayIntersections[unListIndex]
				>= bodyRayIntersection)
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
}
