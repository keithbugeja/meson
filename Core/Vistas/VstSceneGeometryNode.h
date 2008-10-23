//----------------------------------------------------------------------------------------------
#pragma once

#include "VstSceneCompositeNode.h"
#include "VstGeometry.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class SceneGeometryNode : public ISceneNode
	{
		VISTAS_IMPLEMENT_RTTI(SceneGeometryNode, ISceneNode);

	public:
		SceneGeometryNode(const Meson::Common::Text::String& p_strId, Scenegraph* p_pOwner = NULL);
		SceneGeometryNode(Scenegraph* p_pOwner = NULL);
		~SceneGeometryNode();

		virtual void UpdateWorldBound(void);

		/** Accessors for the geometry object contained by this leaf node.
		 */
		virtual void SetGeometry(GeometryPtr& p_pGeometry);
		virtual GeometryPtr GetGeometry(void);

		virtual void SetMaterial(MaterialPtr& p_pMaterial);
		virtual MaterialPtr GetMaterial(void);

	protected:
		GeometryPtr m_pGeometry;
		MaterialPtr m_pMaterial;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END