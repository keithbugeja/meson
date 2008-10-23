//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::BiasRenderState
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstRenderState.h"

Meson_Vistas_BEGIN

	class BiasRenderState : public IRenderState
	{
	public:
		BiasRenderState(void);
		RenderStateType GetType(void) const;

		void SetScaleBias(float p_fScaleBias);
		void SetDepthBias(float p_fDepthBias);

		float GetScaleBias(void) const;
		float GetDepthBias(void) const;

		const BiasRenderState& operator=(const BiasRenderState& p_biasRenderState);
		bool operator==(const BiasRenderState& p_biasRenderState);

	protected:
		float m_fScaleBias,
			  m_fDepthBias;
	};

	typedef Meson::Common::TPointer<BiasRenderState> BiasRenderStatePtr;

Meson_Vistas_END
