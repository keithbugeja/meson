//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstEffect.h"
#include "VstTechnique.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class VISTAS_API RefractiveEffect : public IEffect
	{
		VISTAS_IMPLEMENT_RTTI(RefractiveEffect, IEffect);

	public:
		RefractiveEffect(const Meson::Common::Text::String& p_strCubeMapName, const Meson::Common::Text::String& p_strCubeMapFilename, float p_fRefactiveIndex);
		RefractiveEffect(TexturePtr p_pCubeMap, float p_fRefractiveIndex);
		~RefractiveEffect(void);
		
		float GetRefractiveIndex(void) const;
		void  SetRefractiveIndex(float p_fRefractiveIndex);

		void InitialiseTextures(IRenderDevice* p_pRenderDevice);
		void InitialiseTechniques(IRenderDevice* p_pRenderDevice);
		void InitialiseShaderPrograms(IRenderDevice* p_pRenderDevice);

		void Initialise(IRenderDevice* p_pRenderDevice);
		void Dispose(IRenderDevice* p_pRenderDevice);
	
		void Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity);
		void Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet);

	protected:
		float m_fRefractiveIndex;

		Meson::Common::Text::String		m_strCubeMapFilename,
										m_strCubeMapName;

		TexturePtr				m_pCubeMap;
		TechniquePtr			m_pEnvironmentMappingTechnique;

		AlphaRenderStatePtr		m_pAlphaState;
		SamplerRenderStatePtr	m_pSamplerState;
		SamplerStatePtr		    m_pSampler;

		IShaderProgram *m_pDrawVS, 
					   *m_pDrawPS;

		ShaderProgramVariable m_refractiveIndex;
	};

Meson_Vistas_END