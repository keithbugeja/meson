#include <Windows.h>

#include "StopWatch.h"
#include "LinkedList.h"
#include "Directory.h"

#include "VstVistasEngine.h"

#include "GrvGravitasEngine.h"
#include "GrvRigidBody.h"

#include "MainWindowMessageHandler.h"
#include "Model3D.h"
#include "Entity.h"
#include "TestInstrumentationDevice.h"

#include "GrvTriangle.h"

using namespace Meson::Common;
using namespace Meson::Common::Config;
using namespace Meson::Common::Maths;
using namespace Meson::Common::System;

using namespace Meson::Vistas;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Event;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Space;
using namespace Meson::Gravitas::Simulation;

VistasEngine* g_pVistasEngine;
IRenderDevice* g_pRenderDevice;
WindowRenderTarget* g_pWindowRenderTarget;
MainWindowMessageHandler* g_pMainWindowMessageHandler;
Camera* g_pCamera;
IShaderProgram* g_pVertexProgram;
TextureBufferPtr g_pTextureGroundDiffuse, g_pTextureGroundNormal, g_pTextureBodyDiffuse, g_pTextureBodyNormal;
Light g_light("light");
IShaderProgram* g_pVertexProgramBody;
IShaderProgram* g_pPixelProgramBody;
IShaderProgram* g_pVertexProgramInstrumentation;
IShaderProgram* g_pPixelProgramInstrumentation;


GravitasEngine* g_pGravitasEngine;
SpacePtr g_pSpace;
ConstraintListPtr g_pConstraintList;
SimulatorPtr g_pSimulator;
EntityListPtr g_pEntityList;
EnvironmentPtr g_pEnvironment;

class MyCollisionListener
	: public ICollisionListener
{
	void OnCollisionEvent(CollisionEventPtr p_pCollisionEvent)
	{
		switch(p_pCollisionEvent->Type)
		{
			case CollisionEventType::Start:
				g_pGravitasEngine->Logger().Out << "Collision start!\n";
				break;
			case CollisionEventType::Ongoing:
				g_pGravitasEngine->Logger().Out << "Collision ongoing!\n";
				break;
			case CollisionEventType::End:
				g_pGravitasEngine->Logger().Out << "Collision end!\n";
				break;
			default:
				g_pGravitasEngine->Logger().Out << "Collision unknown!\n";
				break;
		}
	};
};

void InitialiseDisplay(void)
{
	// get Vistas engine instance
	g_pVistasEngine = VistasEngine::GetInstance();

	// load DX9 renderer
	g_pVistasEngine->LoadPlugIn("DirectX9Renderer.dll");
	g_pVistasEngine->LoadPlugIn("CoreCullers.dll");

	//----------------------------------------------------------------------------------------------
	// NEW VISTAS INIT CODE
	//----------------------------------------------------------------------------------------------
	// Check Direct3D9 Device availability
	if (!g_pVistasEngine->GetRenderDeviceManager()->IsAvailable("DirectX9"))
		throw new MesonException("DX9 renderer device should be available!", __FILE__, __LINE__);
	
	// Query configuration for device
	Configuration *pConfigurationDevice = g_pVistasEngine->GetRenderDeviceManager()->GetConfiguration("DirectX9");

	if (MessageBoxA(NULL, "Use full-screen mode?", "Windowing Mode", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		pConfigurationDevice->SetLong("Initialise.CreateWindow.Width", 800);
		pConfigurationDevice->SetLong("Initialise.CreateWindow.Height", 600);
		pConfigurationDevice->SetBool("Initialise.CreateWindow.FullScreen", true);
	}
	else
	{
		pConfigurationDevice->SetLong("Initialise.CreateWindow.Width", 800);
		pConfigurationDevice->SetLong("Initialise.CreateWindow.Height", 600);
	}

	// Initialise device
	g_pWindowRenderTarget = g_pVistasEngine->GetRenderDeviceManager()->Startup("DirectX9");
	
	// Only after initialisation can a valid IRenderDevice* be acquired
	g_pRenderDevice = g_pVistasEngine->GetRenderDeviceManager()->GetActive();
	//----------------------------------------------------------------------------------------------
	// NEW VISTAS INIT CODE
	//----------------------------------------------------------------------------------------------

	g_pMainWindowMessageHandler
		= new MainWindowMessageHandler(g_pWindowRenderTarget);

	// Disable culling
	//CullRenderState cullRenderState;
	//cullRenderState.SetFaceWindingMode(CullRenderState::CRSFW_CW);
	//g_pRenderDevice->SetCullRenderState(&cullRenderState);

	// Initialise a camera and a viewport
	g_pCamera = new Camera("MainCamera");
	g_pCamera->SetFrustum(Mathf::DegreeToRadian(45), 1.25f, 1, 1000);
	g_pCamera->LookAt(Vector3f(0, 5, 10), Vector3f::Zero, Vector3f::Up);
	g_pWindowRenderTarget->CreateViewport(g_pCamera, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0, 1);
	Viewport* pViewport = g_pWindowRenderTarget->GetViewport(0);
	g_pRenderDevice->SetViewport(pViewport);
	pViewport->Update();

	if (!g_pRenderDevice->GetShaderProgramManager()->IsShaderProfileSupported(IShaderProgram::SPT_VERTEX, "vs_3_0"))
		throw new MesonException("No shader support!", __FILE__, __LINE__ );

	// Create vertex programs
	g_pVertexProgramBody = g_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(
		"..\\Gravitas\\GravitasTest\\Resources\\vs_parallax_omni.hlsl", IShaderProgram::SPT_VERTEX, String("ShaderCatalogue"), String("VertexShaderBody"));
	g_pPixelProgramBody = g_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(
		"..\\Gravitas\\GravitasTest\\Resources\\ps_parallax_omni.hlsl", IShaderProgram::SPT_PIXEL, String("ShaderCatalogue"), String("PixelShaderBody"));
	g_pVertexProgramInstrumentation = g_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(
		"..\\Gravitas\\GravitasTest\\Resources\\vs_instrumentation.hlsl", IShaderProgram::SPT_VERTEX, String("ShaderCatalogue"), String("VertexShaderInstrumentation"));
	g_pPixelProgramInstrumentation = g_pRenderDevice->GetShaderProgramManager()->CreateShaderProgramFromFile(
		"..\\Gravitas\\GravitasTest\\Resources\\ps_instrumentation.hlsl", IShaderProgram::SPT_PIXEL, String("ShaderCatalogue"), String("PixelShaderInstrumentation"));

	// Set vertex program details and compile
	g_pVertexProgramBody->SetProfile("vs_3_0");
	g_pPixelProgramBody->SetProfile("ps_3_0");
	g_pVertexProgramInstrumentation->SetProfile("vs_3_0");
	g_pPixelProgramInstrumentation->SetProfile("ps_3_0");

	g_pVertexProgramBody->SetEntryPoint("vs_main");
	g_pPixelProgramBody->SetEntryPoint("ps_main");
	g_pVertexProgramInstrumentation->SetEntryPoint("vs_main");
	g_pPixelProgramInstrumentation->SetEntryPoint("ps_main");

	//pVertexProgram->SetSource(strVProgram);
	g_pVertexProgramBody->Compile();
	g_pPixelProgramBody->Compile();
	g_pVertexProgramInstrumentation->Compile();
	g_pPixelProgramInstrumentation->Compile();

	// Add render constants
	g_pVertexProgramBody->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD_VIEW_PROJECTION);
	g_pVertexProgramBody->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD);
	g_pVertexProgramBody->AddRenderConstant(RenderConstantType::RCT_MATRIX_INVERSE_WORLD);
	g_pVertexProgramBody->AddRenderConstant(RenderConstantType::RCT_VECTOR_LIGHT_POSITION);
	g_pVertexProgramBody->AddRenderConstant(RenderConstantType::RCT_VECTOR_VIEWER_POSITION);

	g_pPixelProgramBody->AddRenderConstant(RenderConstantType::RCT_VECTOR_LIGHT_AMBIENT);
	g_pPixelProgramBody->AddRenderConstant(RenderConstantType::RCT_VECTOR_LIGHT_DIFFUSE);
	g_pPixelProgramBody->AddRenderConstant(RenderConstantType::RCT_VECTOR_LIGHT_SPECULAR);
	g_pPixelProgramBody->AddRenderConstant(RenderConstantType::RCT_VECTOR_LIGHT_ATTENUATION);

	g_pVertexProgramInstrumentation->AddRenderConstant(RenderConstantType::RCT_MATRIX_WORLD_VIEW_PROJECTION);

	// custom shader variables
	g_pPixelProgramBody->AddVariable(new ShaderProgramVariable("objectColour"));
	g_pVertexProgramInstrumentation->AddVariable(new ShaderProgramVariable("instrumentationColour"));

	// textures
	g_pTextureGroundDiffuse = g_pRenderDevice->GetHardwareResourceManager()->CreateTextureBufferFromFile(
		"..\\Gravitas\\GravitasTest\\Resources\\Floor_Diffuse.tga");
	g_pTextureGroundNormal = g_pRenderDevice->GetHardwareResourceManager()->CreateTextureBufferFromFile(
		"..\\Gravitas\\GravitasTest\\Resources\\Floor_Normal.tga");
	g_pTextureBodyDiffuse = g_pRenderDevice->GetHardwareResourceManager()->CreateTextureBufferFromFile(
		"..\\Gravitas\\GravitasTest\\Resources\\Copper_Diffuse.tga");
	g_pTextureBodyNormal = g_pRenderDevice->GetHardwareResourceManager()->CreateTextureBufferFromFile(
		"..\\Gravitas\\GravitasTest\\Resources\\Copper_Normal.tga");


	g_light.SetType(Light::LT_DIRECTIONAL);
	g_light.SetAmbient(Meson::Vistas::Colour(0.0f,0.0f,0.1f,0.0f));
	g_light.SetDiffuse(Meson::Vistas::Colour(0.8f,0.8f,0.8f,0.0f));
	g_light.SetSpecular(Meson::Vistas::Colour(0.8f,0.8f,0.8f, 32.0f));
	g_light.SetPosition(Vector3f(0,0,5));
	//g_light.SetDirection(Vector3f(-1,-1,-1), true);
	g_light.SetAttenuation(1.0f, 0, 0);
	g_light.SetRange(32);
	g_light.SetLight(true);

	// render states

	// Light RS
	LightRenderStatePtr pLightRenderState(new LightRenderState());
	pLightRenderState->SetLighting(true);
	pLightRenderState->SetAmbient(Meson::Vistas::Colour::Black);

	g_pRenderDevice->SetLight(0, &g_light);
	g_pRenderDevice->SetLightRenderState(pLightRenderState);

	// cull RS
	CullRenderStatePtr pCullRenderState(new CullRenderState());
	pCullRenderState->SetCulling(true);
	pCullRenderState->SetFaceWindingMode(CullRenderState::CRSFW_CW);
	pCullRenderState->SetFaceCullingMode(CullRenderState::CRSFC_BACK);
	g_pRenderDevice->SetCullRenderState(pCullRenderState);
}

void TerminateDisplay(void)
{
	delete g_pCamera;
	/** Terminate window */
	g_pMainWindowMessageHandler->Terminate();

	/** Shutdown rendering device */
	g_pVistasEngine->GetRenderDeviceManager()->Shutdown();

	g_pVistasEngine->UnloadPlugIn("CoreCullers.dll");
	g_pVistasEngine->UnloadPlugIn("DirectX9Renderer.dll");

	VistasEngine::Delete();
}


void InitialisePhysics(void)
{
	// get Gravitas engine instance
	g_pGravitasEngine = GravitasEngine::GetInstance();

	// load Gravitas plugins
	g_pGravitasEngine->LoadPlugIn("CoreConstraints.dll");
	g_pGravitasEngine->LoadPlugIn("CoreForces.dll");
	g_pGravitasEngine->LoadPlugIn("CoreGeometry.dll");
	g_pGravitasEngine->LoadPlugIn("CoreIntegrators.dll");
	g_pGravitasEngine->LoadPlugIn("CoreSimulators.dll");
	g_pGravitasEngine->LoadPlugIn("CoreSpaces.dll");

	// prepare constraint list
	g_pConstraintList = new ConstraintArrayList();
	g_pMainWindowMessageHandler->SetConstraintList(g_pConstraintList);

	// prepare space representation
	SpaceManager* pSpaceManager = g_pGravitasEngine->GetSpaceManager();
	ISpaceFactory* pSpaceFactory = pSpaceManager->GetSpaceFactory("Grid");
	g_pSpace = pSpaceFactory->CreateSpace();

	// properties for sweep-and-prune space
	TArrayList< TVector3<Real> > listAxes;
	listAxes.Add(TVector3<Real>::Right);
	listAxes.Add(TVector3<Real>::Out);
	g_pSpace->SetProperty("Axes", listAxes);

	g_pSpace->Rebuild();
	g_pMainWindowMessageHandler->SetSpace(g_pSpace);

	// prepare integrator
	KineticsManager* pKineticsManager = g_pGravitasEngine->GetKineticsManager();
	KineticIntegratorPtr pKineticIntegrator(pKineticsManager->GetKineticIntegrator("Euler"));

	// prepare constraint solver
	ConstraintManager* pConstraintManager = g_pGravitasEngine->GetConstraintManager();
	IConstraintSolver* pConstraintSolver = pConstraintManager->GetConstraintSolver("Sequential");

	// prepare environment
	g_pEnvironment = new Environment();
	g_pEnvironment->Constraints = g_pConstraintList;
	g_pEnvironment->Space = g_pSpace;
	g_pEnvironment->KineticIntegrator = pKineticIntegrator;
	g_pEnvironment->ConstraintSolver = pConstraintSolver;

	// prepare simulation
	SimulationManager* pSimulationManager = g_pGravitasEngine->GetSimulationManager();
	ISimulatorFactory* pSimulatorFactory
		= pSimulationManager->GetSimulatorFactory("Basic");
	g_pSimulator = pSimulatorFactory->CreateSimulator();
	g_pSimulator->Initialise(g_pEnvironment);

	/*
	// test - load materials
	Meson::Gravitas::Materials::MaterialManager* pMaterialManager
		= g_pGravitasEngine->GetMaterialManager();
	pMaterialManager->LoadMaterials("..\\Gravitas\\GravitasTest\\Resources\\materials.grm");

	// test - load geometry
	Meson::Gravitas::Geometry::GeometryManager* pGeometryManager
		= g_pGravitasEngine->GetGeometryManager();
	pGeometryManager->LoadGeometries("..\\Gravitas\\GravitasTest\\Resources\\geometries.grg");

	// test - load forces
	Meson::Gravitas::Force::ForceManager* pForceManager
		= g_pGravitasEngine->GetForceManager();
	pForceManager->LoadForces("..\\Gravitas\\GravitasTest\\Resources\\forces.grf");
	*/
}

void TerminatePhysics(void)
{
	g_pEntityList = NULL;

	// unload Gravitas plugins
	g_pGravitasEngine->UnloadPlugIn("CoreSpaces.dll");
	g_pGravitasEngine->UnloadPlugIn("CoreSimulators.dll");
	g_pGravitasEngine->UnloadPlugIn("CoreIntegrators.dll");
	g_pGravitasEngine->UnloadPlugIn("CoreGeometry.dll");
	g_pGravitasEngine->UnloadPlugIn("CoreForces.dll");
	g_pGravitasEngine->UnloadPlugIn("CoreConstraints.dll");

	GravitasEngine::Delete();
}

void ProtectedMain(void)
{
	InitialiseDisplay();

	InitialisePhysics();
	
	// prepare body geometry
	Meson::Gravitas::Geometry::GeometryManager *pGeometryManager = g_pGravitasEngine->GetGeometryManager();
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryBox = pGeometryManager->GetGeometryFactory("Box");
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryHalfspace = pGeometryManager->GetGeometryFactory("Halfspace");

	Meson::Gravitas::Geometry::GeometryPtr pGeometryGround = pGeometryFactoryHalfspace->CreateGeometry();

	// prepare bodies
	g_pEntityList = new TArrayList<Entity>();
	g_pMainWindowMessageHandler->SetEntityList(g_pEntityList);

	// ground material;
	Meson::Gravitas::Materials::MaterialPtr pMaterialGround(new Meson::Gravitas::Materials::Material());
	pMaterialGround->RestitutionCoefficient = 0.1;
	pMaterialGround->StaticFrictionCoefficient = 0.9;
	pMaterialGround->DynamicFrictionCoefficient = 0.9;
	//pMaterialGround->SurfacePerturbationCoefficient = 0.01;

	// ground
	RigidBody* pRigidBodyGround = new RigidBody();
	g_pSpace->AddBody(BodyPtr(pRigidBodyGround));
	pRigidBodyGround->SetGeometryAndMassProperties(pGeometryGround, (Real) 1.0);
	pRigidBodyGround->GetMassProperties().SetInfiniteMass();
	pRigidBodyGround->SetMaterial(pMaterialGround);

	pRigidBodyGround->GetKineticProperties().Position = TVector3<Real>(0.0, -3.0, 0.0);
	//pRigidBodyGround->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Right, TMaths<Real>::Pi * 0.1);
	//pRigidBodyGround->GetKineticProperties().LinearVelocity = TVector3<Real>::Up * 0.5;
	//pRigidBodyGround->GetKineticProperties().AngularVelocity = TVector3<Real>::Up * TMaths<Real>::PiHalf;


	// Sampler RS	
	SamplerState samplerStateDiffuse, 
				 samplerStateNormal;

	SamplerRenderStatePtr pSamplerRenderStateGround(new SamplerRenderState())
		, pSamplerRenderStateBody(new SamplerRenderState());

	pSamplerRenderStateGround->AddSamplerUnit(0,
		TextureStatePtr(new TextureState(g_pTextureGroundDiffuse)),
		SamplerStatePtr(new SamplerState()));
	pSamplerRenderStateGround->AddSamplerUnit(1,
		TextureStatePtr(new TextureState(g_pTextureGroundNormal)),
		SamplerStatePtr(new SamplerState()));

	pSamplerRenderStateBody->AddSamplerUnit(0,
		TextureStatePtr(new TextureState(g_pTextureBodyDiffuse)),
		SamplerStatePtr(new SamplerState()));
	pSamplerRenderStateBody->AddSamplerUnit(1,
		TextureStatePtr(new TextureState(g_pTextureBodyNormal)),
		SamplerStatePtr(new SamplerState()));

	// prepare visual models (old code with Model3D)
	Model3D* pModel3DSphere = new Model3D();
	Model3D* pModel3DBox = new Model3D();
	Model3D* pModel3DCylinder = new Model3D();
	pModel3DBox->MakeCube(Vector3f(0.6f, 0.5f, 0.7f), 1.0f);
	pModel3DSphere->MakeSphere(0.5f, 16);
	pModel3DCylinder->MakeCylinder(0.5f, 1.25f, 32);
	Model3D* pModel3DGround = new Model3D();
	pModel3DGround->MakeCube(Vector3f(100.0f, 0.0f, 100.0f), 40.0f);

	// prepare entities for ground
	Entity entityGround;
	entityGround.Body = pRigidBodyGround;
	entityGround.Model3D = pModel3DGround;
	g_pEntityList->Add(entityGround);

	// collision listener
	/*
	MyCollisionListener* pMyCollisionListener = new MyCollisionListener();
	g_pGravitasEngine->GetEventManager()->AddCollisionListener(CollisionListenerPtr(pMyCollisionListener));
	g_pGravitasEngine->GetEventManager()->CollisionEventFlags.Start = true;
	g_pGravitasEngine->GetEventManager()->CollisionEventFlags.End = true;
	*/

	// instrumentation
	InstrumentationDevicePtr pTestInstrumentationDevice(new TestInstrumentationDevice(g_pRenderDevice));
	g_pGravitasEngine->GetInstrumentationManager()->SetDevice(pTestInstrumentationDevice);
	//g_pGravitasEngine->GetInstrumentationManager()->Enable(
	//	InstrumentationFlags::Bodies | InstrumentationFlags::Geometry
	//	| InstrumentationFlags::Constraints);
	g_pGravitasEngine->GetInstrumentationManager()->Enable(InstrumentationFlags::All);

	// main loop
	StopWatch stopWatchPhysics, stopWatchGraphics;
	double dCurrentTime = stopWatchPhysics.GetElapsedTime().TotalSeconds();
	double dPreviousTime = dCurrentTime;

	// TEST
	//Triangle triangle(TPoint3<Real>::Origin, TPoint3<Real>(1.0, 0.0, 0.0), TPoint3<Real>(0.0, 1.0, 0.0));
	//TPoint3<Real> ptClosest = triangle.ClosestPoint(TPoint3<Real>(1.6, 1.6, 1.0));

	//float fTemp = 0.0;
	while(g_pMainWindowMessageHandler->Step())
	{
		dPreviousTime = dCurrentTime;
		dCurrentTime = stopWatchPhysics.GetElapsedTime().TotalSeconds();
		double dDeltaTime = dCurrentTime - dPreviousTime;
		//dDeltaTime = TMaths<Real>::Min(0.02, dDeltaTime);

		// log fps
		/*
		String strMessage = "FPS: ";
		strMessage += 1.0 / dDeltaTime;
		strMessage += "\n";
		g_pGravitasEngine->Logger().Out << strMessage;
		*/

		// step simulation
		g_pSimulator->Step(dCurrentTime, dDeltaTime);

		// do not render if inactive
		//if (!g_pWindowRenderTarget->IsActive())
		//	continue;

		if (stopWatchGraphics.GetElapsedTime().TotalSeconds() < 1.0 / 62.0)
			continue;

		stopWatchGraphics.Reset();

		// update camera
		if (g_pEntityList->Size() > 1)
		{
			static Vector3f vecLookAtOld = Vector3f::Zero;
			Vector3f vecLookAt;
			TVector3<Real>& ptPosition = (*g_pEntityList)[g_pEntityList->Size() - 1].Body->GetKineticProperties().Position;
			vecLookAt.Set((float) ptPosition.X, (float) ptPosition.Y, (float) ptPosition.Z);
			vecLookAt = vecLookAt * 0.05f + vecLookAtOld * 0.95f;
			vecLookAtOld = vecLookAt;
			
			Vector3f vecCameraOffset(g_pCamera->GetPosition() - vecLookAt);
			if (vecCameraOffset.Length() > 10.0f)
				vecCameraOffset -= vecCameraOffset.NormaliseCopy() * 10.0f;
			else
				vecCameraOffset.MakeZero();

			g_pCamera->SetPosition(g_pCamera->GetPosition() - vecCameraOffset);
			g_pCamera->LookAt(g_pCamera->GetPosition(), vecLookAt, Vector3f::Up);

			g_pRenderDevice->GetViewport()->Update();
		}

		// Clear colour, stencil and depth buffers 
		g_pRenderDevice->ClearBuffers(Meson::Vistas::Colour(0.0f, 0.0f, 0.5f, 1.0f).ToRGBA(), 1.0f, 0);
		
		// Start rendering scene
		g_pRenderDevice->BeginScene();

		//g_light.SetDirection(Vector3f(Mathf::Cos(fTemp), 0.0f, Mathf::Sin(fTemp)));
		//fTemp += 0.001f;
		g_pRenderDevice->SetLight(0, &g_light);

		// bind vertex shader for bodies
		g_pRenderDevice->BindShaderProgram(g_pVertexProgramBody);
		g_pRenderDevice->BindShaderProgram(g_pPixelProgramBody);

		for (size_t unIndex = 0; unIndex < g_pEntityList->Size(); unIndex++)
		{
			if (unIndex == 0)
				g_pRenderDevice->SetSamplerRenderState(pSamplerRenderStateGround);
			else
				g_pRenderDevice->SetSamplerRenderState(pSamplerRenderStateBody);

			Entity& entity = (*g_pEntityList)[unIndex];
			BodyPtr pBody = entity.Body;
			RigidBody* pRigidBody = (RigidBody*) (IBody*) pBody;

			Model3DPtr pModel = entity.Model3D;

			pModel->Transform.Translation = pRigidBody->GetKineticProperties().Position;
			pModel->Transform.Rotation = pRigidBody->GetKineticProperties().Orientation;
			pModel->SetColour(pRigidBody->GetKineticProperties().Active ? Vector4f(0.0f, 0.0f, 0.0f, 0.0f) : Vector4f(0.0f, 0.0f, 0.2f, 1.0f));
			pModel->Render(g_pRenderDevice);
		}

		// bind vertex shader for instrumentation
		g_pRenderDevice->BindShaderProgram(g_pVertexProgramInstrumentation);
		g_pRenderDevice->BindShaderProgram(g_pPixelProgramInstrumentation);

		// ray cast test
		Ray ray(TVector3<Real>(0.0, 10.0, 0.0), TVector3<Real>::Down);
		static BodyRayIntersectionArrayList listBodyRayIntersections;
		g_pSpace->IntersectRay(ray, listBodyRayIntersections);

		if (listBodyRayIntersections.Size() > 0)
		{
			pTestInstrumentationDevice->SetColour(Meson::Gravitas::Instrumentation::Colour::Green);
			pTestInstrumentationDevice->DrawArrow(listBodyRayIntersections[0].Position, ray.Source);
		}

		// render instrumentation
		if (g_pMainWindowMessageHandler->IsInstrumentationActive())
			g_pGravitasEngine->GetInstrumentationManager()->RenderInstrumentation(g_pEnvironment, (Real) dCurrentTime);

		// Finish scene rendering
		g_pRenderDevice->EndScene();

		// Present back buffer
		g_pWindowRenderTarget->Present();
	}

	delete pModel3DGround;
	delete pModel3DBox;

	//pRigidBodyBox->GetForceAccumulator().ClearForces();

	// clear instrumentation device
	pTestInstrumentationDevice = NULL;

	TerminatePhysics();

	TerminateDisplay();
}

void main(void)
{
	//ProtectedMain();

	try
	{
		ProtectedMain();
	}
	catch(MesonException* pMesonException)
	{
		MessageBoxA(NULL, pMesonException->GetMessageTrace(), "Meson Exception", 0);
	}
	catch (...)
	{
		MessageBoxA(NULL, "A general exception has occured.", "General Exception", 0);
	}
}