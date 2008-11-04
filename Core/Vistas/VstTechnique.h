//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

#include "VstPass.h"
#include "VstAmbientPass.h"
#include "VstIlluminationPass.h"
#include "VstEffectPass.h"

Meson_Vistas_BEGIN

	typedef Meson::Common::Collections::TEnumerator<PassPtr>& PassEnumerator;

	class VISTAS_API Technique : public Meson::Common::TReferenceCounter<Technique>
	{
	protected:
		void SaveState(void);
		void RestoreState(void);

	public:
		Technique(bool p_bSaveState = false);
		Technique(const Meson::Common::Text::String& p_strName, bool p_bSaveState = false);
		~Technique(void);
	
		const Meson::Common::Text::String& GetName(void) const;

		bool IsSaveStateEnabled(void) const;
		void SetSaveState(bool p_bEnabled);

		void RebuildTechnique(void);
		
		uint GetPassCount(void) const;
		uint GetPassCount(IPass::PassType p_ePassType) const;

		bool ContainsPass(const Meson::Common::Text::String& p_strPassName);
		bool ContainsPass(const Meson::Common::Text::String& p_strPassName, PassPtr& p_pPass);
		bool ContainsPassType(IPass::PassType p_ePassType) const;

		PassEnumerator GetPassEnumerator(void);
		PassEnumerator GetPassEnumerator(IPass::PassType p_ePassType);

		PassPtr CreatePass(IPass::PassType p_ePassType = IPass::PT_ILLUMINATION_PASS);
		PassPtr CreatePass(const Meson::Common::Text::String& p_strPassName, IPass::PassType p_ePassType = IPass::PT_ILLUMINATION_PASS);

		PassPtr GetPass(uint p_uiPassNumber);
		PassPtr GetPass(const Meson::Common::Text::String& p_strPassName);

		void AddPass(PassPtr p_pPass);
		void SetPass(uint p_uiPassNumber, PassPtr p_pPass);
		void InsertPass(uint p_uiPassNumber, PassPtr p_pPass);
		void MovePass(uint p_uiSourcePassNumber, uint p_uiDestinationPassNumber);
		
		void RemovePass(const Meson::Common::Text::String& p_strPassName);
		void RemovePass(uint p_uiPassNumber);
		void RemoveAllPasses(void);

		virtual uint BeginTechnique(void);
		virtual void BeginPass(PassPtr p_pPass);
		virtual void BeginPass(uint p_uiPassNumber);
		virtual void EndPass();
		virtual void EndTechnique();

		Technique& operator=(const Technique& p_technique);

	protected:
		Meson::Common::Collections::TArrayList<PassPtr> m_passList;
		Meson::Common::Collections::TArrayList<PassPtr> m_categorisedPassList[IPass::PT_COUNT];

		Meson::Common::Text::String m_strName;
		
		bool m_bIsSaveStateEnabled,
			 m_bIsTechniqueStarted,
			 m_bIsPassStarted;

		uint m_uiCurrentPass;
	};

	typedef  Meson::Common::TPointer<Technique> TechniquePtr;

Meson_Vistas_END