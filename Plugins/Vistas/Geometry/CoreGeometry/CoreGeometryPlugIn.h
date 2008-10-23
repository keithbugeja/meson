//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include "BoxFactory.h"
#include "PlaneFactory.h"
#include "SphereFactory.h"
#include "CylinderFactory.h"
#include "WavefrontMeshFactory.h"

#include "VstNamespaces.h"

Meson_Vistas_BEGIN
	class CoreGeometryPlugIn : public Meson::Common::PlugIn::IPlugIn
	{
	public:
		CoreGeometryPlugIn();
		~CoreGeometryPlugIn();

		const Meson::Common::Text::String& GetName(void) const;
		const Meson::Common::Version& GetVersion(void) const;

		void Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap);
		void Unregister();

		void Initialise(void);
		void Shutdown(void);

	private:
		Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;

		Meson::Common::Text::String	m_pName;
		Meson::Common::Version	m_version;

		BoxFactory*				m_pBoxFactory;
		CylinderFactory*		m_pCylinderFactory;
		PlaneFactory*			m_pPlaneFactory;
		SphereFactory*			m_pSphereFactory;
		WavefrontMeshFactory*	m_pWavefrontMeshFactory;
	};
Meson_Vistas_END
