#include "GrvCollisionFilter.h"

#include "TreeSet.h"
#include "ArrayList.h"
#include "Pair.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Event;

Meson_Gravitas_Collision_BEGIN

struct CollisionFilterState
{
	bool m_bClear;
	bool m_bAllowedByDefault;
	TTreeSet<BodyPtr> m_setAllowedBodies, m_setRejectedBodies;
	TTreeSet<BodyPair> m_setAllowedBodyPairs, m_setRejectedBodyPairs;
	TTreeSet<CollisionGroupPtr> m_setAllowedCollisionGroups, m_setRejectedCollisionGroups;
	TTreeSet<CollisionGroupPair> m_setAllowedCollisionGroupPairs, m_setRejectedCollisionGroupPairs;

	CollisionFilterState(void)
		: m_bClear(true)
		, m_bAllowedByDefault(false)
		, m_setAllowedBodies()
		, m_setRejectedBodies()
		, m_setAllowedBodyPairs()
		, m_setRejectedBodyPairs()
		, m_setAllowedCollisionGroups()
		, m_setRejectedCollisionGroups()
		, m_setAllowedCollisionGroupPairs()
		, m_setRejectedCollisionGroupPairs()
	{ }

	void Update(void)
	{
		m_bClear = m_setAllowedBodies.Size() == 0
			&& m_setRejectedBodies.Size() == 0
			&& m_setAllowedBodyPairs.Size() == 0
			&& m_setRejectedBodyPairs.Size() == 0
			&& m_setAllowedCollisionGroups.Size() == 0
			&& m_setRejectedCollisionGroups.Size() == 0
			&& m_setAllowedCollisionGroupPairs.Size() == 0
			&& m_setRejectedCollisionGroupPairs.Size() == 0;
	}

	BodyPair CreateBodyPair(BodyPtr p_pBody1, BodyPtr p_pBody2) const
	{
		if (p_pBody1 < p_pBody2)
			return BodyPair(p_pBody1, p_pBody2);
		else
			return BodyPair(p_pBody2, p_pBody1);
	}

	CollisionGroupPair CreateCollisionGroupPair(
		CollisionGroupPtr p_pCollisionGroup1,
		CollisionGroupPtr p_pCollisionGroup2) const
	{
		if (p_pCollisionGroup1 < p_pCollisionGroup2)
			return CollisionGroupPair(p_pCollisionGroup1, p_pCollisionGroup2);
		else
			return CollisionGroupPair(p_pCollisionGroup2, p_pCollisionGroup1);
	}

	void EliminateRedundantBodyPairFilters(BodyPtr p_pBody)
	{
		BodyPairArrayList listBodyPairs;
		for (TEnumerator<BodyPair>& enumBodyPairs = m_setAllowedBodyPairs.GetEnumerator();
			enumBodyPairs.HasMoreElements();)
		{
			BodyPair& bodyPair = enumBodyPairs.NextElement();
			if (bodyPair.Element1 == p_pBody || bodyPair.Element2 == p_pBody)
				listBodyPairs.Add(bodyPair);
		}
		for (size_t unIndex = 0; unIndex < listBodyPairs.Size(); unIndex++)
			m_setAllowedBodyPairs.Remove(listBodyPairs[unIndex]);

		listBodyPairs.Clear();
		for (TEnumerator<BodyPair>& enumBodyPairs = m_setRejectedBodyPairs.GetEnumerator();
			enumBodyPairs.HasMoreElements();)
		{
			BodyPair& bodyPair = enumBodyPairs.NextElement();
			if (bodyPair.Element1 == p_pBody || bodyPair.Element2 == p_pBody)
				listBodyPairs.Add(bodyPair);
		}
		for (size_t unIndex = 0; unIndex < listBodyPairs.Size(); unIndex++)
			m_setRejectedBodyPairs.Remove(listBodyPairs[unIndex]);
	}

	void EliminateRedundantCollisionGroupPairFilters(CollisionGroupPtr p_pCollisionGroup)
	{
		TArrayList<CollisionGroupPair> listCollisionGroupPairs;
		for (TEnumerator<CollisionGroupPair>& enumCollisionGroupPairs
				= m_setAllowedCollisionGroupPairs.GetEnumerator();
			enumCollisionGroupPairs.HasMoreElements();)
		{
			CollisionGroupPair& collisionGroupPair = enumCollisionGroupPairs.NextElement();
			if (collisionGroupPair.Element1 == p_pCollisionGroup
				|| collisionGroupPair.Element2 == p_pCollisionGroup)
				listCollisionGroupPairs.Add(collisionGroupPair);
		}
		for (size_t unIndex = 0; unIndex < listCollisionGroupPairs.Size(); unIndex++)
			m_setAllowedCollisionGroupPairs.Remove(listCollisionGroupPairs[unIndex]);

		listCollisionGroupPairs.Clear();
		for (TEnumerator<CollisionGroupPair>& enumCollisionGroupPairs
				= m_setRejectedCollisionGroupPairs.GetEnumerator();
			enumCollisionGroupPairs.HasMoreElements();)
		{
			CollisionGroupPair& collisionGroupPair = enumCollisionGroupPairs.NextElement();
			if (collisionGroupPair.Element1 == p_pCollisionGroup
				|| collisionGroupPair.Element2 == p_pCollisionGroup)
				listCollisionGroupPairs.Add(collisionGroupPair);
		}
		for (size_t unIndex = 0; unIndex < listCollisionGroupPairs.Size(); unIndex++)
			m_setRejectedCollisionGroupPairs.Remove(listCollisionGroupPairs[unIndex]);
	}

	bool IsAllowed(BodyPtr p_pBody1, BodyPtr p_pBody2)
	{
		// early-out test
		if (m_bClear)
			return m_bAllowedByDefault;

		// check individual body rules
		if (m_setAllowedBodies.ContainsElement(p_pBody1)) return true;
		if (m_setAllowedBodies.ContainsElement(p_pBody2)) return true;

		// check individual body exceptions
		if (m_setRejectedBodies.ContainsElement(p_pBody1)) return false;
		if (m_setRejectedBodies.ContainsElement(p_pBody2)) return false;

		// check group rules
		if (m_setAllowedCollisionGroups.Intersects(
			p_pBody1->GetCollisionGroups()))
			return true;
		if (m_setAllowedCollisionGroups.Intersects(
			p_pBody2->GetCollisionGroups()))
			return true;

		// check group exceptions
		if (m_setRejectedCollisionGroups.Intersects(
			p_pBody1->GetCollisionGroups()))
			return true;
		if (m_setRejectedCollisionGroups.Intersects(
			p_pBody2->GetCollisionGroups()))
			return true;

		// check body pair rules
		BodyPair bodyPair = CreateBodyPair(p_pBody1, p_pBody2);
		if (m_setAllowedBodyPairs.ContainsElement(bodyPair))
			return true;

		// check body pair exceptions
		if (m_setRejectedBodyPairs.ContainsElement(bodyPair))
			return false;

		// check collision group pair rules and exceptions
		TEnumerator<CollisionGroupPtr>::Ptr penumCollisionGroups1
			= p_pBody1->GetCollisionGroups().CreateEnumerator();
		while (penumCollisionGroups1->HasMoreElements())
		{
			CollisionGroupPtr& pCollisionGroup1 = penumCollisionGroups1->NextElement();

			TEnumerator<CollisionGroupPtr>::Ptr penumCollisionGroups2
				= p_pBody2->GetCollisionGroups().CreateEnumerator();
			while (penumCollisionGroups2->HasMoreElements())
			{
				CollisionGroupPtr& pCollisionGroup2
					= penumCollisionGroups2->NextElement();

				CollisionGroupPair collisionGroupPair
					= CreateCollisionGroupPair(pCollisionGroup1, pCollisionGroup2);

				// check collision group pair rules
				if (m_setAllowedCollisionGroupPairs.ContainsElement(
					collisionGroupPair))
					return true;

				// check collision group pair exceptions
				if (m_setRejectedCollisionGroupPairs.ContainsElement(
					collisionGroupPair))
					return false;
			}
		}

		// if no matching rule found, assume default result
		return m_bAllowedByDefault;
	}

};

Meson_Gravitas_Collision_END

CollisionFilter::CollisionFilter(void)
	: m_pCollisionFilterState(NULL)
{
	m_pCollisionFilterState = new CollisionFilterState();
}

CollisionFilter::~CollisionFilter(void)
{
	delete m_pCollisionFilterState;
}

bool CollisionFilter::IsAllowedByDefault(void)
{
	return m_pCollisionFilterState->m_bAllowedByDefault;
}

void CollisionFilter::SetAllowedByDefault(bool p_bAllowedByDefault)
{
	m_pCollisionFilterState->m_bAllowedByDefault = p_bAllowedByDefault;
}

void CollisionFilter::Clear(void)
{
	m_pCollisionFilterState->m_bClear = true;
	m_pCollisionFilterState->m_setAllowedBodies.Clear();
	m_pCollisionFilterState->m_setRejectedBodies.Clear();
	m_pCollisionFilterState->m_setRejectedBodyPairs.Clear();
	m_pCollisionFilterState->m_setRejectedBodyPairs.Clear();
	m_pCollisionFilterState->m_setAllowedCollisionGroups.Clear();
	m_pCollisionFilterState->m_setRejectedCollisionGroups.Clear();
	m_pCollisionFilterState->m_setAllowedCollisionGroupPairs.Clear();
	m_pCollisionFilterState->m_setRejectedCollisionGroupPairs.Clear();
}

void CollisionFilter::AddRule(BodyPtr p_pBody)
{
	m_pCollisionFilterState->m_bClear = false;
	m_pCollisionFilterState->m_setAllowedBodies.Insert(p_pBody);

	// ensure there is no conflicting exception for the body
	m_pCollisionFilterState->m_setRejectedBodies.Remove(p_pBody);

	// eliminate redundant body pair filters
	m_pCollisionFilterState->EliminateRedundantBodyPairFilters(p_pBody);
}

void CollisionFilter::RemoveRule(BodyPtr p_pBody)
{
	m_pCollisionFilterState->m_setAllowedBodies.Remove(p_pBody);
	m_pCollisionFilterState->Update();
}

void CollisionFilter::AddRule(BodyPtr p_pBody1, BodyPtr p_pBody2)
{
	// if bodies same, use simpler individual body rule
	if (p_pBody1 == p_pBody2)
	{
		AddRule(p_pBody1);
		return;
	}

	m_pCollisionFilterState->m_bClear = false;
	BodyPair bodyPair
		= m_pCollisionFilterState->CreateBodyPair(p_pBody1, p_pBody2);
	m_pCollisionFilterState->m_setAllowedBodyPairs.Insert(bodyPair);

	// ensure there is no conflicting exception for the body pair
	m_pCollisionFilterState->m_setRejectedBodyPairs.Remove(bodyPair);

	// ensure there are no conflicting criteria for individual bodies
	m_pCollisionFilterState->m_setAllowedBodies.Remove(p_pBody1);
	m_pCollisionFilterState->m_setRejectedBodies.Remove(p_pBody1);
	m_pCollisionFilterState->m_setAllowedBodies.Remove(p_pBody2);
	m_pCollisionFilterState->m_setRejectedBodies.Remove(p_pBody2);
}

void CollisionFilter::RemoveRule(BodyPtr p_pBody1, BodyPtr p_pBody2)
{
	m_pCollisionFilterState->m_setAllowedBodyPairs.Remove(
		m_pCollisionFilterState->CreateBodyPair(p_pBody1, p_pBody2));
	m_pCollisionFilterState->Update();
}

void CollisionFilter::AddRule(CollisionGroupPtr p_pCollisionGroup)
{
	m_pCollisionFilterState->m_bClear = false;
	m_pCollisionFilterState->m_setAllowedCollisionGroups.Insert(p_pCollisionGroup);

	// ensure there is no conflicting exception for the collision group
	m_pCollisionFilterState->m_setRejectedCollisionGroups.Remove(p_pCollisionGroup);

	// eliminate redundant collisoon group pair filters
	m_pCollisionFilterState->EliminateRedundantCollisionGroupPairFilters(p_pCollisionGroup);
}

void CollisionFilter::RemoveRule(CollisionGroupPtr p_pCollisionGroup)
{
	m_pCollisionFilterState->m_setAllowedCollisionGroups.Remove(p_pCollisionGroup);
	m_pCollisionFilterState->Update();
}

void CollisionFilter::AddRule(
	CollisionGroupPtr p_pCollisionGroup1,
	CollisionGroupPtr p_pCollisionGroup2)
{
	// if collision groups same, use simpler individual collision group rule
	if (p_pCollisionGroup1 == p_pCollisionGroup2)
	{
		AddRule(p_pCollisionGroup1);
		return;
	}

	m_pCollisionFilterState->m_bClear = false;
	CollisionGroupPair collisionGroupPair
		= m_pCollisionFilterState->CreateCollisionGroupPair(p_pCollisionGroup1, p_pCollisionGroup2);
	m_pCollisionFilterState->m_setAllowedCollisionGroupPairs.Insert(collisionGroupPair);

	// ensure there is no conflicting exception for the collision group pair
	m_pCollisionFilterState->m_setRejectedCollisionGroupPairs.Remove(collisionGroupPair);

	// ensure there are no conflicting criteria for individual collision groups
	m_pCollisionFilterState->m_setAllowedCollisionGroups.Remove(p_pCollisionGroup1);
	m_pCollisionFilterState->m_setRejectedCollisionGroups.Remove(p_pCollisionGroup1);
	m_pCollisionFilterState->m_setAllowedCollisionGroups.Remove(p_pCollisionGroup2);
	m_pCollisionFilterState->m_setRejectedCollisionGroups.Remove(p_pCollisionGroup2);
}

void CollisionFilter::RemoveRule(
	CollisionGroupPtr p_pCollisionGroup1,
	CollisionGroupPtr p_pCollisionGroup2)
{
	m_pCollisionFilterState->m_setAllowedCollisionGroupPairs.Remove(
		m_pCollisionFilterState->CreateCollisionGroupPair(
			p_pCollisionGroup1, p_pCollisionGroup2));
	m_pCollisionFilterState->Update();
}

void CollisionFilter::AddException(BodyPtr p_pBody)
{
	m_pCollisionFilterState->m_bClear = false;
	m_pCollisionFilterState->m_setRejectedBodies.Insert(p_pBody);

	// ensure there is no conflicting rule for the body
	m_pCollisionFilterState->m_setAllowedBodies.Remove(p_pBody);

	// eliminate redundant body pair filters
	m_pCollisionFilterState->EliminateRedundantBodyPairFilters(p_pBody);
}

void CollisionFilter::RemoveException(BodyPtr p_pBody)
{
	m_pCollisionFilterState->m_setRejectedBodies.Remove(p_pBody);
	m_pCollisionFilterState->Update();
}

void CollisionFilter::AddException(BodyPtr p_pBody1, BodyPtr p_pBody2)
{
	// if bodies same, use simpler individual body exception
	if (p_pBody1 == p_pBody2)
	{
		AddException(p_pBody1);
		return;
	}

	m_pCollisionFilterState->m_bClear = false;
	BodyPair bodyPair
		= m_pCollisionFilterState->CreateBodyPair(p_pBody1, p_pBody2);
	m_pCollisionFilterState->m_setRejectedBodyPairs.Insert(bodyPair);

	// ensure there is no conflicting rule for the body pair
	m_pCollisionFilterState->m_setAllowedBodyPairs.Remove(bodyPair);

	// ensure there are no conflicting criteria for individual bodies
	m_pCollisionFilterState->m_setAllowedBodies.Remove(p_pBody1);
	m_pCollisionFilterState->m_setRejectedBodies.Remove(p_pBody1);
	m_pCollisionFilterState->m_setAllowedBodies.Remove(p_pBody2);
	m_pCollisionFilterState->m_setRejectedBodies.Remove(p_pBody2);
}

void CollisionFilter::RemoveException(BodyPtr p_pBody1, BodyPtr p_pBody2)
{
	m_pCollisionFilterState->m_setRejectedBodyPairs.Remove(
		m_pCollisionFilterState->CreateBodyPair(p_pBody1, p_pBody2));
	m_pCollisionFilterState->Update();
}

void CollisionFilter::AddException(CollisionGroupPtr p_pCollisionGroup)
{
	m_pCollisionFilterState->m_bClear = false;
	m_pCollisionFilterState->m_setRejectedCollisionGroups.Insert(p_pCollisionGroup);

	// ensure there is no conflicting rule for the collision group
	m_pCollisionFilterState->m_setAllowedCollisionGroups.Remove(p_pCollisionGroup);

	// eliminate redundant collisoon group pair filters
	m_pCollisionFilterState->EliminateRedundantCollisionGroupPairFilters(p_pCollisionGroup);
}

void CollisionFilter::RemoveException(CollisionGroupPtr p_pCollisionGroup)
{
	m_pCollisionFilterState->m_setRejectedCollisionGroups.Remove(p_pCollisionGroup);
	m_pCollisionFilterState->Update();
}

void CollisionFilter::AddException(
	CollisionGroupPtr p_pCollisionGroup1,
	CollisionGroupPtr p_pCollisionGroup2)
{
	// if collision groups same, use simpler individual collision group exception
	if (p_pCollisionGroup1 == p_pCollisionGroup2)
	{
		AddException(p_pCollisionGroup1);
		return;
	}

	m_pCollisionFilterState->m_bClear = false;
	CollisionGroupPair collisionGroupPair
		= m_pCollisionFilterState->CreateCollisionGroupPair(p_pCollisionGroup1, p_pCollisionGroup2);
	m_pCollisionFilterState->m_setRejectedCollisionGroupPairs.Insert(collisionGroupPair);

	// ensure there is no conflicting rule for the collision group pair
	m_pCollisionFilterState->m_setAllowedCollisionGroupPairs.Remove(collisionGroupPair);

	// ensure there are no conflicting criteria for individual collision groups
	m_pCollisionFilterState->m_setAllowedCollisionGroups.Remove(p_pCollisionGroup1);
	m_pCollisionFilterState->m_setRejectedCollisionGroups.Remove(p_pCollisionGroup1);
	m_pCollisionFilterState->m_setAllowedCollisionGroups.Remove(p_pCollisionGroup2);
	m_pCollisionFilterState->m_setRejectedCollisionGroups.Remove(p_pCollisionGroup2);
}

void CollisionFilter::RemoveException(
	CollisionGroupPtr p_pCollisionGroup1,
	CollisionGroupPtr p_pCollisionGroup2)
{
	m_pCollisionFilterState->m_setRejectedCollisionGroupPairs.Remove(
		m_pCollisionFilterState->CreateCollisionGroupPair(
			p_pCollisionGroup1, p_pCollisionGroup2));
	m_pCollisionFilterState->Update();
}

bool CollisionFilter::IsAllowed(BodyPtr p_pBody1, BodyPtr p_pBody2)
{
	// compute filter result using criteria
	bool bAllowCollision = m_pCollisionFilterState->IsAllowed(p_pBody1, p_pBody2);

	// if filter event enabled, notify listers and allow overriding of result
	EventManager* pEventManager = GravitasEngine::GetInstance()->GetEventManager();
	if (pEventManager->IsEnabled(EventFlags::CollisionFilter))
	{
		// generate filter event
		CollisionFilterEventPtr pCollisionFilterEvent(
			new CollisionFilterEvent(p_pBody1, p_pBody2, bAllowCollision));

		// notify listeners
		pEventManager->Notify(pCollisionFilterEvent);

		// overide filter result from event after possible modification
		// by listeners
		bAllowCollision = pCollisionFilterEvent->AllowCollision;
	}

	// return result
	return bAllowCollision;
}

bool CollisionFilter::IsRejected(BodyPtr p_pBody1, BodyPtr p_pBody2)
{
	return !IsAllowed(p_pBody1, p_pBody2);
}

void CollisionFilter::FilterList(BodyPairList& p_listBodyPairs)
{
	// early-out
	if (m_pCollisionFilterState->m_bClear) return;

	// process body pairs against filter
	for (int nIndex = (int) p_listBodyPairs.Size() - 1; nIndex >= 0; nIndex--)
	{
		BodyPair& bodyPair = p_listBodyPairs(nIndex);
		if (IsRejected(bodyPair.Element1, bodyPair.Element2))
			p_listBodyPairs.RemoveAt((size_t) nIndex);
	}
}
