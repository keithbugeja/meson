//----------------------------------------------------------------------------------------------
#include "VstBoundingSphere.h"
#include "VstBoundingAxisAlignedBox.h"
#include "VstSceneNode.h"
#include "VstCuller.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
/** Constructor for the base node class in the Scene Graph. The node is constructed with a
	default bounding sphere, identity local and world transforms, enabled visibility,
	and relative transform type.
 */
ISceneNode::ISceneNode(const Meson::Common::Text::String& p_strId, Scenegraph* p_pOwner)
	: VistasObject(p_strId)
	//, WorldBound(new BoundingSphere())
	, WorldBound(new BoundingAxisAlignedBox())
	, m_pOwner(p_pOwner)
	, m_pParentNode(NULL)
	, m_bIsAbsoluteLocalTransform(false)
	, m_bIsNodeVisible(true)
	, m_pCullListener(NULL)
{
	m_eNodeType = SNT_LEAF;
	
	IsWorldTransformValid = false;
	IsWorldBoundValid = false;

	LocalTransformation.MakeIdentity();
	WorldTransformation.MakeIdentity();
}
//----------------------------------------------------------------------------------------------
ISceneNode::ISceneNode(Scenegraph* p_pOwner)
	: VistasObject(true)
	//, WorldBound(new BoundingSphere())
	, WorldBound(new BoundingAxisAlignedBox())
	, m_pOwner(p_pOwner)
	, m_pParentNode(NULL)
	, m_bIsAbsoluteLocalTransform(false)
	, m_bIsNodeVisible(true)
	, m_pCullListener(NULL)
{
	m_eNodeType = SNT_LEAF;
	
	IsWorldTransformValid = false;
	IsWorldBoundValid = false;

	LocalTransformation.MakeIdentity();
	WorldTransformation.MakeIdentity();
}
//----------------------------------------------------------------------------------------------
ISceneNode::~ISceneNode(void)
{
	WorldBound = NULL;

	m_pActiveLightList = NULL;
	m_pActiveRenderContext = NULL;

	m_effectList.Clear();
	m_lightList.Clear();
	m_renderStateMap.Clear();
	m_syncListenerList.Clear();
}
//----------------------------------------------------------------------------------------------
/** Returns one of the possible scene graph node types - internal or leaf.
 */
ISceneNode::NodeType ISceneNode::GetNodeType(void) const
{
	return m_eNodeType;
}
//----------------------------------------------------------------------------------------------
/** If the node is orphaned, it is considered the root of this scene graph.
 */
bool ISceneNode::IsRootNode(void) const
{
	return (m_pParentNode == NULL);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::SetParent(ISceneNode* p_pParentNode)
{
	if (p_pParentNode != NULL)
	{
		if (p_pParentNode->GetNodeType() == SNT_INTERNAL)
			m_pParentNode = p_pParentNode;
		// Removed, otherwise compiler freaks out.
		//else
		//	throw new MesonException("Cannot attach node to parent because parent node is not a composite scene node.", __FILE__, __LINE__);
	} 
	else
		m_pParentNode = NULL;
}
//----------------------------------------------------------------------------------------------
ISceneNode* ISceneNode::GetParent(void) const
{
	return m_pParentNode;
}
//----------------------------------------------------------------------------------------------
Scenegraph* ISceneNode::GetOwner(void) const
{
	return m_pOwner;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::SetOwner(Scenegraph* p_pOwner)
{
	m_pOwner = p_pOwner;
}
//----------------------------------------------------------------------------------------------
/** Determine the visibility of the node - Note that even though a node is not renderable, 
	setting its visibility to false will force its culling.
 */
bool ISceneNode::IsVisible(void) const
{
	return m_bIsNodeVisible;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::SetVisible(bool p_bIsVisible)
{
	m_bIsNodeVisible = p_bIsVisible;
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::IsValidWorldTransform() const
{
	return IsWorldTransformValid;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::ValidateWorldTransform()
{
	IsWorldTransformValid = true;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::InvalidateWorldTransform()
{
	IsWorldTransformValid = false;
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::IsAbsoluteLocalTransform(void) const
{
	return m_bIsAbsoluteLocalTransform;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::SetAbsoluteLocalTransform(bool p_bEnabled)
{
	m_bIsAbsoluteLocalTransform = p_bEnabled;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::SetLocalTransform(const Transformation& p_transformation)
{
	LocalTransformation = p_transformation;

	IsWorldTransformValid = false;
	IsWorldBoundValid = false;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::SetWorldTransform(const Transformation& p_transformation)
{
	WorldTransformation = p_transformation;

	IsWorldBoundValid = false;
	IsWorldTransformValid = true;
}
//----------------------------------------------------------------------------------------------
const Transformation& ISceneNode::GetLocalTransform(void) const
{
	return LocalTransformation;
}
//----------------------------------------------------------------------------------------------
const Transformation& ISceneNode::GetWorldTransform(void)
{
	if (!IsWorldTransformValid)
		UpdateWorldTransform();

	return WorldTransformation;
}
//----------------------------------------------------------------------------------------------
const Transformation& ISceneNode::GetParentTransform(void) const
{
	if (IsRootNode())
		return Transformation::Identity;

	return GetParent()->GetWorldTransform();
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::IsValidWorldBound() const
{
	return IsWorldBoundValid;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::ValidateWorldBound()
{
	IsWorldBoundValid = true;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::InvalidateWorldBound()
{
	IsWorldBoundValid = false;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::SetWorldBound(BoundingVolumePtr p_pBoundingVolume)
{
	MESON_ASSERT(p_pBoundingVolume != NULL, "Bounding volume set to NULL.");
	WorldBound = p_pBoundingVolume;
}
//----------------------------------------------------------------------------------------------
BoundingVolumePtr ISceneNode::GetWorldBound(void)
{
	if (!IsWorldBoundValid)
		UpdateWorldBound();

	return WorldBound;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::UpdateGeometryState(bool p_bIsInitiator, double p_dfApplicationTime)
{
	UpdateWorldSynchronisation(p_dfApplicationTime);
	UpdateWorldBound();

	if (p_bIsInitiator)
		PropagateToRoot();
}
//----------------------------------------------------------------------------------------------
void ISceneNode::UpdateRenderState(bool p_bIsInitiator, RenderContextPtr p_pRenderContext, LightListPtr p_pLightList)
{
	if (p_bIsInitiator)
	{
		// Don't get render context, if not null
		RenderContextPtr pRenderContext(NULL);
		LightListPtr pLightList(NULL);

		// Propagate if either the render context or the light list is null
		if (p_pRenderContext == NULL || p_pLightList == NULL)
			PropagateFromRoot(pRenderContext, pLightList);

		// Prefer supplied context to propagated one
		if (p_pRenderContext != NULL)
			pRenderContext = p_pRenderContext;

		// Prefer supplied lightlist to propagated one
		if (p_pLightList != NULL)
			pLightList = p_pLightList;

		UpdateContext(pRenderContext, pLightList);
	}
	else
	{
		UpdateContext(p_pRenderContext, p_pLightList);
	}
}
//----------------------------------------------------------------------------------------------
void ISceneNode::UpdateContext(RenderContextPtr p_pRenderContext, LightListPtr p_pLightList)
{
	// Update render state
	if (m_renderStateMap.Size() > 0)
	{
		m_pActiveRenderContext = new RenderContext(p_pRenderContext);
		
		RenderStateEnumerator renderStateEnumerator = m_renderStateMap.GetValueEnumerator();
		while(renderStateEnumerator.HasMoreElements()) 
			m_pActiveRenderContext->SetRenderState(renderStateEnumerator.NextElement());
	}
	else
	{
		m_pActiveRenderContext = p_pRenderContext;
	}

	// Update light list
	if (m_lightList.Size() > 0)
	{
		m_pActiveLightList = new LightList(m_lightList);
		m_pActiveLightList->AddList(*p_pLightList);
	}
	else
	{
		m_pActiveLightList = p_pLightList;
	}
}
//----------------------------------------------------------------------------------------------
void ISceneNode::UpdateWorldSynchronisation(double p_dfApplicationTime)
{
	Synchronise(p_dfApplicationTime);
	UpdateWorldTransform();	
}
//----------------------------------------------------------------------------------------------
void ISceneNode::UpdateWorldTransform(void)
{
	if (!IsWorldTransformValid)
	{
		if (IsRootNode() || IsAbsoluteLocalTransform())
			WorldTransformation = LocalTransformation;
		else
			WorldTransformation = m_pParentNode->WorldTransformation.ProductCopy(LocalTransformation);

		IsWorldTransformValid = true;
	}
}
//----------------------------------------------------------------------------------------------
void ISceneNode::PropagateFromRoot(RenderContextPtr& p_pRenderContext, LightListPtr& p_pLightList)
{
	// If this is the root node, we cannot traverse upwards any further
	if (IsRootNode())
	{
		// Create a new active context if still unset
		if (m_pActiveRenderContext == NULL)
		{
			m_pActiveRenderContext = new RenderContext();
			if (m_renderStateMap.Size() > 0)
			{
				RenderStateEnumerator renderStateEnumerator = m_renderStateMap.GetValueEnumerator();
				while(renderStateEnumerator.HasMoreElements()) 
					m_pActiveRenderContext->SetRenderState(renderStateEnumerator.NextElement());
			}
		}

		// Create an active light list if still unset
		if (m_pActiveLightList == NULL)
		{
			m_pActiveLightList = new LightList(m_lightList);
		}
	}
	else
	{
		// If either context or light list are unset, acquire from parent
		if (m_pActiveRenderContext == NULL || m_pActiveLightList == NULL)
		{
			m_pParentNode->PropagateFromRoot(p_pRenderContext, p_pLightList);

			// If the context was unset, create a new one
			if (m_pActiveRenderContext == NULL)
			{
				if (m_renderStateMap.Size() > 0)
				{
					m_pActiveRenderContext = new RenderContext(p_pRenderContext);
					
					RenderStateEnumerator renderStateEnumerator = m_renderStateMap.GetValueEnumerator();
					while(renderStateEnumerator.HasMoreElements()) 
						m_pActiveRenderContext->SetRenderState(renderStateEnumerator.NextElement());
				}
				else
				{
					m_pActiveRenderContext = p_pRenderContext;
				}
			}

			// If the light list was unset, create a new list
			if (m_pActiveLightList == NULL)
			{
				if (m_lightList.Size() > 0)
				{
					m_pActiveLightList = new LightList(m_lightList);
					m_pActiveLightList->AddList(*p_pLightList);
				}
				else
				{
					m_pActiveLightList = p_pLightList;
				}
			}
		}
	}

	p_pRenderContext = m_pActiveRenderContext;
	p_pLightList = m_pActiveLightList;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::PropagateToRoot(void)
{
	if (!IsRootNode())
	{
		m_pParentNode->UpdateWorldBound();
		m_pParentNode->PropagateToRoot();
	}
}
//----------------------------------------------------------------------------------------------
void ISceneNode::Synchronise(double p_dfApplicationTime)
{
	if (m_syncListenerList.Size() > 0)
	{
		SyncListenerEnumerator syncEnumerator = m_syncListenerList.GetEnumerator();
		
		while (syncEnumerator.HasMoreElements())
			syncEnumerator.NextElement()->Synchronise(this, p_dfApplicationTime);
	}
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::ContainsSyncListener(ISyncListener* p_pSyncListener)
{
	return (m_syncListenerList.IndexOf(p_pSyncListener) != -1);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::AttachSyncListener(ISyncListener* p_pSyncListener)
{
	if (!ContainsSyncListener(p_pSyncListener))
		m_syncListenerList.Add(p_pSyncListener);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachSyncListener(ISyncListener* p_pSyncListener)
{
	if (ContainsSyncListener(p_pSyncListener))
		m_syncListenerList.Remove(p_pSyncListener);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachAllSyncListeners(void)
{
	m_syncListenerList.Clear();
}
//----------------------------------------------------------------------------------------------
SyncListenerEnumerator ISceneNode::GetSyncListenerEnumerator()
{
	return m_syncListenerList.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::ContainsEffect(const Meson::Common::Text::String& p_strId)
{
	uint uiEffectCount = (uint)m_effectList.Size();

	if (uiEffectCount  > 0)
	{
		for (uint uiEffectIndex = 0; uiEffectIndex < uiEffectCount; uiEffectIndex++)
		{
			if (m_effectList[uiEffectIndex]->GetId().Compare(p_strId) == 0)
				return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::ContainsEffect(const Meson::Common::Text::String& p_strId, EffectPtr& p_pEffect)
{
	uint uiEffectCount = (uint)m_effectList.Size();

	if (uiEffectCount  > 0)
	{
		for (uint uiEffectIndex = 0; uiEffectIndex < uiEffectCount; uiEffectIndex++)
		{
			if (m_effectList[uiEffectIndex]->GetId().Compare(p_strId) == 0)
			{
				p_pEffect = m_effectList[uiEffectIndex];
				return true;
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::AttachEffect(EffectPtr p_pEffect)
{
	if (m_effectList.IndexOf(p_pEffect) == -1)
		m_effectList.Add(p_pEffect);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachEffect(EffectPtr p_pEffect)
{
	if (m_effectList.IndexOf(p_pEffect) != -1)
		m_effectList.Remove(p_pEffect);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachAllEffects(void)
{
	m_effectList.Clear();
}
//----------------------------------------------------------------------------------------------
uint ISceneNode::GetEffectIndex(EffectPtr p_pEffect)
{
	return m_effectList.IndexOf(p_pEffect);
}
//----------------------------------------------------------------------------------------------
EffectPtr ISceneNode::GetEffect(uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < m_effectList.Size(), "Index out of range.");
	return m_effectList[p_uiIndex];
}
//----------------------------------------------------------------------------------------------
uint ISceneNode::GetEffectCount(void) const
{
	return (uint)m_effectList.Size();
}
//----------------------------------------------------------------------------------------------
EffectEnumerator ISceneNode::GetEffectEnumerator(void)
{
	return m_effectList.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::ContainsLight(const Meson::Common::Text::String& p_strId)
{
	uint uiLightCount = (uint)m_lightList.Size();

	if (uiLightCount > 0)
	{
		for (uint uiLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
		{
			if (m_lightList[uiLightIndex]->GetId().Compare(p_strId) == 0)
				return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::ContainsLight(const Meson::Common::Text::String& p_strId, LightPtr& p_pLight)
{
	uint uiLightCount = (uint)m_lightList.Size();

	if (uiLightCount > 0)
	{
		for (uint uiLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
		{
			if (m_lightList[uiLightIndex]->GetId().Compare(p_strId) == 0)
			{
				p_pLight = m_lightList[uiLightIndex];
				return true;
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
uint ISceneNode::GetLightIndex(LightPtr p_pLight)
{
	return m_lightList.IndexOf(p_pLight);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::AttachLight(LightPtr p_pLight)
{
	if (m_lightList.IndexOf(p_pLight) == -1)
		m_lightList.Add(p_pLight);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachLight(LightPtr p_pLight)
{
	if (m_lightList.IndexOf(p_pLight) != -1)
		m_lightList.Remove(p_pLight);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachAllLights(void)
{
	m_lightList.Clear();
}
//----------------------------------------------------------------------------------------------
LightPtr ISceneNode::GetLight(uint p_uiIndex)
{
	MESON_ASSERT(p_uiIndex < m_lightList.Size(), "Index out of range.");
	return m_lightList[p_uiIndex];
}
//----------------------------------------------------------------------------------------------
uint ISceneNode::GetLightCount(void) const
{
	return (uint)m_lightList.Size();
}
//----------------------------------------------------------------------------------------------
LightEnumerator ISceneNode::GetLightEnumerator(void)
{
	return m_lightList.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::ContainsRenderState(IRenderState::RenderStateType p_eRenderStateType)
{
	return m_renderStateMap.ContainsKey(p_eRenderStateType);
}
//----------------------------------------------------------------------------------------------
bool ISceneNode::ContainsRenderState(IRenderState::RenderStateType p_eRenderStateType, RenderStatePtr& p_pRenderState)
{
	return m_renderStateMap.ContainsKey(p_eRenderStateType, p_pRenderState);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::AttachRenderState(RenderStatePtr p_pRenderState)
{
	m_renderStateMap.Insert(p_pRenderState->GetType(), p_pRenderState);
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachRenderState(RenderStatePtr p_pRenderState)
{
	m_renderStateMap.RemoveKey(p_pRenderState->GetType());
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachAllRenderStates(void)
{
	m_renderStateMap.Clear();
}
//----------------------------------------------------------------------------------------------
RenderStatePtr ISceneNode::GetRenderState(IRenderState::RenderStateType p_eRenderStateType)
{
	return m_renderStateMap[p_eRenderStateType];
}
//----------------------------------------------------------------------------------------------
uint ISceneNode::GetRenderStateCount(void) const
{
	return (uint)m_renderStateMap.Size();
}
//----------------------------------------------------------------------------------------------
RenderStateEnumerator ISceneNode::GetRenderStateEnumerator(void)
{
	return m_renderStateMap.GetValueEnumerator();
}
//----------------------------------------------------------------------------------------------
void ISceneNode::Cull(ICuller& p_culler)
{
	if (m_pCullListener != NULL)
	{
		m_pCullListener->OnPreCull(this, p_culler);

		if (IsVisible() && p_culler.IsVisible(this))
			p_culler.Push(this);

		m_pCullListener->OnPostCull(this, p_culler);
	}
	else
	{
		if (IsVisible() && p_culler.IsVisible(this))
			p_culler.Push(this);
	}
}
//----------------------------------------------------------------------------------------------
ICullListener* ISceneNode::GetCullListener(void) const
{
	return m_pCullListener;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::AttachCullListener(ICullListener* p_pCullListener)
{
	m_pCullListener = p_pCullListener;
}
//----------------------------------------------------------------------------------------------
void ISceneNode::DetachCullListener(void)
{
	m_pCullListener = NULL;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END