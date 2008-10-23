#pragma once

#include "GrvEvent.h"
#include "GrvBody.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas collision filter events.
/// \returns a pointer to the memory manager for Gravitas collision filter events.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetCollisionFilterEventMemoryManager(void);

//------------------------------------------------------------------------------------------
/// \class CollisionFilterEvent
/// Collision filtering event implementation that provides callback functionlity during the
/// collision filtering phase. The event exposes an AllowCollision flag to enable the
/// event listener to override the decision of the filter.
//------------------------------------------------------------------------------------------
class CollisionFilterEvent
	: public IEvent
{
public: // public variables
	//------------------------------------------------------------------------------------------
	/// First body involved in the filtering process.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Body::BodyPtr Body1;

	//------------------------------------------------------------------------------------------
	/// Second body involved in the filtering process.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Body::BodyPtr Body2;

	//------------------------------------------------------------------------------------------
	/// Filtering result, overridable by the event listener.
	//------------------------------------------------------------------------------------------
	bool AllowCollision;

public:
	//------------------------------------------------------------------------------------------
	/// Constructs a collision filter event.
	//------------------------------------------------------------------------------------------
	CollisionFilterEvent(void);

	//------------------------------------------------------------------------------------------
	/// Constructs a collision filter event with the given body references and collision flag.
	/// \param p_pBody1 First body involved in the filtering process.
	/// \param p_pBody2 Second body involved in the filtering process.
	/// \param p_bAllowCollision Flag indicating filter result - false by default.
	//------------------------------------------------------------------------------------------
	CollisionFilterEvent(
		Meson::Gravitas::Body::BodyPtr p_pBody1,
		Meson::Gravitas::Body::BodyPtr p_pBody2,
		bool p_bAllowCollision = false);

	//------------------------------------------------------------------------------------------
	/// Destoys the collision filter event.
	//------------------------------------------------------------------------------------------
	~CollisionFilterEvent(void);

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
/// Managed pointer to a collision filtering event.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<CollisionFilterEvent> CollisionFilterEventPtr;

Meson_Gravitas_Event_END
