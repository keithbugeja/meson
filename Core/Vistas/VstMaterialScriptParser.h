//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstParser.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class MaterialScriptParser : public IParser
	{
	public:
		MaterialScriptParser(Meson::Common::IO::IInputStream* p_pInputStream);
		~MaterialScriptParser(void);

		void Initialise(void);
		void Shutdown(void);

		Token ReadToken(void);

		void Parse(void);

		bool ParseShaderProgramBlock();
		bool ParseTextureUnitBlock();
		bool ParseSamplerUnitBlock();
		bool ParseMaterialBlock();
		
		TechniquePtr ParseTechniqueBlock();
		
		PassPtr ParsePassBlock();
		PassPtr ParseAmbientPassBlock();
		PassPtr ParseIlluminationPassBlock();
		PassPtr ParseEffectPassBlock();

		AlphaRenderStatePtr ParseAlphaStateBlock();
		BiasRenderStatePtr ParseBiasStateBlock();
		CullRenderStatePtr ParseCullStateBlock();
		DepthRenderStatePtr ParseDepthStateBlock();
		LightRenderStatePtr ParseLightStateBlock();
		MaterialRenderStatePtr ParseMaterialStateBlock();
		SamplerRenderStatePtr ParseSamplerStateBlock();
		StencilRenderStatePtr ParseStencilStateBlock();
		TargetRenderStatePtr ParseTargetStateBlock();

	protected:
		bool IsNextSymbol(char p_cSymbol);

		bool NextLiteral(Meson::Common::Text::String& p_strLiteral);
		bool NextKeyword(dword& p_dwKeywordID);
		bool NextNumeric(double& p_dfValue);
		bool NextNumeric(int& p_nValue);
		bool NextSymbol(char p_cSymbol);

		void ReadComment(void);
		Meson::Common::Text::String ReadString(char cInitialSymbol);
		Token ReadLiteral(void);
		Token ReadNumber(char cInitialSymbol);
		Token ReadKeyword(char cInitialSymbol);

	protected:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, RenderConstantType::RenderConstantType> m_renderConstantMap;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, IShaderProgram*> m_shaderProgramMap;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, TextureStatePtr> m_textureStateMap;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, SamplerStatePtr> m_samplerStateMap;

		static const dword  KWID_SHADER_PROGRAM			= 0x0000,
        					KWID_CATALOGUE				= 0x0001,
        					KWID_NAME					= 0x0002,
        					KWID_TYPE					= 0x0003,
        					KWID_SOURCE					= 0x0004,
        					KWID_LANGUAGE				= 0x0005,
        					KWID_PARAM_AUTO				= 0x0006,
        					KWID_PARAM_USER				= 0x0007,
        					KWID_TEXTURE_UNIT			= 0x0008,
        					KWID_SAMPLER_UNIT			= 0x0009,
        					KWID_U_MODE					= 0x000A,
        					KWID_V_MODE					= 0x000B,
        					KWID_W_MODE					= 0x000C,
        					//KWID_FILTER					= 0x000D,
        					KWID_BORDER_COLOUR			= 0x000E,
        					KWID_MATERIAL				= 0x000F,
        					KWID_TECHNIQUE				= 0x0010,
        					KWID_PASS					= 0x0011,
        					KWID_AMBIENT_TYPE			= 0x0012,
        					KWID_ILLUMINATION_TYPE		= 0x0013,
        					KWID_EFFECT_TYPE			= 0x0014,
        					KWID_SIMULTANEOUS_LIGHTS	= 0x0015,
        					KWID_LIGHT_NAME_BINDING		= 0x0016,
        					KWID_LIGHT_TYPE_BINDING		= 0x0017,
        					KWID_ITERATION_COUNT		= 0x0018,
        					KWID_ALPHA_STATE			= 0x0019,
        					KWID_BIAS_STATE				= 0x001A,
        					KWID_CULL_STATE				= 0x001B,
        					KWID_DEPTH_STATE			= 0x001C,
        					KWID_LIGHT_STATE			= 0x001D,
        					KWID_MATERIAL_STATE			= 0x001E,
        					KWID_SAMPLER_STATE			= 0x001F,
        					KWID_STENCIL_STATE			= 0x0020,
        					KWID_TARGET_STATE			= 0x0021,
							KWID_BLEND					= 0x0022,
							KWID_OPERATION				= 0x0023,
							KWID_TEST					= 0x0024,
							KWID_COMPARE				= 0x0025,
							KWID_REFERENCE				= 0x0026,
							KWID_DEPTH_BIAS				= 0x0027,
							KWID_SCALE_BIAS				= 0x0028,
							KWID_CULL					= 0x0029,
							KWID_FACE					= 0x002A,
							KWID_WINDING				= 0x002B,
							KWID_WRITE					= 0x002C,
							KWID_LIGHT					= 0x002D,
							KWID_AMBIENT				= 0x002E,
							KWID_DIFFUSE				= 0x002F,
							KWID_EMISSIVE				= 0x0030,
							KWID_SPECULAR				= 0x0031,
							KWID_SHININESS				= 0x0032,
							KWID_ALPHA					= 0x0033,
							KWID_CHANNEL				= 0x0034,
							KWID_WRITE_MASK				= 0x0035,
							KWID_MASK					= 0x0036,
							KWID_MIPMAP_FILTER			= 0x0037,
							KWID_MAG_FILTER				= 0x0038,
							KWID_MIN_FILTER				= 0x0039;
	};
	//----------------------------------------------------------------------------------------------
Meson_Vistas_END