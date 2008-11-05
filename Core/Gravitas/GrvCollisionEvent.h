#pragma once

#include "GrvEvent.h"
#include "GrvContactManifold.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas collision events.
/// \returns a pointer to the memory manager for Gravitas collision events.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetCollisionEventMemoryManager(void);

namespace CollisionEventType
{
	//------------------------------------------------------------------------------------------
	/// Enumerator for the type of collision event.
	//------------------------------------------------------------------------------------------
	enum CollisionEventType
	{
		None,
		Start,
		Ongoing,
		End
	};
}

//------------------------------------------------------------------------------------------
/// \class CollisionEvent
/// Collision event implementation for capturing collisions in their initial, ongoing and
/// terminal phases.
//------------------------------------------------------------------------------------------
class GRAVITAS_API CollisionEvent :
	public IEvent
{
public: // public variables

	//------------------------------------------------------------------------------------------
	/// Pointer to a contact manifold describing the collision.
	//------------------------------------------------------------------------------------------
	const Meson::Gravitas::Collision::ContactManifold* ContactManifold;

	//------------------------------------------------------------------------------------------
	/// Type of collision event (starting, ongoing or ending).
	//------------------------------------------------------------------------------------------
	CollisionEventType::CollisionEventType Type;

public: // public methods
	//------------------------------------------------------------------------------------------
	/// Constructs a collision event.
	//------------------------------------------------------------------------------------------
	CollisionEvent(void);

	//------------------------------------------------------------------------------------------
	/// Constructs a collision event with the given contact manifold and collision event type.
	/// \param p_pContactManifold Pointer to a contact manifold structure.
	/// \param p_collisionEventType Type of collision event.
	//------------------------------------------------------------------------------------------
	CollisionEvent(const Meson::Gravitas::Collision::ContactManifold* p_pContactManifold,
		CollisionEventType::CollisionEventType p_collisionEventType);

	//------------------------------------------------------------------------------------------
	/// Destroys the collision event.
	//------------------------------------------------------------------------------------------
	~CollisionEvent(void);

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
/// Managed pointer to a collision event.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<CollisionEvent> CollisionEventPtr;

Meson_Gravitas_Event_END
