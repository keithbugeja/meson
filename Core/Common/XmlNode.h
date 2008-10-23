#pragma once

#include "Definitions.h"
#include "Namespaces.h"
#include "Types.h"
#include "Pointer.h"
#include "String8.h"
#include "Enumerator.h"
#include "InputStream.h"
#include "OutputStream.h"

Meson_Common_Xml_BEGIN

namespace XmlNodeType
{
	enum XmlNodeType
	{
		Undefined,
		Declaration,
		Element,
		Attribute,
		Text
	};
}

struct XmlNodeState;

class XmlNode
	: public Meson::Common::TReferenceCounter<XmlNode>
{
private: // private variables
	XmlNodeState* m_pXmlNodeState;

protected: // protected methods
	void SetName(const Meson::Common::Text::String& p_strName);
	void SetParentNode(Meson::Common::TPointer<XmlNode> p_pXmlNode);
	void AddChildNode(Meson::Common::TPointer<XmlNode> p_pXmlNode);
	void RemoveChildNode(Meson::Common::TPointer<XmlNode> p_pXmlNode);

public:
	XmlNode(void);
	~XmlNode(void);

	virtual XmlNodeType::XmlNodeType GetType(void) const;

	const Meson::Common::Text::String& GetName(void) const;

	Meson::Common::TPointer<XmlNode> GetParentNode(void) const;

	Meson::Common::Collections::TEnumerator< Meson::Common::TPointer<XmlNode> >&
		GetChildNodes(void) const;
	bool HasChildNode(const Meson::Common::Text::String& p_strName) const;
	Meson::Common::TPointer<XmlNode> GetChildNode(
		const Meson::Common::Text::String& p_strName) const;
};

typedef Meson::Common::TPointer<XmlNode> XmlNodePtr;

Meson_Common_Xml_END
