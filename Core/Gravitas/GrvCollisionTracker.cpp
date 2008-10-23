#include "GrvCollisionTracker.h"

#include "Maths.h"
#include "TrieMap.h"
#include "StopWatch.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::System;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Event;

Meson_Gravitas_Collision_BEGIN

typedef TTrieMap<BodyPair, ContactManifold> CollisionMap;

struct CollisionTrackerState
{
	Real m_rSamplingFrequency;
	Real m_rSamplingInterval;
	StopWatch m_stopWatch;
	CollisionMap m_mapCollisions1;
	CollisionMap m_mapCollisions2;
	CollisionMap* m_pMapCollisionsCurrent;
	CollisionMap* m_pMapCollisionsPrevious;

	CollisionTrackerState(void)
		: m_rSamplingFrequency((Real) 10.0)
		, m_rSamplingInterval((Real) 0.1)
		, m_stopWatch()
		, m_mapCollisions1()
		, m_mapCollisions2()
		, m_pMapCollisionsCurrent(&m_mapCollisions1)
		, m_pMapCollisionsPrevious(&m_mapCollisions2)
	{ }

	void NotifyEventManager(ContactManifold* p_pContactManifold,
		CollisionEventType::CollisionEventType p_collisionEventType)
	{
		GravitasEngine::GetInstance()->GetEventManager()->Notify(
			CollisionEventPtr(
				new CollisionEvent(p_pContactManifold, p_collisionEventType)));
	}
};

Meson_Gravitas_Collision_END

CollisionTracker::CollisionTracker(void)
	: m_pCollisionTrackerState(NULL)
{
	m_pCollisionTrackerState = new CollisionTrackerState();
}

CollisionTracker::~CollisionTracker(void)
{
	delete m_pCollisionTrackerState;
}

Real CollisionTracker::GetSamplingFrequency(void) const
{
	return m_pCollisionTrackerState->m_rSamplingFrequency;
}

void CollisionTracker::SetSamplingFrequency(Real p_rValue)
{
	m_pCollisionTrackerState->m_rSamplingFrequency = p_rValue;
	if (TMaths<Real>::Equals(p_rValue, (Real) 0.0))
		m_pCollisionTrackerState->m_rSamplingInterval = TMaths<Real>::Maximum;
	else
		m_pCollisionTrackerState->m_rSamplingInterval = (Real) 1.0 / p_rValue;
}

void CollisionTracker::ReportCollision(const ContactManifold& p_contactManifold)
{
	// prepare body pair
	BodyPair bodyPair(p_contactManifold.Body1, p_contactManifold.Body2);
	if (bodyPair.Element1 > bodyPair.Element2)
	{
		BodyPtr pBodyTemp(bodyPair.Element1);
		bodyPair.Element1 = bodyPair.Element2;
		bodyPair.Element2 = pBodyTemp;
	}

	m_pCollisionTrackerState->m_pMapCollisionsCurrent->Insert(bodyPair, p_contactManifold);
}

void CollisionTracker::GenerateCollisionEvents(void)
{
	// handle sampling rate
	if (m_pCollisionTrackerState->m_stopWatch.GetElapsedTime().TotalSeconds()
		< m_pCollisionTrackerState->m_rSamplingInterval)
		return;
	m_pCollisionTrackerState->m_stopWatch.Reset();

	EventManager* pEventManager
		= GravitasEngine::GetInstance()->GetEventManager();

	bool bStartCollisionEvents = pEventManager->IsEnabled(EventFlags::CollisionStart);
	bool bOngoingCollisionEvents = pEventManager->IsEnabled(EventFlags::CollisionOngoing);

	if (bStartCollisionEvents || bOngoingCollisionEvents)
	{
		// determine starting and ongoing collisions
		CollisionMap& collisionMapPrevious
			= *m_pCollisionTrackerState->m_pMapCollisionsPrevious;
		TEnumerator< TKeyValuePair<BodyPair, ContactManifold> >& enumCurrent
			= m_pCollisionTrackerState->m_pMapCollisionsCurrent->GetEnumerator();
		while (enumCurrent.HasMoreElements())
		{
			TKeyValuePair<BodyPair, ContactManifold>& keyValuePair
				= enumCurrent.NextElement();
			if (collisionMapPrevious.ContainsKey(keyValuePair.Key))
			{
				if (bOngoingCollisionEvents)
					m_pCollisionTrackerState->NotifyEventManager(
						&keyValuePair.Value, CollisionEventType::Ongoing);
			}
			else
			{
				if (bStartCollisionEvents)
					m_pCollisionTrackerState->NotifyEventManager(
						&keyValuePair.Value, CollisionEventType::Start);
			}
		}
	}

	if (pEventManager->IsEnabled(EventFlags::CollisionEnd))
	{
		// determine collisions end
		CollisionMap& collisionMapCurrent
			= *m_pCollisionTrackerState->m_pMapCollisionsCurrent;
		TEnumerator< TKeyValuePair<BodyPair, ContactManifold> >& enumPrevious
			= m_pCollisionTrackerState->m_pMapCollisionsPrevious->GetEnumerator();
		while (enumPrevious.HasMoreElements())
		{
			TKeyValuePair<BodyPair, ContactManifold>& keyValuePair = enumPrevious.NextElement();
			if (!collisionMapCurrent.ContainsKey(keyValuePair.Key))
				m_pCollisionTrackerState->NotifyEventManager(
					&keyValuePair.Value, CollisionEventType::End);
		}
	}

	// swap current and prev buffers
	CollisionMap* pCollisionMapTemp(m_pCollisionTrackerState->m_pMapCollisionsCurrent);
	m_pCollisionTrackerState->m_pMapCollisionsCurrent
		= m_pCollisionTrackerState->m_pMapCollisionsPrevious;
	m_pCollisionTrackerState->m_pMapCollisionsPrevious = pCollisionMapTemp;

	// clear new current
	m_pCollisionTrackerState->m_pMapCollisionsCurrent->Clear();
}
