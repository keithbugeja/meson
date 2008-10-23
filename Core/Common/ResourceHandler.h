//----------------------------------------------------------------------------------------------
//	Meson::Common::IResourceHandler
//	Resource handler interface.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Pointer.h"
#include "String8.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "Resource.h"
#include "ResourceProperties.h"

Meson_Common_Resources_BEGIN

class IResourceHandler
	: public Meson::Common::TReferenceCounter<IResourceHandler>
{
public:
	virtual ~IResourceHandler(void) {};

	virtual const Meson::Common::Text::String& GetType(void) const = 0;

	virtual bool SupportsResource(
		const Meson::Common::IO::InputStreamPtr p_pInputStream) = 0;
	virtual void GetResourceProperties(
		const Meson::Common::IO::InputStreamPtr p_pInputStream,
		ResourceProperties& p_resourceProperties) = 0;

	virtual ResourcePtr Load(
		Meson::Common::IO::InputStreamPtr p_inputStream) = 0;

	virtual void Store(Meson::Common::IO::IInputStream& p_inputStream,
		ResourcePtr p_p_Resource) = 0;
};

typedef Meson::Common::TPointer<IResourceHandler> ResourceHandlerPtr;

Meson_Common_Resources_END
