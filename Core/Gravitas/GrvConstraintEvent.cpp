#include "GrvConstraintEvent.h"

using namespace Meson::Common::Memory;

using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Event;

MemoryManager* Meson::Gravitas::Event::GetConstraintEventMemoryManager(void)
{
	MemoryManager* pMemoryManagerEvents = GetEventMemoryManager();
	static MemoryManager s_memoryManagerConstraintEvents
		= MemoryManager("Constraint Events", pMemoryManagerEvents, sizeof(ConstraintEvent), 256);
	return &s_memoryManagerConstraintEvents;
}

ConstraintEvent::ConstraintEvent(void)
	: Constraint()
	, Type(ConstraintEventType::None)
{
}

ConstraintEvent::ConstraintEvent(
	const ConstraintPtr p_pConstraint,
	ConstraintEventType::ConstraintEventType p_constraintEventType)
	: Constraint(p_pConstraint)
	, Type(p_constraintEventType)
{
}

ConstraintEvent::~ConstraintEvent(void)
{
}

void* ConstraintEvent::operator new(size_t p_unSize)
{
	return GetConstraintEventMemoryManager()->AllocatePooled(p_unSize);
}

void ConstraintEvent::operator delete(void* p_pObject, size_t p_unSize)
{
	::operator delete(p_pObject);
}							
