#include "GrvCollisionGroup.h"

#include "TreeSet.h"

#include "GrvBody.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;

Meson_Gravitas_Collision_BEGIN

struct CollisionGroupState
{
	String Name;
	TTreeSet<BodyPtr> Bodies;

	CollisionGroupState(void) : Name(), Bodies() { }
	CollisionGroupState(const String& p_strName) : Name(p_strName), Bodies() { }
};

Meson_Gravitas_Collision_END

CollisionGroup::CollisionGroup(void)
	: m_pCollisionGroupState(new CollisionGroupState())
{
}

CollisionGroup::CollisionGroup(const String& p_strName)
	: m_pCollisionGroupState(new CollisionGroupState(p_strName))
{
}

CollisionGroup::~CollisionGroup(void)
{
	delete m_pCollisionGroupState;
}

const String& CollisionGroup::GetName(void) const
{
	return m_pCollisionGroupState->Name;
}

void CollisionGroup::SetName(const String& p_strName)
{
	m_pCollisionGroupState->Name = p_strName;
}

TEnumerator<BodyPtr>& CollisionGroup::GetAssignedBodies(void) const
{
	return m_pCollisionGroupState->Bodies.GetEnumerator();
}

bool CollisionGroup::Contains(BodyPtr p_pBody) const
{
	TSet<BodyPtr>& setBodies = m_pCollisionGroupState->Bodies;
	return setBodies.ContainsElement(p_pBody);
}

void CollisionGroup::Add(BodyPtr p_pBody)
{
	TSet<BodyPtr>& setBodies = m_pCollisionGroupState->Bodies;
	if (setBodies.ContainsElement(p_pBody))
		return;
	
	setBodies.Insert(p_pBody);
	p_pBody->AddToCollisionGroup(CollisionGroupPtr(this));
}

void CollisionGroup::Remove(BodyPtr p_pBody)
{
	TSet<BodyPtr>& setBodies = m_pCollisionGroupState->Bodies;
	if (!setBodies.ContainsElement(p_pBody))
		return;

	setBodies.Remove(p_pBody);
	p_pBody->RemoveFromCollisionGroup(CollisionGroupPtr(this));
}
