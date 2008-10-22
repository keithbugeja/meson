//----------------------------------------------------------------------------------------------
//	Meson::Common::ResourceHandle
//	Resource handle that maintains a reference and state for a resource.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Pointer.h"
#include "Resource.h"
#include "ResourceDevice.h"
#include "ResourceHandler.h"

Meson_Common_Resources_BEGIN

namespace ResourceState
{
	enum ResourceState
	{
		Invalid,
		Offline,
		Loading,
		Ready
	};
}

class ResourceHandleState;

class ResourceHandle
	: public Meson::Common::TReferenceCounter<ResourceHandle>
{
	friend class ResourceManager;

private: // private variables
	ResourceHandleState* m_pResourceHandleState;

private: // private methods
	void ConfigureHandle(ResourceState::ResourceState p_resourceState,
		const Meson::Common::Text::String& p_strResourceName,
		ResourceHandlerPtr p_pResourceHandler,
		ResourceDevicePtr p_pResourceDevice,
		ResourcePtr p_pResource);

public: // public methods
	ResourceHandle(void);
	~ResourceHandle(void);

	ResourceState::ResourceState GetState(void);
	ResourcePtr GetResource(bool p_bWaitForResult = true);
};

typedef Meson::Common::TPointer<ResourceHandle> ResourceHandlePtr;

Meson_Common_Resources_END
