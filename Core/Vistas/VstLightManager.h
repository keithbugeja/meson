//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "VstTypes.h"
#include "VstLight.h"

Meson_Vistas_BEGIN
	class VISTAS_API LightManager : public Meson::Common::TSingleton<LightManager>
	{
		friend class Meson::Common::TSingleton<LightManager>;

	private: 
		LightManager(void);
		~LightManager(void);

	public:
		LightPtr CreateLight(const Meson::Common::Text::String& p_strId);

		bool ContainsLight(const Meson::Common::Text::String& p_strId);
		bool ContainsLight(const Meson::Common::Text::String& p_strId, LightPtr& p_pLight);

		LightPtr GetLight(const Meson::Common::Text::String& p_strId);

		void RegisterLight(LightPtr p_pLight, bool p_bReplace=true);

		void UnregisterLight(LightPtr p_pLight);
		void UnregisterLight(const Meson::Common::Text::String& p_strId);
		void UnregisterAllLights(void);

		void LoadLight(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadLights(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadLights(const Meson::Common::Text::String& p_strResourceName);

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, LightPtr> m_lightMap;
	};

Meson_Vistas_END
