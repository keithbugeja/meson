//----------------------------------------------------------------------------------------------
#include "VstSceneCompositeNode.h"
#include "VstCuller.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
ISceneCompositeNode::ISceneCompositeNode(const Meson::Common::Text::String& p_strId, Scenegraph* p_pOwner)
	: ISceneNode(p_strId, p_pOwner)
{
	m_eNodeType = SNT_INTERNAL;
}
//----------------------------------------------------------------------------------------------
ISceneCompositeNode::ISceneCompositeNode(Scenegraph* p_pOwner)
	: ISceneNode(p_pOwner)
{
	m_eNodeType = SNT_INTERNAL;
}
//----------------------------------------------------------------------------------------------
ISceneCompositeNode::~ISceneCompositeNode(void)
{
	DetachAllChildren();
}
//----------------------------------------------------------------------------------------------
ISceneNode* ISceneCompositeNode::GetChild(uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < m_childNodeList.Size(), "Child index out of range.");
	return m_childNodeList[p_uiIndex];
}
//----------------------------------------------------------------------------------------------
ISceneNode* ISceneCompositeNode::GetChild(const Meson::Common::Text::String& p_strId)
{
	ISceneNode* pSceneNode;

	if (m_childNodeMap.ContainsKey(p_strId, pSceneNode))
		return pSceneNode;

	return NULL;
}
//----------------------------------------------------------------------------------------------
SceneBaseNodeEnumerator ISceneCompositeNode::GetChildEnumerator(void)
{
	return m_childNodeList.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
uint ISceneCompositeNode::GetChildCount(void)
{
	return (uint)m_childNodeList.Size();
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::AttachChild(ISceneNode* p_pChildNode)
{
	if (m_childNodeList.IndexOf(p_pChildNode) == -1)
	{
		p_pChildNode->SetParent(this);

		m_childNodeList.Add(p_pChildNode);
		m_childNodeMap[p_pChildNode->GetId()] = p_pChildNode;
	}
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::AttachChild(ISceneNode* p_pChildNode, uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex <= m_childNodeList.Size(), "Cannot attach child node: index out of range!");

	if (m_childNodeList.IndexOf(p_pChildNode) == -1) 
	{
		p_pChildNode->SetParent(this);

		m_childNodeList.Insert(p_uiIndex, p_pChildNode);
		m_childNodeMap[p_pChildNode->GetId()] = p_pChildNode;
	} 
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::DetachChild(ISceneNode* p_pChildNode)
{
	if (m_childNodeList.IndexOf(p_pChildNode) != -1)
	{
		m_childNodeList.Remove(p_pChildNode);
		m_childNodeMap.RemoveKey(p_pChildNode->GetId());

		p_pChildNode->SetParent(NULL);
	}
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::DetachChild(uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex <= m_childNodeList.Size(), "Cannot detach child node: index out of range!");

	ISceneNode* pNode = m_childNodeList[p_uiIndex];
	
	m_childNodeList.RemoveAt(p_uiIndex);
	m_childNodeMap.RemoveKey(pNode->GetId());
	pNode->SetParent(NULL);
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::DetachAllChildren(void)
{
	m_childNodeList.Clear();
	m_childNodeMap.Clear();
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::UpdateContext(RenderContextPtr p_pRenderContext, LightListPtr p_pLightList)
{
	ISceneNode::UpdateContext(p_pRenderContext, p_pLightList);

	for (uint uiIndex = 0, uiChildCount = (uint)m_childNodeList.Size(); uiIndex < uiChildCount; uiIndex++)
		m_childNodeList[uiIndex]->UpdateRenderState(false, m_pActiveRenderContext, m_pActiveLightList);
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::UpdateWorldBound(void)
{
	MESON_ASSERT(WorldBound != NULL, "Bounding volume is NULL.");

	uint uiChildCount = (uint)m_childNodeList.Size();

	if (!IsWorldBoundValid && uiChildCount > 0)
	{
		MESON_ASSERT(m_childNodeList[0]->WorldBound != NULL, "Child node's bounding volume is NULL.");
		WorldBound->ComputeFromVolume(*m_childNodeList[0]->WorldBound);

		for (uint uiIndex = 1; uiIndex < uiChildCount; uiIndex++)
		{
			MESON_ASSERT(m_childNodeList[uiIndex]->WorldBound != NULL, "Child node's bounding volume is NULL.");
			WorldBound->GrowToContain(*m_childNodeList[uiIndex]->WorldBound);
		}
	}
}
//----------------------------------------------------------------------------------------------
/** Calls the Synchronisation Listeners for the node, updates the world transformation,
	and calls the Geometry State update for the node children.
 */
void ISceneCompositeNode::UpdateWorldSynchronisation(double p_dfApplicationTime)
{
	ISceneNode::UpdateWorldSynchronisation(p_dfApplicationTime);
	SceneBaseNodeEnumerator nodeEnumerator = m_childNodeList.GetEnumerator();
	
	while(nodeEnumerator.HasMoreElements())
		nodeEnumerator.NextElement()->UpdateGeometryState(false, p_dfApplicationTime);
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::InvalidateWorldTransform(void)
{
	ISceneNode::InvalidateWorldTransform();

	uint uiChildCount = (uint)m_childNodeList.Size();

	for (uint uiIndex = 0; uiIndex < uiChildCount; uiIndex++)
		m_childNodeList[uiIndex]->InvalidateWorldTransform();
}
//----------------------------------------------------------------------------------------------
void ISceneCompositeNode::Cull(ICuller& p_culler)
{
	if (!IsVisible())
		return;

	// Notify cull listeners if any are 
	// attached to this node.
	if (m_pCullListener != NULL)
	{
		// Call PreCull Listener
		ICullListener::CullResponse eCullResponse = m_pCullListener->OnPreCull(this, p_culler);

		if (p_culler.IsVisible(this))
		{
			p_culler.Push(this);

			if (eCullResponse == ICullListener::CR_CONTINUE)
			{
				if (m_effectList.Size() > 0)
				{
					p_culler.Enter(this, m_effectList[0]);

					for (uint uiIndex = 0, uiSize = (uint)m_childNodeList.Size(); uiIndex < uiSize; uiIndex++)
						m_childNodeList[uiIndex]->Cull(p_culler);

					p_culler.Leave(this, m_effectList[0]);
				}
				else
				{
					for (uint uiIndex = 0, uiSize = (uint)m_childNodeList.Size(); uiIndex < uiSize; uiIndex++)
						m_childNodeList[uiIndex]->Cull(p_culler);
				}
			}
		}
		
		// Call PostCull Listener
		m_pCullListener->OnPostCull(this, p_culler);
	}
	else
	{
		if (p_culler.IsVisible(this))
		{
			p_culler.Push(this);

			if (m_effectList.Size() > 0)
			{
				p_culler.Enter(this, m_effectList[0]);

				for (uint uiIndex = 0, uiSize = (uint)m_childNodeList.Size(); uiIndex < uiSize; uiIndex++)
					m_childNodeList[uiIndex]->Cull(p_culler);

				p_culler.Leave(this, m_effectList[0]);
			}
			else
			{
				for (uint uiIndex = 0, uiSize = (uint)m_childNodeList.Size(); uiIndex < uiSize; uiIndex++)
					m_childNodeList[uiIndex]->Cull(p_culler);
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
ISceneNode* ISceneCompositeNode::operator[] (uint p_uiIndex) const
{
	MESON_ASSERT(p_uiIndex < m_childNodeList.Size(), "Child index out of range.");
	return m_childNodeList[p_uiIndex];
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END