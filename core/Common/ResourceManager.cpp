//----------------------------------------------------------------------------------------------
//	Meson::Common::ResourceManager
//	Resource manager singleton.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "ResourceManager.h"

#include "TreeSet.h"
#include "TreeMap.h"
#include "ArrayList.h"
#include "Character8.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Resources;

Meson_Common_Resources_BEGIN

struct ResourceManagerState
{
	TTreeSet<ResourceDevicePtr> m_setResourceDevices;
	ResourceDevicePtr m_pResourceDeviceActive;
	TTreeMap<String, ResourceHandlerPtr> m_mapResourceHandlersByType;
	TTreeMap<String, ResourceHandlerPtr> m_mapResourceHandlersByExtension;
	TTreeSet<ResourceObserverPtr> m_setResourceObservers;

	ResourceManagerState(void)
		: m_setResourceDevices()
		, m_pResourceDeviceActive(NULL)
		, m_mapResourceHandlersByType()
		, m_mapResourceHandlersByExtension()
		, m_setResourceObservers()
	{ }

	bool IsResourceNameValid(const String& p_strResourceName)
	{
		// splti path into nodes using '/ divider
		TArrayList<String> listNodes;
		p_strResourceName.Split('/', listNodes);

		//check all nodes
		for (size_t unNodeIndex = 0; unNodeIndex < listNodes.Size(); unNodeIndex++)
		{
			String& strNode = listNodes(unNodeIndex);

			// ensure node not empty
			if (strNode.IsEmpty()) return false;

			// check node string contents
			for (size_t unCharIndex = 0; unCharIndex < strNode.Length(); unCharIndex++)
			{
				char ch = strNode[unCharIndex];
				if (!Character::IsLetterOrDigit(ch)
					&& ch != '_' && ch != '-' && ch != '.')
					return false;
			}
		}

		// if this point reached, name valid
		return true;
	}

	void ValidateResourceName(const String& p_strResourceName)
	{
		if (!IsResourceNameValid(p_strResourceName))
			throw new MesonException("Invalid resource name: " + p_strResourceName,
				__FILE__, __LINE__);
	}

	String GetResourceNameExtention(const String& p_strResourceName)
	{
		ValidateResourceName(p_strResourceName);
		for (int nIndex = (int) p_strResourceName.Length() - 1; nIndex >= 0; nIndex--)
		{
			char ch = p_strResourceName[nIndex];

			// if divider encountered first, there is no extension
			if (ch == '/') return "";

			// period enncountered - extension found
			if (ch == '.')
				return p_strResourceName.Substring(
					nIndex + 1, p_strResourceName.Length() - nIndex - 1);
		}

		// if this point reached, no extension
		return "";
	}

	void NotifyObserversOnProgress(
		ResourceHandlePtr p_pResourceHandle, float p_fProgress)
	{
		TEnumerator<ResourceObserverPtr>& enumResourceObservers
			= m_setResourceObservers.GetEnumerator();
		while(enumResourceObservers.HasMoreElements())
			enumResourceObservers.NextElement()->OnResourceProgress(
				p_pResourceHandle, p_fProgress);
	}

	void NotifyObserversOnReady(ResourceHandlePtr p_pResourceHandle)
	{
		TEnumerator<ResourceObserverPtr>& enumResourceObservers
			= m_setResourceObservers.GetEnumerator();
		while(enumResourceObservers.HasMoreElements())
			enumResourceObservers.NextElement()->OnResourceReady(
				p_pResourceHandle);
	}
};

Meson_Common_Resources_END

ResourceManager::ResourceManager(void)
	: m_pResourceManagerState(NULL)
{
	m_pResourceManagerState = new ResourceManagerState();
}

ResourceManager::~ResourceManager(void)
{
	delete m_pResourceManagerState;
}

TEnumerator<ResourceDevicePtr>& ResourceManager::GetResourceDevices(void)
{
	return m_pResourceManagerState->m_setResourceDevices.GetEnumerator();
}

void ResourceManager::RegisterResourceDevice(ResourceDevicePtr p_pResourceDevice)
{
	TSet<ResourceDevicePtr>& setResourceDevices =
		m_pResourceManagerState->m_setResourceDevices;

	// ensure device not already registered
	if (setResourceDevices.ContainsElement(p_pResourceDevice))
		throw new MesonException("Resource device already registered.",
			__FILE__, __LINE__);

	setResourceDevices.Insert(p_pResourceDevice);
}

void ResourceManager::UnregisterResourceDevice(ResourceDevicePtr p_pResourceDevice)
{
	TSet<ResourceDevicePtr>& setResourceDevices =
		m_pResourceManagerState->m_setResourceDevices;

	// ensure device is registered first
	if (!setResourceDevices.ContainsElement(p_pResourceDevice))
		throw new MesonException("Resource device is not registered.",
			__FILE__, __LINE__);

	// nullify active device if it is being removed
	if (p_pResourceDevice == m_pResourceManagerState->m_pResourceDeviceActive)
		m_pResourceManagerState->m_pResourceDeviceActive = NULL;

	// remove device
	setResourceDevices.Remove(p_pResourceDevice);
}

void ResourceManager::SetActiveDevice(ResourceDevicePtr p_pResourceDevice)
{
	TSet<ResourceDevicePtr>& setResourceDevices =
		m_pResourceManagerState->m_setResourceDevices;

	// ensure device is registered first
	if (!setResourceDevices.ContainsElement(p_pResourceDevice))
		throw new MesonException("Cannot set an unregistered active device.",
			__FILE__, __LINE__);

	// set device as active
	m_pResourceManagerState->m_pResourceDeviceActive = p_pResourceDevice;
}

TEnumerator<ResourceHandlerPtr>& ResourceManager::GetResourceHandlers(void)
{
	return m_pResourceManagerState->m_mapResourceHandlersByType.GetValueEnumerator();
}

bool ResourceManager::SupportsResourceType(const String& p_strResourceType)
{
	return m_pResourceManagerState->m_mapResourceHandlersByType.ContainsKey(p_strResourceType);
}

ResourceHandlerPtr ResourceManager::GetResourceHandler(const String& p_strResourceType)
{
	ResourceHandlerPtr pResourceHandler;
	if (m_pResourceManagerState->m_mapResourceHandlersByType.ContainsKey(
		p_strResourceType, pResourceHandler))
		return pResourceHandler;

	throw new MesonException(
		"No handler for resource type '" + p_strResourceType + "' registered.",
		__FILE__, __LINE__);
}


void ResourceManager::RegisterResourceHandler(ResourceHandlerPtr p_pResourceHandler)
{
	const String& strResourceType = p_pResourceHandler->GetType();
	if (m_pResourceManagerState->m_mapResourceHandlersByType.ContainsKey(strResourceType))
		throw new MesonException(
			"Handler for resource type '" + strResourceType + "' already registered.",
			__FILE__, __LINE__);

	m_pResourceManagerState->m_mapResourceHandlersByType[strResourceType]
		= p_pResourceHandler;
}

void ResourceManager::RegisterResourceHandler(ResourceHandlerPtr p_pResourceHandler,
	TSet<String>& p_setExtensions)
{
	TTreeMap<String, ResourceHandlerPtr>& mapResourceHandlersByExtension
		= m_pResourceManagerState->m_mapResourceHandlersByExtension;

	// check if extensions defined already
	TEnumerator<String>& enumExtensions = p_setExtensions.GetEnumerator();
	while (enumExtensions.HasMoreElements())
	{
		String& strExtension = enumExtensions.NextElement();
		if (mapResourceHandlersByExtension.ContainsKey(strExtension))
			throw MesonException(
				"A handler for resource extension '" + strExtension + "' is already defined.",
				__FILE__, __LINE__);
	}

	// register handler by type
	RegisterResourceHandler(p_pResourceHandler);

	// map extensions to handler
	enumExtensions.Reset();
	while (enumExtensions.HasMoreElements())
	{
		String& strExtension = enumExtensions.NextElement();
		mapResourceHandlersByExtension[strExtension] = p_pResourceHandler;
	}
}

void ResourceManager::UnregisterResourceHandler(ResourceHandlerPtr p_pResourceHandler)
{
	const String& strResourceType = p_pResourceHandler->GetType();
	if (!m_pResourceManagerState->m_mapResourceHandlersByType.ContainsKey(strResourceType))
		throw new MesonException(
			"Handler for resource type '" + strResourceType + "' is not registered.",
			__FILE__, __LINE__);

	m_pResourceManagerState->m_mapResourceHandlersByType.RemoveKey(strResourceType);

	// remove any mapped extensions
	m_pResourceManagerState->m_mapResourceHandlersByExtension.RemoveValue(
		p_pResourceHandler);
}

bool ResourceManager::ContainsResource(const String& p_strResourceName)
{
	// validate resource name
	m_pResourceManagerState->ValidateResourceName(p_strResourceName);

	// ensure active resource device set
	ResourceDevicePtr pResourceDevice
		= m_pResourceManagerState->m_pResourceDeviceActive;
	if (pResourceDevice == NULL)
		throw new MesonException("No active resource device set.",
			__FILE__, __LINE__);

	return pResourceDevice->ContainsResource(p_strResourceName);
}

String ResourceManager::GetResourceType(const String& p_strResourceName)
{
	// validate resource name
	m_pResourceManagerState->ValidateResourceName(p_strResourceName);

	// ensure active resource device set
	ResourceDevicePtr pResourceDevice
		= m_pResourceManagerState->m_pResourceDeviceActive;
	if (pResourceDevice == NULL)
		throw new MesonException("No active resource device set.",
			__FILE__, __LINE__);

	// ensure resource exists
	if (!pResourceDevice->ContainsResource(p_strResourceName))
		throw new MesonException("Resource '" + p_strResourceName + "' not found.",
			__FILE__, __LINE__);

	// first attempt to map by extension
	String strExtension
		= m_pResourceManagerState->GetResourceNameExtention(p_strResourceName);
	if (!strExtension.IsEmpty())
	{
		TTreeMap<String, ResourceHandlerPtr>& mapResourceHandlersByExtension
			= m_pResourceManagerState->m_mapResourceHandlersByExtension;
		ResourceHandlerPtr pResourceHandler;
		if (mapResourceHandlersByExtension.ContainsKey(strExtension, pResourceHandler))
			return pResourceHandler->GetType();
	}

	// otherwise, attempt find handler that supports resource data format

	// get all resource handlers
	TEnumerator<ResourceHandlerPtr>& enumResourceHandlers
		= m_pResourceManagerState->m_mapResourceHandlersByType.GetValueEnumerator();

	InputStreamPtr pInputStream
		= pResourceDevice->GetInputStream(p_strResourceName);

	while (enumResourceHandlers.HasMoreElements())
	{
		ResourceHandlerPtr pResourceHandler
			= enumResourceHandlers.NextElement();

		if (pResourceHandler->SupportsResource(pInputStream))
			return pResourceHandler->GetType();
	}

	throw new MesonException("Unable to match a handler for the given resource.",
		__FILE__, __LINE__);
}

void ResourceManager::EnumerateResources(
	const String& p_strResourcePath,
	TList<String>& p_listResourceNames)
{
	// ensure active resource device set
	ResourceDevicePtr pResourceDevice
		= m_pResourceManagerState->m_pResourceDeviceActive;
	if (pResourceDevice == NULL)
		throw new MesonException("No active resource device set.",
			__FILE__, __LINE__);

	pResourceDevice->EnumerateResources(p_strResourcePath, p_listResourceNames);
}

void ResourceManager::EnumerateResources(
	const String& p_strResourcePath, const String& p_strResourceType,
	TList<String>& p_listResourceNames)
{
	// enumerate all by path
	EnumerateResources(p_strResourcePath, p_listResourceNames);

	// filter by type
	for (size_t unIndex = 0; unIndex < p_listResourceNames.Size();)
	{
		if (GetResourceType(p_listResourceNames(unIndex) != p_strResourceType))
			p_listResourceNames.RemoveAt(unIndex);
		else
			++unIndex;
	}
}

void ResourceManager::EnumerateResources(
	const String& p_strResourcePath, const String& p_strResourceType,
	const ResourceProperties& p_resourceProperties,
	TList<String>& p_listResourceNames)
{
	// enumerate all by path
	EnumerateResources(p_strResourcePath, p_listResourceNames);

	// ensure active resource device set
	ResourceDevicePtr pResourceDevice
		= m_pResourceManagerState->m_pResourceDeviceActive;
	if (pResourceDevice == NULL)
		throw new MesonException("No active resource device set.",
			__FILE__, __LINE__);

	// TODO: filter by resource properties
	for (size_t unIndex = 0; unIndex < p_listResourceNames.Size();)
	{
		// get resource name and determine type
		const String& strResourceName = p_listResourceNames(unIndex);
		String strResourceType = GetResourceType(strResourceName);

		// filter by type
		if (GetResourceType(p_listResourceNames(unIndex) != p_strResourceType))
		{
			p_listResourceNames.RemoveAt(unIndex);
			continue;
		}

		// get resource properties through handler and device
		ResourceHandlerPtr pResourceHandler
			= m_pResourceManagerState->m_mapResourceHandlersByType[strResourceType];
		InputStreamPtr pInputStream = pResourceDevice->GetInputStream(strResourceName);
		ResourceProperties resourceProperties;
		pResourceHandler->GetResourceProperties(pInputStream, resourceProperties);

		// filter by satisfied properties
		if (p_resourceProperties.Satisfy(resourceProperties))
			p_listResourceNames.RemoveAt(unIndex);
		else
			++unIndex;
	}
}

ResourceHandlePtr ResourceManager::LoadResource(
	const String& p_strResourceName, const String& p_strType,
	RequestMethod::RequestMethod p_requestMethod)
{
	// validate resource name
	m_pResourceManagerState->ValidateResourceName(p_strResourceName);

	// ensure resource device set
	ResourceDevicePtr pResourceDevice
		= m_pResourceManagerState->m_pResourceDeviceActive;
	if (pResourceDevice == NULL)
		throw new MesonException("No active resource device set.",
			__FILE__, __LINE__);

	// ensure resource exists
	if (!pResourceDevice->ContainsResource(p_strResourceName))
		throw new MesonException("Resource '" + p_strType + "' not found.",
			__FILE__, __LINE__);

	// ensure matching type handler registered
	ResourceHandlerPtr pResourceHandler;
	if (!m_pResourceManagerState->m_mapResourceHandlersByType.ContainsKey(
		p_strType, pResourceHandler))
		throw new MesonException("No handler registered for resource type '" + p_strType + "'.",
			__FILE__, __LINE__);

	// get input stream for the resource from the active device
	InputStreamPtr pInputStream
		= pResourceDevice->GetInputStream(p_strResourceName);

	// ensure resource data format supported
	if (!pResourceHandler->SupportsResource(pInputStream))
		throw new MesonException(
			"The resource data format does not match the resource type '"
			+ p_strType + "'.", __FILE__, __LINE__);

	// prepare resource handle and set common properties
	ResourceHandlePtr pResourceHandle = ResourceHandlePtr(new ResourceHandle());

	ResourceState::ResourceState resourceState = ResourceState::Invalid;
	ResourcePtr pResource;

	switch (p_requestMethod)
	{
	case RequestMethod::Immediate:
		// set resource state and load resource immediately
		resourceState = ResourceState::Ready;
		pResource = (pResourceHandler->Load(pInputStream));
		break;
	case RequestMethod::Background:
		// set resource state
		resourceState = ResourceState::Loading;
		break;
	case RequestMethod::Deferred:
		// set resource state
		resourceState = ResourceState::Offline;
		break;
	default:
		throw new MesonException("Invalid request mode.", __FILE__, __LINE__);
	}

	// configure the handle
	pResourceHandle->ConfigureHandle(resourceState, p_strResourceName,
		pResourceHandler, pResourceDevice, pResource);

	// in case of background loading, trigger resource
	if (resourceState == ResourceState::Loading)
		pResourceHandle->GetResource();

	return pResourceHandle;
}

ResourceHandlePtr ResourceManager::LoadResource(
	const String& p_strResourceName,
	RequestMethod::RequestMethod p_requestMethod)
{
	String strResourceType = GetResourceType(p_strResourceName);

	return LoadResource(p_strResourceName, strResourceType, p_requestMethod);
}

void ResourceManager::LoadResources(
	const TList<String>& p_listResourceNames,
	RequestMethod::RequestMethod p_requestMethod,
	TList<ResourceHandlePtr>& p_listResourceHandles)
{
	p_listResourceHandles.Clear();

	if (p_requestMethod == RequestMethod::Immediate)
		m_pResourceManagerState->NotifyObserversOnProgress(ResourceHandlePtr(), 0.0f);

	for (size_t unIndex = 0; unIndex < p_listResourceNames.Size(); unIndex++)
	{
		ResourceHandlePtr pResourceHandle
			= LoadResource(p_listResourceNames(unIndex), p_requestMethod);
		p_listResourceHandles.Add(pResourceHandle);

		if (p_requestMethod == RequestMethod::Immediate)
		{
			float fProgress = (float) unIndex / (float) p_listResourceNames.Size();
			m_pResourceManagerState->NotifyObserversOnProgress(ResourceHandlePtr(), fProgress);
			m_pResourceManagerState->NotifyObserversOnReady(pResourceHandle);
		}
	}

	if (p_requestMethod == RequestMethod::Immediate)
	{
		m_pResourceManagerState->NotifyObserversOnProgress(ResourceHandlePtr(), 1.0f);
		m_pResourceManagerState->NotifyObserversOnReady(ResourceHandlePtr());
	}
}

void ResourceManager::AddObserver(ResourceObserverPtr p_pResourceObserver)
{
	m_pResourceManagerState->m_setResourceObservers.Insert(p_pResourceObserver);
}

void ResourceManager::RemoveObserver(ResourceObserverPtr p_pResourceObserver)
{
	m_pResourceManagerState->m_setResourceObservers.Remove(p_pResourceObserver);
}
