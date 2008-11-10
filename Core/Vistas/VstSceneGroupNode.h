//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstSceneCompositeNode.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class VISTAS_API SceneGroupNode : public ISceneCompositeNode
	{
		VISTAS_IMPLEMENT_RTTI(SceneGroupNode, ISceneCompositeNode);

	public:
		SceneGroupNode(Scenegraph* p_pOwner = NULL)
			: ISceneCompositeNode(p_pOwner)
		{
		}

		SceneGroupNode(const Meson::Common::Text::String& p_strId, Scenegraph* p_pOwner = NULL)
			: ISceneCompositeNode(p_strId, p_pOwner)
		{
		}
	};

	//----------------------------------------------------------------------------------------------

Meson_Vistas_END