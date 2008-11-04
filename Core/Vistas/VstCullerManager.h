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
#include "VstCullerFactory.h"

Meson_Vistas_BEGIN

	typedef Meson::Common::Collections::TEnumerator<ICullerFactory*>& CullerFactoryEnumerator;

	class VISTAS_API CullerManager : public Meson::Common::TSingleton<CullerManager>
	{
		friend class Meson::Common::TSingleton<CullerManager>;

	private: 
		CullerManager(void);
		~CullerManager(void);

	public:
		CullerFactoryEnumerator GetCullerFactories(void);
		ICullerFactory* GetCullerFactory(const Meson::Common::Text::String& p_strName);
		
		void RegisterCullerFactory(Meson::Vistas::ICullerFactory *p_pCullerFactory);
		void UnregisterCullerFactory(Meson::Vistas::ICullerFactory *p_pCullerFactory);
	
	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, ICullerFactory*> m_cullerFactoryMap;
	};

Meson_Vistas_END
