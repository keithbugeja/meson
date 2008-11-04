//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::StencilRenderState
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstRenderState.h"

Meson_Vistas_BEGIN

	class VISTAS_API StencilRenderState : public IRenderState
	{
	public:
		/** Stencil comparison function
		 */
		enum StencilCompare
		{
			SRSCMP_NEVER,
			SRSCMP_LESS,
			SRSCMP_EQUAL,
			SRSCMP_LESSEQUAL,
			SRSCMP_GREATER,
			SRSCMP_NOTEQUAL,
			SRSCMP_GREATEREQUAL,
			SRSCMP_ALWAYS,
			SRSCMP_COUNT
		};

		/** Stencil operation
		 */
		enum StencilOperation
		{
			SRSOP_KEEP,
			SRSOP_ZERO,
			SRSOP_REPLACE,
			SRSOP_INCREMENT,
			SRSOP_DECREMENT,
			SRSOP_INCREMENT_CLAMP,
			SRSOP_DECREMENT_CLAMP,
			SRSOP_INVERT,
			SRSOP_COUNT
		};

	public:
		StencilRenderState();

		RenderStateType GetType(void) const;

		StencilCompare GetCompareType(void) const;

		StencilOperation GetPassOperation(void) const;
		StencilOperation GetFailOperation(void) const;
		StencilOperation GetDepthFailOperation(void) const;
		
		dword GetStencilReference(void) const;
		dword GetStencilWriteMask(void) const;
		dword GetStencilMask(void) const;

		bool IsStencilTestEnabled(void) const;

		void SetCompareType( StencilCompare p_compareType );

		void SetPassOperation( StencilOperation p_passOperation );
		void SetFailOperation( StencilOperation p_failOperation );
		void SetDepthFailOperation( StencilOperation p_depthFailOperation );

		void SetStencilReference( dword p_dwStencilReference );
		void SetStencilWriteMask( dword p_dwStencilWriteMask );
		void SetStencilMask( dword p_dwStencilMask );

		void SetStencilTest(bool p_bEnabled);

		const StencilRenderState& operator= (const StencilRenderState& p_stencilRenderState);
		bool operator==(const StencilRenderState& p_stencilRenderState);

	protected:
		bool m_bIsStencilEnabled;
		
		StencilCompare m_compareType;
		
		StencilOperation m_stencilPassOperation,
						 m_stencilFailOperation,
						 m_stencilDepthFailOperation;

		dword	m_dwStencilReference,
				m_dwStencilMask,
				m_dwStencilWriteMask;
	};

	typedef Meson::Common::TPointer<StencilRenderState> StencilRenderStatePtr;

Meson_Vistas_END
