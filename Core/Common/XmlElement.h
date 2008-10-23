#pragma once

#include "XmlNode.h"
#include "XmlAttribute.h"
#include "XmlText.h"

Meson_Common_Xml_BEGIN

struct XmlElementState;

class XmlElement
	: public XmlNode
{
private: // private variables
	XmlElementState* m_pXmlElementState;

public: // public methods
	XmlElement(void);
	XmlElement(const XmlElement& p_xmlElement);
	XmlElement(const Meson::Common::Text::String& p_strName);
	~XmlElement(void);

	// XmlNode interface
	XmlNodeType::XmlNodeType GetType(void) const;

	// custom interface
	Meson::Common::Collections::TEnumerator<XmlAttributePtr>& GetAttributes(void) const;
	void AddAttribute(XmlAttributePtr p_pXmlAttribute);
	void RemoveAttribute(XmlAttributePtr p_pXmlAttribute);

	Meson::Common::Collections::TEnumerator< Meson::Common::TPointer<XmlElement> >& GetChildElements(void) const;
	void AddChildElement(Meson::Common::TPointer<XmlElement> p_pXmlElement);
	void RemoveChildElement(Meson::Common::TPointer<XmlElement> p_pXmlElement);

	Meson::Common::Collections::TEnumerator< XmlTextPtr >& GetChildText(void) const;
	void AddChildText(XmlTextPtr p_pXmlText);
	void RemoveChildText(XmlTextPtr p_pXmlText);
};

typedef Meson::Common::TPointer<XmlElement> XmlElementPtr;

Meson_Common_Xml_END
