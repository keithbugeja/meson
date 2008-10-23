#pragma once

#include "GrvNamespaces.h"
#include "GrvMaterialEvent.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// \class IMaterialListener
/// Abstract listener interface for material events.
//------------------------------------------------------------------------------------------
class IMaterialListener
	: public Meson::Common::TReferenceCounter<IMaterialListener>
{
public:
	virtual ~IMaterialListener(void) {};

	//------------------------------------------------------------------------------------------
	/// Handles a material event.
	/// \param p_pMaterialEvent A managed pointer to a material event.
	//------------------------------------------------------------------------------------------
	virtual void OnMaterialEvent(MaterialEventPtr p_pMaterialEvent) = 0;
};

//------------------------------------------------------------------------------------------
/// A managed pointer to a material listener interface.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IMaterialListener> MaterialListenerPtr;

Meson_Gravitas_Event_END
