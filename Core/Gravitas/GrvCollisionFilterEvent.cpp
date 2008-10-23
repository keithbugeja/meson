#include "GrvCollisionFilterEvent.h"

using namespace Meson::Common::Memory;

using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Event;

MemoryManager* Meson::Gravitas::Event::GetCollisionFilterEventMemoryManager(void)
{
	MemoryManager* pMemoryManagerEvents = GetEventMemoryManager();
	static MemoryManager s_memoryManagerCollisionFilterEvents
		= MemoryManager("Collision Filter Events", pMemoryManagerEvents, sizeof(CollisionFilterEvent), 256);
	return &s_memoryManagerCollisionFilterEvents;
}

CollisionFilterEvent::CollisionFilterEvent(void)
	: Body1()
	, Body2()
	, AllowCollision(false)
{
}

CollisionFilterEvent::CollisionFilterEvent(
	BodyPtr p_pBody1, BodyPtr p_pBody2, bool p_bAllowCollision)
	: Body1(p_pBody1)
	, Body2(p_pBody2)
	, AllowCollision(p_bAllowCollision)
{
}

CollisionFilterEvent::~CollisionFilterEvent(void)
{
}

void* CollisionFilterEvent::operator new(size_t p_unSize)
{
	return GetCollisionFilterEventMemoryManager()->AllocatePooled(p_unSize);
}

void CollisionFilterEvent::operator delete(void* p_pObject, size_t p_unSize)
{
	::operator delete(p_pObject);
}							
