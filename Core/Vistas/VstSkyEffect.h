//----------------------------------------------------------------------------------------------
#pragma once

#include "VstEffect.h"
#include "VstTechnique.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class SkyEffect : public IEffect
	{
		VISTAS_IMPLEMENT_RTTI(SkyEffect, IEffect);

	public:
		SkyEffect(const Meson::Common::Text::String& p_strCubeMapName, const Meson::Common::Text::String& p_strCubeMapFilename);
		SkyEffect(TexturePtr p_pCubeMap);
		~SkyEffect(void);
		
		void InitialiseTextures(IRenderDevice* p_pRenderDevice);
		void InitialiseTechniques(IRenderDevice* p_pRenderDevice);
		void InitialiseShaderPrograms(IRenderDevice* p_pRenderDevice);

		void Initialise(IRenderDevice* p_pRenderDevice);
		void Dispose(IRenderDevice* p_pRenderDevice);
	
		void Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity);
		void Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet);

	protected:
		Meson::Common::Text::String		m_strCubeMapFilename,
										m_strCubeMapName;

		TexturePtr				m_pCubeMap;
		TechniquePtr			m_pSkyVolumeTechnique;

		DepthRenderStatePtr		m_pDepthState;
		AlphaRenderStatePtr		m_pAlphaState;
		SamplerRenderStatePtr	m_pSamplerState;
		SamplerStatePtr		    m_pSampler;

		IShaderProgram *m_pDrawVS, 
					   *m_pDrawPS;
	};

Meson_Vistas_END