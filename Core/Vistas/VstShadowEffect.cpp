//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"
#include "VstShadowEffect.h"

#include "VstCuller.h"
#include "VstSceneNode.h"
#include "VstSceneGeometryNode.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
ShadowEffect::ShadowEffect(const Meson::Common::Text::String& p_strProjectionName, Camera* p_pProjector, uint p_uiWidth, uint p_uiHeight)
	: m_uiWidth(p_uiWidth)
	, m_uiHeight(p_uiHeight)
	, m_strProjectionName(p_strProjectionName)
	, m_lightWVPMatrix("matLightWV")
	, m_pShadowCasterSet(NULL)
	, m_pRenderer(NULL)
{	
	MESON_ASSERT(p_pProjector != NULL, "Projector must be set to an instance of an object.");
	m_pProjector = p_pProjector;
}
//----------------------------------------------------------------------------------------------
ShadowEffect::~ShadowEffect(void) 
{
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::InitialiseTechniques(IRenderDevice* p_pRenderDevice)
{
	// Draw technique
	m_pDrawTechnique = new Technique("DrawProjection", true);

	PassPtr pPass = m_pDrawTechnique->CreatePass("DrawProjectionPass", IPass::PT_EFFECT_PASS);
	pPass->SetShaderProgram(IShaderProgram::SPT_VERTEX, m_pDrawVS);
	pPass->SetShaderProgram(IShaderProgram::SPT_PIXEL, m_pDrawPS);

	m_pDrawTechnique->RebuildTechnique();

	// Apply technique
	m_pApplyTechnique = new Technique("ApplyProjection", true);

	pPass = m_pApplyTechnique->CreatePass("ApplyShadowMapPass", IPass::PT_EFFECT_PASS);
	pPass->SetShaderProgram(IShaderProgram::SPT_VERTEX, m_pApplyVS);
	pPass->SetShaderProgram(IShaderProgram::SPT_PIXEL, m_pApplyPS);

	m_pSamplerState = new SamplerRenderState();
	m_pSamplerState->AddSamplerUnit(0, TextureStatePtr(new TextureState(m_pProjectionTexture)), SamplerStatePtr(new SamplerState()));

	SamplerStatePtr pSamplerState = m_pSamplerState->GetSamplerState(0);
	pSamplerState->SetBorderColour(Colour::White);
	pSamplerState->SetUWrapType(SamplerState::WT_BORDER);
	pSamplerState->SetVWrapType(SamplerState::WT_BORDER);
	pSamplerState->SetMinificationFilter(SamplerState::FT_POINT);
	pSamplerState->SetMagnificationFilter(SamplerState::FT_POINT);

	pPass->AddRenderState(m_pSamplerState);

	m_pAlphaState = new AlphaRenderState();
	m_pAlphaState->SetAlphaBlend(true);
	m_pAlphaState->SetSourceBlendMode(AlphaRenderState::ARSB_ZERO);
	m_pAlphaState->SetDestinationBlendMode(AlphaRenderState::ARSB_SRC_COLOUR);

	pPass->AddRenderState(m_pAlphaState);

	m_pApplyTechnique->RebuildTechnique();
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::InitialiseShaderPrograms(IRenderDevice* p_pRenderDevice)
{
	// Create shader catalogue
	ShaderProgramManager* pShaderProgramManager = p_pRenderDevice->GetShaderProgramManager();
	bool bIsCatalogueAvailable = pShaderProgramManager->ContainsCatalogue("BasicEffects");

	// Create vertex shader for shadow map generation
	if (!bIsCatalogueAvailable || !pShaderProgramManager->ContainsShaderProgram("BasicEffects", "vs_shadowmap_draw", m_pDrawVS))
	{
		String strName = "vs_shadow_draw." + pShaderProgramManager->GetShaderProgramExtension();
		
		m_pDrawVS = p_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(strName, IShaderProgram::SPT_VERTEX, "BasicEffects", "vs_shadowmap_draw");
		m_pDrawVS->SetProfile("vs_2_0");
		m_pDrawVS->SetEntryPoint("vs_main");
		m_pDrawVS->Compile();

		m_pDrawVS->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD_VIEW_PROJECTION);
		m_pDrawVS->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD);
		m_pDrawVS->AddRenderConstant(RenderConstantType::RCT_VECTOR_LIGHT_POSITION);
		m_pDrawVS->AddRenderConstant(RenderConstantType::RCT_VECTOR_VIEWER_POSITION);
	}

	// Create pixel shader for shadow map generation
	if (!bIsCatalogueAvailable || !pShaderProgramManager->ContainsShaderProgram("BasicEffects", "ps_shadowmap_draw", m_pDrawPS))
	{
		String strName = "ps_shadow_draw." + pShaderProgramManager->GetShaderProgramExtension();;
		m_pDrawPS = p_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(strName, IShaderProgram::SPT_PIXEL, "BasicEffects", "ps_shadowmap_draw");
		m_pDrawPS->SetProfile("ps_2_0");
		m_pDrawPS->SetEntryPoint("ps_main");
		m_pDrawPS->Compile();
	}

	// Create vertex shader for shadow map application
	//if (!bCatalogueExists || !pShaderProgramManager->ContainsShaderProgram("BasicEffects", "ApplyShadowMapVS", m_pApplyShadowMapVS))
	{
		String strName = "vs_shadow_apply." + pShaderProgramManager->GetShaderProgramExtension();
		String strApplyShadowMapVSName = "vs_shadow_apply_" + VistasEngine::GetInstance()->GenerateGuid();
		m_pApplyVS = p_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(strName, IShaderProgram::SPT_VERTEX, "BasicEffects", strApplyShadowMapVSName);
		m_pApplyVS->SetProfile("vs_2_0");
		m_pApplyVS->SetEntryPoint("vs_main");
		m_pApplyVS->Compile();

		// Set user variables and render constants for vertex shader
		m_pApplyVS->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD_VIEW_PROJECTION);
		m_pApplyVS->AddRenderConstant(RenderConstantType::RCT_MATRIX_PROJECTION);
		m_pApplyVS->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD);
		m_pApplyVS->AddVariable(&m_lightWVPMatrix);
	}

	// Create pixel shader for shadow map application
	if (!bIsCatalogueAvailable || !pShaderProgramManager->ContainsShaderProgram("BasicEffects", "ps_shadow_apply", m_pApplyPS))
	{
		String strName = "ps_shadow_apply." + pShaderProgramManager->GetShaderProgramExtension();
		m_pApplyPS = p_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(strName, IShaderProgram::SPT_PIXEL, "BasicEffects", "ps_shadow_apply");
		m_pApplyPS->SetProfile("ps_2_0");
		m_pApplyPS->SetEntryPoint("ps_main");
		m_pApplyPS->Compile();
	}
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::InitialiseProjectionTarget(IRenderDevice* p_pRenderDevice)
{
	// Create shadow map render target
	m_strProjectionName = m_strProjectionName;

	m_pProjectionTexture = VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateTextureBuffer(	TextureBuffer::TF_R32F, 
																											TextureBuffer::TT_2D, 
																											m_uiWidth, m_uiHeight, 0, 1,
																											HardwareResource::MP_DEFAULT, 
																											TextureBuffer::TU_RENDERTARGET);

	m_pProjectionTarget = p_pRenderDevice->CreateTextureRenderTarget(m_strProjectionName, m_pProjectionTexture, true);
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::Initialise(IRenderDevice* p_pRenderDevice) 
{ 
	InitialiseProjectionTarget(p_pRenderDevice);
	InitialiseShaderPrograms(p_pRenderDevice);
	InitialiseTechniques(p_pRenderDevice);
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::Dispose(IRenderDevice* p_pRenderDevice) 
{
	m_pProjector = NULL;

	m_pSampler = NULL;
	m_pSamplerState = NULL;
	m_pAlphaState = NULL;

	m_pDrawTechnique = NULL;
	m_pApplyTechnique = NULL;

	m_pRenderer = NULL;
	m_pShadowCasterSet = NULL;
	m_pProjectionTexture = NULL;

	p_pRenderDevice->DestroyRenderTarget(m_strProjectionName);
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::SetRenderer(RendererPtr p_pRenderer)
{
	m_pRenderer = p_pRenderer->CreateInstance();
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::SetShadowCasterSet(VisibleEntitySetPtr p_pShadowCasterSet)
{
	m_pShadowCasterSet = p_pShadowCasterSet;
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::Draw(IRenderDevice* p_pRenderDevice, VisibleEntityPtr p_pVisibleEntity) 
{ 
}
//----------------------------------------------------------------------------------------------
void ShadowEffect::Draw(IRenderDevice* p_pRenderDevice, VisibleEntitySetPtr p_pVisibleEntitySet) 
{
	ISceneNode* pNode = NULL;

	// Set viewport camera to projector
	Viewport* pViewport = p_pRenderDevice->GetViewport();
	Camera* pViewportCamera = pViewport->GetCamera();
	pViewport->SetCamera(m_pProjector);
	pViewport->Update(true);

	// Render to shadow map
	p_pRenderDevice->PushRenderTarget();
	p_pRenderDevice->SetRenderTarget(m_strProjectionName);
	p_pRenderDevice->ClearBuffers(Colour(1,1,1,1), 1.0f, 0);

	// Determine render set
	VisibleEntitySetPtr pShadowCasterSet(m_pShadowCasterSet);
	
	if (pShadowCasterSet == NULL)
		pShadowCasterSet = p_pVisibleEntitySet;

	// Start rendering
	m_pDrawTechnique->BeginTechnique();
	m_pDrawTechnique->BeginPass(0);

	for (uint uiIndex = 0; uiIndex < pShadowCasterSet->Size(); uiIndex++)
	{
		pNode = (ISceneNode*)pShadowCasterSet->GetElement(uiIndex)->GetEntity();
		if (pNode->IsDerived(SceneGeometryNode::GetStaticType()))
		{
			p_pRenderDevice->SetWorldMatrix(pNode->WorldTransformation.ExtractMatrix());

			m_pDrawVS->LoadParameters();

			((SceneGeometryNode*)pNode)->GetGeometry()->Draw(p_pRenderDevice);
		}
	}

	m_pDrawTechnique->EndPass();
	m_pDrawTechnique->EndTechnique();

	//return;

	// Restore render target
	p_pRenderDevice->PopRenderTarget();

	// Restore original camera
	pViewport->SetCamera(pViewportCamera);
	pViewport->Update(true);

	// Draw objects if a renderer has been set
	if (m_pRenderer != NULL)
	{
		m_pRenderer->SetVisibleSet(p_pVisibleEntitySet);
		m_pRenderer->SetGlobalEffects(false);
		m_pRenderer->Draw();
		m_pRenderer->SetGlobalEffects(true);
	}

	// Set projection matrix
	m_lightWVPMatrix.Set(m_pProjector->GetViewMatrix());

	// Draw shadow
	m_pApplyTechnique->BeginTechnique();
	m_pApplyTechnique->BeginPass(0);

	for (uint uiIndex = 0; uiIndex < p_pVisibleEntitySet->Size(); uiIndex++)
	{
		pNode = (ISceneNode*)p_pVisibleEntitySet->GetElement(uiIndex)->GetEntity();
		if (pNode->IsDerived(SceneGeometryNode::GetStaticType()))
		{
			p_pRenderDevice->SetWorldMatrix(pNode->WorldTransformation.ExtractMatrix());
			
			m_pApplyVS->LoadParameters();
			
			((SceneGeometryNode*)pNode)->GetGeometry()->Draw(p_pRenderDevice);
		}
	}

	m_pApplyTechnique->EndPass();
	m_pApplyTechnique->EndTechnique();
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END