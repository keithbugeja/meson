#include "MainWindowMessageHandler.h"

#include "GrvGravitasEngine.h"
#include "GrvRigidBody.h"

#include "RagdollExample.h"
#include "CartExample.h"
#include "PolyhedronExample.h"
#include "TriangleMeshExample.h"

using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;

enum GeometryType
{
	Box,
	Sphere,
	Cylinder
};

enum JointType
{
	None,
	Spherical,
	Revolute,
	AngularMotor,
	Rigid
};

static Model3DPtr s_pModel3DSphere, s_pModel3DBox, s_pModel3DCylinder;

static GeometryType s_geometryType = Box;
static int s_nMultiple = 1;
static JointType s_jointType = None;
static Meson::Gravitas::Materials::MaterialPtr s_pMaterialBox,
	s_pMaterialSphere, s_pMaterialCylinder;

MainWindowMessageHandler::MainWindowMessageHandler(
	WindowRenderTarget* p_pWindowRenderTarget)
	: m_pWindowRenderTarget(p_pWindowRenderTarget)
	, m_pSpace()
	, m_pConstraintList()
	, m_pEntityList()
	, m_bInstrumentationActive(false)
{
	VistasEngine::GetInstance()->GetWindowMessageManager()->AttachMessageHandler(
		m_pWindowRenderTarget, this);
}

void MainWindowMessageHandler::SetSpace(SpacePtr p_pSpace)
{
	m_pSpace = p_pSpace;
}

void MainWindowMessageHandler::SetConstraintList(ConstraintListPtr p_pConstraintList)
{
	m_pConstraintList = p_pConstraintList;
}

void MainWindowMessageHandler::SetEntityList(EntityListPtr p_pEntityList)
{
	m_pEntityList = p_pEntityList;
}

void MainWindowMessageHandler::Terminate(void)
{
	VistasEngine::GetInstance()->GetWindowMessageManager()->DetachMessageHandler(
		m_pWindowRenderTarget);
}

void MainWindowMessageHandler::OnMouseDown(MouseKey p_eMouseKey)
{
	if (m_pSpace == NULL) return;
	
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();

	// prepare gravity force
	ForceManager* pForceManager = pGravitasEngine->GetForceManager();
	IForceFactory* pForceFectoryGravity = pForceManager->GetForceFactory("Gravity");
	ForcePtr pForceGravity = pForceFectoryGravity->CreateForce();
	pForceGravity->SetProperty("Acceleration", TVector3<Real>((Real) 0.0, (Real) -5.0, (Real) 0.0));

	// prepare body geometry
	Meson::Gravitas::Geometry::GeometryManager *pGeometryManager = pGravitasEngine->GetGeometryManager();
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactorySphere = pGeometryManager->GetGeometryFactory("Sphere");
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryBox = pGeometryManager->GetGeometryFactory("Box");
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryCylinder = pGeometryManager->GetGeometryFactory("Cylinder");

	static Meson::Gravitas::Geometry::GeometryPtr s_pGeometrySphere, s_pGeometryBox, s_pGeometryCylinder;
	if (s_pGeometrySphere == NULL)
	{
		s_pGeometrySphere = pGeometryFactorySphere->CreateGeometry();
		s_pGeometrySphere->SetProperty("Radius", 0.5);

		s_pModel3DSphere = new Model3D();
		s_pModel3DSphere->MakeSphere(0.5f, 32);
	}
	if (s_pGeometryBox == NULL)
	{
		s_pGeometryBox = pGeometryFactoryBox->CreateGeometry();
		s_pGeometryBox->SetProperty("Extent", TVector3<Real>(0.6, 0.5, 0.7));

		s_pModel3DBox = new Model3D();
		s_pModel3DBox->MakeCube(Vector3f(0.6f, 0.5f, 0.7f), 1.0f);
	}
	if (s_pGeometryCylinder == NULL)
	{
		s_pGeometryCylinder = pGeometryFactoryCylinder->CreateGeometry();
		s_pGeometryCylinder->SetProperty("Radius", 0.5);
		s_pGeometryCylinder->SetProperty("Height", 1.25);

		s_pModel3DCylinder = new Model3D();
		s_pModel3DCylinder->MakeCylinder(0.5f, 1.25f, 32);
	}

	if (s_pMaterialBox == NULL)
	{
		s_pMaterialBox = new Meson::Gravitas::Materials::Material();
		s_pMaterialBox->RestitutionCoefficient = 0.1;
		s_pMaterialBox->StaticFrictionCoefficient = 0.4;
		s_pMaterialBox->DynamicFrictionCoefficient = 0.3;
	}

	if (s_pMaterialSphere == NULL)
	{
		s_pMaterialSphere = new Meson::Gravitas::Materials::Material();
		s_pMaterialSphere->RestitutionCoefficient = 1.0;
		s_pMaterialSphere->StaticFrictionCoefficient = 0.1;
		s_pMaterialSphere->DynamicFrictionCoefficient = 0.05;
		s_pMaterialSphere->SurfacePerturbationCoefficient = 0.1;
	}

	if (s_pMaterialCylinder == NULL)
	{
		s_pMaterialCylinder = new Meson::Gravitas::Materials::Material();
		s_pMaterialCylinder->RestitutionCoefficient = 0.2;
		s_pMaterialCylinder->StaticFrictionCoefficient = 0.4;
		s_pMaterialCylinder->DynamicFrictionCoefficient = 0.3;
		s_pMaterialCylinder->SurfacePerturbationCoefficient = 0.0;
	}

	for (int nIndex = 0; nIndex < s_nMultiple; nIndex++)
	{
		Model3DPtr pModel3D;
		RigidBody* pRigidBody = new RigidBody();

		if (s_geometryType == Box)
		{
			pRigidBody->SetGeometryAndMassProperties(s_pGeometryBox, (Real) 1.0);
			pRigidBody->SetMaterial(s_pMaterialBox);

			pModel3D = s_pModel3DBox;
		}
		else if (s_geometryType == Sphere)
		{
			pRigidBody->SetGeometryAndMassProperties(s_pGeometrySphere, (Real) 1.0);
			pRigidBody->SetMaterial(s_pMaterialSphere);

			pModel3D = s_pModel3DSphere;
		}
		else // cylinder
		{
			pRigidBody->SetGeometryAndMassProperties(s_pGeometryCylinder, (Real) 1.0);
			pRigidBody->SetMaterial(s_pMaterialCylinder);

			pModel3D = s_pModel3DCylinder;
		}

		static Real rAngle = 0.0;
		if (p_eMouseKey == MK_LEFT)
		{
			pRigidBody->GetKineticProperties().Position = TVector3<Real>(0.0, 7.0, 0.0);
			pRigidBody->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Up, rAngle);
			pRigidBody->GetKineticProperties().OrientationConjugate = pRigidBody->GetKineticProperties().Orientation.ConjugateCopy();
			//pRigidBody->GetKineticProperties().LinearVelocity = TVector3<Real>::Out;
			rAngle += TMaths<Real>::Pi * 0.25;
		}
		else
		{
			pRigidBody->GetKineticProperties().Position = TVector3<Real>(-3.0, 2.0, 0.0);
			pRigidBody->GetKineticProperties().LinearVelocity = TVector3<Real>::Right;
		}

		// apply gravity force
		//ForcePtr pGravityForce(new BasicForce(TVector3<Real>::Down * pRigidBody->GetMassProperties().Mass * 2.0));
		//pRigidBody->GetForceAccumulator().AddForce(pGravityForce);
		pRigidBody->GetForceAccumulator().AddForce(pForceGravity);

		// bounding sphere
		Meson::Gravitas::Geometry::BoundingSphere* pBoundingSphere = new Meson::Gravitas::Geometry::BoundingSphere();
		pRigidBody->GetGeometry()->ComputeBoundingVolume(*pBoundingSphere);
		pRigidBody->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

		// add body to list for simulation
		static BodyPtr pBodyPrev(NULL);
		BodyPtr pBody(pRigidBody);
		m_pSpace->AddBody(pBody);

		// add entity for rendering etc.
		Entity entity;
		entity.Body = pBody;
		entity.Model3D = pModel3D;
		m_pEntityList->Add(entity);

		// test joint
		if (s_jointType != None)
		{
			BodyPtr pBodyLast = pBody;

			IConstraintFactory *pConstraintFactory = NULL;
			if (s_jointType == Spherical)
				pConstraintFactory = pGravitasEngine->GetConstraintManager()->GetConstraintFactory("SphericalJoint");
			else if (s_jointType == Revolute)
				pConstraintFactory = pGravitasEngine->GetConstraintManager()->GetConstraintFactory("RevoluteJoint");
			else if (s_jointType == Rigid)
				pConstraintFactory = pGravitasEngine->GetConstraintManager()->GetConstraintFactory("RigidJoint");
			else
				pConstraintFactory = pGravitasEngine->GetConstraintManager()->GetConstraintFactory("AngularMotor");

			ConstraintPtr pConstraint = pConstraintFactory->CreateConstraint();

			pConstraint->SetBody1(pBodyPrev);
			pConstraint->SetBody2(pBodyLast);

			TVector3<Real> vecOffset = pBodyLast->GetKineticProperties().Position - pBodyPrev->GetKineticProperties().Position;
			TVector3<Real> vecJointPosition = pBodyPrev->GetKineticProperties().Position 
				+ vecOffset * (Real) 0.5;

			if (s_jointType == Spherical)
			{
				pConstraint->SetProperty("BindPoint", vecJointPosition);
				pConstraint->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.33);
			}
			else if (s_jointType == Revolute)
			{
				pConstraint->SetProperty("BindAxisPoint1", vecJointPosition + TVector3<Real>::In * 0.5);
				pConstraint->SetProperty("BindAxisPoint2", vecJointPosition + TVector3<Real>::Out * 0.5);
				pConstraint->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.33);
			}
			else if (s_jointType == AngularMotor)
			{
				pConstraint->SetProperty("BindAxisPoint1", pBodyPrev->GetKineticProperties().Position);
				pConstraint->SetProperty("BindAxisPoint2", pBodyLast->GetKineticProperties().Position);
				pConstraint->SetProperty("AngularSpeed", TMaths<Real>::Pi * 0.5);
			}
			else // rigid
			{
				// no dynamic parameters required
			}
			pConstraint->Bind();

			m_pConstraintList->Add(pConstraint);

			pGravitasEngine->Logger().Out << "Joint created.\n";
		}

		pBodyPrev = pBody;
	}

	pGravitasEngine->Logger().Out << "Bodies: " << (uint) m_pSpace->GetBodies().Size() << "\n";
}

void MainWindowMessageHandler::OnClose(void)
{
	s_pModel3DSphere = NULL;
	s_pModel3DBox = NULL;
	s_pModel3DCylinder = NULL;

	m_bRunning = false;
}

void MainWindowMessageHandler::OnKeyDown(KeyCode p_eKey)
{
	if (p_eKey == KC_A)
	{
		Camera* pCamera = m_pWindowRenderTarget->GetViewport(0)->GetCamera();
		Vector3f vecPosition = pCamera->GetPosition();
		vecPosition.Z -= 0.25f;
		pCamera->SetPosition(vecPosition);
		m_pWindowRenderTarget->GetViewport(0)->Update();
	}

	if (p_eKey == KC_I)
		m_bInstrumentationActive = !m_bInstrumentationActive;
	
	if (p_eKey == KC_J)
	{
		s_jointType = (JointType) ((s_jointType + 1) % 5);
		if (s_jointType == None)
			GravitasEngine::GetInstance()->Logger().Out << "Joint type: None\n";
		else if (s_jointType == Spherical)
			GravitasEngine::GetInstance()->Logger().Out << "Joint type: Spherical\n";
		else if (s_jointType == Revolute)
			GravitasEngine::GetInstance()->Logger().Out << "Joint type: Revolute\n";
		else if (s_jointType == AngularMotor)
			GravitasEngine::GetInstance()->Logger().Out << "Joint type: AngularMotor\n";
		else
			GravitasEngine::GetInstance()->Logger().Out << "Joint type: Rigid\n";
	}

	if (p_eKey == KC_Z)
	{
		Camera* pCamera = m_pWindowRenderTarget->GetViewport(0)->GetCamera();
		Vector3f vecPosition = pCamera->GetPosition();
		vecPosition.Z += 0.25f;
		pCamera->SetPosition(vecPosition);
		m_pWindowRenderTarget->GetViewport(0)->Update();
	}

	if (p_eKey == KC_R)
	{
		//s_nMultiple = 11 - s_nMultiple;
		//GravitasEngine::GetInstance()->Logger().Out << "Repeat: " << s_nMultiple << "\n";
		CreateRagdoll(m_pSpace, m_pEntityList, m_pConstraintList);
	}

	if (p_eKey == KC_T)
	{
		CreateTriangleMesh(m_pSpace, m_pEntityList, m_pConstraintList);
	}

	if (p_eKey == KC_C)
	{
		CreateCart(m_pSpace, m_pEntityList, m_pConstraintList);
	}

	if (p_eKey == KC_P)
	{
		CreatePolyhedron(m_pSpace, m_pEntityList, m_pConstraintList);
	}

	if (p_eKey == KC_S)
	{
		s_geometryType = (GeometryType) ((s_geometryType + 1) % 3);
		if (s_geometryType == Box)
			GravitasEngine::GetInstance()->Logger().Out << "Geometry: Box\n";
		else if (s_geometryType == Sphere)
			GravitasEngine::GetInstance()->Logger().Out << "Geometry: Sphere\n";
		else
			GravitasEngine::GetInstance()->Logger().Out << "Geometry: Cylinder\n";
	}
}

void MainWindowMessageHandler::OnKeyUp(KeyCode p_eKey)
{
}

void MainWindowMessageHandler::OnResize(
	int p_nHorizontal, int p_nVertical, WindowState p_windowState)
{
}

void MainWindowMessageHandler::OnMove(
	int p_nHorizontal, int p_nVertical)
{
}

bool MainWindowMessageHandler::Step(void)
{
	VistasEngine::GetInstance()->GetWindowMessageManager()->Dispatch();
	return m_bRunning;
}

void MainWindowMessageHandler::Run(void)
{
	m_bRunning = true;

	while( m_bRunning )
		VistasEngine::GetInstance()->GetWindowMessageManager()->Dispatch();
}

bool MainWindowMessageHandler::IsInstrumentationActive(void)
{
	return m_bInstrumentationActive;
}
