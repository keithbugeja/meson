//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "ArrayList.h"
#include "VstTypes.h"
#include "VstRenderState.h"
#include "VstSamplerUnit.h"

Meson_Vistas_BEGIN
	class SamplerRenderState : public IRenderState
	{
	public:
		SamplerRenderState();
		~SamplerRenderState();

		RenderStateType GetType(void) const;
		uint GetSamplerUnitCount(void) const;

		bool Contains(uint p_uiSamplerIndex) const;
		bool Contains(uint p_uiSamplerIndex, SamplerUnitPtr& p_pSamplerUnit) const;

		SamplerUnitEnumerator GetSamplerUnitEnumerator(void);

		SamplerUnitPtr GetSamplerUnit(uint p_uiSamplerIndex) const;
		TextureStatePtr GetTextureState(uint p_uiSamplerIndex) const;
		SamplerStatePtr GetSamplerState(uint p_uiSamplerIndex) const;

		SamplerUnitPtr AddSamplerUnit(uint p_uiSamplerIndex);
		SamplerUnitPtr AddSamplerUnit(uint p_uiSamplerIndex, TextureStatePtr p_pTextureState, SamplerStatePtr p_pSamplerState);

		void SetSamplerUnit(uint p_uiSamplerIndex, TextureStatePtr p_pTextureState, SamplerStatePtr p_pSamplerState);
		void SetTextureState(uint p_uiSamplerIndex, TextureStatePtr p_pTextureState);
		void SetSamplerState(uint p_uiSamplerIndex, SamplerStatePtr p_pSamplerState);

		void RemoveSamplerUnit(uint p_uiSamplerIndex);
		void RemoveSamplerUnit(SamplerUnitPtr p_pSamplerUnit);
		void RemoveAllSamplerUnits(void);

		const SamplerRenderState& operator=(const SamplerRenderState& p_samplerRenderState);
		bool operator==(const SamplerRenderState& p_samplerRenderState);

	protected:
		Meson::Common::Collections::THashMap<uint, SamplerUnitPtr> m_samplerUnitMap;
	};

	typedef Meson::Common::TPointer<SamplerRenderState> SamplerRenderStatePtr;

Meson_Vistas_END
