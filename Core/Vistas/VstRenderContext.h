#pragma once

#include "VstTypes.h"

#include "VstRenderState.h"
#include "VstAlphaRenderState.h"
#include "VstBiasRenderState.h"
#include "VstCullRenderState.h"
#include "VstDepthRenderState.h"
#include "VstLightRenderState.h"
#include "VstMaterialRenderState.h"
#include "VstSamplerRenderState.h"
#include "VstStencilRenderState.h"
#include "VstTargetRenderState.h"

Meson_Vistas_BEGIN

	class VISTAS_API RenderContext : public Meson::Common::TReferenceCounter<RenderContext>
	{
	protected:
		void ClearRenderContext(void)
		{
			for (uint uiStateIndex = 0; uiStateIndex < IRenderState::RST_COUNT; uiStateIndex++)
				m_pRenderStateList[uiStateIndex] = NULL;
		}

		void InitialiseRenderContext(void)
		{
			m_pRenderStateList[IRenderState::RST_ALPHA] = new AlphaRenderState();
			m_pRenderStateList[IRenderState::RST_BIAS] = new BiasRenderState();
			m_pRenderStateList[IRenderState::RST_CULL] = new CullRenderState();
			m_pRenderStateList[IRenderState::RST_DEPTH] = new DepthRenderState();
			m_pRenderStateList[IRenderState::RST_LIGHT] = new LightRenderState();
			m_pRenderStateList[IRenderState::RST_MATERIAL] = new MaterialRenderState();
			m_pRenderStateList[IRenderState::RST_SAMPLER] = new SamplerRenderState();
			m_pRenderStateList[IRenderState::RST_STENCIL] = new StencilRenderState();
			m_pRenderStateList[IRenderState::RST_TARGET] = new TargetRenderState();
		}

	public:
		RenderContext(bool p_bInitialiseRenderContext = true)
			: m_pParentRenderContext(NULL)
		{
			if (p_bInitialiseRenderContext)
				InitialiseRenderContext();
			else
				ClearRenderContext();
		}

		RenderContext(Meson::Common::TPointer<RenderContext> m_pParentContext)
			: m_pParentRenderContext(m_pParentContext)
		{
		}

		RenderContext(const RenderContext& p_renderContext)
			: m_pParentRenderContext(p_renderContext.m_pParentRenderContext)
		{
			for (uint uiStateIndex = 0; uiStateIndex < IRenderState::RST_COUNT; uiStateIndex++)
				m_pRenderStateList[uiStateIndex] = p_renderContext.m_pRenderStateList[uiStateIndex];
		}

		~RenderContext(void)
		{
			m_pParentRenderContext = NULL;

			for (uint uiStateIndex = 0; uiStateIndex < IRenderState::RST_COUNT; uiStateIndex++)
				m_pRenderStateList[uiStateIndex] = NULL;
		}

		RenderStatePtr& GetRenderState(IRenderState::RenderStateType p_eRenderStateType)
		{
			if (m_pRenderStateList[p_eRenderStateType] == NULL)
				return m_pParentRenderContext->GetRenderState(p_eRenderStateType);

			return m_pRenderStateList[p_eRenderStateType];
		}

		void SetRenderState(RenderStatePtr p_pRenderState)
		{
			m_pRenderStateList[p_pRenderState->GetType()] = p_pRenderState;
		}

		void SetRenderState(IRenderState* p_pRenderState)
		{
			m_pRenderStateList[p_pRenderState->GetType()] = p_pRenderState;
		}

		void SetParentContext(Meson::Common::TPointer<RenderContext> p_pRenderContext)
		{
			m_pParentRenderContext = p_pRenderContext;
		}

	protected:
		RenderStatePtr m_pRenderStateList[IRenderState::RST_COUNT];
		Meson::Common::TPointer<RenderContext> m_pParentRenderContext;
	};

	typedef Meson::Common::TPointer<RenderContext> RenderContextPtr;

Meson_Vistas_END