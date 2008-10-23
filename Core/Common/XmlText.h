#pragma once

#include "XmlNode.h"

Meson_Common_Xml_BEGIN

struct XmlTextState;

class XmlText
	: public XmlNode
{
private: // private variables
	XmlTextState* m_pXmlTextState;

protected: // protected methods
	void SetText(const Meson::Common::Text::String& p_strText);

public: // public methods
	XmlText(void);
	XmlText(const XmlText& p_xmlText);
	XmlText(const Meson::Common::Text::String& p_strText);
	~XmlText(void);

	// XmlNode interface
	XmlNodeType::XmlNodeType GetType(void) const;

	// custom interface
	const Meson::Common::Text::String& GetText(void) const;
};

typedef Meson::Common::TPointer<XmlText> XmlTextPtr;

Meson_Common_Xml_END
