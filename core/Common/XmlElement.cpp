#include "XmlElement.h"

#include "ArrayList.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Xml;

Meson_Common_Xml_BEGIN

struct XmlElementState
{
	TArrayList<XmlAttributePtr> m_listAttributes;
	TArrayList<XmlElementPtr> m_listChildElements;
	TArrayList<XmlTextPtr> m_listChildText;

	XmlElementState(void)
		: m_listAttributes()
		, m_listChildElements()
		, m_listChildText()
	{ }
};

Meson_Common_Xml_END

XmlElement::XmlElement(void)
	: m_pXmlElementState(NULL)
{
	m_pXmlElementState = new XmlElementState();
}

XmlElement::XmlElement(const XmlElement& p_xmlElement)
	: m_pXmlElementState(NULL)
{
	m_pXmlElementState = new XmlElementState();
	SetName(p_xmlElement.GetName());
}

XmlElement::XmlElement(const String& p_strName)
	: m_pXmlElementState(NULL)
{
	m_pXmlElementState = new XmlElementState();
	SetName(p_strName);
}

XmlElement::~XmlElement(void)
{
	delete m_pXmlElementState;
}

XmlNodeType::XmlNodeType XmlElement::GetType(void) const
{
	return XmlNodeType::Element;
}

TEnumerator<XmlAttributePtr>& XmlElement::GetAttributes(void) const
{
	return m_pXmlElementState->m_listAttributes.GetEnumerator();
}

void XmlElement::AddAttribute(XmlAttributePtr p_pXmlAttribute)
{
	AddChildNode(XmlNodePtr(p_pXmlAttribute));

	m_pXmlElementState->m_listAttributes.Add(p_pXmlAttribute);
}

void XmlElement::RemoveAttribute(XmlAttributePtr p_pXmlAttribute)
{
	RemoveChildNode(XmlNodePtr(p_pXmlAttribute));
	m_pXmlElementState->m_listAttributes.Remove(p_pXmlAttribute);
}

TEnumerator<XmlElementPtr>& XmlElement::GetChildElements(void) const
{
	return m_pXmlElementState->m_listChildElements.GetEnumerator();
}

void XmlElement::AddChildElement(XmlElementPtr p_pXmlElement)
{
	AddChildNode(XmlNodePtr(p_pXmlElement));
	m_pXmlElementState->m_listChildElements.Add(p_pXmlElement);
}

void XmlElement::RemoveChildElement(XmlElementPtr p_pXmlElement)
{
	RemoveChildNode(XmlNodePtr(p_pXmlElement));
	m_pXmlElementState->m_listChildElements.Remove(p_pXmlElement);
}

TEnumerator<XmlTextPtr>& XmlElement::GetChildText(void) const
{
	return m_pXmlElementState->m_listChildText.GetEnumerator();
}

void XmlElement::AddChildText(XmlTextPtr p_pXmlText)
{
	AddChildNode(XmlNodePtr(p_pXmlText));
	m_pXmlElementState->m_listChildText.Add(p_pXmlText);
}

void XmlElement::RemoveChildText(XmlTextPtr p_pXmlText)
{
	RemoveChildNode(XmlNodePtr(p_pXmlText));
	m_pXmlElementState->m_listChildText.Remove(p_pXmlText);
}
