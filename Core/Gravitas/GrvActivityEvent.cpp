#include "GrvActivityEvent.h"

using namespace Meson::Common::Memory;

using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Event;

MemoryManager* Meson::Gravitas::Event::GetActivityEventMemoryManager(void)
{
	MemoryManager* pMemoryManagerEvents = GetEventMemoryManager();
	static MemoryManager s_memoryManagerActivityEvents
		= MemoryManager("~Activity Events", pMemoryManagerEvents, sizeof(ActivityEvent), 256);
	return &s_memoryManagerActivityEvents;
}

ActivityEvent::ActivityEvent(void)
	: Body(NULL)
	, Type(ActivityEventType::None)
{
}

ActivityEvent::ActivityEvent(
	BodyPtr p_pBody,
	ActivityEventType::ActivityEventType p_activityEventType)
	: Body(p_pBody)
	, Type(p_activityEventType)
{
}


ActivityEvent::~ActivityEvent(void)
{
}

void* ActivityEvent::operator new(size_t p_unSize)
{
	return GetActivityEventMemoryManager()->AllocatePooled(p_unSize);
}

void ActivityEvent::operator delete(void* p_pObject, size_t p_unSize)
{
	::operator delete(p_pObject);
}							
