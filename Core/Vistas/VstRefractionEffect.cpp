//----------------------------------------------------------------------------------------------
#include "VstRefractionEffect.h"
#include "VstVistasEngine.h"

#include "VstSceneNode.h"
#include "VstSceneGeometryNode.h"

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
RefractiveEffect::RefractiveEffect(const Meson::Common::Text::String& p_strCubeMapName, const Meson::Common::Text::String& p_strCubeMapFilename, float p_fRefractiveIndex)
	: m_strCubeMapName(p_strCubeMapName) 
	, m_strCubeMapFilename(p_strCubeMapFilename)
	, m_fRefractiveIndex(p_fRefractiveIndex)
	, m_refractiveIndex("refractiveIndex")
	, m_pCubeMap(NULL)
{
	m_refractiveIndex.Set(p_fRefractiveIndex);
}
//----------------------------------------------------------------------------------------------
RefractiveEffect::RefractiveEffect(TexturePtr p_pCubeMap, float p_fRefractiveIndex)
	: m_fRefractiveIndex(p_fRefractiveIndex)
	, m_refractiveIndex("refractiveIndex")
	, m_pCubeMap(p_pCubeMap)
{
	m_refractiveIndex.Set(p_fRefractiveIndex);
}
//----------------------------------------------------------------------------------------------
RefractiveEffect::~RefractiveEffect(void)
{
}
//----------------------------------------------------------------------------------------------
void RefractiveEffect::SetRefractiveIndex(float p_fRefractiveIndex)
{
	m_fRefractiveIndex = p_fRefractiveIndex;
	m_refractiveIndex.Set(p_fRefractiveIndex);
}
//----------------------------------------------------------------------------------------------
float RefractiveEffect::GetRefractiveIndex(void) const
{
	return m_fRefractiveIndex;
}
//----------------------------------------------------------------------------------------------
void RefractiveEffect::InitialiseTextures(IRenderDevice* p_pRenderDevice)
{
	if (m_pCubeMap == NULL)
		m_pCubeMap = VistasEngine::GetInstance()->GetTextureManager()->CreateTextureFromFile(m_strCubeMapName, m_strCubeMapFilename, TextureBuffer::TT_CUBEMAP);
}
//----------------------------------------------------------------------------------------------
void RefractiveEffect::InitialiseTechniques(IRenderDevice* p_pRenderDevice)
{
	m_pEnvironmentMappingTechnique = new Technique("DrawRefraction", true);

	PassPtr pPass = m_pEnvironmentMappingTechnique->CreatePass("DrawRefraction", IPass::PT_EFFECT_PASS);
	pPass->SetShaderProgram(IShaderProgram::SPT_VERTEX, m_pDrawVS);
	pPass->SetShaderProgram(IShaderProgram::SPT_PIXEL, m_pDrawPS);

	// Apply technique
	m_pSamplerState = new SamplerRenderState();
	m_pSamplerState->AddSamplerUnit(0, TextureStatePtr(new TextureState(m_pCubeMap)), SamplerStatePtr(new SamplerState()));

	SamplerStatePtr pSamplerState = m_pSamplerState->GetSamplerState(0);
	pSamplerState->SetBorderColour(Colour::White);
	pSamplerState->SetUWrapType(SamplerState::WT_BORDER);
	pSamplerState->SetVWrapType(SamplerState::WT_BORDER);
	pSamplerState->SetMinificationFilter(SamplerState::FT_POINT);
	pSamplerState->SetMagnificationFilter(SamplerState::FT_POINT);
	pSamplerState->SetSamplerType(SamplerState::ST_SAMPLER_CUBE);

	pPass->AddRenderState(m_pSamplerState);

	m_pAlphaState = new AlphaRenderState();
	m_pAlphaState->SetAlphaBlend(true);
	m_pAlphaState->SetSourceBlendMode(AlphaRenderState::ARSB_SRC_COLOUR);
	m_pAlphaState->SetDestinationBlendMode(AlphaRenderState::ARSB_ONE);

	//pPass->AddRenderState(m_pAlphaState);

	m_pEnvironmentMappingTechnique->RebuildTechnique();
}
//----------------------------------------------------------------------------------------------
void RefractiveEffect::InitialiseShaderPrograms(IRenderDevice* p_pRenderDevice)
{
	// Create shader catalogue
	ShaderProgramManager* pShaderProgramManager = p_pRenderDevice->GetShaderProgramManager();
	bool bIsCatalogueAvailable = pShaderProgramManager->ContainsCatalogue("BasicEffects");

	// Create vertex shader for shadow map generation
	if (!bIsCatalogueAvailable || !pShaderProgramManager->ContainsShaderProgram("BasicEffects", "vs_refract_envmap_draw", m_pDrawVS))
	{
		String strName = "vs_refraction_draw." + pShaderProgramManager->GetShaderProgramExtension();
		
		m_pDrawVS = p_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(strName, IShaderProgram::SPT_VERTEX, "BasicEffects", "vs_refract_envmap_draw");
		m_pDrawVS->SetProfile("vs_2_0");
		m_pDrawVS->SetEntryPoint("vs_main");
		m_pDrawVS->Compile();

		m_pDrawVS->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD_VIEW_PROJECTION);
		m_pDrawVS->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD);
		m_pDrawVS->AddRenderConstant(RenderConstantType::RCT_VECTOR_VIEWER_POSITION);

		m_pDrawVS->AddVariable(&m_refractiveIndex);
	}

	// Create pixel shader for shadow map generation
	if (!bIsCatalogueAvailable || !pShaderProgramManager->ContainsShaderProgram("BasicEffects", "ps_refract_envmap_draw", m_pDrawPS))
	{
		String strName = "ps_refraction_draw." + pShaderProgramManager->GetShaderProgramExtension();
		m_pDrawPS = p_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(strName, IShaderProgram::SPT_PIXEL, "BasicEffects", "ps_refract_envmap_draw");
		m_pDrawPS->SetProfile("ps_2_0");
		m_pDrawPS->SetEntryPoint("ps_main");
		m_pDrawPS->Compile();
	}
}
//----------------------------------------------------------------------------------------------
void RefractiveEffect::Initialise(IRenderDevice* p_pRenderDevice)
{
	InitialiseTextures(p_pRenderDevice);
	InitialiseShaderPrograms(p_pRenderDevice);
	InitialiseTechniques(p_pRenderDevice);
}
//----------------------------------------------------------------------------------------------
void RefractiveEffect::Dispose(IRenderDevice* p_pRenderDevice)
{
	m_pCubeMap = NULL;
	m_pEnvironmentMappingTechnique = NULL;

	m_pSampler = NULL;
	m_pSamplerState = NULL;
	m_pAlphaState = NULL;
}
//----------------------------------------------------------------------------------------------
void RefractiveEffect::Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity)
{
	MESON_ASSERT(p_pVisibleEntity->GetEntity()->IsDerived(SceneGeometryNode::GetStaticType()), "Entity is not drawable : Drawable nodes inherit from SceneGeometryNode");

	SceneGeometryNode* pNode = ((SceneGeometryNode*)p_pVisibleEntity->GetEntity());
	p_pRenderDevice->SetWorldMatrix(pNode->WorldTransformation.ExtractMatrix());

	m_pEnvironmentMappingTechnique->BeginTechnique();
	m_pEnvironmentMappingTechnique->BeginPass(0);

	pNode->GetGeometry()->Draw(p_pRenderDevice);

	m_pEnvironmentMappingTechnique->EndPass();
	m_pEnvironmentMappingTechnique->EndTechnique();
}
//----------------------------------------------------------------------------------------------
void RefractiveEffect::Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet)
{
	ISceneNode* pNode = NULL;

	m_pEnvironmentMappingTechnique->BeginTechnique();
	m_pEnvironmentMappingTechnique->BeginPass(0);

	for (uint uiIndex = 0; uiIndex < p_pVisibleEntitySet->Size(); uiIndex++)
	{
		pNode = (ISceneNode*)p_pVisibleEntitySet->GetElement(uiIndex)->GetEntity();
		if (pNode->IsDerived(SceneGeometryNode::GetStaticType()))
		{
			p_pRenderDevice->SetWorldMatrix(pNode->WorldTransformation.ExtractMatrix());
			
			m_pDrawVS->LoadParameters();
			
			((SceneGeometryNode*)pNode)->GetGeometry()->Draw(p_pRenderDevice);
		}
	}

	m_pEnvironmentMappingTechnique->EndPass();
	m_pEnvironmentMappingTechnique->EndTechnique();
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END