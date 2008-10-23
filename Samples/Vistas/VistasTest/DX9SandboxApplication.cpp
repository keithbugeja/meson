#include "DX9SandboxApplication.h"
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
DX9SandboxApplication::DX9SandboxApplication(const String& p_strTitle, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver)
	: VistasSandbox("DirectX9", p_strTitle, p_uiWidth, p_uiHeight, p_bFullScreen, p_bInstrumentationDriver)
	, EnvironmentFilename("scene.vst")
{ }
//----------------------------------------------------------------------------------------------
DX9SandboxApplication::~DX9SandboxApplication()
{ }
//----------------------------------------------------------------------------------------------
void DX9SandboxApplication::OnInitialise(void)
{
	VistasSandbox::OnInitialise();

	CreateScene();
}
//----------------------------------------------------------------------------------------------
void DX9SandboxApplication::OnShutdown(void)
{
	DestroyScene();
	
	VistasEngine::GetInstance()->GetEffectManager()->DestroyAllEffects();

	VistasSandbox::OnShutdown();
}
//----------------------------------------------------------------------------------------------
void DX9SandboxApplication::LoadEnvironment(const Meson::Common::Text::String& p_strFilename)
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
void DX9SandboxApplication::InitialiseCamera(void)
{
	VistasSandbox::InitialiseCamera();
}
//----------------------------------------------------------------------------------------------
void DX9SandboxApplication::ShutdownCamera(void)
{
	m_pCamera = NULL;

	VistasSandbox::ShutdownCamera();
}
//----------------------------------------------------------------------------------------------
void DX9SandboxApplication::CreateScene(void)
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
}
//----------------------------------------------------------------------------------------------
void DX9SandboxApplication::DestroyScene(void)
{
}
//----------------------------------------------------------------------------------------------
void DX9SandboxApplication::OnDraw(void)
{
	// Quick and dirty code to allow moving light and
	// shadow sources using the keyboard
	static Vector3f vecShadow(-30, 120, -40);
	static int nLightID = 0;
	static String strLight("DefaultLight");

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
		vecShadow.Set(-30,120,-40);

	CameraPtr pCamera = m_pVistasEngine->GetCameraManager()->GetCamera("ShadowCamera");
	pCamera->LookAt(vecShadow, Vector3f::Zero, Vector3f::Up);

	if (m_pVistasEngine->GetLightManager()->ContainsLight(strLight))
	{
		LightPtr pLight = m_pVistasEngine->GetLightManager()->GetLight(strLight);
		pLight->SetDirection(-vecShadow, true);
	}
	
	// Cull and render
	m_pVistasEngine->GetVisibleEntitySetManager()->ExecuteOperations();
}
//----------------------------------------------------------------------------------------------
