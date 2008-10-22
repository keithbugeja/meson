//----------------------------------------------------------------------------------------------
//	Meson::Common::IResource
//	Resource data interface.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"

Meson_Common_Resources_BEGIN

class IResource
	: public Meson::Common::TReferenceCounter<IResource>
{
public:
	virtual ~IResource(void) {};
};

typedef Meson::Common::TPointer<IResource> ResourcePtr;

Meson_Common_Resources_END
