//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::DepthRenderState
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstRenderState.h"

Meson_Vistas_BEGIN

	class VISTAS_API DepthRenderState : public IRenderState
	{
	public:
		/** Depth comparison functions
		 */
		enum DepthCompare
		{
			DRSCMP_NEVER,
			DRSCMP_LESS,
			DRSCMP_EQUAL,
			DRSCMP_LESSEQUAL,
			DRSCMP_GREATER,
			DRSCMP_NOTEQUAL,
			DRSCMP_GREATEREQUAL,
			DRSCMP_ALWAYS,
			DRSCMP_COUNT
		};

	public:
		DepthRenderState(void);
		RenderStateType GetType(void) const;

		DepthCompare GetCompareType(void) const;
		bool IsDepthTestEnabled(void) const;
		bool IsDepthWriteEnabled(void) const;

		void SetCompareType( DepthCompare p_compareType );
		void SetDepthTest(bool p_bEnabled);
		void SetDepthWrite(bool p_bEnabled);

		const DepthRenderState& operator= (const DepthRenderState& p_depthRenderState);
		bool operator==(const DepthRenderState& p_depthRenderState);

	protected:
		bool m_bIsDepthTestEnabled;
		bool m_bIsDepthWriteEnabled;
		
		DepthCompare m_compareType;
	};

	typedef Meson::Common::TPointer<DepthRenderState> DepthRenderStatePtr;

Meson_Vistas_END
