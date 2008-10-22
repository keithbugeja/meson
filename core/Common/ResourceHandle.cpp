//----------------------------------------------------------------------------------------------
//	Meson::Common::ResourceHandle
//	Resource handle that maintains a reference and state for a resource.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "ResourceHandle.h"

#include "Thread.h"
#include "MesonException.h"

#include "ResourceHandler.h"
#include "ResourceDevice.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::IO;
using namespace Meson::Common::Resources;

Meson_Common_Resources_BEGIN

class ResourceHandleState
	: public Meson::Common::System::Thread
{
public:
	ResourceState::ResourceState m_resourceState;
	String m_strResourceName;
	ResourceHandlerPtr m_pResourceHandler;
	ResourceDevicePtr m_pResourceDevice;
	ResourcePtr m_pResource;

	ResourceHandleState(void)
		: m_resourceState(ResourceState::Invalid)
		, m_strResourceName("")
		, m_pResourceHandler(NULL)
		, m_pResourceDevice(NULL)
		, m_pResource(NULL) {}

	void LoadResource(void)
	{
		// get input stream from associated device
		InputStreamPtr pInputStream
			= m_pResourceDevice->GetInputStream(m_strResourceName);

		// load from associated handler
		m_pResourceHandler->Load(pInputStream);

		// set resource to Ready state
		m_resourceState = ResourceState::Ready;
	}

	void Execute(void)
	{
		LoadResource();
	}
};

Meson_Common_Resources_END

void ResourceHandle::ConfigureHandle(
	ResourceState::ResourceState p_resourceState,
	const String& p_strResourceName,
	ResourceHandlerPtr p_pResourceHandler,
	ResourceDevicePtr p_pResourceDevice,
	ResourcePtr p_pResource)
{
	m_pResourceHandleState->m_resourceState = p_resourceState;
	m_pResourceHandleState->m_strResourceName = p_strResourceName;
	m_pResourceHandleState->m_pResourceHandler = p_pResourceHandler;
	m_pResourceHandleState->m_pResourceDevice = p_pResourceDevice;
	m_pResourceHandleState->m_pResource = p_pResource;
}

ResourceHandle::ResourceHandle(void)
	: m_pResourceHandleState(NULL)
{
	m_pResourceHandleState = new ResourceHandleState();
}

ResourceHandle::~ResourceHandle(void)
{
	delete m_pResourceHandleState;
}

ResourceState::ResourceState ResourceHandle::GetState(void)
{
	return m_pResourceHandleState->m_resourceState;
}

ResourcePtr ResourceHandle::GetResource(bool p_bWaitForResult)
{
	// ensure handle not in invalid state
	if (m_pResourceHandleState->m_resourceState == ResourceState::Invalid)
		throw new MesonException("Cannot query a resource handle in an invalid state.",
			__FILE__, __LINE__);

	// return data immediately if ready (waiting irrelevant)
	if (m_pResourceHandleState->m_resourceState == ResourceState::Ready)
		return m_pResourceHandleState->m_pResource;

	// case where resource already loading
	if (m_pResourceHandleState->m_resourceState == ResourceState::Loading)
	{
		if (p_bWaitForResult)
		{
			// wait for loading thread to finish
			while (m_pResourceHandleState->IsRunning())
				m_pResourceHandleState->Sleep(0.0);

			// return resource
			return m_pResourceHandleState->m_pResource;
		}
		else
			// return immediately with null
			return ResourcePtr(NULL);
	}

	// case where resource offline
	if (p_bWaitForResult)
	{
		// wait requested - load now
		m_pResourceHandleState->LoadResource();

		// return resource
		return m_pResourceHandleState->m_pResource;
	}
	else
	{
		// wait not required - start resource loading in the background
		// and return null
		m_pResourceHandleState->Start();
		return ResourcePtr(NULL);
	}
}
