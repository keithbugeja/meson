#include "XmlText.h"

using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Xml;

Meson_Common_Xml_BEGIN

struct XmlTextState
{
	String m_strText;

	XmlTextState(void) : m_strText() { }
};

Meson_Common_Xml_END

void XmlText::SetText(const String& p_strText)
{
	m_pXmlTextState->m_strText = p_strText;
}

XmlText::XmlText(void)
	: m_pXmlTextState(NULL)
{
	m_pXmlTextState = new XmlTextState();
}

XmlText::XmlText(const String& p_strText)
	: m_pXmlTextState(NULL)
{
	static int s_nId = 0;
	String strName = String("Text") + (s_nId++);
	m_pXmlTextState = new XmlTextState();
	SetName(strName);
	SetText(p_strText);
}

XmlText::~XmlText(void)
{
	delete m_pXmlTextState;
}

XmlNodeType::XmlNodeType XmlText::GetType(void) const
{
	return XmlNodeType::Text;
}

const String& XmlText::GetText(void) const
{
	return m_pXmlTextState->m_strText;
}
