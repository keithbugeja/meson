#include "GravitasSandboxApplication.h"

#include "FileInputStream.h"

#include "VstVistasEngine.h"
#include "VstRenderDevice.h"
#include "VstMeshForge.h"

#include "VstLocalEffect.h"

#include "VstSceneNode.h"
#include "VstSceneLightNode.h"
#include "VstSceneGroupNode.h"
#include "VstSceneGeometryNode.h"

#include "VstRepositoryManager.h"

#include "VstWaveFrontLoader.h"

#include "SandboxInstrumentationDevice.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Maths;

using namespace Meson::Vistas;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Simulation;
using namespace Meson::Gravitas::Space;

//----------------------------------------------------------------------------------------------
GravitasSandboxApplication::GravitasSandboxApplication(uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver)
	: VistasSandbox("DirectX9", "Meson: Gravitas sandbox application (Powered by Meson: Vistas)",
		p_uiWidth, p_uiHeight, p_bFullScreen, p_bInstrumentationDriver)
	, m_strSimulationFilename()
	, m_listSandboxEntities()
	, m_stopWatchPhysics()
	, m_stopWatchGraphics()
	, m_dCurrentTime(0.0)
	, m_bRunning(false)
	, m_nCameraLock(-1)
	, m_pBodyPicking()
	, m_rBodyPickingDepth((Real) 0.0)
	, m_pGravitasEngine(NULL)
	, m_pSimulator()
	, m_pEnvironment()
	, m_pShadowMapEffect(NULL)
	, m_pReflectionEffect(NULL)
	, m_pSkyEffect(NULL)
{
	m_strSimulationFilename = "..\\Gravitas\\GravitasSandbox\\Resources\\default.grv";
}
//----------------------------------------------------------------------------------------------
GravitasSandboxApplication::~GravitasSandboxApplication()
{ }
//----------------------------------------------------------------------------------------------
void GravitasSandboxApplication::OnInitialise(void)
{
	VistasSandbox::OnInitialise();

	InitialiseGravitas();
	LoadEnvironment();

	CreateScene();
}
//----------------------------------------------------------------------------------------------
void GravitasSandboxApplication::OnShutdown(void)
{
	DestroyScene();

	ShutdownGravitas();

	VistasSandbox::OnShutdown();
}
//----------------------------------------------------------------------------------------------
void GravitasSandboxApplication::CreateScene(void)
{
	//----------------------------------------------------------------------------------------------
	/** Configure sandbox camera
	 	Set initial position and motion parameters
	 */
	//----------------------------------------------------------------------------------------------
	//m_pCamera->SetPosition(Vector3f(0,5,15));
	m_pCamera->LookAt(Vector3f(0,5,15), Vector3f::Zero, Vector3f::Up);
	VistasSandbox::ConfigureCamera(Mathf::Pi * 0.25f, 3.0f);

	//----------------------------------------------------------------------------------------------
	/** Create a spot light for use within the scene
	 */
	//----------------------------------------------------------------------------------------------
	LightPtr pDefaultLight(new Light("DefaultLight"));
	pDefaultLight->Reset();
	pDefaultLight->SetType(Light::LT_DIRECTIONAL);
	pDefaultLight->SetDiffuse(Colour(1.0, 1.0f, 1.0f, 0));
	pDefaultLight->SetSpecular(Colour(0.5f, 0.5f, 0.5f, 32));

	pDefaultLight->SetPosition(Vector3f(37, 37, 37));
	pDefaultLight->SetDirection(Vector3f(-1, -1, -1), true);

	pDefaultLight->SetAttenuation(1.0f, 0.0f, 0.0f);
	pDefaultLight->SetRange(32);
	pDefaultLight->SetLight(true);

	//----------------------------------------------------------------------------------------------
	/** Create geometry objects for display
	 */
	//----------------------------------------------------------------------------------------------

	// Skybox
	GeometryPtr pSkyBoxGeometry = MeshForge::CreateBoxMesh(32, 32, 32, true);

	// vistas material
	MaterialPtr pMaterialDefault = m_pVistasEngine->GetMaterialManager()->GetMaterial("default");


	//----------------------------------------------------------------------------------------------
	/** Create root node and register for automatic disposing on exit
	 */
	//----------------------------------------------------------------------------------------------
	m_pRootNode = new SceneGroupNode("RootNode");
	RegisterNode(m_pRootNode);

	//----------------------------------------------------------------------------------------------
	/** Set up the scene culler, attach it to the scene and
		attach a camera to it, for frustum extraction.
	 */
	//----------------------------------------------------------------------------------------------
	m_pCuller->SetRootNode(m_pRootNode);
	m_pCuller->SetCamera(m_pCamera);
	m_pCuller->Rebuild();
	m_pCuller->Update();

	//----------------------------------------------------------------------------------------------
	/** Create skybox node : this node should be processed before all other
		nodes, hence it is added first.
	 */
	//----------------------------------------------------------------------------------------------
	SceneGeometryNode* pSkyBoxGeometryNode = new SceneGeometryNode("SkyBoxNode");
	pSkyBoxGeometryNode->SetGeometry(pSkyBoxGeometry);
	m_pRootNode->AttachChild(pSkyBoxGeometryNode);
	RegisterNode(pSkyBoxGeometryNode);

	//----------------------------------------------------------------------------------------------
	/** Create shadow node : this node is a grouping node which contains all
		objects that will be receive shadows.
	 */
	//----------------------------------------------------------------------------------------------
	SceneGroupNode* pShadowGroupNode = new SceneGroupNode("ShadowGroupNode");
	m_pRootNode->AttachChild(pShadowGroupNode);
	RegisterNode(pShadowGroupNode);

	SceneGroupNode* pObjectGroupNode = new SceneGroupNode("ObjectGroupNode");
	pShadowGroupNode->AttachChild(pObjectGroupNode);
	RegisterNode(pObjectGroupNode);

	//----------------------------------------------------------------------------------------------
	/** Create light node : note that a light node is only required if the light position 
		and orientation is to be affected by the scene hierarchy.
	 */
	//----------------------------------------------------------------------------------------------
	//SceneLightNode* pLightNode = new SceneLightNode(NULL, "LightNode");
	//pLightNode->SetLight(pDefaultLight);
	//pShadowGroupNode->AttachChild(pLightNode);
	//RegisterNode(pLightNode);

	//----------------------------------------------------------------------------------------------
	/** Initialise shadow map effect
	    Note that the shadow effect requires two imporatant input parameters: a camera for the shadow projection, 
	 	and a visible object set comprising of the shadow casting object set.
	 */
	//----------------------------------------------------------------------------------------------
	Camera* pShadowCamera = new Camera("ShadowCamera");
	*pShadowCamera = *m_pCamera;
	pShadowCamera->SetFrustum(Mathf::DegreeToRadian(45), 1.25f, 1.0f, 100.0f);
	pShadowCamera->LookAt(pDefaultLight->GetPosition(), Vector3f::Zero, Vector3f::Up);
	pShadowCamera->Update();

	//----------------------------------------------------------------------------------------------
	/** To generate the shadow casting object set, we create a new instance of the current culler
		and attach the shadow projection camera to it. We then compute the visibility set, which
		should contain all objects in the projection frustum.
	 */
	//----------------------------------------------------------------------------------------------
	CullerPtr pShadowCuller = m_pCuller->CreateInstance();
	pShadowCuller->SetCamera(pShadowCamera);
	pShadowCuller->SetRootNode(pShadowGroupNode);
	pShadowCuller->Update();
	VisibleEntitySetPtr pShadowCasterSet = pShadowCuller->ComputeVisibleSet();

	//----------------------------------------------------------------------------------------------
	/** Create new effect, with a shadow map of size 1024 x 1024; attach the effect
	    to the root node, as this is a global effect, i.e., and effect that affects
	    the children of the node hosting it.
	 */
	//----------------------------------------------------------------------------------------------
	m_pShadowMapEffect = new ShadowEffect("ProjectionMap", pShadowCamera, 2048, 2048);
	m_pShadowMapEffect->Initialise(m_pRenderDevice);
	m_pShadowMapEffect->SetRenderer(m_pRenderer);
	m_pShadowMapEffect->SetShadowCasterSet(pShadowCasterSet);
	EffectPtr pEffect(m_pShadowMapEffect);
	pShadowGroupNode->AttachEffect(pEffect);

	//----------------------------------------------------------------------------------------------
	/** Initialise the environment mapping effect (reflection). Note that this effect
		is can be both used as a global and a local effect.
	 */
	//----------------------------------------------------------------------------------------------
	m_pReflectionEffect = new ReflectionEffect("ReflectionMap", "envmap.dds", 0.5f);
	m_pReflectionEffect->Initialise(m_pRenderDevice);


	// create geometry for simulated bodies
	SpacePtr pSpace = m_pEnvironment->Space;
	TEnumerator<BodyPtr>& enumBodies = pSpace->GetBodies();
	while (enumBodies.HasMoreElements())
	{
		BodyPtr pBody = enumBodies.NextElement();
		Meson::Gravitas::Geometry::GeometryPtr pGeometryPhysical = pBody->GetGeometry();
		Meson::Vistas::GeometryPtr pGeometryVisual;
		if (pGeometryPhysical->GetTypeName() == "Sphere")
		{
			Real rRadius;
			pGeometryPhysical->GetProperty("Radius", rRadius);
			pGeometryVisual = MeshForge::CreateSphereMesh((float) rRadius, 32);
		}
		if (pGeometryPhysical->GetTypeName() == "Box")
		{
			TVector3<Real> vecExtent;
			pGeometryPhysical->GetProperty("Extent", vecExtent);
			pGeometryVisual = MeshForge::CreateBoxMesh(
				(float) vecExtent.X * 2.0f, (float) vecExtent.Y * 2.0f, (float) vecExtent.Z * 2.0f);
		}
		if (pGeometryPhysical->GetTypeName() == "Cylinder")
		{
			Real rRadius, rHeight;
			pGeometryPhysical->GetProperty("Radius", rRadius);
			pGeometryPhysical->GetProperty("Height", rHeight);
			pGeometryVisual = MeshForge::CreateCylinderMesh((float) rRadius, (float) rHeight, 32, true);
		}
		else if (pGeometryPhysical->GetTypeName() == "Halfspace")
		{
			pGeometryVisual = MeshForge::CreatePlaneMesh(
				Vector3f::Up, 0.0f, 32.f, 32.0f, 8.0f, 8.0f);
		}
		else if (pGeometryPhysical->GetTypeName() == "ConvexPolyhedron")
		{
			String strWavefrontModelFilename;
			pGeometryPhysical->GetProperty("WavefrontModelFilename", strWavefrontModelFilename);
			if (strWavefrontModelFilename.Length() > 0)
				pGeometryVisual = MeshForge::CreateMeshFromFile(strWavefrontModelFilename);
		}
		else if (pGeometryPhysical->GetTypeName() == "TriangleMesh")
		{
			String strWavefrontModelFilename;
			pGeometryPhysical->GetProperty("WavefrontModelFilename", strWavefrontModelFilename);
			if (strWavefrontModelFilename.Length() > 0)
				pGeometryVisual = MeshForge::CreateMeshFromFile(strWavefrontModelFilename);
		}

		// attach material to geometry
		if (pGeometryVisual->GetMaterial() == NULL)
		{
			MaterialPtr pMaterial
				= m_pVistasEngine->GetMaterialManager()->GetMaterial(
					pBody->GetMaterial()->GetId());
			if (pMaterial == NULL)
				pMaterial = pMaterialDefault;
			pGeometryVisual->SetMaterial(pMaterial);
		}

		// create geometry node, bind geometry and attach to group node
		SceneGeometryNode* pGeometryNode = new SceneGeometryNode(pBody->GetId());
		pGeometryNode->SetGeometry(pGeometryVisual);
		pObjectGroupNode->AttachChild(pGeometryNode);

		// attach light and reflection effect
		pGeometryNode->AttachLight(pDefaultLight);
		//pGeometryNode->AttachEffect(EffectPtr(m_pReflectionEffect));

		// register geometry node
		RegisterNode(pGeometryNode);

		// add to sandbox entity list
		SandboxEntity sandboxEntity(pBody, pGeometryNode);
		m_listSandboxEntities.Add(sandboxEntity);
	}

	// update shadow caster set
	pShadowCuller->Update();
	m_pShadowMapEffect->SetShadowCasterSet(pShadowCuller->ComputeVisibleSet());

	//----------------------------------------------------------------------------------------------
	/** Initialise the skyvolume effect.
	 */
	//----------------------------------------------------------------------------------------------
	m_pSkyEffect = new SkyEffect(m_pVistasEngine->GetInstance()->GetTextureManager()->GetTexture("ReflectionMap"));
	m_pSkyEffect->Initialise(m_pRenderDevice);
	pEffect = m_pSkyEffect;
	pSkyBoxGeometryNode->AttachEffect(pEffect);

	//----------------------------------------------------------------------------------------------
	/** Force scene graph to update its graphical and spatial states.
	 */
	//----------------------------------------------------------------------------------------------
	LightRenderState* pLightRenderState = m_pRenderDevice->GetLightRenderState();
	pLightRenderState->SetAmbient(Colour(0.2f, 0.2f, 0.2f, 1));
	m_pRenderDevice->SetLightRenderState(pLightRenderState);
	m_pRootNode->UpdateGeometryState(true, 0);
	m_pRootNode->UpdateRenderState(true, m_pRenderDevice->GetRenderContext(IRenderDevice::RTT_GLOBAL_CONTEXT), LightListPtr(NULL));

	// set Vistas-based instrumentation device
	Meson::Gravitas::Instrumentation::InstrumentationDevicePtr pInstrumentationDevice(
		new SandboxInstrumentationDevice(m_pRenderDevice));
	m_pGravitasEngine->GetInstrumentationManager()->SetDevice(pInstrumentationDevice);

	// initialise visual entities prior to running simulation
	for (size_t unIndex = 0; unIndex < m_listSandboxEntities.Size(); unIndex++)
		m_listSandboxEntities[unIndex].Update();
}
//----------------------------------------------------------------------------------------------
void GravitasSandboxApplication::DestroyScene(void)
{
	m_pShadowMapEffect->Dispose(m_pRenderDevice);
	m_pReflectionEffect->Dispose(m_pRenderDevice);
	m_pSkyEffect->Dispose(m_pRenderDevice);
}
//----------------------------------------------------------------------------------------------

void GravitasSandboxApplication::SetSimulationFilename(const String& p_strSimulationFilename)
{
	m_strSimulationFilename = p_strSimulationFilename;
}

void GravitasSandboxApplication::InitialiseGravitas(void)
{
	m_pGravitasEngine = GravitasEngine::GetInstance();

	// load Gravitas plugins
	m_pGravitasEngine->LoadPlugIn("CoreConstraints.dll");
	m_pGravitasEngine->LoadPlugIn("CoreForces.dll");
	m_pGravitasEngine->LoadPlugIn("CoreGeometry.dll");
	m_pGravitasEngine->LoadPlugIn("CoreIntegrators.dll");
	m_pGravitasEngine->LoadPlugIn("CoreSimulators.dll");
	m_pGravitasEngine->LoadPlugIn("CoreSpaces.dll");
}

void GravitasSandboxApplication::ShutdownGravitas(void)
{
	m_pGravitasEngine->GetInstrumentationManager()->SetDevice(
		Meson::Gravitas::Instrumentation::InstrumentationDevicePtr());

	// unload Gravitas plugins
	m_pGravitasEngine->UnloadPlugIn("CoreSpaces.dll");
	m_pGravitasEngine->UnloadPlugIn("CoreSimulators.dll");
	m_pGravitasEngine->UnloadPlugIn("CoreIntegrators.dll");
	m_pGravitasEngine->UnloadPlugIn("CoreGeometry.dll");
	m_pGravitasEngine->UnloadPlugIn("CoreForces.dll");
	m_pGravitasEngine->UnloadPlugIn("CoreConstraints.dll");

	GravitasEngine::Delete();
}

void GravitasSandboxApplication::OnIdle(void)
{
	if (m_bRunning)
	{
		// update simulation
		double dCurrentTime = m_stopWatchPhysics.GetElapsedTime().TotalSeconds();
		double dDeltaTime = TMaths<double>::Min(dCurrentTime - m_dCurrentTime, 0.005);
		m_dCurrentTime = dCurrentTime;

		// update visual geometry through entities
		m_pSimulator->Step(m_dCurrentTime, dDeltaTime);
		for (size_t unIndex = 0; unIndex < m_listSandboxEntities.Size(); unIndex++)
			m_listSandboxEntities[unIndex].Update();

		// picking
		if (m_pBodyPicking != NULL)
		{
			TVector3<Real> vecDirection = m_pCamera->GetPickingDirection(m_fLastMouseX, m_fLastMouseY);
			TPoint3<Real>& ptBodyPosition = m_pBodyPicking->GetKineticProperties().Position;
			TPoint3<Real> ptMousePosition = TPoint3<Real>::Origin + m_pCamera->GetPosition() + vecDirection * m_rBodyPickingDepth;

			TVector3<Real> vecForceValue = ptMousePosition - ptBodyPosition;
			vecForceValue *= m_rBodyPickingDepth;
			vecForceValue *= m_pBodyPicking->GetMassProperties().Mass;

			ForceManager* pForceManager = m_pGravitasEngine->GetForceManager();
			IForceFactory* pForceFactory = pForceManager->GetForceFactory("Basic");
			ForcePtr pForce = pForceFactory->CreateForce();
			pForce->SetProperty("Instantaneous", true);
			pForce->SetProperty("ApplicationPoint", TPoint3<Real>::Origin);
			pForce->SetProperty("ForceValue", vecForceValue);
			m_pBodyPicking->GetForceAccumulator().AddForce(pForce);
		}
	}

	VistasSandbox::OnIdle();
}

void GravitasSandboxApplication::OnFrameDraw(void)
{
	if (m_stopWatchGraphics.GetElapsedTime().TotalSeconds() < 1.0 / 60.0)
	{
		m_pCamera->SetFrame(m_vec3CameraPosition, m_vec3CameraAxes[0], m_vec3CameraAxes[1], m_vec3CameraAxes[2]);
		m_pViewport->Update();
		return;
	}
	m_stopWatchGraphics.Reset();

	// Update camera
	if (m_nCameraLock >= 0)
	{	
		Vector3f vecPosition = m_pCamera->GetPosition();
		m_pCamera->LookAt(vecPosition,
			m_listSandboxEntities[m_nCameraLock].Body->GetKineticProperties().Position.ToVector(),
			Vector3f::Up);
	}
	else
		m_pCamera->SetFrame(m_vec3CameraPosition, m_vec3CameraAxes[0], m_vec3CameraAxes[1], m_vec3CameraAxes[2]);
	m_pViewport->Update();

	// Clear colour, stencil and depth buffers 
	m_pRenderDevice->ClearBuffers(BackgroundColour, 1.0f, 0);			

	// Start rendering scene
	m_pRenderDevice->BeginScene();

	// Update and synchronise transformations
	m_pRootNode->UpdateGeometryState(true, m_dfDeltaTime);

	// Compute visible set
	VisibleEntitySetPtr pVisibleSet(m_pCuller->ComputeVisibleSet());
	
	// Set visible set and render
	m_pRenderer->SetVisibleSet(pVisibleSet);
	m_pRenderer->Draw();

	// -- Instrumentation rendering - start ----------------------------------

	// Switch to instrumentation shaders
	IShaderProgram* pShaderProgram
		= m_pRenderDevice->GetShaderProgramManager()->GetShaderProgram(
			"ShaderCatalogue", "vs_instrumentation");
	m_pRenderDevice->BindShaderProgram(pShaderProgram);
	pShaderProgram
		= m_pRenderDevice->GetShaderProgramManager()->GetShaderProgram(
			"ShaderCatalogue", "ps_instrumentation");
	m_pRenderDevice->BindShaderProgram(pShaderProgram);

	// render gravitas engine instrumentation
	m_pGravitasEngine->GetInstrumentationManager()->RenderInstrumentation(
		m_pEnvironment, m_dCurrentTime);

	// -- Instrumentation rendering - end ------------------------------------

	// Finish scene rendering
	m_pRenderDevice->EndScene();

	// Present back buffer
	m_pWindow->Present();
}

void GravitasSandboxApplication::OnKeyDown(KeyCode p_eKey)
{
	VistasSandbox::OnKeyDown(p_eKey);

	// keys
	if (p_eKey == KC_I)
	{
		Meson::Gravitas::Instrumentation::InstrumentationManager* pInstrumentationManager
			= m_pGravitasEngine->GetInstrumentationManager();
		if (pInstrumentationManager->IsEnabled(Meson::Gravitas::Instrumentation::InstrumentationFlags::All))
		{
			pInstrumentationManager->Disable(Meson::Gravitas::Instrumentation::InstrumentationFlags::All);
			m_pGravitasEngine->Logger().Out << "Instumentation off\n";
		}
		else
		{
			pInstrumentationManager->Enable(Meson::Gravitas::Instrumentation::InstrumentationFlags::All);
			m_pGravitasEngine->Logger().Out << "Instumentation on\n";
		}
	}
	else if (p_eKey == KC_C)
	{
		++m_nCameraLock;
		if (m_nCameraLock >= (int) m_listSandboxEntities.Size())
			m_nCameraLock = -1;
	}
	else if (p_eKey == KC_ESCAPE)
	{
		// reset bodies
		for (size_t unIndex = 0; unIndex < m_listSandboxEntities.Size(); unIndex++)
			m_listSandboxEntities[unIndex].Reset();

		// reset constraints
		for (size_t unIndex = 0; unIndex < m_pEnvironment->Constraints->Size(); unIndex++)
			m_pEnvironment->Constraints->GetElement(unIndex)->SetBroken(false);

		m_dCurrentTime = 0.0;
		m_pEnvironment->Space->Rebuild();
	}
	else if (p_eKey == KC_F5)
	{
		m_bRunning = !m_bRunning;
	}
}

void GravitasSandboxApplication::OnMouseMove(float p_fMouseX, float p_fMouseY)
{
	VistasSandbox::OnMouseMove(p_fMouseX, p_fMouseY);
}

void GravitasSandboxApplication::OnMouseDown(IWindowMessageHandler::MouseKey p_eKey)
{
	VistasSandbox::OnMouseDown(p_eKey);

	if (p_eKey == IWindowMessageHandler::MK_RIGHT)
	{
		m_pBodyPicking = NULL;

		TVector3<Real> vecDirection = m_pCamera->GetPickingDirection(
			m_vec2CameraMouseTrack.X, m_vec2CameraMouseTrack.Y);
		Meson::Gravitas::Geometry::Ray rayPicking(
			TPoint3<Real>::Origin + m_pCamera->GetPosition(), vecDirection);
		Meson::Gravitas::Space::BodyRayIntersectionArrayList listIntersections;
		this->m_pEnvironment->Space->IntersectRay(rayPicking, listIntersections);
		
		for (size_t unIndex = 0; unIndex < listIntersections.Size(); unIndex++)
		{
			Meson::Gravitas::Space::BodyRayIntersection& intersection = listIntersections[0];
			BodyPtr pBody = intersection.Body;
			if (!pBody->GetMassProperties().HasInfiniteMass())
			{
				m_pBodyPicking = pBody;
				m_pBodyPicking->GetKineticProperties().Active = true;
				m_pBodyPicking->GetKineticProperties().LastActive = m_dCurrentTime;
				m_rBodyPickingDepth = (pBody->GetKineticProperties().Position.ToVector()
					- m_pCamera->GetPosition()).Length();
				break;
			}
		}
	}
}

void GravitasSandboxApplication::OnMouseUp(IWindowMessageHandler::MouseKey p_eKey)
{
	VistasSandbox::OnMouseUp(p_eKey);

	if (p_eKey == IWindowMessageHandler::MK_RIGHT)
		m_pBodyPicking = NULL;
}

void GravitasSandboxApplication::LoadEnvironment(void)
{
	InputStreamPtr pInputStream(new FileInputStream(m_strSimulationFilename));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));

	// load geometry
	Meson::Gravitas::Geometry::GeometryManager* pGeometryManager
		= m_pGravitasEngine->GetGeometryManager();
	pGeometryManager->LoadGeometries(pLookaheadInputStream);

	// load materials
	Meson::Gravitas::Materials::MaterialManager* pMaterialManager
		= m_pGravitasEngine->GetMaterialManager();
	pMaterialManager->LoadMaterials(pLookaheadInputStream);

	// load forces
	Meson::Gravitas::Force::ForceManager* pForceManager
		= m_pGravitasEngine->GetForceManager();
	pForceManager->LoadForces(pLookaheadInputStream);

	// load simulator
	Meson::Gravitas::Simulation::SimulationManager* pSimulationManager
		= m_pGravitasEngine->GetSimulationManager();
	m_pSimulator = pSimulationManager->LoadSimulator(pLookaheadInputStream);

	// load environment
	m_pEnvironment = pSimulationManager->LoadEnvironment(pLookaheadInputStream);

	pLookaheadInputStream->Close();

	// initialise simulator
	m_pSimulator->Initialise(m_pEnvironment);
	m_stopWatchPhysics.Reset();
	m_stopWatchGraphics.Reset();
}
