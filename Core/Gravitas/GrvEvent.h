#pragma once

#include "Memory.h"
#include "Pointer.h"

#include "GrvNamespaces.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas events.
/// \returns a pointer to the memory manager for Gravitas events.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetEventMemoryManager(void);

//------------------------------------------------------------------------------------------
/// \class IEvent
/// Base interface for an event.
//------------------------------------------------------------------------------------------
class IEvent
	: public Meson::Common::TReferenceCounter<IEvent>
{
public:
	virtual ~IEvent(void) {};
};

//------------------------------------------------------------------------------------------
/// Managed pointer to an event.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IEvent> EventPtr;

Meson_Gravitas_Event_END
