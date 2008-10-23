//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::AlphaRenderState
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstRenderState.h"

Meson_Vistas_BEGIN

	class AlphaRenderState : public IRenderState
	{
	public:
		/** Alpha comparison functions
		 */
		enum AlphaCompare
		{
			ARSCMP_NEVER,
			ARSCMP_LESS,
			ARSCMP_EQUAL,
			ARSCMP_LESSEQUAL,
			ARSCMP_GREATER,
			ARSCMP_NOTEQUAL,
			ARSCMP_GREATEREQUAL,
			ARSCMP_ALWAYS,
			ARSCMP_COUNT
		};

		/** Alpha blending operations
		 */
		enum AlphaBlend
		{
			ARSB_ZERO,
			ARSB_ONE,
			ARSB_SRC_COLOUR,
			ARSB_ONE_MINUS_SRC_COLOUR,
			ARSB_SRC_ALPHA,
			ARSB_ONE_MINUS_SRC_ALPHA,
			ARSB_DST_COLOUR,
			ARSB_ONE_MINUS_DST_COLOUR,
			ARSB_DST_ALPHA,
			ARSB_ONE_MINUS_DST_ALPHA,
			ARSB_CONSTANT_ALPHA_COLOUR,
			ARSB_ONE_MINUS_CONSTANT_ALPHA_COLOUR,
			ARSB_CONSTANT_ALPHA,
			ARSB_ONE_MINUS_CONSTANT_ALPHA,
			ARSB_CONSTANT_COLOUR,
			ARSB_ONE_MINUS_CONSTANT_COLOUR,

			ARSB_COUNT
		};

	public:
		AlphaRenderState(void);

		RenderStateType GetType(void) const;
		
		AlphaBlend GetSourceBlendMode(void) const;
		AlphaBlend GetDestinationBlendMode(void) const;
		bool IsAlphaBlendEnabled(void) const;

		void SetSourceBlendMode(AlphaBlend p_eSourceBlendMode);
		void SetDestinationBlendMode(AlphaBlend p_eDestinationBlendMode);
		void SetAlphaBlend(bool p_bEnabled);

		AlphaCompare GetCompareType(void) const;
		dword GetAlphaReference(void) const;
		bool IsAlphaTestEnabled(void) const;

		void SetCompareType(AlphaCompare p_eCompareType);
		void SetAlphaReference(dword p_dwAlphaReference);
		void SetAlphaTest(bool p_bEnabled);

		const AlphaRenderState& operator= (const AlphaRenderState& p_alphaRenderState);
		bool operator==(const AlphaRenderState& p_alphaRenderState);

	protected:
		bool	m_bIsAlphaTestEnabled,
				m_bIsAlphaBlendEnabled;

		dword	m_dwAlphaReference;
		
		AlphaCompare m_eCompareType;
		
		AlphaBlend   m_eSourceBlend,
					 m_eDestinationBlend;
	};

	typedef Meson::Common::TPointer<AlphaRenderState> AlphaRenderStatePtr;

Meson_Vistas_END
