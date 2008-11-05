#pragma once

#include "GrvNamespaces.h"
#include "GrvConstraintEvent.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// \class IConstraintListener
/// Abstract listener interface for constraint events.
//------------------------------------------------------------------------------------------
class GRAVITAS_API IConstraintListener
	: public Meson::Common::TReferenceCounter<IConstraintListener>
{
public:
	virtual ~IConstraintListener(void) {};

	//------------------------------------------------------------------------------------------
	/// Handles a constraint event.
	/// \param p_pConstraintEvent A managed pointer to a constraint event.
	//------------------------------------------------------------------------------------------
	virtual void OnConstraintEvent(ConstraintEventPtr p_pConstraintEvent) = 0;
};

//------------------------------------------------------------------------------------------
/// A managed pointer to a constraint listener interface.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IConstraintListener> ConstraintListenerPtr;

Meson_Gravitas_Event_END
