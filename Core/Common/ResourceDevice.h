//----------------------------------------------------------------------------------------------
//	Meson::Common::IResourceDevice
//	Resource device interface.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Pointer.h"
#include "InputStream.h"
#include "OutputStream.h"

Meson_Common_Resources_BEGIN

class IResourceDevice
	: public Meson::Common::TReferenceCounter<IResourceDevice>
{
public:
	virtual ~IResourceDevice(void) {};

	virtual void EnumerateResources(const Meson::Common::Text::String& p_strResourcePath,
		Meson::Common::Collections::TList<Meson::Common::Text::String>& p_listResourceNames) = 0;
	virtual bool ContainsResource(const Meson::Common::Text::String& p_strResourceName) = 0;

	virtual Meson::Common::IO::InputStreamPtr GetInputStream(
		const Meson::Common::Text::String& p_strResourceName) = 0;
	virtual Meson::Common::IO::OutputStreamPtr GetOutputStream(
		const Meson::Common::Text::String& p_strResourceName) = 0;
};

typedef Meson::Common::TPointer<IResourceDevice> ResourceDevicePtr;

Meson_Common_Resources_END
