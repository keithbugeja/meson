//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "ArrayList.h"
#include "VstTypes.h"
#include "VstSamplerState.h"
#include "VstTextureState.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class SamplerUnit : public Meson::Common::TReferenceCounter<SamplerUnit>
	{
	public:
		SamplerUnit(const SamplerUnit& p_samplerUnit);
		SamplerUnit(uint p_uiSamplerIndex, TextureStatePtr p_pTextureState, SamplerStatePtr p_pSamplerState);
		SamplerUnit(uint p_uiSamplerIndex);

		~SamplerUnit(void);

		uint GetSamplerIndex(void) const;

		TextureStatePtr GetTextureState(void);
		void SetTextureState(TextureStatePtr p_pTextureState);

		SamplerStatePtr GetSamplerState(void);
		void SetSamplerState(SamplerStatePtr p_pSamplerState);

		const SamplerUnit& operator=(const SamplerUnit& p_samplerUnit);
	
	protected:
		uint m_uiSamplerIndex;

		TextureStatePtr m_pTextureState;
		SamplerStatePtr m_pSamplerState;
	};

	typedef Meson::Common::TPointer<SamplerUnit> SamplerUnitPtr;
	typedef Meson::Common::Collections::TEnumerator<SamplerUnitPtr>& SamplerUnitEnumerator;
	//----------------------------------------------------------------------------------------------
	
Meson_Vistas_END
