//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"
#include "InputStream.h"

#include "VstTypes.h"
#include "VstMaterial.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class MaterialManager : public Meson::Common::TSingleton<MaterialManager>
	{
		friend class Meson::Common::TSingleton<MaterialManager>;

	private: 
		MaterialManager(void);
		~MaterialManager(void);

	public:
		MaterialPtr CreateMaterial(const Meson::Common::Text::String& p_strName);

		bool ContainsMaterial(const Meson::Common::Text::String& p_strName);
		bool ContainsMaterial(const Meson::Common::Text::String& p_strName, MaterialPtr& p_pMaterial);

		MaterialPtr GetMaterial(const Meson::Common::Text::String& p_strName);

		void ReleaseMaterial(MaterialPtr p_pMaterial);
		void ReleaseMaterial(const Meson::Common::Text::String& p_strName);
		void ReleaseAllMaterials(void);

		void ExecuteMaterialScript(Meson::Common::IO::IInputStream& p_inputStream);

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, MaterialPtr> m_materialMap;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
