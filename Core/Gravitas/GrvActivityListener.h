#pragma once

#include "GrvNamespaces.h"
#include "GrvActivityEvent.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// \class IActivityListener
/// Abstract listener interface for activity events.
//------------------------------------------------------------------------------------------
class IActivityListener
	: public Meson::Common::TReferenceCounter<IActivityListener>
{
public:
	virtual ~IActivityListener(void) {};

	//------------------------------------------------------------------------------------------
	/// Handles an activity event.
	/// \param p_pActivityEvent A managed pointer to an activity event.
	//------------------------------------------------------------------------------------------
	virtual void OnActivityEvent(ActivityEventPtr p_pActivityEvent) = 0;
};

//------------------------------------------------------------------------------------------
/// A managed pointer to an activity listener interface.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IActivityListener> ActivityListenerPtr;

Meson_Gravitas_Event_END
