
#include "ResourceProperties.h"
#include "HashMap.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Resources;

Meson_Common_Resources_BEGIN

struct ResourcePropertyState
{
	String m_strName;
	ResourcePropertyType::ResourcePropertyType m_resourcePropertyType;
	union Values
	{
		bool m_bValue;
		long m_lValue;
		double m_dValue;
		String* m_pstrValue;
	} Values;

	ResourcePropertyState(void)
		: m_strName()
		, m_resourcePropertyType(ResourcePropertyType::Boolean)
	{
		Values.m_bValue = false;
	}

	~ResourcePropertyState(void)
	{
		if (m_resourcePropertyType == ResourcePropertyType::String)
			delete Values.m_pstrValue;
	}

	void SetValue(bool p_bValue)
	{
		if (m_resourcePropertyType == ResourcePropertyType::String)
			delete Values.m_pstrValue;
		m_resourcePropertyType = ResourcePropertyType::Boolean;
		Values.m_bValue = p_bValue;
	}

	void SetValue(long p_lValue)
	{
		if (m_resourcePropertyType == ResourcePropertyType::String)
			delete Values.m_pstrValue;
		m_resourcePropertyType = ResourcePropertyType::Integer;
		Values.m_lValue = p_lValue;
	}

	void SetValue(double p_dValue)
	{
		if (m_resourcePropertyType == ResourcePropertyType::String)
			delete Values.m_pstrValue;
		m_resourcePropertyType = ResourcePropertyType::Real;
		Values.m_dValue = p_dValue;
	}

	void SetValue(const String& p_strValue)
	{
		if (m_resourcePropertyType != ResourcePropertyType::String)
		{
			m_resourcePropertyType = ResourcePropertyType::String;
			Values.m_pstrValue = new String();
		}
		*Values.m_pstrValue = p_strValue;
	}

	bool operator==(const ResourcePropertyState& p_resourcePropertyState) const
	{
		if (m_strName != p_resourcePropertyState.m_strName) return false;
		if (m_resourcePropertyType != p_resourcePropertyState.m_resourcePropertyType)
			return false;
		switch (m_resourcePropertyType)
		{
		case ResourcePropertyType::Boolean:
			return Values.m_bValue == p_resourcePropertyState.Values.m_bValue;
		case ResourcePropertyType::Integer:
			return Values.m_lValue == p_resourcePropertyState.Values.m_lValue;
		case ResourcePropertyType::Real:
			return Values.m_dValue == p_resourcePropertyState.Values.m_dValue;
		case ResourcePropertyType::String:
			return *Values.m_pstrValue == *p_resourcePropertyState.Values.m_pstrValue;
		default:
			return false;
		}
	}
};

Meson_Common_Resources_END

ResourceProperty::ResourceProperty(void)
	: m_pResourcePropertyState(NULL)
{
	m_pResourcePropertyState = new ResourcePropertyState();
}

ResourceProperty::ResourceProperty(const String& p_strName)
	: m_pResourcePropertyState(NULL)
{
	m_pResourcePropertyState = new ResourcePropertyState();
	m_pResourcePropertyState->m_strName = p_strName;
}

ResourceProperty::ResourceProperty(const String& p_strName,
	ResourcePropertyType::ResourcePropertyType p_resourcePropertyType)
	: m_pResourcePropertyState(NULL)
{
	m_pResourcePropertyState = new ResourcePropertyState();
	m_pResourcePropertyState->m_strName = p_strName;
	switch (p_resourcePropertyType)
	{
	case ResourcePropertyType::Boolean: return;
	case ResourcePropertyType::Integer:
		m_pResourcePropertyState->SetValue(0L);
		break;
	case ResourcePropertyType::Real:
		m_pResourcePropertyState->SetValue(0.0);
		break;
	case ResourcePropertyType::String:
		m_pResourcePropertyState->SetValue("");
		break;
	default: return;
	}
}

ResourceProperty& ResourceProperty::operator=(const ResourceProperty& p_resourceProperty)
{
	m_pResourcePropertyState->m_strName = p_resourceProperty.m_pResourcePropertyState->m_strName;
	switch (p_resourceProperty.m_pResourcePropertyState->m_resourcePropertyType)
	{
	case ResourcePropertyType::Boolean:
		m_pResourcePropertyState->SetValue(
			p_resourceProperty.m_pResourcePropertyState->Values.m_bValue);
		break;
	case ResourcePropertyType::Integer:
		m_pResourcePropertyState->SetValue(
			p_resourceProperty.m_pResourcePropertyState->Values.m_lValue);
		break;
	case ResourcePropertyType::Real:
		m_pResourcePropertyState->SetValue(
			p_resourceProperty.m_pResourcePropertyState->Values.m_dValue);
		break;
	case ResourcePropertyType::String:
		m_pResourcePropertyState->SetValue(
			*p_resourceProperty.m_pResourcePropertyState->Values.m_pstrValue);
		break;
	default:
		break;
	}
	return *this;
}

ResourceProperty& ResourceProperty::operator=(bool p_bValue)
{
	m_pResourcePropertyState->SetValue(p_bValue);
	return *this;
}

ResourceProperty& ResourceProperty::operator=(long p_lValue)
{
	m_pResourcePropertyState->SetValue(p_lValue);
	return *this;
}

ResourceProperty& ResourceProperty::operator=(double p_dValue)
{
	m_pResourcePropertyState->SetValue(p_dValue);
	return *this;
}

ResourceProperty& ResourceProperty::operator=(const String& p_strValue)
{
	m_pResourcePropertyState->SetValue(p_strValue);
	return *this;
}

ResourceProperty::~ResourceProperty(void)
{
	delete m_pResourcePropertyState;
}

const String& ResourceProperty::GetName(void) const
{
	return m_pResourcePropertyState->m_strName;
}

ResourcePropertyType::ResourcePropertyType ResourceProperty::GetType(void) const
{
	return m_pResourcePropertyState->m_resourcePropertyType;
}

bool ResourceProperty::operator==(const ResourceProperty& p_resourceProperty) const
{
	return *m_pResourcePropertyState == *p_resourceProperty.m_pResourcePropertyState;
}

bool ResourceProperty::operator!=(const ResourceProperty& p_resourceProperty) const
{
	return !(*this == p_resourceProperty);
}

ResourceProperty::operator bool(void)
{
	if (m_pResourcePropertyState->m_resourcePropertyType != ResourcePropertyType::Boolean)
		throw new MesonException("Resource property type is not a boolean.",
			__FILE__, __LINE__);
	return m_pResourcePropertyState->Values.m_bValue;
}

ResourceProperty::operator long(void)
{
	if (m_pResourcePropertyState->m_resourcePropertyType != ResourcePropertyType::Integer)
		throw new MesonException("Resource property type is not an integer.",
			__FILE__, __LINE__);
	return m_pResourcePropertyState->Values.m_lValue;
}

ResourceProperty::operator double(void)
{
	if (m_pResourcePropertyState->m_resourcePropertyType != ResourcePropertyType::Real)
		throw new MesonException("Resource property type is not a real number.",
			__FILE__, __LINE__);
	return m_pResourcePropertyState->Values.m_dValue;
}

ResourceProperty::operator String&(void)
{
	if (m_pResourcePropertyState->m_resourcePropertyType != ResourcePropertyType::String)
		throw new MesonException("Resource property type is not a string.",
			__FILE__, __LINE__);
	return *m_pResourcePropertyState->Values.m_pstrValue;
}

Meson_Common_Resources_BEGIN

struct ResourcePropertiesState
{
	THashMap<String, ResourceProperty> m_mapProperties;

	ResourcePropertiesState(void) : m_mapProperties() {}
};

Meson_Common_Resources_END

ResourceProperties::ResourceProperties(void)
	: m_pResourcePropertiesState(NULL)
{
	m_pResourcePropertiesState = new ResourcePropertiesState();
}

ResourceProperties::~ResourceProperties(void)
{
	delete m_pResourcePropertiesState;
}

size_t ResourceProperties::Count(void) const
{
	return m_pResourcePropertiesState->m_mapProperties.Size();
}

TEnumerator<ResourceProperty>& ResourceProperties::GetProperties(void)
{
	return m_pResourcePropertiesState->m_mapProperties.GetValueEnumerator();
}

const ResourceProperty& ResourceProperties::operator[](const String& p_strName) const
{
	if (!m_pResourcePropertiesState->m_mapProperties.ContainsKey(p_strName))
		throw new MesonException("Resource property '" + p_strName + "' not defined.",
			__FILE__, __LINE__);
	return m_pResourcePropertiesState->m_mapProperties[p_strName];
}

ResourceProperty& ResourceProperties::operator[](const String& p_strName)
{
	if (!m_pResourcePropertiesState->m_mapProperties.ContainsKey(p_strName))
		m_pResourcePropertiesState->m_mapProperties[p_strName]
			= ResourceProperty(p_strName);
	return m_pResourcePropertiesState->m_mapProperties[p_strName];
}

bool ResourceProperties::Satisfy(const ResourceProperties& p_resourceProperties) const
{
	THashMap<String, ResourceProperty>& mapProperties
		= m_pResourcePropertiesState->m_mapProperties;

	// if this set of properties is less in count that the set of properties
	// to satsify, definitely will not have enough matching properties
	if (Count() < p_resourceProperties.Count())
		return false;

	TEnumerator<ResourceProperty>& enumProperties
		= ((ResourceProperties&) p_resourceProperties).GetProperties();
	while (enumProperties.HasMoreElements())
	{
		ResourceProperty& resourcePropertyToSatisfy = enumProperties.NextElement();

		// if property with matching name not found, not satisfied
		const String& strPropertyName = resourcePropertyToSatisfy.GetName();
		if (!mapProperties.ContainsKey(strPropertyName))
			return false;

		// if properties mismatch, not satisfied
		ResourceProperty& resourceProperty = mapProperties[strPropertyName];
		if (resourceProperty != resourcePropertyToSatisfy)
			return false;
	}
		
	// otherwise, this set of properties satisfy given properties
	return true;
}
