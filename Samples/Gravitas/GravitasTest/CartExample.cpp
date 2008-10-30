#include "RagdollExample.h"

#include "GrvGravitasEngine.h"

#include "GrvRigidBody.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Space;

void CreateCart(SpacePtr p_pSpace, EntityList* p_pEntityList, ConstraintList* p_pConstraintList)
{
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();

	// prepare body geometry
	Meson::Gravitas::Geometry::GeometryManager *pGeometryManager = pGravitasEngine->GetGeometryManager();
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactorySphere = pGeometryManager->GetGeometryFactory("Sphere");
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryBox = pGeometryManager->GetGeometryFactory("Box");
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryCylinder = pGeometryManager->GetGeometryFactory("Cylinder");

	// chassis material
	Meson::Gravitas::Materials::MaterialPtr pMaterialChassis(new Meson::Gravitas::Materials::Material());
	pMaterialChassis->RestitutionCoefficient = 0.1;
	pMaterialChassis->StaticFrictionCoefficient = 0.4;
	pMaterialChassis->DynamicFrictionCoefficient = 0.3;

	// wheel material
	Meson::Gravitas::Materials::MaterialPtr pMaterialWheel(new Meson::Gravitas::Materials::Material());
	pMaterialWheel->RestitutionCoefficient = 0.8;
	pMaterialWheel->StaticFrictionCoefficient = 0.9;
	pMaterialWheel->DynamicFrictionCoefficient = 0.5;

	// prepare gravity force
	ForceManager* pForceManager = pGravitasEngine->GetForceManager();
	IForceFactory* pForceFectoryGravity = pForceManager->GetForceFactory("Gravity");
	ForcePtr pForceGravity = pForceFectoryGravity->CreateForce();
	pForceGravity->SetProperty("Acceleration", TVector3<Real>((Real) 0.0, (Real) -5.0, (Real) 0.0));

	// -- chassis --

	// chassis geometry
	Meson::Gravitas::Geometry::GeometryPtr	pGeometryChassis = pGeometryFactoryBox->CreateGeometry();
	pGeometryChassis->SetProperty("Extent", TVector3<Real>(1.25, 0.25, 1.0));

	// head 3D model
	Model3DPtr pModel3DChassis(new Model3D());
	pModel3DChassis->MakeCube(TVector3<Real>(1.25f, 0.25f, 1.0f), 1.0);

	// chasses rigid body
	RigidBody* pRigidBodyChassis = new RigidBody();

	// set chassis geometry, mass and material
	pRigidBodyChassis->SetGeometryAndMassProperties(pGeometryChassis, (Real) 1.0);
	pRigidBodyChassis->SetMaterial(pMaterialChassis);

	// prepare gravity force and apply
	/*
	ForcePtr pGravityForceChassis(
		new BasicForce(
			TVector3<Real>::Down * pRigidBodyChassis->GetMassProperties().Mass * 5.0));
	pRigidBodyChassis->GetForceAccumulator().AddForce(pGravityForceChassis);
	*/

	pRigidBodyChassis->GetForceAccumulator().AddForce(pForceGravity);

	// position chassis
	pRigidBodyChassis->GetKineticProperties().Position = TVector3<Real>(-10.0, 0.0, 0.0);

	// bounding sphere
	Meson::Gravitas::Geometry::BoundingSphere* pBoundingSphere = new Meson::Gravitas::Geometry::BoundingSphere();
	pRigidBodyChassis->GetGeometry()->ComputeBoundingVolume(*pBoundingSphere);
	pRigidBodyChassis->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add chassis body
	BodyPtr pBodyChassis(pRigidBodyChassis);
	p_pSpace->AddBody(pBodyChassis);

	// add entity for rendering etc.
	Entity entity;
	entity.Body = pBodyChassis;
	entity.Model3D = pModel3DChassis;
	p_pEntityList->Add(entity);

	// -- wheels --

	// wheel geometry
	Meson::Gravitas::Geometry::GeometryPtr	pGeometryWheel = pGeometryFactoryCylinder->CreateGeometry();
	pGeometryWheel->SetProperty("Height", 0.5);
	pGeometryWheel->SetProperty("Radius", 0.5);

	// bounding sphere
	pBoundingSphere = new Meson::Gravitas::Geometry::BoundingSphere();
	pGeometryWheel->ComputeBoundingVolume(*pBoundingSphere);

	// wheel 3D model
	Model3DPtr pModel3DWheel(new Model3D());
	pModel3DWheel->MakeCylinder(0.5f, 0.5f, 32);

	// -- front left wheel -

	// front left wheel rigid body
	RigidBody* pRigidBodyWheelFrontLeft = new RigidBody();

	// set front left wheel geometry, mass and material
	pRigidBodyWheelFrontLeft->SetGeometryAndMassProperties(pGeometryWheel, (Real) 1.0);
	pRigidBodyWheelFrontLeft->SetMaterial(pMaterialWheel);

	// wheel gravity
	/*
	ForcePtr pGravityForceWheel(
		new BasicForce(
			TVector3<Real>::Down * pRigidBodyWheelFrontLeft->GetMassProperties().Mass * 5.0));
	// apply gravity force
	pRigidBodyWheelFrontLeft->GetForceAccumulator().AddForce(pGravityForceWheel);
	*/
	pRigidBodyWheelFrontLeft->GetForceAccumulator().AddForce(pForceGravity);

	// position front left wheel
	pRigidBodyWheelFrontLeft->GetKineticProperties().Position = TVector3<Real>(-9.25, 0.0, -1.5);
	pRigidBodyWheelFrontLeft->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Right, TMaths<Real>::PiHalf);
	pRigidBodyWheelFrontLeft->GetKineticProperties().OrientationConjugate = pRigidBodyWheelFrontLeft->GetKineticProperties().Orientation.ConjugateCopy();

	// bounding sphere
	pRigidBodyWheelFrontLeft->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add front left wheel body
	BodyPtr pBodyWheelFrontLeft(pRigidBodyWheelFrontLeft);
	p_pSpace->AddBody(pBodyWheelFrontLeft);

	// add entity for rendering etc.
	entity.Body = pBodyWheelFrontLeft;
	entity.Model3D = pModel3DWheel;
	p_pEntityList->Add(entity);


	// -- front right wheel -

	// front right wheel rigid body
	RigidBody* pRigidBodyWheelFrontRight = new RigidBody();

	// set front right wheel geometry, mass and material
	pRigidBodyWheelFrontRight->SetGeometryAndMassProperties(pGeometryWheel, (Real) 1.0);
	pRigidBodyWheelFrontRight->SetMaterial(pMaterialWheel);

	// apply gravity force
	//pRigidBodyWheelFrontRight->GetForceAccumulator().AddForce(pGravityForceWheel);
	pRigidBodyWheelFrontRight->GetForceAccumulator().AddForce(pForceGravity);

	// position front left wheel
	pRigidBodyWheelFrontRight->GetKineticProperties().Position = TVector3<Real>(-9.25, 0.0, 1.5);
	pRigidBodyWheelFrontRight->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Right, TMaths<Real>::PiHalf);
	pRigidBodyWheelFrontRight->GetKineticProperties().OrientationConjugate = pRigidBodyWheelFrontRight->GetKineticProperties().Orientation.ConjugateCopy();

	// bounding sphere
	pRigidBodyWheelFrontRight->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add front left wheel body
	BodyPtr pBodyWheelFrontRight(pRigidBodyWheelFrontRight);
	p_pSpace->AddBody(pBodyWheelFrontRight);

	// add entity for rendering etc.
	entity.Body = pBodyWheelFrontRight;
	entity.Model3D = pModel3DWheel;
	p_pEntityList->Add(entity);

	// -- rear left wheel -

	// rear left wheel rigid body
	RigidBody* pRigidBodyWheelRearLeft = new RigidBody();

	// set rear left wheel geometry, mass and material
	pRigidBodyWheelRearLeft->SetGeometryAndMassProperties(pGeometryWheel, (Real) 1.0);
	pRigidBodyWheelRearLeft->SetMaterial(pMaterialWheel);

	// apply gravity force
	//pRigidBodyWheelRearLeft->GetForceAccumulator().AddForce(pGravityForceWheel);
	pRigidBodyWheelRearLeft->GetForceAccumulator().AddForce(pForceGravity);

	// position rear rear wheel
	pRigidBodyWheelRearLeft->GetKineticProperties().Position = TVector3<Real>(-10.75, 0.0, -1.5);
	pRigidBodyWheelRearLeft->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Right, TMaths<Real>::PiHalf);
	pRigidBodyWheelRearLeft->GetKineticProperties().OrientationConjugate = pRigidBodyWheelRearLeft->GetKineticProperties().Orientation.ConjugateCopy();

	// bounding sphere
	pRigidBodyWheelRearLeft->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add rear left wheel body
	BodyPtr pBodyWheelRearLeft(pRigidBodyWheelRearLeft);
	p_pSpace->AddBody(pBodyWheelRearLeft);

	// add entity for rendering etc.
	entity.Body = pBodyWheelRearLeft;
	entity.Model3D = pModel3DWheel;
	p_pEntityList->Add(entity);


	// -- rear right wheel -

	// rear right wheel rigid body
	RigidBody* pRigidBodyWheelRearRight = new RigidBody();

	// set rear right wheel geometry, mass and material
	pRigidBodyWheelRearRight->SetGeometryAndMassProperties(pGeometryWheel, (Real) 1.0);
	pRigidBodyWheelRearRight->SetMaterial(pMaterialWheel);

	// apply gravity force
	//pRigidBodyWheelRearRight->GetForceAccumulator().AddForce(pGravityForceWheel);
	pRigidBodyWheelRearRight->GetForceAccumulator().AddForce(pForceGravity);

	// position front left wheel
	pRigidBodyWheelRearRight->GetKineticProperties().Position = TVector3<Real>(-10.75, 0.0, 1.5);
	pRigidBodyWheelRearRight->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Right, TMaths<Real>::PiHalf);
	pRigidBodyWheelRearRight->GetKineticProperties().OrientationConjugate = pRigidBodyWheelRearRight->GetKineticProperties().Orientation.ConjugateCopy();

	// bounding sphere
	pRigidBodyWheelRearRight->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add rear left wheel body
	BodyPtr pBodyWheelRearRight(pRigidBodyWheelRearRight);
	p_pSpace->AddBody(pBodyWheelRearRight);

	// add entity for rendering etc.
	entity.Body = pBodyWheelRearRight;
	entity.Model3D = pModel3DWheel;
	p_pEntityList->Add(entity);

	//-- constraints --
	IConstraintFactory* pConstraintFactoryAngularMotor
		= pGravitasEngine->GetConstraintManager()->GetConstraintFactory("AngularMotor");

	Real rAxleLength(0.75);

	//-- front left motor constraint --
	ConstraintPtr pConstraintFrontLeftMotor = pConstraintFactoryAngularMotor->CreateConstraint();
	pConstraintFrontLeftMotor->SetBody1(pBodyChassis);
	pConstraintFrontLeftMotor->SetBody2(pBodyWheelFrontLeft);
	pConstraintFrontLeftMotor->SetProperty("BindAxisPoint1", TVector3<Real>(-9.25, 0.0, -0.75 + rAxleLength));
	pConstraintFrontLeftMotor->SetProperty("BindAxisPoint2", TVector3<Real>(-9.25, 0.0, -0.75 - rAxleLength));
	pConstraintFrontLeftMotor->SetProperty("AngularSpeed", TMaths<Real>::Pi);
	pConstraintFrontLeftMotor->Bind();
	p_pConstraintList->Add(pConstraintFrontLeftMotor);

	//-- front right motor constraint --
	ConstraintPtr pConstraintFrontRightMotor = pConstraintFactoryAngularMotor->CreateConstraint();
	pConstraintFrontRightMotor->SetBody1(pBodyChassis);
	pConstraintFrontRightMotor->SetBody2(pBodyWheelFrontRight);
	pConstraintFrontRightMotor->SetProperty("BindAxisPoint1", TVector3<Real>(-9.25, 0.0, 0.75 + rAxleLength));
	pConstraintFrontRightMotor->SetProperty("BindAxisPoint2", TVector3<Real>(-9.25, 0.0, 0.75 - rAxleLength));
	pConstraintFrontRightMotor->SetProperty("AngularSpeed", TMaths<Real>::Pi);
	pConstraintFrontRightMotor->Bind();
	p_pConstraintList->Add(pConstraintFrontRightMotor);

	//-- rear left motor constraint --
	ConstraintPtr pConstraintRearLeftMotor = pConstraintFactoryAngularMotor->CreateConstraint();
	pConstraintRearLeftMotor->SetBody1(pBodyChassis);
	pConstraintRearLeftMotor->SetBody2(pBodyWheelRearLeft);
	pConstraintRearLeftMotor->SetProperty("BindAxisPoint1", TVector3<Real>(-10.75, 0.0, -0.75 + rAxleLength));
	pConstraintRearLeftMotor->SetProperty("BindAxisPoint2", TVector3<Real>(-10.75, 0.0, -0.75 - rAxleLength));
	pConstraintRearLeftMotor->SetProperty("AngularSpeed", TMaths<Real>::Pi);
	pConstraintRearLeftMotor->Bind();
	p_pConstraintList->Add(pConstraintRearLeftMotor);

	//-- rear right motor constraint --
	ConstraintPtr pConstraintRearRightMotor = pConstraintFactoryAngularMotor->CreateConstraint();
	pConstraintRearRightMotor->SetBody1(pBodyChassis);
	pConstraintRearRightMotor->SetBody2(pBodyWheelRearRight);
	pConstraintRearRightMotor->SetProperty("BindAxisPoint1", TVector3<Real>(-10.75, 0.0, 0.75 + rAxleLength));
	pConstraintRearRightMotor->SetProperty("BindAxisPoint2", TVector3<Real>(-10.75, 0.0, 0.75 - rAxleLength));
	pConstraintRearRightMotor->SetProperty("AngularSpeed", TMaths<Real>::Pi);
	pConstraintRearRightMotor->Bind();
	p_pConstraintList->Add(pConstraintRearRightMotor);
}
