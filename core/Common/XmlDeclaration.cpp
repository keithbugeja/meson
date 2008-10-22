#include "XmlDeclaration.h"

#include "ArrayList.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Xml;

Meson_Common_Xml_BEGIN

struct XmlDeclarationState
{
	TArrayList<XmlAttributePtr> m_listAttributes;

	XmlDeclarationState(void)
		: m_listAttributes()
	{ }
};

Meson_Common_Xml_END

XmlDeclaration::XmlDeclaration(void)
	: m_pXmlDeclarationState(NULL)
{
	m_pXmlDeclarationState = new XmlDeclarationState();
	SetName("?xml");
}

XmlDeclaration::XmlDeclaration(const XmlDeclaration& p_xmlDeclaration)
	: m_pXmlDeclarationState(NULL)
{
	m_pXmlDeclarationState = new XmlDeclarationState();
	SetName(p_xmlDeclaration.GetName());
}

XmlDeclaration::~XmlDeclaration(void)
{
	delete m_pXmlDeclarationState;
}

XmlNodeType::XmlNodeType XmlDeclaration::GetType(void) const
{
	return XmlNodeType::Declaration;
}

TEnumerator<XmlAttributePtr>& XmlDeclaration::GetAttributes(void) const
{
	return m_pXmlDeclarationState->m_listAttributes.GetEnumerator();
}

void XmlDeclaration::AddAttribute(XmlAttributePtr p_pXmlAttribute)
{
	AddChildNode(XmlNodePtr(p_pXmlAttribute));

	m_pXmlDeclarationState->m_listAttributes.Add(p_pXmlAttribute);
}

void XmlDeclaration::RemoveAttribute(XmlAttributePtr p_pXmlAttribute)
{
	RemoveChildNode(XmlNodePtr(p_pXmlAttribute));
	m_pXmlDeclarationState->m_listAttributes.Remove(p_pXmlAttribute);
}
