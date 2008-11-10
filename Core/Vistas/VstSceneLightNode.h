//----------------------------------------------------------------------------------------------
#pragma once

#include "VstSceneCompositeNode.h"
#include "VstLight.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class VISTAS_API SceneLightNode : public ISceneNode
	{
		VISTAS_IMPLEMENT_RTTI(SceneLightNode, ISceneNode);

	public:
		SceneLightNode(Scenegraph* p_pOwner = NULL);
		SceneLightNode(const Meson::Common::Text::String& p_strId, Scenegraph* p_pOwner = NULL);
		~SceneLightNode();

		virtual void UpdateWorldSynchronisation(double p_dfApplicationTime);
		virtual void UpdateWorldBound(void);

		/** Accessors for the light object contained by this leaf node.
		 */
		virtual void SetLight(LightPtr& p_pLight);
		virtual LightPtr GetLight(void);

	protected:
		LightPtr m_pLight;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END