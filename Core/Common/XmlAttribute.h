#pragma once

#include "XmlNode.h"

Meson_Common_Xml_BEGIN

struct XmlAttributeState;

class XmlAttribute
	: public XmlNode
{
private: // private variables
	XmlAttributeState* m_pXmlAttributeState;

protected: // protected methods
	void SetValue(const Meson::Common::Text::String& p_strValue);

public: // public methods
	XmlAttribute(void);
	XmlAttribute(const XmlAttribute& p_xmlAttribute);
	XmlAttribute(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Text::String& p_strValue = "");
	~XmlAttribute(void);

	// XmlNode interface
	XmlNodeType::XmlNodeType GetType(void) const;

	// custom interface
	const Meson::Common::Text::String& GetValue(void) const;
};

typedef Meson::Common::TPointer<XmlAttribute> XmlAttributePtr;

Meson_Common_Xml_END
