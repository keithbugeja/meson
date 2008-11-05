#pragma once

#include "GrvNamespaces.h"
#include "GrvCollisionFilterEvent.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// \class ICollisionFilterListener
/// Abstract listener interface for collision filtering events.
//------------------------------------------------------------------------------------------
class GRAVITAS_API ICollisionFilterListener
	: public Meson::Common::TReferenceCounter<ICollisionFilterListener>
{
public:
	virtual ~ICollisionFilterListener(void) {};

	//------------------------------------------------------------------------------------------
	/// Handles a collision filter event.
	/// \param p_pCollisionFilterEvent A managed pointer to a collision filter event.
	//------------------------------------------------------------------------------------------
	virtual void OnCollisionFilterEvent(CollisionFilterEventPtr p_pCollisionFilterEvent) = 0;
};

//------------------------------------------------------------------------------------------
/// A managed pointer to a collision filter listener interface.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ICollisionFilterListener> CollisionFilterListenerPtr;

Meson_Gravitas_Event_END
