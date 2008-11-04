//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "LookaheadInputStream.h"

#include "VstTypes.h"
#include "VstSceneCompositeNode.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class VISTAS_API Scenegraph : public VistasObject
	{
		VISTAS_IMPLEMENT_RTTI(Scenegraph, VistasObject);

	public:
		Scenegraph();
		Scenegraph(const Meson::Common::Text::String& p_strName);

		~Scenegraph();

		bool ContainsNode(const Meson::Common::Text::String& p_strName);
		bool ContainsNode(const Meson::Common::Text::String& p_strName, ISceneNode*& p_pNode);

		ISceneNode* GetNode(const Meson::Common::Text::String& p_strName);

		void RegisterNode(ISceneNode* p_pNode, bool p_bTakeOwnership=false);

		void UnregisterNode(ISceneNode* p_pNode);
		void UnregisterNode(const Meson::Common::Text::String& p_strName);
		void UnregisterAllNodes(void);

		void LoadNode(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadNodes(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadNodes(const Meson::Common::Text::String& p_strResourceName);

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, ISceneNode*> m_nodeMap;
	};

	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<Scenegraph> ScenegraphPtr;
	typedef Meson::Common::Collections::TEnumerator<ScenegraphPtr>& ScenegraphEnumerator;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END