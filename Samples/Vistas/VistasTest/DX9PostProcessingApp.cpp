#include "DX9PostProcessingApp.h"
#include "FileInputStream.h"

#include "VstVistasSandbox.h"
#include "VstVistasEngine.h"
#include "VstRenderDevice.h"
#include "VstMeshForge.h"
#include "VstLexer.h"

#include "VstLocalEffect.h"

#include "VstSceneNode.h"
#include "VstSceneLightNode.h"
#include "VstSceneGroupNode.h"
#include "VstSceneGeometryNode.h"

#include "VstRepositoryManager.h"

#include "VstWaveFrontLoader.h"
#include "FileOutputStream.h"

using namespace Meson::Common::IO;
using namespace Meson::Common::Maths;
using namespace Meson::Common;
using namespace Meson::Vistas;

//----------------------------------------------------------------------------------------------
DX9PostProcessingApp::DX9PostProcessingApp(const String& p_strTitle, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver)
	: VistasSandbox("DirectX9", p_strTitle, p_uiWidth, p_uiHeight, p_bFullScreen, p_bInstrumentationDriver)
	, EnvironmentFilename("scene.vst")
{ }
//----------------------------------------------------------------------------------------------
DX9PostProcessingApp::~DX9PostProcessingApp()
{ }
//----------------------------------------------------------------------------------------------
void DX9PostProcessingApp::OnInitialise(void)
{
	VistasSandbox::OnInitialise();

	CreateScene();
}
//----------------------------------------------------------------------------------------------
void DX9PostProcessingApp::OnShutdown(void)
{
	DestroyScene();
	
	VistasEngine::GetInstance()->GetEffectManager()->DestroyAllEffects();

	VistasSandbox::OnShutdown();
}
//----------------------------------------------------------------------------------------------
void DX9PostProcessingApp::LoadEnvironment(const Meson::Common::Text::String& p_strFilename)
{
	LightManager* pLightManager = m_pVistasEngine->GetLightManager();
	CameraManager* pCameraManager = m_pVistasEngine->GetCameraManager();
	EffectManager* pEffectManager = m_pVistasEngine->GetEffectManager();
	GeometryManager* pGeometryManager = m_pVistasEngine->GetGeometryManager();
	ScenegraphManager* pScenegraphManager = m_pVistasEngine->GetScenegraphManager();
	VisibleEntitySetManager* pVisibleEntitySetManager = m_pVistasEngine->GetVisibleEntitySetManager();
	
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strFilename, strResourcePath))
		throw new MesonException("File not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	
	// Load environment
	Lexer lexer(pLookaheadInputStream);
	lexer.ReadKeyword("Environment");
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);
	
	pVisibleEntitySetManager->LoadVisibleEntitySets(pLookaheadInputStream);
	pCameraManager->LoadCameras(pLookaheadInputStream);
	pLightManager->LoadLights(pLookaheadInputStream);
	pEffectManager->LoadEffects(pLookaheadInputStream);
	pGeometryManager->LoadGeometries(pLookaheadInputStream);
	pScenegraphManager->LoadScenegraphs(pLookaheadInputStream);
	pVisibleEntitySetManager->LoadOperations(pLookaheadInputStream);
	
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);
}
//----------------------------------------------------------------------------------------------
void DX9PostProcessingApp::InitialiseCamera(void)
{
	VistasSandbox::InitialiseCamera();
}
//----------------------------------------------------------------------------------------------
void DX9PostProcessingApp::ShutdownCamera(void)
{
	m_pCamera = NULL;
	VistasSandbox::ShutdownCamera();
}
//----------------------------------------------------------------------------------------------
void DX9PostProcessingApp::CreateScene(void)
{
	// Load scene
	LoadEnvironment(EnvironmentFilename);

	// Alias managers
	LightManager* pLightManager = m_pVistasEngine->GetLightManager();
	CameraManager* pCameraManager = m_pVistasEngine->GetCameraManager();
	EffectManager* pEffectManager = m_pVistasEngine->GetEffectManager();
	GeometryManager* pGeometryManager = m_pVistasEngine->GetGeometryManager();
	ScenegraphManager* pScenegraphManager = m_pVistasEngine->GetScenegraphManager();
	VisibleEntitySetManager* pVisibleEntitySetManager = m_pVistasEngine->GetVisibleEntitySetManager();

	// TODO: Camera* in sandbox and viewport must be changed to CameraPtr
	// Will keep it as is till next Tuesday to avoid affecting Gravitas sandbox
	delete m_pCamera;

	// Use environment camera instead of default sandbox camera
	m_pCamera = pCameraManager->GetCamera("DefaultCamera");
	m_pViewport->SetCamera(m_pCamera);

	// Configure camera for use with user input
	ConfigureCamera(Mathf::PiHalf * 0.5f, 2.0f);
	
	// Use a dark ambient colour
	LightRenderState* pLightRenderState = m_pRenderDevice->GetLightRenderState();
	pLightRenderState->SetAmbient(Colour(0.2f,0.2f,0.05f,0));
	m_pRenderDevice->SetLightRenderState(pLightRenderState);

	// Update render state for all graphs
	ScenegraphEnumerator graphEnumerator = pScenegraphManager->GetScenegraphEnumerator();
	while (graphEnumerator.HasMoreElements())
	{
		ScenegraphPtr& pGraph = graphEnumerator.NextElement();
		ISceneNode* pNode = pGraph->GetNode("Root");
		pNode->UpdateGeometryState(true, 0);
		pNode->UpdateRenderState(true, m_pRenderDevice->GetRenderContext(IRenderDevice::RTT_GLOBAL_CONTEXT), LightListPtr(NULL));
	}

	// TODO: improve render target and texture sub-systems

	// Create screen-aligned quad
	GeometryPtr pScreenAlignedQuad = MeshForge::CreatePlaneMesh(Vector3f::Out, 1.0, 1.0f, 1.0f);

	TexturePtr pBackBufferTexture = m_pVistasEngine->GetTextureManager()->CreateTexture2D("backbuffer", 
		TextureBuffer::TF_X8R8G8B8, TextureBuffer::TU_RENDERTARGET, 
		m_uiWidth, m_uiHeight);

	TexturePtr pBackBufferDepthTexture = m_pVistasEngine->GetTextureManager()->CreateTexture2D("backbufferDepth",
		TextureBuffer::TF_X8R8G8B8, TextureBuffer::TU_RENDERTARGET,
		m_uiWidth, m_uiHeight);

	TextureRenderTarget* pBack = m_pRenderDevice->CreateTextureRenderTarget("backbuffer", pBackBufferTexture->GetTextureBuffer(), true);
	TextureRenderTarget* pBackDepth = m_pRenderDevice->CreateTextureRenderTarget("backbufferDepth", pBackBufferDepthTexture->GetTextureBuffer(), true);
	
	MultipleTextureRenderTarget* pBackMulti = m_pRenderDevice->CreateMultipleTextureRenderTarget("backbufferMulti");
	pBackMulti->AttachTextureRenderTarget(pBackDepth);
	pBackMulti->AttachTextureRenderTarget(pBack);

	pScreenAlignedQuad->SetId("ScreenAlignedQuad");
	pScreenAlignedQuad->SetMaterial(m_pVistasEngine->GetMaterialManager()->GetMaterial("postprocess"));
	
	pGeometryManager->RegisterGeometry(pScreenAlignedQuad);
}
//----------------------------------------------------------------------------------------------
void DX9PostProcessingApp::DestroyScene(void)
{
	m_pRenderDevice->DestroyRenderTarget("backbuffer");
	m_pRenderDevice->DestroyRenderTarget("backbufferDepth");
	m_pRenderDevice->DestroyRenderTarget("backbufferMulti");
}
//----------------------------------------------------------------------------------------------
void DX9PostProcessingApp::OnDraw(void)
{
	static Vector3f vecShadow(-30, 120, -40);
	static int nLightID = 0;
	static String strLight("DefaultLight");

	if (m_bKey[KC_N])
	{
		strLight = "DefaultLight"; 
	}

	if (m_bKey[KC_M])
	{
		strLight = "DefaultLight1"; 
	}

	if (m_bKey[KC_I])
	{
		Matrix3f matRotation(Vector3f::Right, Mathf::DegreeToRadian(-0.5));
		vecShadow = matRotation * vecShadow;
	}
	
	if (m_bKey[KC_K])
	{
		Matrix3f matRotation(Vector3f::Right, Mathf::DegreeToRadian(0.5));
		vecShadow = matRotation * vecShadow;
	}

	if (m_bKey[KC_J])
	{
		Matrix3f matRotation(Vector3f::Up, Mathf::DegreeToRadian(-0.5));
		vecShadow = matRotation * vecShadow;
	}
	
	if (m_bKey[KC_L])
	{
		Matrix3f matRotation(Vector3f::Up, Mathf::DegreeToRadian(0.5));
		vecShadow = matRotation * vecShadow;
	}

	if (m_bKey[KC_P])
		vecShadow.Set(10,120,20);

	CameraPtr pCamera = m_pVistasEngine->GetCameraManager()->GetCamera("ShadowCamera");
	pCamera->LookAt(vecShadow, Vector3f::Zero, Vector3f::Up);

	LightPtr pLight = m_pVistasEngine->GetLightManager()->GetLight(strLight);
	pLight->SetDirection(-vecShadow, true);
	
	m_pRenderDevice->PushRenderTarget();
	//m_pRenderDevice->SetRenderTarget("backbuffer");
	m_pRenderDevice->SetRenderTarget("backbufferMulti");
	m_pRenderDevice->ClearDepthBuffer(1.0f);
	//m_pRenderDevice->ClearBuffers(Colour::Black, 1.0f, 0);
	m_pVistasEngine->GetVisibleEntitySetManager()->ExecuteOperations();

	m_pRenderDevice->PopRenderTarget();
	GeometryPtr p = m_pVistasEngine->GetGeometryManager()->GetGeometry("ScreenAlignedQuad");
	MaterialPtr m = p->GetMaterial();
	
	TechniquePtr t = m->GetBestTechnique();
	uint tmax = t->BeginTechnique();
	for (uint ti = 0; ti < tmax; ti--)
	{
		t->BeginPass(ti);
		m_pRenderDevice->SetTexture(1, m_pVistasEngine->GetTextureManager()->GetTexture("backbuffer")->GetTextureBuffer());
		m_pRenderDevice->SetTexture(0, m_pVistasEngine->GetTextureManager()->GetTexture("backbufferDepth")->GetTextureBuffer());
		p->Draw(m_pRenderDevice);
		t->EndPass();
	}
	t->EndTechnique();
}
//----------------------------------------------------------------------------------------------
