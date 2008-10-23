//----------------------------------------------------------------------------------------------
//	Meson::Common::IResourceObserver
//	Resource device interface.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "ResourceHandle.h"

Meson_Common_Resources_BEGIN

class IResourceObserver
	: public Meson::Common::TReferenceCounter<IResourceObserver>
{
public:
	virtual ~IResourceObserver(void) {};

	virtual void OnResourceProgress(
		ResourceHandlePtr p_pResourceHandle, float p_fProgress) = 0;
	virtual void OnResourceReady(ResourceHandlePtr p_pResourceHandle) = 0;
};

typedef Meson::Common::TPointer<IResourceObserver> ResourceObserverPtr;

Meson_Common_Resources_END
