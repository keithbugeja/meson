#include "GrvCollisionEvent.h"

using namespace Meson::Common::Memory;

using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Event;

MemoryManager* Meson::Gravitas::Event::GetCollisionEventMemoryManager(void)
{
	MemoryManager* pMemoryManagerEvents = GetEventMemoryManager();
	static MemoryManager s_memoryManagerCollisionEvents
		= MemoryManager("Collision Events", pMemoryManagerEvents, sizeof(CollisionEvent), 256);
	return &s_memoryManagerCollisionEvents;
}

CollisionEvent::CollisionEvent(void)
	: ContactManifold(NULL)
	, Type(CollisionEventType::None)
{
}

CollisionEvent::CollisionEvent(
	const Meson::Gravitas::Collision::ContactManifold* p_pContactManifold,
	CollisionEventType::CollisionEventType p_collisionEventType)
	: ContactManifold(p_pContactManifold)
	, Type(p_collisionEventType)
{
}


CollisionEvent::~CollisionEvent(void)
{
}

void* CollisionEvent::operator new(size_t p_unSize)
{
	return GetCollisionEventMemoryManager()->AllocatePooled(p_unSize);
}

void CollisionEvent::operator delete(void* p_pObject, size_t p_unSize)
{
	::operator delete(p_pObject);
}							
