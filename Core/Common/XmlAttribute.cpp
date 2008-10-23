#include "XmlAttribute.h"

using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Xml;

Meson_Common_Xml_BEGIN

struct XmlAttributeState
{
	String m_strValue;

	XmlAttributeState(void) : m_strValue() { }
};

Meson_Common_Xml_END

void XmlAttribute::SetValue(const String& p_strValue)
{
	m_pXmlAttributeState->m_strValue = p_strValue;
}

XmlAttribute::XmlAttribute(void)
	: m_pXmlAttributeState(NULL)
{
	m_pXmlAttributeState = new XmlAttributeState();
}

XmlAttribute::XmlAttribute(const XmlAttribute& p_xmlAttribute)
	: m_pXmlAttributeState(NULL)
{
	m_pXmlAttributeState = new XmlAttributeState();
	SetName(p_xmlAttribute.GetName());
	SetValue(p_xmlAttribute.GetValue());
}

XmlAttribute::XmlAttribute(const String& p_strName, const String& p_strValue)
	: m_pXmlAttributeState(NULL)
{
	m_pXmlAttributeState = new XmlAttributeState();
	SetName(p_strName);
	SetValue(p_strValue);
}

XmlAttribute::~XmlAttribute(void)
{
	delete m_pXmlAttributeState;
}

XmlNodeType::XmlNodeType XmlAttribute::GetType(void) const
{
	return XmlNodeType::Attribute;
}

const String& XmlAttribute::GetValue(void) const
{
	return m_pXmlAttributeState->m_strValue;
}
