//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstEffect.h"
#include "VstRenderer.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class ShadowEffect : public IEffect
	{
		VISTAS_IMPLEMENT_RTTI(ShadowEffect, IEffect);

	public:
		ShadowEffect(const Meson::Common::Text::String& p_strProjectionName, Camera* p_pProjector, uint p_uiWidth, uint p_uiHeight);
		~ShadowEffect(void);
		
		void InitialiseTechniques(IRenderDevice* p_pRenderDevice);
		void InitialiseShaderPrograms(IRenderDevice* p_pRenderDevice);
		void InitialiseProjectionTarget(IRenderDevice* p_pRenderDevice);

		void SetRenderer(RendererPtr p_pRenderer);
		void SetShadowCasterSet(VisibleEntitySetPtr p_pShadowCasterSet);

		void Initialise(IRenderDevice* p_pRenderDevice);
		void Dispose(IRenderDevice* p_pRenderDevice);
	
		void Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity);
		void Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet); 

	protected:
		uint m_uiWidth,
			 m_uiHeight;

		Camera *m_pProjector;

		Meson::Common::Text::String m_strProjectionName;
		
		TextureBufferPtr		m_pProjectionTexture;
		TextureRenderTarget*	m_pProjectionTarget;

		SamplerRenderStatePtr	m_pSamplerState;
		SamplerStatePtr		    m_pSampler;

		ShaderProgramVariable	m_lightWVPMatrix;

		IShaderProgram *m_pDrawVS, 
					   *m_pDrawPS,
					   *m_pApplyVS,
					   *m_pApplyPS;

		TechniquePtr	m_pDrawTechnique,
						m_pApplyTechnique;

		AlphaRenderStatePtr m_pAlphaState;
		BiasRenderStatePtr  m_pBiasState;

		VisibleEntitySetPtr m_pShadowCasterSet;

		RendererPtr			m_pRenderer;
	};

Meson_Vistas_END