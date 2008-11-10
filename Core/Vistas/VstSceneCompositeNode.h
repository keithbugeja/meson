//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstSceneNode.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class VISTAS_API ISceneCompositeNode : public ISceneNode
	{
		VISTAS_IMPLEMENT_RTTI(ISceneCompositeNode, ISceneNode);

	protected:
		ISceneCompositeNode(Scenegraph* p_pOwner = NULL);
		ISceneCompositeNode(const Meson::Common::Text::String& p_strId, Scenegraph* p_pOwner = NULL);

	public:
		virtual ~ISceneCompositeNode(void);

		virtual ISceneNode* GetChild(uint p_uiIndex);
		virtual ISceneNode* GetChild(const Meson::Common::Text::String& p_strId);
		virtual SceneBaseNodeEnumerator GetChildEnumerator(void);
		virtual uint GetChildCount(void);

		virtual void AttachChild(ISceneNode* p_pChildNode);
		virtual void AttachChild(ISceneNode* p_pChildNode, uint p_uiIndex);
		virtual void DetachChild(ISceneNode* p_pChildNode);
		virtual void DetachChild(uint p_uiIndex);
		virtual void DetachAllChildren(void);

		virtual void UpdateContext(RenderContextPtr p_pRenderContext, LightListPtr p_pLightList);
		virtual void UpdateWorldSynchronisation(double p_dfApplicationTime);
		virtual void UpdateWorldBound(void);		

		virtual void InvalidateWorldTransform(void);

		virtual void Cull(ICuller& p_culler);

		virtual ISceneNode* operator[] (uint p_uiIndex) const;

	protected:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, ISceneNode*> m_childNodeMap;
		Meson::Common::Collections::TArrayList<ISceneNode*> m_childNodeList;
	};

	//----------------------------------------------------------------------------------------------

Meson_Vistas_END