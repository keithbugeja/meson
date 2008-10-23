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
	class ReflectionEffect : public IEffect
	{
		VISTAS_IMPLEMENT_RTTI(ReflectionEffect, IEffect);

	public:
		ReflectionEffect(const Meson::Common::Text::String& p_strCubeMapName, const Meson::Common::Text::String& p_strCubeMapFilename, float p_fReflectivity);
		ReflectionEffect(TexturePtr p_pCubeMap, float p_fReflectivity);
		~ReflectionEffect(void);
		
		float GetReflectivity(void) const;
		void  SetReflectivity(float p_fReflectivity);

		void InitialiseTextures(IRenderDevice* p_pRenderDevice);
		void InitialiseTechniques(IRenderDevice* p_pRenderDevice);
		void InitialiseShaderPrograms(IRenderDevice* p_pRenderDevice);

		void Initialise(IRenderDevice* p_pRenderDevice);
		void Dispose(IRenderDevice* p_pRenderDevice);
	
		void Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity);
		void Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet);

	protected:
		float m_fReflectivity;

		Meson::Common::Text::String		m_strCubeMapFilename,
										m_strCubeMapName;

		TexturePtr				m_pCubeMap;
		TechniquePtr			m_pEnvironmentMappingTechnique;

		AlphaRenderStatePtr		m_pAlphaState;
		SamplerRenderStatePtr	m_pSamplerState;
		SamplerStatePtr		    m_pSampler;

		IShaderProgram *m_pDrawVS, 
					   *m_pDrawPS;

		ShaderProgramVariable m_reflectivity;
	};

Meson_Vistas_END