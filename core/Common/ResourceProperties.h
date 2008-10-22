//----------------------------------------------------------------------------------------------
//	Meson::Common::ResourceProperties
//	Representation of a resource's properties.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		12/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Definitions.h"
#include "String8.h"
#include "Enumerator.h"

Meson_Common_Resources_BEGIN

namespace ResourcePropertyType
{
	enum ResourcePropertyType
	{
		Boolean,
		Integer,
		Real,
		String
	};
}

struct ResourcePropertyState;

class ResourceProperty
{
private: // private variables
	ResourcePropertyState* m_pResourcePropertyState;

public: // public methods
	ResourceProperty(void);
	~ResourceProperty(void);
	ResourceProperty(const Meson::Common::Text::String& p_strName);
	ResourceProperty(const Meson::Common::Text::String& p_strName,
		ResourcePropertyType::ResourcePropertyType p_resourcePropertyType);

	const Meson::Common::Text::String& GetName(void) const;
	ResourcePropertyType::ResourcePropertyType GetType(void) const;

	bool operator==(const ResourceProperty& p_resourceProperty) const;
	bool operator!=(const ResourceProperty& p_resourceProperty) const;

	ResourceProperty& operator=(const ResourceProperty& p_resourceProperty);
	ResourceProperty& operator=(bool p_bValue);
	ResourceProperty& operator=(long p_lValue);
	ResourceProperty& operator=(double p_dValue);
	ResourceProperty& operator=(const Meson::Common::Text::String& p_strValue);

	operator bool(void);
	operator long(void);
	operator double(void);
	operator Meson::Common::Text::String&(void);
};

struct ResourcePropertiesState;

class ResourceProperties
{
private: // private variables
	ResourcePropertiesState *m_pResourcePropertiesState;

public: // public methods
	ResourceProperties(void);
	~ResourceProperties(void);
	size_t Count(void) const;
	Meson::Common::Collections::TEnumerator<ResourceProperty>& GetProperties(void);
	const ResourceProperty& operator[](const Meson::Common::Text::String& p_strName) const;
	ResourceProperty& operator[](const Meson::Common::Text::String& p_strName);
	bool Satisfy(const ResourceProperties& p_resourceProperties) const;
};

Meson_Common_Resources_END
