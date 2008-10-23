#include "Converter.h"

#include "VstVistasEngine.h"
#include "VstRenderDevice.h"
#include "VstMaterialScriptParser.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
MaterialScriptParser::MaterialScriptParser(IInputStream* p_pInputStream)
	: IParser(p_pInputStream)
{
}
//----------------------------------------------------------------------------------------------
MaterialScriptParser::~MaterialScriptParser(void)
{
}
//----------------------------------------------------------------------------------------------
void MaterialScriptParser::Initialise(void)
{
	// Keywords
	ILexer::AddKeyword("shader_program", KWID_SHADER_PROGRAM);
	ILexer::AddKeyword("catalogue", KWID_CATALOGUE);
	ILexer::AddKeyword("name", KWID_NAME);
	ILexer::AddKeyword("source", KWID_SOURCE);
	ILexer::AddKeyword("language", KWID_LANGUAGE);
	ILexer::AddKeyword("type", KWID_TYPE);
	ILexer::AddKeyword("param_auto", KWID_PARAM_AUTO);
	ILexer::AddKeyword("param_user", KWID_PARAM_USER);
	ILexer::AddKeyword("texture_unit", KWID_TEXTURE_UNIT);
	ILexer::AddKeyword("sampler_unit", KWID_SAMPLER_UNIT);
	ILexer::AddKeyword("u_mode", KWID_U_MODE);
	ILexer::AddKeyword("v_mode", KWID_V_MODE);
	ILexer::AddKeyword("w_mode", KWID_W_MODE);
	ILexer::AddKeyword("mipmap_filter", KWID_MIPMAP_FILTER);
	ILexer::AddKeyword("mag_filter", KWID_MAG_FILTER);
	ILexer::AddKeyword("min_filter", KWID_MIN_FILTER);
	ILexer::AddKeyword("border_colour", KWID_BORDER_COLOUR);
	ILexer::AddKeyword("material", KWID_MATERIAL);
	ILexer::AddKeyword("technique", KWID_TECHNIQUE);
	ILexer::AddKeyword("pass", KWID_PASS);
	ILexer::AddKeyword("illumination_type", KWID_ILLUMINATION_TYPE);
	ILexer::AddKeyword("simultaneous_lights", KWID_SIMULTANEOUS_LIGHTS);
	ILexer::AddKeyword("light_name_binding", KWID_LIGHT_NAME_BINDING);
	ILexer::AddKeyword("light_type_binding", KWID_LIGHT_TYPE_BINDING);
	ILexer::AddKeyword("effect_type", KWID_EFFECT_TYPE);
	ILexer::AddKeyword("iteration_count", KWID_ITERATION_COUNT);
	ILexer::AddKeyword("ambient_type", KWID_AMBIENT_TYPE);
	ILexer::AddKeyword("alpha_state", KWID_ALPHA_STATE);
	ILexer::AddKeyword("blend", KWID_BLEND);
	ILexer::AddKeyword("operation", KWID_OPERATION);
	ILexer::AddKeyword("test", KWID_TEST);
	ILexer::AddKeyword("compare", KWID_COMPARE);
	ILexer::AddKeyword("reference", KWID_REFERENCE);
	ILexer::AddKeyword("bias_state", KWID_BIAS_STATE);
	ILexer::AddKeyword("depth_bias", KWID_DEPTH_BIAS);
	ILexer::AddKeyword("scale_bias", KWID_SCALE_BIAS);
	ILexer::AddKeyword("cull_state", KWID_CULL_STATE);
	ILexer::AddKeyword("cull", KWID_CULL);
	ILexer::AddKeyword("face", KWID_FACE);
	ILexer::AddKeyword("winding", KWID_WINDING);
	ILexer::AddKeyword("depth_state", KWID_DEPTH_STATE);
	ILexer::AddKeyword("write", KWID_WRITE);
	ILexer::AddKeyword("light_state", KWID_LIGHT_STATE);
	ILexer::AddKeyword("light", KWID_LIGHT);
	ILexer::AddKeyword("ambient", KWID_AMBIENT);
	ILexer::AddKeyword("material_state", KWID_MATERIAL_STATE);
	ILexer::AddKeyword("diffuse", KWID_DIFFUSE);
	ILexer::AddKeyword("emissive", KWID_EMISSIVE);
	ILexer::AddKeyword("specular", KWID_SPECULAR);
	ILexer::AddKeyword("shininess", KWID_SHININESS);
	ILexer::AddKeyword("alpha", KWID_ALPHA);
	ILexer::AddKeyword("sampler_state", KWID_SAMPLER_STATE);  
	ILexer::AddKeyword("channel", KWID_CHANNEL);
	ILexer::AddKeyword("stencil_state", KWID_STENCIL_STATE);
	ILexer::AddKeyword("writemask", KWID_WRITE_MASK);
	ILexer::AddKeyword("mask", KWID_MASK);
	ILexer::AddKeyword("target_state", KWID_TARGET_STATE);
	
	// Symbols
	ILexer::AddSymbol('{');
	ILexer::AddSymbol('}');
	ILexer::AddSymbol('"');
	ILexer::AddSymbol('-');
	ILexer::AddSymbol('+');
	ILexer::AddSymbol('#');

	// Render constants
	m_renderConstantMap.Insert("matrix_world", RenderConstantType::RCT_MATRIX_WORLD); 
	m_renderConstantMap.Insert("matrix_transpose_world", RenderConstantType::RCT_MATRIX_TRANSPOSE_WORLD); 
	m_renderConstantMap.Insert("matrix_inverse_world", RenderConstantType::RCT_MATRIX_INVERSE_WORLD);  
	m_renderConstantMap.Insert("matrix_inverse_transpose_world", RenderConstantType::RCT_MATRIX_INVERSE_TRANSPOSE_WORLD);  
	m_renderConstantMap.Insert("matrix_view", RenderConstantType::RCT_MATRIX_VIEW);  
	m_renderConstantMap.Insert("matrix_transpose_view", RenderConstantType::RCT_MATRIX_TRANSPOSE_VIEW);  
	m_renderConstantMap.Insert("matrix_inverse_view", RenderConstantType::RCT_MATRIX_INVERSE_VIEW);  
	m_renderConstantMap.Insert("matrix_inverse_transpose_view", RenderConstantType::RCT_MATRIX_TRANSPOSE_VIEW); 
	m_renderConstantMap.Insert("matrix_projection", RenderConstantType::RCT_MATRIX_PROJECTION);  
	m_renderConstantMap.Insert("matrix_transpose_projection", RenderConstantType::RCT_MATRIX_TRANSPOSE_PROJECTION);  
	m_renderConstantMap.Insert("matrix_inverse_projection", RenderConstantType::RCT_MATRIX_INVERSE_PROJECTION);  
	m_renderConstantMap.Insert("matrix_inverse_transpose_projection", RenderConstantType::RCT_MATRIX_INVERSE_TRANSPOSE_PROJECTION); 
	m_renderConstantMap.Insert("matrix_view_projection", RenderConstantType::RCT_MATRIX_VIEW_PROJECTION);  
	m_renderConstantMap.Insert("matrix_transpose_view_projection", RenderConstantType::RCT_MATRIX_TRANSPOSE_VIEW_PROJECTION);  
	m_renderConstantMap.Insert("matrix_inverse_view_projection", RenderConstantType::RCT_MATRIX_INVERSE_VIEW_PROJECTION);  
	m_renderConstantMap.Insert("matrix_inverse_transpose_view_projection", RenderConstantType::RCT_MATRIX_INVERSE_TRANSPOSE_VIEW_PROJECTION); 
	m_renderConstantMap.Insert("matrix_world_view_projection", RenderConstantType::RCT_MATRIX_WORLD_VIEW_PROJECTION);  
	m_renderConstantMap.Insert("matrix_transpose_world_view_projection", RenderConstantType::RCT_MATRIX_TRANSPOSE_WORLD_VIEW_PROJECTION);  
	m_renderConstantMap.Insert("matrix_inverse_world_view_projection", RenderConstantType::RCT_MATRIX_INVERSE_WORLD_VIEW_PROJECTION);  
	m_renderConstantMap.Insert("matrix_inverse_transpose_world_view_projection", RenderConstantType::RCT_MATRIX_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION); 
	m_renderConstantMap.Insert("vector_material_ambient", RenderConstantType::RCT_VECTOR_MATERIAL_AMBIENT);  
	m_renderConstantMap.Insert("vector_material_diffuse", RenderConstantType::RCT_VECTOR_MATERIAL_DIFFUSE);  
	m_renderConstantMap.Insert("vector_material_specular", RenderConstantType::RCT_VECTOR_MATERIAL_SPECULAR);  
	m_renderConstantMap.Insert("vector_material_emissive", RenderConstantType::RCT_VECTOR_MATERIAL_EMISSIVE); 
	m_renderConstantMap.Insert("vector_light_position", RenderConstantType::RCT_VECTOR_LIGHT_POSITION);  
	m_renderConstantMap.Insert("vector_light_direction", RenderConstantType::RCT_VECTOR_LIGHT_DIRECTION);  
	m_renderConstantMap.Insert("vector_light_ambient", RenderConstantType::RCT_VECTOR_LIGHT_AMBIENT);  
	m_renderConstantMap.Insert("vector_light_diffuse", RenderConstantType::RCT_VECTOR_LIGHT_DIFFUSE);  
	m_renderConstantMap.Insert("vector_light_specular", RenderConstantType::RCT_VECTOR_LIGHT_SPECULAR);  
	m_renderConstantMap.Insert("vector_light_cone", RenderConstantType::RCT_VECTOR_LIGHT_CONE);  
	m_renderConstantMap.Insert("vector_light_attenuation", RenderConstantType::RCT_VECTOR_LIGHT_ATTENUATION); 
	m_renderConstantMap.Insert("vector_array_light_position", RenderConstantType::RCT_VECTOR_ARRAY_LIGHT_POSITION); 
	m_renderConstantMap.Insert("vector_array_light_direction", RenderConstantType::RCT_VECTOR_ARRAY_LIGHT_DIRECTION);  
	m_renderConstantMap.Insert("vector_array_light_ambient", RenderConstantType::RCT_VECTOR_ARRAY_LIGHT_AMBIENT);  
	m_renderConstantMap.Insert("vector_array_light_diffuse", RenderConstantType::RCT_VECTOR_ARRAY_LIGHT_DIFFUSE);  
	m_renderConstantMap.Insert("vector_array_light_specular", RenderConstantType::RCT_VECTOR_ARRAY_LIGHT_SPECULAR); 
	m_renderConstantMap.Insert("vector_array_light_cone", RenderConstantType::RCT_VECTOR_ARRAY_LIGHT_CONE);  
	m_renderConstantMap.Insert("vector_array_light_attenuation", RenderConstantType::RCT_VECTOR_ARRAY_LIGHT_ATTENUATION);  
	m_renderConstantMap.Insert("integer_light_active_count", RenderConstantType::RCT_INTEGER_LIGHT_ACTIVE_COUNT); 
	m_renderConstantMap.Insert("vector_viewer_position", RenderConstantType::RCT_VECTOR_VIEWER_POSITION);  
	m_renderConstantMap.Insert("vector_viewer_direction", RenderConstantType::RCT_VECTOR_VIEWER_DIRECTION); 
	m_renderConstantMap.Insert("vector_global_ambient", RenderConstantType::RCT_VECTOR_AMBIENT); 
}
//----------------------------------------------------------------------------------------------
void MaterialScriptParser::Shutdown(void)
{
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::ParseShaderProgramBlock(void)
{
	// Initialise render constant array
	Meson::Common::Collections::TTreeSet<RenderConstantType::RenderConstantType> renderConstantArray;
	Meson::Common::Collections::TTreeSet<String> userParamArray;

	// Initialise shader program parameters to default values
	String	strShaderProgramAlias,
			strName = "default_program_name",
			strCatalogue = "default_catalogue",
			strLanguage = "hlsl",
			strType = "pixel",
			strSource = "default_program_name.hlsl";

	bool bIsNameSupplied = false,
		 bIsCatalogueSupplied = false,
		 bIsLanguageSupplied = false,
		 bIsTypeSupplied = false,
		 bIsSourceSupplied = false;

	dword dwKeywordID;

	// Read program alias
	if (!NextLiteral(strShaderProgramAlias))
		return false;

	// If next character is not a left brace, exit
	if (!IsNextSymbol('{'))
		return false;
	
	// Parse block
	while(m_tokenStream.HasMoreTokens())
	{
		// Close block parsing
		if (IsNextSymbol('}'))
		{
			// Get active render device and set appropriate shader factory
			IRenderDevice* pRenderDevice = VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();
			ShaderProgramManager* pShaderProgramManager = pRenderDevice->GetShaderProgramManager();
			
			// If language is supplied, switch shader program factory to specified
			if (bIsLanguageSupplied)
				pShaderProgramManager->SetShaderProgramFactory(strLanguage);

			// Determine shader program type (whether vertex or pixel) and set respective entry points
			IShaderProgram::ShaderProgramType eProgramType;
			Meson::Common::Text::String strEntryPoint;
				
			if (!strType.Compare("vertex"))
			{
				eProgramType = IShaderProgram::SPT_VERTEX;
				strEntryPoint = "vs_main";
			}
			else if (!strType.Compare("pixel"))
			{
				eProgramType = IShaderProgram::SPT_PIXEL;
				strEntryPoint = "ps_main";
			}
			else
				return false;

			// Create new shader program
			IShaderProgram* pShaderProgram = NULL;
			
			try 
			{
				// Use factory to get new shader program
				pShaderProgram = pShaderProgramManager->CreateShaderProgramFromFile(strSource, eProgramType, strCatalogue, strName);
				
				// Set shader program profile
				pShaderProgram->SetProfile(pShaderProgramManager->GetShaderProfile(eProgramType));
				
				// Set program entry point
				pShaderProgram->SetEntryPoint(strEntryPoint);
				
				// Compile program
				pShaderProgram->Compile();

				// Attach render constants to program
				Meson::Common::Collections::TEnumerator<RenderConstantType::RenderConstantType>& renderConstantEnumerator = 
					renderConstantArray.GetEnumerator();

				while(renderConstantEnumerator.HasMoreElements())
					pShaderProgram->AddRenderConstant(renderConstantEnumerator.NextElement());

				Meson::Common::Collections::TEnumerator<String>& userParamEnumerator = 
					userParamArray.GetEnumerator();

				while(userParamEnumerator.HasMoreElements())
				{
					ShaderProgramVariable* pShaderProgramVariable = new ShaderProgramVariable(userParamEnumerator.NextElement());
					pShaderProgram->AddVariable(pShaderProgramVariable);
				}
			} 
			catch (int) 
			{ 
				// If program creation, setup and compilation failed, delete program
				if (pShaderProgram != NULL) 
					pShaderProgramManager->DestroyShaderProgram(strCatalogue, strName);
				
				return false;
			};

			// Insert shader program reference into script map
			m_shaderProgramMap.Insert(strShaderProgramAlias, pShaderProgram);

			return true;
		}

		// Parse next keyword
		if (NextKeyword(dwKeywordID))
		{
			// Use token code to select keyword action
			switch(dwKeywordID)
			{
				// Shader program name
				case KWID_NAME:
				{	
					bIsNameSupplied = NextLiteral(strName);
					break;
				}

				// Shader program catalogue
				case KWID_CATALOGUE:
				{
					bIsCatalogueSupplied = NextLiteral(strCatalogue);
					break;
				}

				// Shader program type
				case KWID_TYPE:
				{
					bIsTypeSupplied = NextLiteral(strType);
					break;
				}
				
				// Shader program source
				case KWID_SOURCE:
				{
					bIsSourceSupplied = NextLiteral(strSource);
					break;
				}

				// Shader program language
				case KWID_LANGUAGE:
				{
					bIsLanguageSupplied = NextLiteral(strLanguage);
					break;
				}

				// Shader program render constants
				case KWID_PARAM_AUTO:
				{
					String strRenderConstant;

					if (NextLiteral(strRenderConstant))
					{
						RenderConstantType::RenderConstantType eRenderConstantType;

						if (m_renderConstantMap.ContainsKey(strRenderConstant, eRenderConstantType))
							renderConstantArray.Insert(eRenderConstantType);
					}
					break;
				}

				// Shader program variables
				case KWID_PARAM_USER:
				{
					String strParamName;

					if (NextLiteral(strParamName))
						userParamArray.Insert(strParamName);

					break;
				}
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::ParseTextureUnitBlock(void)
{
	String strTextureUnitAlias,
		   strName = "default_texture_name",
		   strCatalogue = "default_catalogue",
		   strSource = "default_texture_name",
		   strType = "2D";

	bool bIsNameSupplied = false,
		 bIsCatalogueSupplied = false,
		 bIsTypeSupplied = false,
		 bIsSourceSupplied = false;

	dword dwKeywordID;

	if (!NextLiteral(strTextureUnitAlias))
		return false;

	if (!IsNextSymbol('{'))
		return false;

	// Read commands within brace block
	while(m_tokenStream.HasMoreTokens())
	{
		// End of block
		if (IsNextSymbol('}'))
		{
			TextureStatePtr pTextureState(NULL);

			// Do we load texture from file, or is it just a named reference?
			if (bIsSourceSupplied)
			{
				// If name is not supplied, generate a unique tag
				if (!bIsNameSupplied) 
					strName = VistasEngine::GetInstance()->GenerateGuid();

				// If type is supplied, evaluate
				TextureBuffer::TextureType eType = TextureBuffer::TT_2D;

				if (bIsTypeSupplied)
				{
					if (!strType.Compare("1D"))
						eType = TextureBuffer::TT_1D;
					else if (!strType.Compare("2D"))
						eType = TextureBuffer::TT_2D;
					else if (!strType.Compare("3D"))
						eType = TextureBuffer::TT_3D;
					else if (!strType.Compare("cube"))
						eType = TextureBuffer::TT_CUBEMAP;
				}

				// Load texture from file
				TexturePtr pTexture = VistasEngine::GetInstance()->GetTextureManager()->CreateTextureFromFile(strName, strSource, eType);

				// Create new texture state (texture unit)
				pTextureState = new TextureState(pTexture);
			}
			else
			{
				// Named references should always be valid.
				if (!bIsNameSupplied) 
					return false;

				// Create a named-reference texture unit
				pTextureState = new TextureState(strCatalogue, strName);
			}

			// Insert texture unit into map
			m_textureStateMap.Insert(strTextureUnitAlias, pTextureState);

			return true;
		}

		if (NextKeyword(dwKeywordID))
		{
			// Use token code to select keyword action
			switch(dwKeywordID)
			{
				// Shader program name
				case KWID_NAME:
				{	
					bIsNameSupplied = NextLiteral(strName);
					break;
				}

				// Texture type
				case KWID_TYPE:
				{
					bIsTypeSupplied = NextLiteral(strType);
					break;
				}

				// Shader program catalogue
				case KWID_CATALOGUE:
				{
					bIsCatalogueSupplied = NextLiteral(strCatalogue);
					break;
				}
				
				// Shader program source
				case KWID_SOURCE:
				{
					bIsSourceSupplied = NextLiteral(strSource);
					break;
				}
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::ParseSamplerUnitBlock(void)
{
	String strSamplerUnitAlias;
	dword dwKeywordID;
	
	if (!NextLiteral(strSamplerUnitAlias))
		return false;

	if (!IsNextSymbol('{'))
		return false;

	// Create new sampler state
	SamplerStatePtr pSamplerState(new SamplerState());

	// Read commands within brace block
	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
		{
			m_samplerStateMap.Insert(strSamplerUnitAlias, pSamplerState);
			return true;
		}

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				// Sampler type
				case KWID_TYPE:
				{
					String strType;

					if (NextLiteral(strType))
					{
						strType.ToLowerCase();

						if (!strType.Compare("1D"))
							pSamplerState->SetSamplerType(SamplerState::ST_SAMPLER_1D);
						else if (!strType.Compare("2D"))
							pSamplerState->SetSamplerType(SamplerState::ST_SAMPLER_2D);
						else if (!strType.Compare("3D"))
							pSamplerState->SetSamplerType(SamplerState::ST_SAMPLER_3D);
						else if (!strType.Compare("cube"))
							pSamplerState->SetSamplerType(SamplerState::ST_SAMPLER_CUBE);
						else if (!strType.Compare("projection"))
							pSamplerState->SetSamplerType(SamplerState::ST_SAMPLER_PROJ);
					}

					break;
				}

				// Sampler wrapping
				case KWID_U_MODE:
				case KWID_V_MODE:
				case KWID_W_MODE:
				{	
					String strType;

					if (NextLiteral(strType))
					{
						SamplerState::WrapType eWrapType;
	
						strType.ToLowerCase();

						if (!strType.Compare("wrap"))
							eWrapType = SamplerState::WT_WRAP;
						else if (!strType.Compare("mirror"))
							eWrapType = SamplerState::WT_MIRROR;
						else if (!strType.Compare("clamp"))
							eWrapType = SamplerState::WT_CLAMP;
						else if (!strType.Compare("border"))
							eWrapType = SamplerState::WT_BORDER;
						else if (!strType.Compare("mirroronce"))
							eWrapType = SamplerState::WT_MIRRORONCE;

						switch(dwKeywordID)
						{
							case KWID_U_MODE:
								pSamplerState->SetUWrapType(eWrapType);
								break;
							case KWID_V_MODE:
								pSamplerState->SetVWrapType(eWrapType);
								break;
							case KWID_W_MODE:
								pSamplerState->SetWWrapType(eWrapType);
								break;
						}
					}

					break;
				}
				
				// Sampler mipmap/minification/magnification filter
				case KWID_MIPMAP_FILTER:
				case KWID_MAG_FILTER:
				case KWID_MIN_FILTER:
				{	
					String strType;
					
					if (NextLiteral(strType))
					{
						SamplerState::FilterType eFilterType;

						strType.ToLowerCase();

						if (!strType.Compare("none"))
							eFilterType = SamplerState::FT_NONE;
						else if (!strType.Compare("point"))
							eFilterType = SamplerState::FT_POINT;
						else if (!strType.Compare("linear"))
							eFilterType = SamplerState::FT_LINEAR;
						else if (!strType.Compare("anisotropic"))
							eFilterType = SamplerState::FT_ANISOTROPIC;

						switch(dwKeywordID)
						{
							case KWID_MIPMAP_FILTER:
								pSamplerState->SetMipmapFilter(eFilterType);
								break;
							case KWID_MAG_FILTER:
								pSamplerState->SetMagnificationFilter(eFilterType);
								break;
							case KWID_MIN_FILTER:
								pSamplerState->SetMinificationFilter(eFilterType);
								break;
						}
					}

					break;
				}

				// Sampler border colour
				case KWID_BORDER_COLOUR:
				{
					double dfRed, dfGreen, dfBlue;

					if (NextNumeric(dfRed) && NextNumeric(dfGreen) && NextNumeric(dfBlue))
						pSamplerState->SetBorderColour(Colour((float)dfRed, (float)dfGreen, (float)dfBlue, 1.0f));

					break;
				}
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::ParseMaterialBlock(void)
{
	if (!IsNextSymbol('{')) 
		return false;

	String strName, 
		   strCatalogue;
	
	bool bIsNameSupplied = false,
		 bIsCatalogueSupplied = false;

	dword dwKeywordID;

	TechniqueGroupPtr pTechniqueGroup(new TechniqueGroup());

	// Parse block 
	while (m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
		{
			if (bIsNameSupplied)
			{
				MaterialManager* pMaterialManager = VistasEngine::GetInstance()->GetMaterialManager();
				
				if (pMaterialManager->ContainsMaterial(strName))
					return false;

				MaterialPtr pMaterial = pMaterialManager->CreateMaterial(strName);
				*(TechniqueGroup*)pMaterial = *pTechniqueGroup;

				return true;
			}

			return false;
		}

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_NAME:
				{	
					bIsNameSupplied = NextLiteral(strName);
					break;
				}

				case KWID_CATALOGUE:
				{
					bIsCatalogueSupplied = NextLiteral(strCatalogue);
					break;
				}

				case KWID_TECHNIQUE:
				{
					TechniquePtr pTechnique = ParseTechniqueBlock();
					if (pTechnique != NULL) pTechniqueGroup->AddTechnique(pTechnique);
					break;
				}
			}
		}
	}
	
	return false;
}
//----------------------------------------------------------------------------------------------
TechniquePtr MaterialScriptParser::ParseTechniqueBlock(void)
{
	if (!IsNextSymbol('{'))
		return TechniquePtr(NULL);

	TechniquePtr pTechnique(new Technique());
	dword dwKeywordID;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
		{
			pTechnique->RebuildTechnique();
			return pTechnique;
		}

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_PASS:
				{
					PassPtr pPass = ParsePassBlock();
					pTechnique->AddPass(pPass);
					break;
				}
			}
		}
	}
	
	return TechniquePtr(NULL);
}
//----------------------------------------------------------------------------------------------
PassPtr MaterialScriptParser::ParsePassBlock(void)
{
	PassPtr pPass(NULL);
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pPass;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pPass;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_AMBIENT_TYPE:
				{
					if (pPass == NULL) 
						pPass = ParseAmbientPassBlock();

					break;
				}
				
				case KWID_ILLUMINATION_TYPE:
				{
					if (pPass == NULL) 
						pPass = ParseIlluminationPassBlock();

					break;
				}

				case KWID_EFFECT_TYPE:
				{
					if (pPass == NULL)
						pPass = ParseEffectPassBlock();

					break;
				}

				case KWID_ALPHA_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseAlphaStateBlock());

					break;
				}

				case KWID_BIAS_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseBiasStateBlock());

					break;
				}

				case KWID_CULL_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseCullStateBlock());

					break;
				}

				case KWID_DEPTH_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseDepthStateBlock());

					break;
				}
				
				case KWID_LIGHT_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseLightStateBlock());

					break;
				}

				case KWID_MATERIAL_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseMaterialStateBlock());

					break;
				}

				case KWID_SAMPLER_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseSamplerStateBlock());

					break;
				}

				case KWID_STENCIL_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseStencilStateBlock());

					break;
				}

				case KWID_TARGET_STATE:
				{
					if (pPass != NULL)
						pPass->AddRenderState((IRenderState*)ParseTargetStateBlock());

					break;
				}

				case KWID_SHADER_PROGRAM:
				{
					if (pPass != NULL)
					{
						String strProgramAlias;
						IShaderProgram* pShaderProgram;

						if (NextLiteral(strProgramAlias))
						{
							if (m_shaderProgramMap.ContainsKey(strProgramAlias, pShaderProgram))
								pPass->SetShaderProgram(pShaderProgram->GetType(), pShaderProgram);
						}
					}

					break;
				}
			}
		}
	}
	
	return PassPtr(NULL);
}
//----------------------------------------------------------------------------------------------
PassPtr MaterialScriptParser::ParseAmbientPassBlock(void)
{
	return PassPtr(new AmbientPass());
}
//----------------------------------------------------------------------------------------------
PassPtr MaterialScriptParser::ParseIlluminationPassBlock(void)
{
	IlluminationPass* pIlluminationPass = new IlluminationPass();
	PassPtr pPass(pIlluminationPass);
	
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pPass;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pPass;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_LIGHT_NAME_BINDING:
				{
					String strNameBinding;

					if (NextLiteral(strNameBinding))
						pIlluminationPass->BindToNamedLight(strNameBinding);

					break;
				}
				
				case KWID_LIGHT_TYPE_BINDING:
				{
					String strTypeBinding;

					if (NextLiteral(strTypeBinding))
					{
						strTypeBinding.ToLowerCase();

						if (!strTypeBinding.Compare("spot"))
							pIlluminationPass->BindToLightType(Light::LT_SPOT);
						else if (!strTypeBinding.Compare("point"))
							pIlluminationPass->BindToLightType(Light::LT_POINT);
						else if (!strTypeBinding.Compare("directional"))
							pIlluminationPass->BindToLightType(Light::LT_DIRECTIONAL);
					}

					break;
				}

				case KWID_SIMULTANEOUS_LIGHTS:
				{
					int nSimultaneousLights;

					if (NextNumeric(nSimultaneousLights))
					{
						if (nSimultaneousLights < 0) 
							nSimultaneousLights = 1;

						pIlluminationPass->SetSimultaneousLightsProcessed(nSimultaneousLights);
					}

					break;
				}
			}
		}
	}

	return PassPtr(NULL);
}
//----------------------------------------------------------------------------------------------
PassPtr MaterialScriptParser::ParseEffectPassBlock(void)
{
	EffectPass* pEffectPass = new EffectPass();
	PassPtr pPass(pEffectPass);
	
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pPass;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pPass;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_ITERATION_COUNT:
				{
					int nCount;

					if (NextNumeric(nCount))
					{
						if (nCount < 0) nCount = 1;
						pEffectPass->SetIterationCount(nCount);
					}

					break;
				}
			}
		}
	}

	return PassPtr(NULL);
}
//----------------------------------------------------------------------------------------------
AlphaRenderStatePtr MaterialScriptParser::ParseAlphaStateBlock()
{
	AlphaRenderStatePtr pAlphaState(new AlphaRenderState());
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pAlphaState;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pAlphaState;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_BLEND:
				{
					String strBlend;

					if (NextLiteral(strBlend))
					{
						strBlend.ToLowerCase();

						if (!strBlend.Compare("on"))
							pAlphaState->SetAlphaBlend(true);
						else if (!strBlend.Compare("off"))
							pAlphaState->SetAlphaBlend(false);
					}

					break;
				}

				case KWID_OPERATION:
				{
					String strOperand, 
						   strOperator;

					if (NextLiteral(strOperand) && NextLiteral(strOperator))
					{
						AlphaRenderState::AlphaBlend eBlendType;

						strOperand.ToLowerCase();
						strOperator.ToLowerCase();

						if (strOperator.Compare("zero") == 0)
							eBlendType = AlphaRenderState::ARSB_ZERO;
						else if (strOperator.Compare("one") == 0)
							eBlendType = AlphaRenderState::ARSB_ONE;
						else if (strOperator.Compare("source_colour") == 0)
							eBlendType = AlphaRenderState::ARSB_SRC_COLOUR;
						else if (strOperator.Compare("one_minus_source_colour") == 0)
							eBlendType = AlphaRenderState::ARSB_ONE_MINUS_SRC_COLOUR;
						else if (strOperator.Compare("source_alpha") == 0)
							eBlendType = AlphaRenderState::ARSB_SRC_ALPHA;
						else if (strOperator.Compare("one_minus_source_alpha") == 0)
							eBlendType = AlphaRenderState::ARSB_ONE_MINUS_SRC_ALPHA;
						else if (strOperator.Compare("destination_colour") == 0)
							eBlendType = AlphaRenderState::ARSB_DST_COLOUR;
						else if (strOperator.Compare("one_minus_destination_colour") == 0)
							eBlendType = AlphaRenderState::ARSB_ONE_MINUS_DST_COLOUR;
						else if (strOperator.Compare("destination_alpha") == 0)
							eBlendType = AlphaRenderState::ARSB_DST_ALPHA;
						else if (strOperator.Compare("one_minus_destination_alpha") == 0)
							eBlendType = AlphaRenderState::ARSB_ONE_MINUS_DST_ALPHA;
						else if (strOperator.Compare("constant_alpha_colour") == 0)
							eBlendType = AlphaRenderState::ARSB_CONSTANT_ALPHA_COLOUR;
						else if (strOperator.Compare("one_minus_constant_alpha_colour") == 0)
							eBlendType = AlphaRenderState::ARSB_ONE_MINUS_CONSTANT_ALPHA_COLOUR;
						else if (strOperator.Compare("constant_alpha") == 0)
							eBlendType = AlphaRenderState::ARSB_CONSTANT_ALPHA;
						else if (strOperator.Compare("one_minus_constant_alpha") == 0)
							eBlendType = AlphaRenderState::ARSB_ONE_MINUS_CONSTANT_ALPHA;
						else if (strOperator.Compare("constant_colour") == 0)
							eBlendType = AlphaRenderState::ARSB_CONSTANT_COLOUR;
						else if (strOperator.Compare("one_minus_constant_colour") == 0)
							eBlendType = AlphaRenderState::ARSB_ONE_MINUS_CONSTANT_COLOUR;

						if (strOperand.Compare("source") == 0)
							pAlphaState->SetSourceBlendMode(eBlendType);
						else if (strOperand.Compare("destination") == 0)
							pAlphaState->SetDestinationBlendMode(eBlendType);
					}

					break;
				}

				case KWID_TEST:
				{
					String strTest;

					if (NextLiteral(strTest))
					{
						strTest.ToLowerCase();

						if (strTest.Compare("on") == 0)
							pAlphaState->SetAlphaTest(true);
						else if (strTest.Compare("off") == 0)
							pAlphaState->SetAlphaTest(false);
					}

					break;
				}

				case KWID_COMPARE:
				{
					String strCompare;

					if (NextLiteral(strCompare))
					{
						strCompare.ToLowerCase();

						if (strCompare.Compare("always") == 0)
							pAlphaState->SetCompareType(AlphaRenderState::ARSCMP_ALWAYS);
						else if (strCompare.Compare("never") == 0)
							pAlphaState->SetCompareType(AlphaRenderState::ARSCMP_NEVER);
						else if (strCompare.Compare("less") == 0)
							pAlphaState->SetCompareType(AlphaRenderState::ARSCMP_LESS);
						else if (strCompare.Compare("lessequal") == 0)
							pAlphaState->SetCompareType(AlphaRenderState::ARSCMP_LESSEQUAL);
						else if (strCompare.Compare("equal") == 0)
							pAlphaState->SetCompareType(AlphaRenderState::ARSCMP_EQUAL);
						else if (strCompare.Compare("notequal") == 0)
							pAlphaState->SetCompareType(AlphaRenderState::ARSCMP_NOTEQUAL);
						else if (strCompare.Compare("greater") == 0)
							pAlphaState->SetCompareType(AlphaRenderState::ARSCMP_GREATER);
						else if (strCompare.Compare("greaterequal") == 0)
							pAlphaState->SetCompareType(AlphaRenderState::ARSCMP_GREATEREQUAL);
					}

					break;
				}

				case KWID_REFERENCE:
				{
					double dfReference;

					if (NextNumeric(dfReference))
						pAlphaState->SetAlphaReference((int)(255.0 * Mathf::Min(Mathf::Max((float)dfReference, 0), 1)));

					break;
				}
			}
		}
	}

	return pAlphaState;
}
//----------------------------------------------------------------------------------------------
BiasRenderStatePtr MaterialScriptParser::ParseBiasStateBlock()
{
	BiasRenderStatePtr pBiasState(new BiasRenderState());
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pBiasState;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pBiasState;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_DEPTH_BIAS:
				{
					double dfDepth;

					if (NextNumeric(dfDepth))
						pBiasState->SetDepthBias((float)dfDepth);

					break;
				}

				case KWID_SCALE_BIAS:
				{
					double dfScale;

					if (NextNumeric(dfScale))
						pBiasState->SetScaleBias((float)dfScale);

					break;
				}
			}
		}
	}

	return pBiasState;
}
//----------------------------------------------------------------------------------------------
CullRenderStatePtr MaterialScriptParser::ParseCullStateBlock()
{
	CullRenderStatePtr pCullState(new CullRenderState());
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pCullState;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pCullState;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_CULL:
				{
					String strCull;

					if (NextLiteral(strCull))
					{
						strCull.ToLowerCase();

						if (strCull.Compare("on") == 0)
							pCullState->SetCulling(true);
						else if (strCull.Compare("off") == 0)
							pCullState->SetCulling(false);
					}

					break;
				}

				case KWID_FACE:
				{
					String strFace;

					if (NextLiteral(strFace))
					{
						strFace.ToLowerCase();

						if (strFace.Compare("front") == 0)
							pCullState->SetFaceCullingMode(CullRenderState::CRSFC_FRONT);
						else if (strFace.Compare("back") == 0)
							pCullState->SetFaceCullingMode(CullRenderState::CRSFC_BACK);
					}

					break;
				}

				case KWID_WINDING:
				{
					String strWinding;

					if (NextLiteral(strWinding))
					{
						strWinding.ToLowerCase();

						if (strWinding.Compare("clockwise") == 0)
							pCullState->SetFaceWindingMode(CullRenderState::CRSFW_CW);
						else if (strWinding.Compare("counterclockwise") == 0)
							pCullState->SetFaceWindingMode(CullRenderState::CRSFW_CCW);
					}

					break;
				}
			}
		}
	}

	return pCullState;
}
//----------------------------------------------------------------------------------------------
DepthRenderStatePtr MaterialScriptParser::ParseDepthStateBlock()
{
	DepthRenderStatePtr pDepthState(new DepthRenderState());
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pDepthState;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pDepthState;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_TEST:
				{
					String strTest;

					if (NextLiteral(strTest))
					{
						strTest.ToLowerCase();

						if (strTest.Compare("on") == 0)
							pDepthState->SetDepthTest(true);
						else if (strTest.Compare("off") == 0)
							pDepthState->SetDepthTest(false);
					}

					break;
				}

				case KWID_WRITE:
				{
					String strWrite;

					if (NextLiteral(strWrite))
					{
						strWrite.ToLowerCase();

						if (strWrite.Compare("on") == 0)
							pDepthState->SetDepthWrite(true);
						else if (strWrite.Compare("off") == 0)
							pDepthState->SetDepthWrite(false);
					}

					break;
				}

				case KWID_COMPARE:
				{
					String strCompare;

					if (NextLiteral(strCompare))
					{
						strCompare.ToLowerCase();

						if (strCompare.Compare("always") == 0)
							pDepthState->SetCompareType(DepthRenderState::DRSCMP_ALWAYS);
						else if (strCompare.Compare("never") == 0)
							pDepthState->SetCompareType(DepthRenderState::DRSCMP_NEVER);
						else if (strCompare.Compare("less") == 0)
							pDepthState->SetCompareType(DepthRenderState::DRSCMP_LESS);
						else if (strCompare.Compare("lessequal") == 0)
							pDepthState->SetCompareType(DepthRenderState::DRSCMP_LESSEQUAL);
						else if (strCompare.Compare("equal") == 0)
							pDepthState->SetCompareType(DepthRenderState::DRSCMP_EQUAL);
						else if (strCompare.Compare("notequal") == 0)
							pDepthState->SetCompareType(DepthRenderState::DRSCMP_NOTEQUAL);
						else if (strCompare.Compare("greater") == 0)
							pDepthState->SetCompareType(DepthRenderState::DRSCMP_GREATER);
						else if (strCompare.Compare("greaterequal") == 0)
							pDepthState->SetCompareType(DepthRenderState::DRSCMP_GREATEREQUAL);
					}

					break;
				}
			}
		}
	}

	return pDepthState;
}
//----------------------------------------------------------------------------------------------
LightRenderStatePtr MaterialScriptParser::ParseLightStateBlock()
{
	LightRenderStatePtr pLightState(new LightRenderState());
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pLightState;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pLightState;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_LIGHT:
				{
					String strLight;

					if (NextLiteral(strLight))
					{
						strLight.ToLowerCase();

						if (strLight.Compare("on") == 0)
							pLightState->SetLighting(true);
						else if (strLight.Compare("off") == 0)
							pLightState->SetLighting(false);
					}

					break;
				}

				case KWID_AMBIENT:
				{
					double dfRed, dfGreen, dfBlue;

					if (NextNumeric(dfRed) && NextNumeric(dfGreen) && NextNumeric(dfBlue))
						pLightState->SetAmbient(Colour((float)dfRed, (float)dfGreen, (float)dfBlue, 1.0f));

					break;
				}
			}
		}
	}

	return pLightState;
}
//----------------------------------------------------------------------------------------------
MaterialRenderStatePtr MaterialScriptParser::ParseMaterialStateBlock()
{
	MaterialRenderStatePtr pMaterialState(new MaterialRenderState());
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pMaterialState;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pMaterialState;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_AMBIENT:
				{
					double dfRed, dfGreen, dfBlue;

					if (NextNumeric(dfRed) && NextNumeric(dfGreen) && NextNumeric(dfBlue))
						pMaterialState->SetAmbient(Colour((float)dfRed, (float)dfGreen, (float)dfBlue, 1.0f));

					break;
				}

				case KWID_DIFFUSE:
				{
					double dfRed, dfGreen, dfBlue;

					if (NextNumeric(dfRed) && NextNumeric(dfGreen) && NextNumeric(dfBlue))
						pMaterialState->SetDiffuse(Colour((float)dfRed, (float)dfGreen, (float)dfBlue, 1.0f));

					break;
				}

				case KWID_EMISSIVE:
				{
					double dfRed, dfGreen, dfBlue;

					if (NextNumeric(dfRed) && NextNumeric(dfGreen) && NextNumeric(dfBlue))
						pMaterialState->SetEmissive(Colour((float)dfRed, (float)dfGreen, (float)dfBlue, 1.0f));

					break;
				}

				case KWID_SPECULAR:
				{
					double dfRed, dfGreen, dfBlue;

					if (NextNumeric(dfRed) && NextNumeric(dfGreen) && NextNumeric(dfBlue))
						pMaterialState->SetSpecular(Colour((float)dfRed, (float)dfGreen, (float)dfBlue, 1.0f));

					break;
				}

				case KWID_SHININESS:
				{
					double dfShininess;

					if (NextNumeric(dfShininess))
						pMaterialState->SetShininess((float)dfShininess);

					break;
				}

				case KWID_ALPHA:
				{
					double dfAlpha;

					if (NextNumeric(dfAlpha))
						pMaterialState->SetAlpha(Mathf::Min(1, Mathf::Max(0, (float)dfAlpha)));

					break;
				}
			}
		}
	}

	return pMaterialState;
}
//----------------------------------------------------------------------------------------------
SamplerRenderStatePtr MaterialScriptParser::ParseSamplerStateBlock()
{
	SamplerRenderStatePtr pSamplerState(new SamplerRenderState());
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pSamplerState;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pSamplerState;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_CHANNEL:
				{
					String strTextureUnit,
						   strSamplerUnit;

					int nChannelID;

					if (NextNumeric(nChannelID) && NextLiteral(strSamplerUnit) && NextLiteral(strTextureUnit))
					{
						SamplerStatePtr pSampler;
						TextureStatePtr pTexture;

						if (m_samplerStateMap.ContainsKey(strSamplerUnit, pSampler))
						{
							if (m_textureStateMap.ContainsKey(strTextureUnit, pTexture))
							{
								pSamplerState->AddSamplerUnit(nChannelID, pTexture, pSampler);
							}
						}
					}

					break;
				}
			}
		}
	}

	return pSamplerState;
}
//----------------------------------------------------------------------------------------------
StencilRenderStatePtr MaterialScriptParser::ParseStencilStateBlock()
{
	return StencilRenderStatePtr(new StencilRenderState());
}
//----------------------------------------------------------------------------------------------
TargetRenderStatePtr MaterialScriptParser::ParseTargetStateBlock()
{
	TargetRenderStatePtr pTargetState(new TargetRenderState());
	dword dwKeywordID;

	if (!IsNextSymbol('{'))
		return pTargetState;

	while(m_tokenStream.HasMoreTokens())
	{
		if (IsNextSymbol('}'))
			return pTargetState;

		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_NAME:
				{
					String strTargetName;

					if (NextLiteral(strTargetName))
						pTargetState->SetRenderTarget(strTargetName);

					break;
				}
			}
		}
	}

	return pTargetState;
}
//----------------------------------------------------------------------------------------------
void MaterialScriptParser::Parse(void)
{
	IParser::Parse();

	dword dwKeywordID;

	m_tokenStream.Reset();
	
	while(m_tokenStream.HasMoreTokens())
	{
		if (NextKeyword(dwKeywordID))
		{
			switch(dwKeywordID)
			{
				case KWID_SHADER_PROGRAM:
				{
					ParseShaderProgramBlock();
					break;
				}
				case KWID_TEXTURE_UNIT:
				{
					ParseTextureUnitBlock();
					break;
				}
				case KWID_SAMPLER_UNIT:
				{
					ParseSamplerUnitBlock();
					break;
				}
				case KWID_MATERIAL:
				{
					ParseMaterialBlock();
					break;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::NextLiteral(Meson::Common::Text::String& p_strLiteral)
{
	if (!m_tokenStream.HasMoreTokens())
		return false;

	Token& token = m_tokenStream.NextToken();

	if (!token.IsLiteral())
		return false;

	p_strLiteral = token.GetString();

	return true;
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::NextKeyword(dword& p_dwKeywordID)
{
	if (!m_tokenStream.HasMoreTokens())
		return false;

	Token& token = m_tokenStream.NextToken();

	if (!token.IsKeyword())
		return false;

	p_dwKeywordID = token.GetInteger();

	return true;
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::NextNumeric(double& p_dfValue)
{
	if (!m_tokenStream.HasMoreTokens())
		return false;

	Token& token = m_tokenStream.NextToken();

	if (!token.IsNumeric())
		return false;

	p_dfValue = token.GetReal();

	return true;
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::NextNumeric(int& p_nValue)
{
	if (!m_tokenStream.HasMoreTokens())
		return false;

	Token& token = m_tokenStream.NextToken();

	if (!token.IsNumeric())
		return false;

	p_nValue = (int)token.GetReal();

	return true;
}
//----------------------------------------------------------------------------------------------
bool MaterialScriptParser::IsNextSymbol(char p_cSymbol)
{
	if (!m_tokenStream.HasMoreTokens())
		return false;

	if (!m_tokenStream.GetToken(0).IsSymbol())
		return false;

	if (m_tokenStream.GetToken(0).GetInteger() != p_cSymbol)
		return false;

	m_tokenStream.Forward();
	return true;
}
//----------------------------------------------------------------------------------------------
Token MaterialScriptParser::ReadToken(void)
{
	while(true)
	{
		char cNext = NextChar();
		
		if (cNext == -1)
		{
			Token token;
			token.SetType(Token::TT_EOS);
			return token;
		}
		else if (IsSymbol(cNext))
		{
			switch(cNext)
			{
				case '"':
					{
						return ReadLiteral();
					}
				case '#':
					{
						ReadComment();
						continue;
					}
				case '}':
				case '{':
					{
						Token token;
						token.SetType(Token::TT_SYMBOL);
						token.SetInteger(cNext);
						return token;
					}
				case '-':
				case '+':
					{
						return ReadNumber(cNext);
					}
				default:
					{
						continue;
					}
			}
		}
		else if (IsDigit(cNext))
		{
			return ReadNumber(cNext);
		}
		else
		{
			return ReadKeyword(cNext);
		}
	}
}
//----------------------------------------------------------------------------------------------
Token MaterialScriptParser::ReadLiteral()
{
	Meson::Common::Text::String strLiteral;
	char cNext;

	while(true)
	{
		cNext = ReadChar();
		if (cNext == -1 || cNext == '"')
			break;

		strLiteral.Append(cNext);
	}

	Token token;

	token.SetType(Token::TT_LITERAL);
	token.SetString(strLiteral);

	return token;
}

//----------------------------------------------------------------------------------------------
Token MaterialScriptParser::ReadNumber(char cInitialSymbol)
{
	Meson::Common::Text::String strNumber = ReadString(cInitialSymbol);

	Token token;
	token.SetType(Token::TT_REAL);
	token.SetReal(Meson::Common::Converter::ToDouble(strNumber));
	
	return token;
}
//----------------------------------------------------------------------------------------------
Token MaterialScriptParser::ReadKeyword(char cInitialSymbol)
{
	Meson::Common::Text::String strKeyword = ReadString(cInitialSymbol);

	Token token;
	token.SetString(strKeyword);

	int nKeywordID;

	if (m_keywordMap.ContainsKey(strKeyword, nKeywordID))
	{
		token.SetType(Token::TT_KEYWORD);
		token.SetInteger(nKeywordID);
	}

	return token;
}
//----------------------------------------------------------------------------------------------
Meson::Common::Text::String MaterialScriptParser::ReadString(char cInitialSymbol)
{
	Meson::Common::Text::String strString;
	strString.Append(cInitialSymbol);

	while(true)
	{
		char cNext = ReadChar();
		
		if (!IsValidTokenChar(cNext))
			break;

		strString.Append(cNext);
	}

	return strString;
}
//----------------------------------------------------------------------------------------------
void MaterialScriptParser::ReadComment()
{
	while(ReadChar() != '\n');
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END