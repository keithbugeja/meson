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
#include "VstGeometry.h"
#include "VstGeometryFactory.h"

Meson_Vistas_BEGIN
	class IGeometryFactory;
	typedef Meson::Common::Collections::TEnumerator<IGeometryFactory*>& GeometryFactoryEnumerator;

	class VISTAS_API GeometryManager : public Meson::Common::TSingleton<GeometryManager>
	{
		friend class Meson::Common::TSingleton<GeometryManager>;

	private: 
		GeometryManager(void);
		~GeometryManager(void);

	public:
		bool ContainsGeometry(const Meson::Common::Text::String& p_strId);
		bool ContainsGeometry(const Meson::Common::Text::String& p_strId, GeometryPtr& p_pGeometry);

		GeometryPtr GetGeometry(const Meson::Common::Text::String& p_strId);

		void RegisterGeometry(GeometryPtr p_pGeometry, bool p_bReplace=true);

		void UnregisterGeometry(GeometryPtr p_pGeometry);
		void UnregisterGeometry(const Meson::Common::Text::String& p_strId);
		void UnregisterAllGeometry(void);

		GeometryFactoryEnumerator GetGeometryFactories(void);
		IGeometryFactory* GetGeometryFactory(const Meson::Common::Text::String& p_strName);

		void RegisterGeometryFactory(Meson::Vistas::IGeometryFactory *p_pRendererFactory);
		void UnregisterGeometryFactory(Meson::Vistas::IGeometryFactory *p_pRendererFactory);
		void UnregisterAllGeometryFactories(void);

		void LoadGeometry(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadGeometries(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadGeometries(const Meson::Common::Text::String& p_strResourceName);

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, IGeometryFactory*> m_geometryFactoryMap;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, GeometryPtr> m_geometryMap;
	};

Meson_Vistas_END
