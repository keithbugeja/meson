#pragma once

#include "XmlNode.h"
#include "XmlAttribute.h"

Meson_Common_Xml_BEGIN

struct XmlDeclarationState;

class XmlDeclaration
	: public XmlNode
{
private: // private variables
	XmlDeclarationState* m_pXmlDeclarationState;

public: // public methods
	XmlDeclaration(void);
	XmlDeclaration(const XmlDeclaration& p_xmlDeclaration);
	~XmlDeclaration(void);

	// XmlNode interface
	XmlNodeType::XmlNodeType GetType(void) const;

	// custom interface
	Meson::Common::Collections::TEnumerator<XmlAttributePtr>& GetAttributes(void) const;
	void AddAttribute(XmlAttributePtr p_pXmlAttribute);
	void RemoveAttribute(XmlAttributePtr p_pXmlAttribute);
};

typedef Meson::Common::TPointer<XmlDeclaration> XmlDeclarationPtr;

Meson_Common_Xml_END
