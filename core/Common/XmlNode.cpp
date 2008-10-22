#include "XmlNode.h"

#include "LinkedList.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Text;
using namespace Meson::Common::Xml;

Meson_Common_Xml_BEGIN

struct XmlNodeState
{
	String m_strName;
	XmlNodePtr m_pXmlNodeParent;
	TLinkedList<XmlNodePtr> m_listChildNodes;

	XmlNodeState(void)
		: m_strName()
		, m_pXmlNodeParent(NULL)
		, m_listChildNodes()
	{ }
};

Meson_Common_Xml_END

void XmlNode::SetName(const String& p_strName)
{
	m_pXmlNodeState->m_strName = p_strName;
}

void XmlNode::SetParentNode(XmlNodePtr p_pXmlNode)
{
	m_pXmlNodeState->m_pXmlNodeParent = p_pXmlNode;
}

void XmlNode::AddChildNode(Meson::Common::TPointer<XmlNode> p_pXmlNode)
{
	if (p_pXmlNode->GetParentNode() != NULL)
		throw new MesonException(
			"The given XML node is already assigned to a parent node.",
			__FILE__, __LINE__);

	m_pXmlNodeState->m_listChildNodes.Add(p_pXmlNode);
	p_pXmlNode->SetParentNode(XmlNodePtr(this));
}

void XmlNode::RemoveChildNode(Meson::Common::TPointer<XmlNode> p_pXmlNode)
{
	if (p_pXmlNode->GetParentNode() != this)
		throw new MesonException(
			"The given XML node is not a child of this node.",
			__FILE__, __LINE__);

	m_pXmlNodeState->m_listChildNodes.Remove(p_pXmlNode);
	p_pXmlNode->SetParentNode(XmlNodePtr());
}

XmlNode::XmlNode(void)
	: m_pXmlNodeState(NULL)
{
	m_pXmlNodeState = new XmlNodeState();
}

XmlNode::~XmlNode(void)
{
	delete m_pXmlNodeState;
}

XmlNodeType::XmlNodeType XmlNode::GetType(void) const
{
	return XmlNodeType::Undefined;
}

const String& XmlNode::GetName(void) const
{
	return m_pXmlNodeState->m_strName;
}

XmlNodePtr XmlNode::GetParentNode(void) const
{
	return m_pXmlNodeState->m_pXmlNodeParent;
}

TEnumerator< XmlNodePtr >& XmlNode::GetChildNodes(void) const
{
	return m_pXmlNodeState->m_listChildNodes.GetEnumerator();
}

bool XmlNode::HasChildNode(const String& p_strName) const
{
	size_t unCount = 0;
	for (size_t unIndex= 0; unIndex < unCount; unIndex++)
		if (m_pXmlNodeState->m_listChildNodes[unIndex]->GetName() == p_strName)
			return true;
	return false;
}

XmlNodePtr XmlNode::GetChildNode(const String& p_strName) const
{
	size_t unCount = 0;
	for (size_t unIndex= 0; unIndex < unCount; unIndex++)
	{
		XmlNodePtr pXmlNode = m_pXmlNodeState->m_listChildNodes[unIndex];
		if (pXmlNode->GetName() == p_strName)
			return pXmlNode;
	}
	throw new MesonException(
		"No child XML node found with name '" + p_strName + "'.",
		__FILE__, __LINE__);
}
