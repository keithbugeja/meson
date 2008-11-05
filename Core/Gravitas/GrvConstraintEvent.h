#pragma once

#include "GrvEvent.h"
#include "GrvConstraint.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas constraint events.
/// \returns a pointer to the memory manager for Gravitas constraint events.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetConstraintEventMemoryManager(void);

namespace ConstraintEventType
{
	//------------------------------------------------------------------------------------------
	/// Enumerator for the type of constraint event.
	//------------------------------------------------------------------------------------------
	enum ConstraintEventType
	{
		None,
		Broken
	};
}

//------------------------------------------------------------------------------------------
/// \class ConstraintEvent
/// Constraint event implementation for capturing constraint-related events. This event type
/// currently only support constraint breakage events.
//------------------------------------------------------------------------------------------
class GRAVITAS_API ConstraintEvent :
	public IEvent
{
public: // public variables

	//------------------------------------------------------------------------------------------
	/// Managed pointer to a constraint related to this event.
	//------------------------------------------------------------------------------------------
	const Meson::Gravitas::Constraint::ConstraintPtr Constraint;

	//------------------------------------------------------------------------------------------
	/// Type of constraint event.
	//------------------------------------------------------------------------------------------
	ConstraintEventType::ConstraintEventType Type;

public: // public methods

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint event.
	//------------------------------------------------------------------------------------------
	ConstraintEvent(void);

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint event with the given constraint and event type.
	/// \param p_pConstraint A managed pointer to a constraint.
	/// \param p_ConstraintEventType The type of constraint event.
	//------------------------------------------------------------------------------------------
	ConstraintEvent(const Meson::Gravitas::Constraint::ConstraintPtr p_pConstraint,
		ConstraintEventType::ConstraintEventType p_ConstraintEventType);

	//------------------------------------------------------------------------------------------
	/// Destroys the constraint event.
	//------------------------------------------------------------------------------------------
	~ConstraintEvent(void);

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
/// Managed pointer to a constraint event.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ConstraintEvent> ConstraintEventPtr;

Meson_Gravitas_Event_END
