#pragma once

#include "GrvEvent.h"
#include "GrvBody.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas activity events.
/// \returns a pointer to the memory manager for Gravitas activity events.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetActivityEventMemoryManager(void);

namespace ActivityEventType
{
	//------------------------------------------------------------------------------------------
	/// Enumerator for the type of activity event.
	//------------------------------------------------------------------------------------------
	enum ActivityEventType
	{
		None,
		Suspend,
		Resume
	};
}

//------------------------------------------------------------------------------------------
/// \class ActivityEvent
/// Activity event implementation for capturing changes in the activity status of bodies.
//------------------------------------------------------------------------------------------
class ActivityEvent :
	public IEvent
{
public: // public variables

	//------------------------------------------------------------------------------------------
	/// Managed pointer to a body involved in the activity event.
	//------------------------------------------------------------------------------------------
	const Meson::Gravitas::Body::BodyPtr Body;

	//------------------------------------------------------------------------------------------
	/// Type of activity event (suspended, resumed).
	//------------------------------------------------------------------------------------------
	ActivityEventType::ActivityEventType Type;

public: // public methods
	//------------------------------------------------------------------------------------------
	/// Constructs an activity event.
	//------------------------------------------------------------------------------------------
	ActivityEvent(void);

	//------------------------------------------------------------------------------------------
	/// Constructs an activity event with the given contact manifold and collision event type.
	/// \param p_pBody Managed pointer to a body.
	/// \param p_activityEventType Type of activity event.
	//------------------------------------------------------------------------------------------
	ActivityEvent(Meson::Gravitas::Body::BodyPtr p_pBody,
		ActivityEventType::ActivityEventType p_activityEventType);

	//------------------------------------------------------------------------------------------
	/// Destroys the activity event.
	//------------------------------------------------------------------------------------------
	~ActivityEvent(void);

	//------------------------------------------------------------------------------------------
	/// Allocates memory for the object prior to construction.
	/// \param p_unSize Size of the required memory buffer in bytes.
	/// \returns a pointer to the allocated buffer.
	//------------------------------------------------------------------------------------------
	void* operator new(size_t p_unSize);

	//------------------------------------------------------------------------------------------
	/// Deallocates the object's memory following destruction.
	/// \param p_pObject Pointer to the memory buffer to deallocate.
	/// \param p_unSize Size of the memory buffer in bytes.
	//------------------------------------------------------------------------------------------
	void operator delete(void* p_pObject, size_t p_unSize);
};

//------------------------------------------------------------------------------------------
/// Managed pointer to an activity event.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ActivityEvent> ActivityEventPtr;

Meson_Gravitas_Event_END
