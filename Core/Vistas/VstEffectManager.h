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
#include "VstEffect.h"

Meson_Vistas_BEGIN
	class VISTAS_API EffectManager : public Meson::Common::TSingleton<EffectManager>
	{
		friend class Meson::Common::TSingleton<EffectManager>;

	private: 
		EffectManager(void);
		~EffectManager(void);

	public:
		bool ContainsEffect(const Meson::Common::Text::String& p_strId);
		bool ContainsEffect(const Meson::Common::Text::String& p_strId, EffectPtr& p_pEffect);

		EffectPtr GetEffect(const Meson::Common::Text::String& p_strId);

		void RegisterEffect(EffectPtr p_pEffect, bool p_bReplace=true);

		void UnregisterEffect(EffectPtr p_pEffect);
		void UnregisterEffect(const Meson::Common::Text::String& p_strId);
		void UnregisterAllEffects(void);

		void DestroyAllEffects(void);

		void LoadEffect(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadEffects(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadEffects(const Meson::Common::Text::String& p_strResourceName);

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, EffectPtr> m_effectMap;
	};

Meson_Vistas_END
