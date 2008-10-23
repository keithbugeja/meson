//----------------------------------------------------------------------------------------------
#include "Stopwatch.h"
#include "Directory.h"
#include "FileInputStream.h"

#include "VstVistasSandbox.h"
#include "VstVistasEngine.h"
#include "VstRenderDevice.h"
#include "VstMeshForge.h"

#include "VstSceneNode.h"
#include "VstSceneLightNode.h"
#include "VstSceneGroupNode.h"
#include "VstSceneGeometryNode.h"

//----------------------------------------------------------------------------------------------
using namespace Meson::Common::IO;
using namespace Meson::Common::Maths;
using namespace Meson::Common;
//----------------------------------------------------------------------------------------------

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
VistasSandbox::VistasSandbox(const String& p_strDeviceName, const String& p_strTitle, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver)
	: VistasApplication(p_strDeviceName, p_strTitle, p_uiWidth, p_uiHeight, p_bFullScreen, p_bInstrumentationDriver)
{
	MaterialCatalogue = "default.ms";
	BackgroundColour.Set(0, 0, 0, 0.75);
}
//----------------------------------------------------------------------------------------------
VistasSandbox::~VistasSandbox()
{
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnLoadPlugIns(void)
{
	m_pVistasEngine->LoadPlugIn("DirectX9Renderer.dll");
	m_pVistasEngine->LoadPlugIn("CoreGeometries.dll");
	m_pVistasEngine->LoadPlugIn("CoreCullers.dll");
	m_pVistasEngine->LoadPlugIn("CoreRenderers.dll");
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnUnloadPlugIns(void)
{
	m_pVistasEngine->UnloadPlugIn("CoreRenderers.dll");
	m_pVistasEngine->UnloadPlugIn("CoreCullers.dll");
	m_pVistasEngine->UnloadPlugIn("CoreGeometries.dll");
	m_pVistasEngine->UnloadPlugIn("DirectX9Renderer.dll");
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnInitialise(void)
{
	VistasApplication::OnInitialise();

	InitialisePlugIns();
	InitialiseRepository();
	InitialiseCamera();
	InitialiseMaterialCatalogue();
	InitialiseContext();
	InitialiseTimer();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnShutdown(void)
{
	ShutdownPlugIns();
	ShutdownCamera();

	VistasApplication::OnShutdown();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnIdle(void)
{
	VistasApplication::OnIdle();
	
	OnMoveCamera();
	OnFrameStart();
	OnFrameDraw();
	OnFrameEnd();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnFrameStart(void)
{
	// Work time difference for time-based animation
	m_dfPreviousTime = m_dfCurrentTime;
	m_dfCurrentTime = m_stopwatch.GetElapsedTime().TotalSeconds();
	m_dfDeltaTime = m_dfCurrentTime - m_dfPreviousTime;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnFrameEnd(void)
{
	m_dfFrameTime = m_stopwatch.GetElapsedTime().TotalSeconds() - m_dfCurrentTime;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnFrameDraw(void)
{
	OnDrawStart();

	OnDraw();

	OnDrawEnd();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnDrawStart(void)
{
	// Update camera
	m_pCamera->SetFrame(m_vec3CameraPosition, m_vec3CameraAxes[0], m_vec3CameraAxes[1], m_vec3CameraAxes[2]);
	m_pViewport->Update();

	// Clear colour, stencil and depth buffers 
	m_pRenderDevice->ClearBuffers(BackgroundColour, 1.0f, 0);			

	// Start rendering scene
	m_pRenderDevice->BeginScene();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnDraw(void)
{
	// Update and synchronise transformations
	m_pRootNode->UpdateGeometryState(true, m_dfDeltaTime);

	// Compute visible set
	VisibleEntitySetPtr pVisibleSet(m_pCuller->ComputeVisibleSet());
	
	// Set visible set and render
	m_pRenderer->SetVisibleSet(pVisibleSet);
	m_pRenderer->Draw();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnDrawEnd(void)
{
	// Finish scene rendering
	m_pRenderDevice->EndScene();

	// Present back buffer
	m_pWindow->Present();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::InitialiseRepository(void)
{
	RepositoryManager* pRepositoryManager = m_pVistasEngine->GetRepositoryManager();

	// Register main application path
	pRepositoryManager->RegisterPath(ApplicationPath);

	// Register shader library path
	String strShaderExtension = m_pRenderDevice->GetShaderProgramManager()->GetShaderProgramExtension();
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + strShaderExtension);
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + strShaderExtension + Directory::Delimeter + "2.0");
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + strShaderExtension + Directory::Delimeter + "3.0");
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + strShaderExtension + Directory::Delimeter + "4.0");

	// Register script paths (materials, etc)
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + "script");
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + "script" + Directory::Delimeter + "environment");
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + "script" + Directory::Delimeter + "material");
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + "script" + Directory::Delimeter + "geometry");

	// Register model paths
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + "model");
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + "model" + Directory::Delimeter + "obj");

	// Register texture paths
	pRepositoryManager->RegisterPath(ApplicationPath + Directory::Delimeter + "texture");
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::InitialiseTimer(void)
{
	m_stopwatch.Reset();
	m_dfCurrentTime = m_dfPreviousTime = m_stopwatch.GetElapsedTime().TotalSeconds();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::InitialiseMaterialCatalogue(void)
{
	// If material catalogue is invalid, perform an early out
	if (MaterialCatalogue.IsEmpty())
		return;

	String strMaterialScriptPath;
	
	if (m_pVistasEngine->GetRepositoryManager()->ContainsResource(MaterialCatalogue, strMaterialScriptPath))
	{
		FileInputStream inputFileStream(strMaterialScriptPath);
		m_pVistasEngine->GetMaterialManager()->ExecuteMaterialScript(inputFileStream);
		inputFileStream.Close();
	}
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::InitialiseContext(void)
{
	// Initialise render device context
	RenderContextPtr pRenderContext = m_pRenderDevice->GetRenderContext(IRenderDevice::RTT_GLOBAL_CONTEXT);

	BiasRenderStatePtr pBiasRenderState((BiasRenderState*)(IRenderState*)pRenderContext->GetRenderState(IRenderState::RST_BIAS));
	pBiasRenderState->SetDepthBias(0);
	pBiasRenderState->SetScaleBias(0);
	m_pRenderDevice->SetBiasRenderState(pBiasRenderState);

	// Enable CCW culling on back faces
	CullRenderStatePtr pCullRenderState((CullRenderState*)(IRenderState*)pRenderContext->GetRenderState(IRenderState::RST_CULL));
	pCullRenderState->SetCulling(true);
	pCullRenderState->SetFaceWindingMode(CullRenderState::CRSFW_CCW);
	pCullRenderState->SetFaceCullingMode(CullRenderState::CRSFC_BACK);
	m_pRenderDevice->SetCullRenderState(pCullRenderState);

	// Enable lighting and set ambient to grey
	LightRenderStatePtr pLightRenderState((LightRenderState*)(IRenderState*)pRenderContext->GetRenderState(IRenderState::RST_LIGHT));
	pLightRenderState->SetLighting(true);
	pLightRenderState->SetAmbient(Colour::Black);
	m_pRenderDevice->SetLightRenderState(pLightRenderState);
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::InitialisePlugIns(void)
{
	// Initialise a basic culler and renderer
	m_pCuller	= m_pVistasEngine->GetCullerManager()->GetCullerFactory("Basic")->CreateCuller();
	m_pRenderer	= m_pVistasEngine->GetRendererManager()->GetRendererFactory("Basic")->CreateRenderer();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::ShutdownPlugIns(void)
{
	// Release culler and renderer instances
	m_pCuller = NULL;
	m_pRenderer = NULL;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::InitialiseCamera(void)
{
	// Create a default camera and viewport
	m_pCamera = new Camera("DefaultCamera");
	m_pCamera->SetProjection(Camera::CP_PERSPECTIVE);
	m_pCamera->SetFrustum(Mathf::DegreeToRadian(45), 1.25f, 1, 100);
	m_pCamera->LookAt(Vector3f(0, 0, 5), Vector3f::Zero, Vector3f::Up);
	
	m_pWindow->CreateViewport(m_pCamera);
	m_pViewport = m_pWindow->GetViewport(0);
	m_pRenderDevice->SetViewport(m_pViewport);
	m_pViewport->Update();

	ConfigureCamera(Mathf::Pi / 12.5f, 1.0f);
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::ShutdownCamera(void)
{
	SAFE_DELETE(m_pCamera);
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::ConfigureCamera(float p_fAngularSpeed, float p_fLinearSpeed)
{
	// Initialise camera motion
	m_vec3CameraAxes[0] = m_pCamera->GetDirection();
	m_vec3CameraAxes[1] = m_pCamera->GetVerticalAxis();
	m_vec3CameraAxes[2] = m_pCamera->GetHorizontalAxis();

	m_vec3CameraPosition = m_pCamera->GetPosition();

	m_fCameraRotationSpeed = p_fAngularSpeed;
	m_fCameraMovementSpeed = p_fLinearSpeed;

	m_vec2CameraMouseTrack = 0;
	m_vec2CameraMouseDelta = 0;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnMouseMove(float p_fMouseX, float p_fMouseY)
{
	VistasApplication::OnMouseMove(p_fMouseX, p_fMouseY);

	if (m_bMouseKey[MK_LEFT])
	{
		m_vec2CameraMouseDelta.Set( GetWidth() * (m_fLastMouseX - m_vec2CameraMouseTrack.X),
									GetHeight() * (m_fLastMouseY - m_vec2CameraMouseTrack.Y) );
	}
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnMouseDown(IWindowMessageHandler::MouseKey p_eKey)
{
	VistasApplication::OnMouseDown(p_eKey);

	m_vec2CameraMouseTrack.Set(m_fLastMouseX, m_fLastMouseY);
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnMouseUp(IWindowMessageHandler::MouseKey p_eKey)
{
	VistasApplication::OnMouseUp(p_eKey);

	m_vec2CameraMouseTrack = 0;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::OnMoveCamera(void)
{
	if (Mathf::Abs(m_vec2CameraMouseDelta.LengthSquared()) > 2.0f)
		LookFree(m_vec2CameraMouseDelta.X, m_vec2CameraMouseDelta.Y);

	if (m_bKey[KC_UP])
		LookUp();

	if (m_bKey[KC_DOWN])
		LookDown();

	if (m_bKey[KC_LEFT])
		TurnLeft();

	if (m_bKey[KC_RIGHT])
		TurnRight();

	if (m_bKey[KC_Q])
		RollLeft();

	if (m_bKey[KC_E])
		RollRight();

	if (m_bKey[KC_W])
		MoveForward();

	if (m_bKey[KC_S])
		MoveBack();

	if (m_bKey[KC_A])
		MoveLeft();

	if (m_bKey[KC_D])
		MoveRight();

	if (m_bKey[KC_R])
		MoveUp();

	if (m_bKey[KC_F])
		MoveDown();
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::LookFree(float p_fDeltaX, float p_fDeltaY)
{
	Matrix3f mat3Rotate(m_vec3CameraAxes[1], -m_fCameraRotationSpeed * (float)m_dfDeltaTime * p_fDeltaX);
	m_vec3CameraAxes[0] = mat3Rotate * m_vec3CameraAxes[0];
	m_vec3CameraAxes[2] = mat3Rotate * m_vec3CameraAxes[2];

	mat3Rotate.MakeRotation(m_vec3CameraAxes[2], m_fCameraRotationSpeed * (float)m_dfDeltaTime * p_fDeltaY);
	m_vec3CameraAxes[0] = mat3Rotate * m_vec3CameraAxes[0];
	m_vec3CameraAxes[1] = mat3Rotate * m_vec3CameraAxes[1];

	m_vec2CameraMouseTrack.Set(m_fLastMouseX, m_fLastMouseY);
	m_vec2CameraMouseDelta = 0;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::TurnLeft(void)
{
	Matrix3f mat3Rotate(m_vec3CameraAxes[1], m_fCameraRotationSpeed * (float)m_dfDeltaTime);
	m_vec3CameraAxes[0] = mat3Rotate * m_vec3CameraAxes[0];
	m_vec3CameraAxes[2] = mat3Rotate * m_vec3CameraAxes[2];
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::TurnRight(void)
{
	Matrix3f mat3Rotate(m_vec3CameraAxes[1], -m_fCameraRotationSpeed * (float)m_dfDeltaTime);
	m_vec3CameraAxes[0] = mat3Rotate * m_vec3CameraAxes[0];
	m_vec3CameraAxes[2] = mat3Rotate * m_vec3CameraAxes[2];
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::LookUp(void)
{
	Matrix3f mat3Rotate(m_vec3CameraAxes[2], -m_fCameraRotationSpeed * (float)m_dfDeltaTime);
	m_vec3CameraAxes[0] = mat3Rotate * m_vec3CameraAxes[0];
	m_vec3CameraAxes[1] = mat3Rotate * m_vec3CameraAxes[1];
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::LookDown(void)
{
	Matrix3f mat3Rotate(m_vec3CameraAxes[2], m_fCameraRotationSpeed * (float)m_dfDeltaTime);
	m_vec3CameraAxes[0] = mat3Rotate * m_vec3CameraAxes[0];
	m_vec3CameraAxes[1] = mat3Rotate * m_vec3CameraAxes[1];
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::RollLeft(void)
{
	Matrix3f mat3Rotate(m_vec3CameraAxes[0], -m_fCameraRotationSpeed * (float)m_dfDeltaTime);
	m_vec3CameraAxes[1] = mat3Rotate * m_vec3CameraAxes[1];
	m_vec3CameraAxes[2] = mat3Rotate * m_vec3CameraAxes[2];
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::RollRight(void)
{
	Matrix3f mat3Rotate(m_vec3CameraAxes[0], m_fCameraRotationSpeed * (float)m_dfDeltaTime);
	m_vec3CameraAxes[1] = mat3Rotate * m_vec3CameraAxes[1];
	m_vec3CameraAxes[2] = mat3Rotate * m_vec3CameraAxes[2];
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::MoveForward(void)
{
	m_vec3CameraPosition += m_vec3CameraAxes[0] * m_fCameraMovementSpeed * (float)m_dfDeltaTime;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::MoveBack(void)
{
	m_vec3CameraPosition -= m_vec3CameraAxes[0] * m_fCameraMovementSpeed * (float)m_dfDeltaTime;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::MoveLeft(void)
{
	m_vec3CameraPosition += m_vec3CameraAxes[2] * m_fCameraMovementSpeed * (float)m_dfDeltaTime;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::MoveRight(void)
{
	m_vec3CameraPosition -= m_vec3CameraAxes[2] * m_fCameraMovementSpeed * (float)m_dfDeltaTime;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::MoveUp(void)
{
	m_vec3CameraPosition += m_vec3CameraAxes[1] * m_fCameraMovementSpeed * (float)m_dfDeltaTime;
}
//----------------------------------------------------------------------------------------------
void VistasSandbox::MoveDown(void)
{
	m_vec3CameraPosition -= m_vec3CameraAxes[1] * m_fCameraMovementSpeed * (float)m_dfDeltaTime;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END

