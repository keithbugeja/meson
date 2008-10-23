//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

#include "VstScenegraph.h"

Meson_Vistas_BEGIN
	//----------------------------------------------------------------------------------------------
	class ScenegraphManager : public Meson::Common::TSingleton<ScenegraphManager>
	{
		friend class Meson::Common::TSingleton<ScenegraphManager>;
	private:
		ScenegraphManager(void);
		~ScenegraphManager(void);
	public:
		ScenegraphPtr CreateScenegraph(const Meson::Common::Text::String& p_strId);

		bool ContainsScenegraph(const Meson::Common::Text::String& p_strId);
		bool ContainsScenegraph(const Meson::Common::Text::String& p_strId, ScenegraphPtr& p_pScenegraph);

		ScenegraphPtr GetScenegraph(const Meson::Common::Text::String& p_strId);
		ScenegraphEnumerator GetScenegraphEnumerator(void);

		void RegisterScenegraph(ScenegraphPtr p_pScenegraph, bool p_bReplace=true);

		void UnregisterScenegraph(ScenegraphPtr p_pScenegraph);
		void UnregisterScenegraph(const Meson::Common::Text::String& p_strId);
		void UnregisterAllScenegraphs(void);

		void LoadScenegraph(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadScenegraphs(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadScenegraphs(const Meson::Common::Text::String& p_strResourceName);

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, ScenegraphPtr> m_scenegraphMap;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END