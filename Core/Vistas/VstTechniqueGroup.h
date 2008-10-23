//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstTechnique.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	// TODO: Implement a technique group / technique / material manager
	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::Collections::TEnumerator<TechniquePtr>& TechniqueEnumerator;

	class TechniqueGroup : public Meson::Common::TReferenceCounter<TechniqueGroup>
	{
	public:
		TechniqueGroup(void);
		TechniqueGroup(const TechniqueGroup& p_techniqueGroup);
		virtual ~TechniqueGroup(void);
	
		//----------------------------------------------------------------------------------------------
		uint GetTechniqueCount(void) const;

		bool ContainsTechnique(const Meson::Common::Text::String& p_strTechniqueName);
		bool ContainsTechnique(const Meson::Common::Text::String& p_strTechniqueName, TechniquePtr& p_pTechnique);

		TechniqueEnumerator GetTechniqueEnumerator(void);

		TechniquePtr CreateTechnique(void);
		TechniquePtr CreateTechnique(const Meson::Common::Text::String& p_strTechniqueName);

		TechniquePtr GetTechnique(uint p_uiTechniqueIndex);
		TechniquePtr GetTechnique(const Meson::Common::Text::String& p_strTechniqueName);
		
		void AddTechnique(TechniquePtr p_pTechnique);
		void AddTechniqueGroup(const TechniqueGroup& p_techniqueGroup);

		void RemoveTechnique(const Meson::Common::Text::String& p_strTechniqueName);
		void RemoveTechnique(uint p_uiTechniqueIndex);
		void RemoveTechniqueRange(uint p_uiStartIndex, uint p_uiCount);
		void RemoveAllTechniques(void);
		//----------------------------------------------------------------------------------------------

		TechniqueGroup& operator=(const TechniqueGroup& p_techniqueGroup);

	protected:
		Meson::Common::Collections::TArrayList<TechniquePtr> m_techniqueList;
	};

	typedef Meson::Common::TPointer<TechniqueGroup> TechniqueGroupPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END