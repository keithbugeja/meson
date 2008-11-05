#pragma once

#include "GrvNamespaces.h"
#include "GrvCollisionEvent.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// \class ICollisionListener
/// Abstract listener interface for collision events.
//------------------------------------------------------------------------------------------
class GRAVITAS_API ICollisionListener
	: public Meson::Common::TReferenceCounter<ICollisionListener>
{
public:
	virtual ~ICollisionListener(void) {};

	//------------------------------------------------------------------------------------------
	/// Handles a collision event.
	/// \param p_pCollisionEvent A managed pointer to a collision event.
	//------------------------------------------------------------------------------------------
	virtual void OnCollisionEvent(CollisionEventPtr p_pCollisionEvent) = 0;
};

//------------------------------------------------------------------------------------------
/// A managed pointer to a collision listener interface.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ICollisionListener> CollisionListenerPtr;

Meson_Gravitas_Event_END
