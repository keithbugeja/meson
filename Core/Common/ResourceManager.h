//----------------------------------------------------------------------------------------------
//	Meson::Common::ResourceManager
//	Resource manager singleton.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Singleton.h"
#include "Enumerator.h"
#include "Set.h"
#include "List.h"
#include "ResourceDevice.h"
#include "ResourceHandler.h"
#include "ResourceHandle.h"
#include "ResourceObserver.h"

Meson_Common_Resources_BEGIN

namespace RequestMethod
{
	enum RequestMethod
	{
		Immediate,
		Background,
		Deferred
	};
}

struct ResourceManagerState;

class ResourceManager
	: public Meson::Common::TSingleton<ResourceManager>
{
	friend class Meson::Common::TSingleton<ResourceManager>;

private: // private manager;
	ResourceManagerState *m_pResourceManagerState;

private: // private methods
	ResourceManager(void);
	~ResourceManager(void);

public: // public methods

	// resource devices
	Meson::Common::Collections::TEnumerator<ResourceDevicePtr>& GetResourceDevices(void);
	void RegisterResourceDevice(ResourceDevicePtr p_pResourceDevice);
	void UnregisterResourceDevice(ResourceDevicePtr p_pResourceDevice);
	void SetActiveDevice(ResourceDevicePtr p_pResourceDevice);

	// resource handlers
	Meson::Common::Collections::TEnumerator<ResourceHandlerPtr>& GetResourceHandlers(void);
	bool SupportsResourceType(const Meson::Common::Text::String& p_strResourceType);
	ResourceHandlerPtr GetResourceHandler(const Meson::Common::Text::String& p_strResourceType);
	void RegisterResourceHandler(ResourceHandlerPtr p_pResourceHandler);
	void RegisterResourceHandler(ResourceHandlerPtr p_pResourceHandler,
		Meson::Common::Collections::TSet<Meson::Common::Text::String>& p_setExtensions);
	void UnregisterResourceHandler(ResourceHandlerPtr p_pResourceHandler);

	// resource queries
	bool ContainsResource(const Meson::Common::Text::String& p_strResourceName);
	Meson::Common::Text::String GetResourceType(
		const Meson::Common::Text::String& p_strResourceName);
	void EnumerateResources(
		const Meson::Common::Text::String& p_strResourcePath,
		Meson::Common::Collections::TList<Meson::Common::Text::String>& p_listResourceNames);
	void EnumerateResources(
		const Meson::Common::Text::String& p_strResourcePath,
		const Meson::Common::Text::String& p_strResourceType,
		Meson::Common::Collections::TList<Meson::Common::Text::String>& p_listResourceNames);
	void EnumerateResources(
		const Meson::Common::Text::String& p_strResourcePath,
		const Meson::Common::Text::String& p_strResourceType,
		const ResourceProperties& p_resourceProperties,
		Meson::Common::Collections::TList<Meson::Common::Text::String>& p_listResourceNames);

	// resource loading
	ResourceHandlePtr LoadResource(
		const Meson::Common::Text::String& p_strResourceName,
		const Meson::Common::Text::String& p_strResourceType,
		RequestMethod::RequestMethod p_requestMethod = RequestMethod::Immediate);
	ResourceHandlePtr LoadResource(
		const Meson::Common::Text::String& p_strResourceName,
		RequestMethod::RequestMethod p_requestMethod = RequestMethod::Immediate);
	void LoadResources(
		const Meson::Common::Collections::TList<Meson::Common::Text::String>& p_listResourceNames,
		RequestMethod::RequestMethod m_requestMethod,
		Meson::Common::Collections::TList<ResourceHandlePtr>& p_listResourceHandles);

	// resource loading observers
	void AddObserver(ResourceObserverPtr p_pResourceObserver);
	void RemoveObserver(ResourceObserverPtr p_pResourceObserver);
};

Meson_Common_Resources_END
