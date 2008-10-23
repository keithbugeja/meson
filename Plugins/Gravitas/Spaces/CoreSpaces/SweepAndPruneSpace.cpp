#include "SweepAndPruneSpace.h"

#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Space;

SweepAndPruneLimit::SweepAndPruneLimit(void)
	: Type(Begin)
{
	for (size_t unIndex = 0; unIndex < MAX_AXES; unIndex++)
	{
		Previous[unIndex] = Next[unIndex] = NULL;
		Value[unIndex] = (Real) 0.0;
	}
}

SweepAndPruneLimit::SweepAndPruneLimit(SweepAndPruneLimit::LimitType p_limitType)
	: Type(p_limitType)
{
	for (size_t unIndex = 0; unIndex < MAX_AXES; unIndex++)
	{
		Previous[unIndex] = Next[unIndex] = NULL;
		Value[unIndex] = (Real) 0.0;
	}
}

SweepAndPruneEntity* SweepAndPruneLimit::GetEntity(void)
{
	return (SweepAndPruneEntity*) (Type == Begin ? this : this - 1); 
}

SweepAndPruneLimit* SweepAndPruneLimit::GetTwinLimit(void)
{
	return Type == Begin ? this + 1 : this - 1; 
}

SweepAndPruneEntity::SweepAndPruneEntity(void)
	: Begin(SweepAndPruneLimit::Begin)
	, End(SweepAndPruneLimit::End)
	, Body()
{
}

const SweepAndPruneEntity& SweepAndPruneEntity::operator=(
	const SweepAndPruneEntity& p_sweepAndPruneEntity)
{
	Begin = p_sweepAndPruneEntity.Begin;
	End = p_sweepAndPruneEntity.End;
	Body = p_sweepAndPruneEntity.Body;
	return *this;
}

bool SweepAndPruneEntity::operator==(
	const SweepAndPruneEntity& p_sweepAndPruneEntity) const
{
	return Body == p_sweepAndPruneEntity.Body;
}

SweepAndPruneSpace::SweepAndPruneSpace(void)
	: m_listBodies()
	, m_unAxes(SweepAndPruneLimit::MAX_AXES)
	, m_limitMin(SweepAndPruneLimit::End)
	, m_limitMax(SweepAndPruneLimit::Begin)
	, m_listBoundedBodies()
	, m_listUnboundedBodies()
	, m_mapBodyPairIntervalCounts()
{
	m_vecAxes[0] = TVector3<Real>::Right;
	m_vecAxes[1] = TVector3<Real>::Up;
	m_vecAxes[2] = TVector3<Real>::Out;

	// prepare sentinel limits
	for (size_t unIndex = 0; unIndex < SweepAndPruneLimit::MAX_AXES; unIndex++)
	{

		m_limitMin.Next[unIndex] = &m_limitMax;
		m_limitMax.Previous[unIndex] = &m_limitMin;
		
		m_limitMin.Value[unIndex] = -TMaths<Real>::Maximum;
		m_limitMax.Value[unIndex] = TMaths<Real>::Maximum;
	}
}

SweepAndPruneSpace::SweepAndPruneSpace(const String& p_strId)
	: ISpace(p_strId)
	, m_listBodies()
	, m_unAxes(SweepAndPruneLimit::MAX_AXES)
	, m_limitMin(SweepAndPruneLimit::End)
	, m_limitMax(SweepAndPruneLimit::Begin)
	, m_listBoundedBodies()
	, m_listUnboundedBodies()
	, m_mapBodyPairIntervalCounts()
{
	m_vecAxes[0] = TVector3<Real>::Right;
	m_vecAxes[1] = TVector3<Real>::Up;
	m_vecAxes[2] = TVector3<Real>::Out;

	// prepare sentinel limits
	for (size_t unIndex = 0; unIndex < SweepAndPruneLimit::MAX_AXES; unIndex++)
	{

		m_limitMin.Next[unIndex] = &m_limitMax;
		m_limitMax.Previous[unIndex] = &m_limitMin;
		
		m_limitMin.Value[unIndex] = -TMaths<Real>::Maximum;
		m_limitMax.Value[unIndex] = TMaths<Real>::Maximum;
	}
}

SweepAndPruneSpace::~SweepAndPruneSpace(void)
{
	// delete entities allocated on the heap
	TArrayList<SweepAndPruneEntity*> listEntities;
	SweepAndPruneLimit* pLimit = m_limitMin.Next[0];
	while (pLimit != &m_limitMax)
	{
		if (pLimit->Type == SweepAndPruneLimit::Begin)
			listEntities.Add(pLimit->GetEntity());
		pLimit = pLimit->Next[0];
	}

	for (size_t unIndex = 0; unIndex < listEntities.Size(); unIndex++)
		delete listEntities[unIndex];
}

void SweepAndPruneSpace::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties.Insert("Axes",
		PropertyDescriptor("Axes", PropertyType::VectorList, false)); 
}

void SweepAndPruneSpace::GetProperty(const String& p_strName,
	VectorList& p_listValues) const
{
	if (p_strName == "Axes")
	{
		p_listValues.Clear();
		for (size_t unIndex = 0; unIndex < m_unAxes; unIndex++)
			p_listValues.Add(m_vecAxes[unIndex]);
	}
	else
		throw new MesonException("Sweep-And-Prune Space: Invalid property specified.", __FILE__, __LINE__);
}

void SweepAndPruneSpace::SetProperty(const String& p_strName,
	const VectorList& p_listValues)
{
	if (p_strName == "Axes")
	{
		m_unAxes = p_listValues.Size();
		for (size_t unIndex = 0; unIndex < m_unAxes; unIndex++)
			m_vecAxes[unIndex] = p_listValues[unIndex];
		Update();
	}
	else
		throw new MesonException("Sweep-And-Prune Space: Invalid property specified.", __FILE__, __LINE__);
}

void SweepAndPruneSpace::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	for (size_t unAxisIndex = 0; unAxisIndex < m_unAxes; unAxisIndex++)
	{
		TVector3<Real>& vecAxis = m_vecAxes[unAxisIndex];

		SweepAndPruneLimit* pLimit = m_limitMin.Next[unAxisIndex];
		while (pLimit != &m_limitMax)
		{
			if (pLimit->Type == SweepAndPruneLimit::Begin)
			{
				SweepAndPruneEntity* pEntity = pLimit->GetEntity();
				Real rValueBegin = pLimit->Value[unAxisIndex];
				Real rValueEnd = pEntity->End.Value[unAxisIndex];

				TPoint3<Real> ptStart = TPoint3<Real>::Origin + vecAxis * rValueBegin;
				TPoint3<Real> ptEnd = TPoint3<Real>::Origin + vecAxis * rValueEnd;
				p_pInstrumentationDevice->DrawArrow(ptStart, ptEnd);
				p_pInstrumentationDevice->DrawArrow(ptEnd, ptStart);
			}

			pLimit = pLimit->Next[unAxisIndex];
		}
	}
}

TEnumerator<BodyPtr>& SweepAndPruneSpace::GetBodies(void)
{
	return m_listBodies.GetEnumerator();
}

BodyPtr SweepAndPruneSpace::GetBody(const String& p_strId)
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

void SweepAndPruneSpace::AddBody(BodyPtr p_pBody)
{
	// do nothing if already in space
	if (m_listBodies.IndexOf(p_pBody) >= 0)
		return;

	// add to main list
	m_listBodies.Add(p_pBody);

	// if no bounding volume, add to list of unbounded bodies
	// and do nothing else
	BoundingVolumePtr pBoundingVolume = p_pBody->GetBoundingVolume();
	if (pBoundingVolume == NULL)
	{
		m_listUnboundedBodies.Add(p_pBody);
		return;
	}

	m_listBoundedBodies.Add(p_pBody);

	// otherwise, insert into sweep and prune structure
	SweepAndPruneEntity* pEntity = new SweepAndPruneEntity();
	pEntity->Body = p_pBody;

	TInterval<Real> itvProjection;
	for (size_t unAxisIndex = 0; unAxisIndex < m_unAxes; unAxisIndex++)
	{
		pBoundingVolume->ProjectToInterval(m_vecAxes[unAxisIndex], itvProjection);
		pEntity->Begin.Value[unAxisIndex] = itvProjection.Min;
		pEntity->End.Value[unAxisIndex] = itvProjection.Max;

		SweepAndPruneLimit* pLimit = &m_limitMin;
		while (pLimit->Value[unAxisIndex] < pEntity->Begin.Value[unAxisIndex])
			pLimit = pLimit->Next[unAxisIndex];
		pEntity->Begin.Previous[unAxisIndex] = pLimit->Previous[unAxisIndex];
		pEntity->Begin.Next[unAxisIndex] = pLimit;
		pLimit->Previous[unAxisIndex]->Next[unAxisIndex] = &pEntity->Begin;
		pLimit->Previous[unAxisIndex] = &pEntity->Begin;

		while (pLimit->Value[unAxisIndex] < pEntity->End.Value[unAxisIndex])
			pLimit = pLimit->Next[unAxisIndex];
		pEntity->End.Previous[unAxisIndex] = pLimit->Previous[unAxisIndex];
		pEntity->End.Next[unAxisIndex] = pLimit;
		pLimit->Previous[unAxisIndex]->Next[unAxisIndex] = &pEntity->End;
		pLimit->Previous[unAxisIndex] = &pEntity->End;
	}
}

void SweepAndPruneSpace::RemoveBody(BodyPtr p_pBody)
{
	m_listBodies.Remove(p_pBody);

	// if no bounding volume, remove from list of unbounded bodies
	// and do nothing else
	BoundingVolumePtr pBoundingVolume = p_pBody->GetBoundingVolume();
	if (pBoundingVolume == NULL)
	{
		m_listUnboundedBodies.Remove(p_pBody);
		return;
	}

	// otherwise, remove from prune and sweep structure

	// get pointer to entity
	SweepAndPruneEntity *pEntity = NULL;
	SweepAndPruneLimit* pLimit = &m_limitMin;
	while (pLimit->GetEntity()->Body != p_pBody)
		pLimit = pLimit->Next[0];
	pEntity = pLimit->GetEntity();

	TInterval<Real> itvProjection;
	for (size_t unAxisIndex = 0; unAxisIndex < m_unAxes; unAxisIndex++)
	{
		pEntity->Begin.Previous[unAxisIndex]->Next[unAxisIndex]
			= pEntity->Begin.Next[unAxisIndex];
		pEntity->Begin.Next[unAxisIndex]->Previous[unAxisIndex]
			= pEntity->Begin.Previous[unAxisIndex];

		pEntity->End.Previous[unAxisIndex]->Next[unAxisIndex]
			= pEntity->End.Next[unAxisIndex];
		pEntity->End.Next[unAxisIndex]->Previous[unAxisIndex]
			= pEntity->End.Previous[unAxisIndex];
	}
	delete pEntity;

	m_listBoundedBodies.Remove(p_pBody);
}

void SweepAndPruneSpace::Rebuild(void)
{
	Update();
}

void SweepAndPruneSpace::Update(void)
{
	TInterval<Real> itvProjection;
	for (size_t unAxisIndex = 0; unAxisIndex < m_unAxes; unAxisIndex++)
	{
		// update limit values
		SweepAndPruneLimit* pLimit = m_limitMin.Next[unAxisIndex];
		while (pLimit != &m_limitMax)
		{
			if (pLimit->Type == SweepAndPruneLimit::Begin)
			{
				SweepAndPruneEntity* pEntity = pLimit->GetEntity();
				BodyPtr pBody = pEntity->Body;
				BoundingVolumePtr pBoundingVolume = pBody->GetBoundingVolume();
				pBoundingVolume->ProjectToInterval(m_vecAxes[unAxisIndex], itvProjection);
				pEntity->Begin.Value[unAxisIndex] = itvProjection.Min;
				pEntity->End.Value[unAxisIndex] = itvProjection.Max;
			}

			pLimit = pLimit->Next[unAxisIndex];
		}

		// resort limit list (use bubble sort)
		bool bSorted = false;
		while (!bSorted)
		{
			bSorted = true;
			pLimit = m_limitMin.Next[unAxisIndex];
			while (pLimit != &m_limitMax)
			{
				SweepAndPruneLimit* pLimitNext = pLimit->Next[unAxisIndex];

				if (pLimit->Value[unAxisIndex] > pLimitNext->Value[unAxisIndex])
				{
					bSorted = false;
					SweepAndPruneLimit* pLimitPrevious = pLimit->Previous[unAxisIndex];
					SweepAndPruneLimit* pLimitNextNext = pLimitNext->Next[unAxisIndex];

					// swap
					pLimitPrevious->Next[unAxisIndex] = pLimitNext;
					pLimitNext->Previous[unAxisIndex] = pLimitPrevious;
					pLimitNext->Next[unAxisIndex] = pLimit;
					pLimit->Previous[unAxisIndex] = pLimitNext;
					pLimit->Next[unAxisIndex] = pLimitNextNext;
					pLimitNextNext->Previous[unAxisIndex] = pLimit;
				}
				else
					pLimit = pLimit->Next[unAxisIndex];
			}
		}
	}
}

void SweepAndPruneSpace::ComputeCandidateCollisions(
	BodyPairList& p_listCandidateCollisions)
{
	p_listCandidateCollisions.Clear();

	size_t unCount = m_listBodies.Size();
	if (unCount < 2) return;

	m_mapBodyPairIntervalCounts.Clear();

	// get candidate collison pairs from prune and sweep structure
	BodyPair bodyPair;
	for (size_t unAxisIndex = 0; unAxisIndex < m_unAxes; unAxisIndex++)
	{
		SweepAndPruneLimit* pLimit = m_limitMin.Next[unAxisIndex];
		while (pLimit != &m_limitMax)
		{
			if (pLimit->Type == SweepAndPruneLimit::Begin)
			{
				SweepAndPruneEntity* pEntity = pLimit->GetEntity();
				BodyPtr pBody1 = pEntity->Body;
				bool bInfiniteMass1 = pBody1->GetMassProperties().HasInfiniteMass();
				bool bActive1 = pBody1->GetKineticProperties().Active;

				SweepAndPruneLimit* pLimit2 = pLimit->Next[unAxisIndex];
				while (pLimit2->GetEntity() != pEntity)
				{
					if (pLimit2->Type == SweepAndPruneLimit::Begin)
					{
						BodyPtr pBody2 = pLimit2->GetEntity()->Body;

						// ignore potential collision between bodies both of infinite mass
						if (!bInfiniteMass1 || !pBody2->GetMassProperties().HasInfiniteMass())
						{
							// ignore potential collision between inactive objects
							if (bActive1 || pBody2->GetKineticProperties().Active)
							{
								if (pBody1 < pBody2)
								{
									bodyPair.Element1 = pBody1;
									bodyPair.Element2 = pBody2;
								}
								else
								{
									bodyPair.Element1 = pBody2;
									bodyPair.Element2 = pBody1;
								}

								if (m_mapBodyPairIntervalCounts.ContainsKey(bodyPair))
									m_mapBodyPairIntervalCounts[bodyPair]++;
								else
									m_mapBodyPairIntervalCounts.Insert(bodyPair, (byte) 1);
							}
						}
					}
					pLimit2 = pLimit2->Next[unAxisIndex];
				}
			}
			pLimit = pLimit->Next[unAxisIndex];
		}
	}

	TEnumerator< TKeyValuePair<BodyPair, byte> >& enumBodyPairIntervalCounts = m_mapBodyPairIntervalCounts.GetEnumerator();
	while (enumBodyPairIntervalCounts.HasMoreElements())
	{
		TKeyValuePair<BodyPair, byte>& pair = enumBodyPairIntervalCounts.NextElement();
		if (pair.Value == m_unAxes)
			p_listCandidateCollisions.Add(pair.Key);
	}

	// process unbounded bodies against bounded
	size_t unUnboundedSize = m_listUnboundedBodies.Size();
	if (unUnboundedSize == 0)
		return;

	size_t unBoundedSize = m_listBoundedBodies.Size();
	if (unBoundedSize == 0)
		return;

	// compute bounded with unbounded body collision candidates
	for (size_t unUnboundedIndex = 0; unUnboundedIndex < unUnboundedSize; unUnboundedIndex++)
	{
		BodyPtr pBodyUnbounded = m_listUnboundedBodies[unUnboundedIndex];
		bool bInfiniteMassUnbounded = pBodyUnbounded->GetMassProperties().HasInfiniteMass();
		bool bUnboundedActive(pBodyUnbounded->GetKineticProperties().Active);
		for (size_t unBoundedIndex = 0; unBoundedIndex < unBoundedSize; unBoundedIndex++)
		{
			BodyPtr pBodyBounded = m_listBoundedBodies[unBoundedIndex];

			if (bInfiniteMassUnbounded && pBodyBounded->GetMassProperties().HasInfiniteMass())
				continue;

			if (!bUnboundedActive && !pBodyBounded->GetKineticProperties().Active)
				continue;

			p_listCandidateCollisions.Add(BodyPair(pBodyUnbounded, pBodyBounded));
		}
	}
}

void SweepAndPruneSpace::IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
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
