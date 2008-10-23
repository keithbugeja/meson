//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "ArrayList.h"

#include "VstTypes.h"
#include "VstTechniqueGroup.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class Material : public TechniqueGroup
	{
	public:
		Material(const Meson::Common::Text::String& p_strName);
		Material(const Material& p_material);
		~Material(void);
	
		const Meson::Common::Text::String& GetName(void) const;
		Material& operator=(const Material& p_material);

		TechniquePtr GetBestTechnique(void);

	protected:
		Meson::Common::Text::String m_strName;
	};

	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<Material> MaterialPtr;
	typedef Meson::Common::Collections::TEnumerator<MaterialPtr>& MaterialEnumerator;
	typedef Meson::Common::Collections::TArrayList<MaterialPtr> MaterialList;
	typedef Meson::Common::TPointer<MaterialList> MaterialListPtr; 
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END