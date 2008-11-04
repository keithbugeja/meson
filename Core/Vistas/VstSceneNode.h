//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Pointer.h"
#include "HashMap.h"

#include "VstTypes.h"
#include "VstVistasObject.h"
#include "VstBoundingVolume.h"
#include "VstTransformation.h"
#include "VstRenderContext.h"
#include "VstEffect.h"
#include "VstLight.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class ICullListener
	{
	public:
		enum CullResponse
		{
			CR_CULL,
			CR_CONTINUE,
			
			CR_COUNT
		};

		virtual CullResponse OnPreCull(ISceneNode* p_pSceneNode, ICuller& p_culler) { return CR_CONTINUE; }
		virtual void OnPostCull(ISceneNode* p_pSceneNode, ICuller& p_culler) { }
	};

	typedef Meson::Common::Collections::TArrayList<ICullListener*> CullListenerList;
	typedef Meson::Common::Collections::TEnumerator<ICullListener*>& CullListenerEnumerator;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class ISyncListener
	{
	public:
		virtual void Synchronise(ISceneNode* p_pSceneNode, double p_dfApplicationTime) = 0;
	};

	typedef Meson::Common::Collections::TArrayList<ISyncListener*> SynchListenerList;
	typedef Meson::Common::Collections::TEnumerator<ISyncListener*>& SyncListenerEnumerator;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class VISTAS_API ISceneNode : public VistasObject
	{
		VISTAS_IMPLEMENT_RTTI(ISceneNode, VistasObject);

	public:
		//----------------------------------------------------------------------------------------------
		/** Node types:
			Leaf nodes can only be attached to internal nodes.
			Internal nodes can have children.
		 */
		//----------------------------------------------------------------------------------------------
		enum NodeType
		{
			SNT_INTERNAL,
			SNT_LEAF
		};

	protected:
		ISceneNode(Scenegraph* p_pOwner = NULL);
		ISceneNode(const Meson::Common::Text::String& p_strId, Scenegraph* p_pOwner = NULL);

public:
		virtual ~ISceneNode(void);

		//----------------------------------------------------------------------------------------------
		NodeType GetNodeType(void) const;

		bool IsRootNode(void) const;
		void SetParent(ISceneNode* p_pParentNode);
		ISceneNode* GetParent(void) const;

		//----------------------------------------------------------------------------------------------
		Scenegraph* GetOwner(void) const;
		void SetOwner(Scenegraph* p_pOwner);

		//----------------------------------------------------------------------------------------------
		bool IsVisible(void) const;
		void SetVisible(bool p_bIsVisible);

		//----------------------------------------------------------------------------------------------
		Transformation	LocalTransformation;
		Transformation	WorldTransformation;
		bool IsWorldTransformValid;

		bool IsValidWorldTransform() const;
		void ValidateWorldTransform();
		void InvalidateWorldTransform();

		bool IsAbsoluteLocalTransform(void) const;
		void SetAbsoluteLocalTransform(bool p_bEnabled);

		void SetLocalTransform(const Transformation& p_transformation);
		void SetWorldTransform(const Transformation& p_transformation);

		const Transformation& GetLocalTransform(void) const;
		const Transformation& GetWorldTransform(void);
		const Transformation& GetParentTransform(void) const;

		//----------------------------------------------------------------------------------------------
		BoundingVolumePtr WorldBound;
		bool IsWorldBoundValid;

		bool IsValidWorldBound() const;
		void ValidateWorldBound();
		void InvalidateWorldBound();

		void SetWorldBound(BoundingVolumePtr p_pBoundingVolume);
		BoundingVolumePtr GetWorldBound(void);

		//----------------------------------------------------------------------------------------------
		virtual void UpdateGeometryState(bool p_bIsInitiator, double p_dfApplicationTime);

		virtual void UpdateWorldSynchronisation(double p_dfApplicationTime);
		virtual void UpdateWorldTransform(void);
		virtual void UpdateWorldBound(void) = 0;

		virtual void PropagateToRoot(void);

		//----------------------------------------------------------------------------------------------
		virtual void UpdateRenderState(bool p_bIsInitiator, RenderContextPtr p_pRenderContext, LightListPtr p_pLightList);

		virtual void UpdateContext(RenderContextPtr p_pRenderContext, LightListPtr p_pLightList);

		virtual void PropagateFromRoot(RenderContextPtr& p_pRenderContext, LightListPtr& p_pLightList);

		//----------------------------------------------------------------------------------------------
		void Synchronise(double p_dfApplicationTime);
		
		bool ContainsSyncListener(ISyncListener* p_pSyncListener);
		
		void AttachSyncListener(ISyncListener* p_pSyncListener);
		void DetachSyncListener(ISyncListener* p_pSyncListener);
		void DetachAllSyncListeners(void);

		SyncListenerEnumerator GetSyncListenerEnumerator();

		//----------------------------------------------------------------------------------------------
		bool ContainsEffect(const Meson::Common::Text::String& p_strId);
		bool ContainsEffect(const Meson::Common::Text::String& p_strId, EffectPtr& p_pEffect);

		void AttachEffect(EffectPtr p_pEffect);
		void DetachEffect(EffectPtr p_pEffect);
		void DetachAllEffects(void);

		uint GetEffectIndex(EffectPtr p_pEffect);
		EffectPtr GetEffect(uint p_uiIndex);

		uint GetEffectCount(void) const;

		EffectEnumerator GetEffectEnumerator(void);

		//----------------------------------------------------------------------------------------------
		bool ContainsLight(const Meson::Common::Text::String& p_strId);
		bool ContainsLight(const Meson::Common::Text::String& p_strId, LightPtr& p_pLight);

		void AttachLight(LightPtr p_pLight);
		void DetachLight(LightPtr p_pLight);
		void DetachAllLights(void);
		
		uint GetLightIndex(LightPtr p_pLight);
		LightPtr GetLight(uint p_uiIndex);

		uint GetLightCount(void) const;

		LightEnumerator GetLightEnumerator(void);
		
		//----------------------------------------------------------------------------------------------
		bool ContainsRenderState(IRenderState::RenderStateType p_eRenderStateType);
		bool ContainsRenderState(IRenderState::RenderStateType p_eRenderStateType, RenderStatePtr& p_pRenderState);
		
		void AttachRenderState(RenderStatePtr p_pRenderState);
		void DetachRenderState(RenderStatePtr p_pRenderState);
		void DetachAllRenderStates(void);

		RenderStatePtr GetRenderState(IRenderState::RenderStateType p_eRenderStateType);

		uint GetRenderStateCount(void) const;
		
		RenderStateEnumerator GetRenderStateEnumerator(void);

		//----------------------------------------------------------------------------------------------
		virtual void Cull(ICuller& p_culler);
		
		void AttachCullListener(ICullListener* p_pCullListener);
		void DetachCullListener(void);
		
		ICullListener* GetCullListener(void) const;

		//----------------------------------------------------------------------------------------------
		LightListPtr GetActiveLightList(void) { return m_pActiveLightList; }
		RenderContextPtr GetActiveRenderContext(void) { return m_pActiveRenderContext; }
		//----------------------------------------------------------------------------------------------

	protected:
		NodeType m_eNodeType;

		bool m_bIsNodeVisible,
			 m_bIsAbsoluteLocalTransform;

		Scenegraph* m_pOwner;
		ISceneNode* m_pParentNode;
		ICullListener*  m_pCullListener;
		
		SynchListenerList	m_syncListenerList;
		
		RenderStateMap		m_renderStateMap;
		EffectList			m_effectList;
		LightList			m_lightList;

		RenderContextPtr	m_pActiveRenderContext;
		LightListPtr		m_pActiveLightList;
	};

	typedef Meson::Common::TPointer<ISceneNode> SceneNodePtr;
	typedef Meson::Common::Collections::TEnumerator<ISceneNode*>& SceneBaseNodeEnumerator;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END