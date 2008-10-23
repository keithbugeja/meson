//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"

#include "VstTypes.h"
#include "VstRendererFactory.h"

Meson_Vistas_BEGIN

	typedef Meson::Common::Collections::TEnumerator<IRendererFactory*>& RendererFactoryEnumerator;

	class RendererManager : public Meson::Common::TSingleton<RendererManager>
	{
		friend class Meson::Common::TSingleton<RendererManager>;

	private: 
		RendererManager(void);
		~RendererManager(void);

	public:
		RendererFactoryEnumerator GetRendererFactories(void);
		IRendererFactory* GetRendererFactory(const Meson::Common::Text::String& p_strName);

		void RegisterRendererFactory(Meson::Vistas::IRendererFactory *p_pRendererFactory);
		void UnregisterRendererFactory(Meson::Vistas::IRendererFactory *p_pRendererFactory);
	
	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, IRendererFactory*> m_rendererFactoryMap;
	};

Meson_Vistas_END
