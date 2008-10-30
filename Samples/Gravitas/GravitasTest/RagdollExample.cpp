#include "RagdollExample.h"

#include "GrvGravitasEngine.h"

#include "GrvRigidBody.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Materials;
using namespace Meson::Gravitas::Space;

void CreateRagdoll(SpacePtr p_pSpace, EntityList* p_pEntityList, ConstraintList* p_pConstraintList)
{
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();

	// prepare body geometry
	Meson::Gravitas::Geometry::GeometryManager *pGeometryManager = pGravitasEngine->GetGeometryManager();
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactorySphere = pGeometryManager->GetGeometryFactory("Sphere");
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryBox = pGeometryManager->GetGeometryFactory("Box");
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryCylinder = pGeometryManager->GetGeometryFactory("Cylinder");

	// ragdoll material
	Meson::Gravitas::Materials::MaterialPtr pMaterialRagdoll(new Meson::Gravitas::Materials::Material());
	pMaterialRagdoll->RestitutionCoefficient = 0.0;
	pMaterialRagdoll->StaticFrictionCoefficient = 0.4;
	pMaterialRagdoll->DynamicFrictionCoefficient = 0.3;

	// prepare gravity force
	ForceManager* pForceManager = pGravitasEngine->GetForceManager();
	IForceFactory* pForceFectoryGravity = pForceManager->GetForceFactory("Gravity");
	ForcePtr pForceGravity = pForceFectoryGravity->CreateForce();
	pForceGravity->SetProperty("Acceleration", TVector3<Real>((Real) 0.0, (Real) -5.0, (Real) 0.0));

	// -- head --

	// head geometry
	Meson::Gravitas::Geometry::GeometryPtr	pGeometryHead = pGeometryFactorySphere->CreateGeometry();
	pGeometryHead->SetProperty("Radius", 0.5);

	// head 3D model
	Model3DPtr pModel3DHead(new Model3D());
	pModel3DHead->MakeSphere(0.5f, 32);

	// head rigid body
	RigidBody* pRigidBodyHead = new RigidBody();

	// set head geometry, mass and material
	pRigidBodyHead->SetGeometryAndMassProperties(pGeometryHead, (Real) 1.0);
	pRigidBodyHead->SetMaterial(pMaterialRagdoll);

	// prepare gravity force and apply
	//ForcePtr pGravityForceHead(
	//	new BasicForce(
	//		TVector3<Real>::Down * pRigidBodyHead->GetMassProperties().Mass * 5.0));
	//pRigidBodyHead->GetForceAccumulator().AddForce(pGravityForceHead);
	pRigidBodyHead->GetForceAccumulator().AddForce(pForceGravity);

	// position head
	pRigidBodyHead->GetKineticProperties().Position = TVector3<Real>(0.0, 3.0, 0.0);
	pRigidBodyHead->GetKineticProperties().LinearVelocity = TVector3<Real>(1.0, 0.0, -4.0);

	// bounding sphere
	Meson::Gravitas::Geometry::BoundingSphere* pBoundingSphere = new Meson::Gravitas::Geometry::BoundingSphere();
	pRigidBodyHead->GetGeometry()->ComputeBoundingVolume(*pBoundingSphere);
	pRigidBodyHead->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add head body
	BodyPtr pBodyHead(pRigidBodyHead);
	p_pSpace->AddBody(pBodyHead);

	// add entity for rendering etc.
	Entity entityHead;
	entityHead.Body = pBodyHead;
	entityHead.Model3D = pModel3DHead;
	p_pEntityList->Add(entityHead);

	// -- torso --

	// torso geometry
	Meson::Gravitas::Geometry::GeometryPtr	pGeometryTorso = pGeometryFactoryBox->CreateGeometry();
	pGeometryTorso->SetProperty("Extent", TVector3<Real>(0.5, 0.75, 0.25));

	// head 3D model
	Model3DPtr pModel3DTorso(new Model3D());
	pModel3DTorso->MakeCube(Vector3f(0.5f, 0.75f, 0.25f), 1.0);

	// torso rigid body
	RigidBody* pRigidBodyTorso = new RigidBody();

	// set torso geometry, mass and material
	pRigidBodyTorso->SetGeometryAndMassProperties(pGeometryTorso, (Real) 1.0);
	pRigidBodyTorso->SetMaterial(pMaterialRagdoll);

	// prepare gravity force and apply
	/*
	ForcePtr pGravityForceTorso(
		new BasicForce(
			TVector3<Real>::Down * pRigidBodyTorso->GetMassProperties().Mass * 5.0));
	pRigidBodyTorso->GetForceAccumulator().AddForce(pGravityForceTorso);
	*/
	pRigidBodyTorso->GetForceAccumulator().AddForce(pForceGravity);

	// position torso
	pRigidBodyTorso->GetKineticProperties().Position = TVector3<Real>(0.0, 1.5, 0.0);

	// bounding sphere
	pBoundingSphere = new Meson::Gravitas::Geometry::BoundingSphere();
	pRigidBodyTorso->GetGeometry()->ComputeBoundingVolume(*pBoundingSphere);
	pRigidBodyTorso->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add torso body
	BodyPtr pBodyTorso(pRigidBodyTorso);

	// add entity for rendering etc.
	Entity entityTorso;
	entityTorso.Body = pBodyTorso;
	entityTorso.Model3D = pModel3DTorso;
	p_pEntityList->Add(entityTorso);

	// -- limbs --

	// limb geometry
	Meson::Gravitas::Geometry::GeometryPtr	pGeometryLimb = pGeometryFactoryCylinder->CreateGeometry();
	pGeometryLimb->SetProperty("Height", 1.0);
	pGeometryLimb->SetProperty("Radius", 0.25);

	// bounding sphere
	pBoundingSphere = new Meson::Gravitas::Geometry::BoundingSphere();
	pGeometryLimb->ComputeBoundingVolume(*pBoundingSphere);

	// limb 3D model
	Model3DPtr pModel3DLimb(new Model3D());
	pModel3DLimb->MakeCylinder(0.25f, 1.0f, 16);

	// -- upper arm - left --

	// upper arm left rigid body
	RigidBody* pRigidBodyUpperArmLeft = new RigidBody();

	// set upper arm left geometry, mass and material
	pRigidBodyUpperArmLeft->SetGeometryAndMassProperties(pGeometryLimb, (Real) 1.0);
	pRigidBodyUpperArmLeft->SetMaterial(pMaterialRagdoll);

	// limb gravity
	/*
	ForcePtr pGravityForceLimb(
		new BasicForce(
			TVector3<Real>::Down * pRigidBodyUpperArmLeft->GetMassProperties().Mass * 5.0));
	// apply gravity force
	pRigidBodyUpperArmLeft->GetForceAccumulator().AddForce(pGravityForceLimb);
	*/
	pRigidBodyUpperArmLeft->GetForceAccumulator().AddForce(pForceGravity);

	// position upper arm left
	pRigidBodyUpperArmLeft->GetKineticProperties().Position = TVector3<Real>(1.5, 2.25, 0.0);
	pRigidBodyUpperArmLeft->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Out, TMaths<Real>::PiHalf);
	pRigidBodyUpperArmLeft->GetKineticProperties().OrientationConjugate = pRigidBodyUpperArmLeft->GetKineticProperties().Orientation.ConjugateCopy();

	// bounding sphere
	pRigidBodyUpperArmLeft->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add upper arm left body
	BodyPtr pBodyUpperArmLeft(pRigidBodyUpperArmLeft);
	p_pSpace->AddBody(pBodyUpperArmLeft);

	// add entity for rendering etc.
	Entity entityLimb;
	entityLimb.Body = pBodyUpperArmLeft;
	entityLimb.Model3D = pModel3DLimb;
	p_pEntityList->Add(entityLimb);

	// -- upper arm - right --

	// upper arm right rigid body
	RigidBody* pRigidBodyUpperArmRight = new RigidBody();

	// set upper arm left geometry, mass and material
	pRigidBodyUpperArmRight->SetGeometryAndMassProperties(pGeometryLimb, (Real) 1.0);
	pRigidBodyUpperArmRight->SetMaterial(pMaterialRagdoll);

	// apply gravity force
	//pRigidBodyUpperArmRight->GetForceAccumulator().AddForce(pGravityForceLimb);
	pRigidBodyUpperArmRight->GetForceAccumulator().AddForce(pForceGravity);


	// position upper arm right
	pRigidBodyUpperArmRight->GetKineticProperties().Position = TVector3<Real>(-1.5, 2.25, 0.0);
	pRigidBodyUpperArmRight->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Out, TMaths<Real>::PiHalf);
	pRigidBodyUpperArmRight->GetKineticProperties().OrientationConjugate = pRigidBodyUpperArmLeft->GetKineticProperties().Orientation.ConjugateCopy();

	// bounding sphere
	pRigidBodyUpperArmRight->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add upper arm left body
	BodyPtr pBodyUpperArmRight(pRigidBodyUpperArmRight);
	p_pSpace->AddBody(pBodyUpperArmRight);

	// add entity for rendering etc.
	entityLimb.Body = pBodyUpperArmRight;
	entityLimb.Model3D = pModel3DLimb;
	p_pEntityList->Add(entityLimb);

	// -- lower arm - left --

	// lower arm right rigid body
	RigidBody* pRigidBodyLowerArmLeft = new RigidBody();

	// set lower arm left geometry, mass and material
	pRigidBodyLowerArmLeft->SetGeometryAndMassProperties(pGeometryLimb, (Real) 1.0);
	pRigidBodyLowerArmLeft->SetMaterial(pMaterialRagdoll);

	// apply gravity force
	//pRigidBodyLowerArmLeft->GetForceAccumulator().AddForce(pGravityForceLimb);
	pRigidBodyLowerArmLeft->GetForceAccumulator().AddForce(pForceGravity);

	// position lower arm right
	pRigidBodyLowerArmLeft->GetKineticProperties().Position = TVector3<Real>(2.75, 2.25, 0.0);
	pRigidBodyLowerArmLeft->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Out, TMaths<Real>::PiHalf);
	pRigidBodyLowerArmLeft->GetKineticProperties().OrientationConjugate = pRigidBodyUpperArmLeft->GetKineticProperties().Orientation.ConjugateCopy();

	// bounding sphere
	pRigidBodyLowerArmLeft->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add upper arm left body
	BodyPtr pBodyLowerArmLeft(pRigidBodyLowerArmLeft);
	p_pSpace->AddBody(pBodyLowerArmLeft);

	// add entity for rendering etc.
	entityLimb.Body = pBodyLowerArmLeft;
	entityLimb.Model3D = pModel3DLimb;
	p_pEntityList->Add(entityLimb);

	// -- lower arm - right --

	// lower arm right rigid body
	RigidBody* pRigidBodyLowerArmRight = new RigidBody();

	// set lower arm left geometry, mass and material
	pRigidBodyLowerArmRight->SetGeometryAndMassProperties(pGeometryLimb, (Real) 1.0);
	pRigidBodyLowerArmRight->SetMaterial(pMaterialRagdoll);

	// apply gravity force
	//pRigidBodyLowerArmRight->GetForceAccumulator().AddForce(pGravityForceLimb);
	pRigidBodyLowerArmRight->GetForceAccumulator().AddForce(pForceGravity);

	// position lower arm right
	pRigidBodyLowerArmRight->GetKineticProperties().Position = TVector3<Real>(-2.75, 2.25, 0.0);
	pRigidBodyLowerArmRight->GetKineticProperties().Orientation.MakeRotation(TVector3<Real>::Out, TMaths<Real>::PiHalf);
	pRigidBodyLowerArmRight->GetKineticProperties().OrientationConjugate = pRigidBodyUpperArmLeft->GetKineticProperties().Orientation.ConjugateCopy();

	// bounding sphere
	pRigidBodyLowerArmRight->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add upper arm right body
	BodyPtr pBodyLowerArmRight(pRigidBodyLowerArmRight);
	p_pSpace->AddBody(pBodyLowerArmRight);

	// add entity for rendering etc.
	entityLimb.Body = pBodyLowerArmRight;
	entityLimb.Model3D = pModel3DLimb;
	p_pEntityList->Add(entityLimb);

	// -- upper leg - left --

	// upper leg left rigid body
	RigidBody* pRigidBodyUpperLegLeft = new RigidBody();

	// set upper leg left geometry, mass and material
	pRigidBodyUpperLegLeft->SetGeometryAndMassProperties(pGeometryLimb, (Real) 1.0);
	pRigidBodyUpperLegLeft->SetMaterial(pMaterialRagdoll);

	// apply gravity force
	//pRigidBodyUpperLegLeft->GetForceAccumulator().AddForce(pGravityForceLimb);
	pRigidBodyUpperLegLeft->GetForceAccumulator().AddForce(pForceGravity);

	// position upper leg left
	pRigidBodyUpperLegLeft->GetKineticProperties().Position = TVector3<Real>(0.3, 0.0, 0.0);

	// bounding sphere
	pRigidBodyUpperLegLeft->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add upper leg right body
	BodyPtr pBodyUpperLegLeft(pRigidBodyUpperLegLeft);
	p_pSpace->AddBody(pBodyUpperLegLeft);

	// add entity for rendering etc.
	entityLimb.Body = pRigidBodyUpperLegLeft;
	entityLimb.Model3D = pModel3DLimb;
	p_pEntityList->Add(entityLimb);

	// -- upper leg - right --

	// upper leg right rigid body
	RigidBody* pRigidBodyUpperLegRight = new RigidBody();

	// set upper leg right geometry, mass and material
	pRigidBodyUpperLegRight->SetGeometryAndMassProperties(pGeometryLimb, (Real) 1.0);
	pRigidBodyUpperLegRight->SetMaterial(pMaterialRagdoll);

	// apply gravity force
	//pRigidBodyUpperLegRight->GetForceAccumulator().AddForce(pGravityForceLimb);
	pRigidBodyUpperLegRight->GetForceAccumulator().AddForce(pForceGravity);

	// position upper leg right
	pRigidBodyUpperLegRight->GetKineticProperties().Position = TVector3<Real>(-0.3, 0.0, 0.0);

	// bounding sphere
	pRigidBodyUpperLegRight->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add upper leg right body
	BodyPtr pBodyUpperLegRight(pRigidBodyUpperLegRight);
	p_pSpace->AddBody(pBodyUpperLegRight);

	// add entity for rendering etc.
	entityLimb.Body = pBodyUpperLegRight;
	entityLimb.Model3D = pModel3DLimb;
	p_pEntityList->Add(entityLimb);

	// -- lower leg - left --

	// lower leg left rigid body
	RigidBody* pRigidBodyLowerLegLeft = new RigidBody();

	// set lower leg left geometry, mass and material
	pRigidBodyLowerLegLeft->SetGeometryAndMassProperties(pGeometryLimb, (Real) 1.0);
	pRigidBodyLowerLegLeft->SetMaterial(pMaterialRagdoll);

	// apply gravity force
	//pRigidBodyLowerLegLeft->GetForceAccumulator().AddForce(pGravityForceLimb);
	pRigidBodyLowerLegLeft->GetForceAccumulator().AddForce(pForceGravity);

	// position lower leg left
	pRigidBodyLowerLegLeft->GetKineticProperties().Position = TVector3<Real>(0.3, -1.25, 0.0);

	// bounding sphere
	pRigidBodyLowerLegLeft->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add upper leg right body
	BodyPtr pBodyLowerLegLeft(pRigidBodyLowerLegLeft);
	p_pSpace->AddBody(pBodyLowerLegLeft);

	// add entity for rendering etc.
	entityLimb.Body = pRigidBodyLowerLegLeft;
	entityLimb.Model3D = pModel3DLimb;
	p_pEntityList->Add(entityLimb);

	// -- lower leg - right --

	// lower leg left rigid body
	RigidBody* pRigidBodyLowerLegRight = new RigidBody();

	// set lower leg left geometry, mass and material
	pRigidBodyLowerLegRight->SetGeometryAndMassProperties(pGeometryLimb, (Real) 1.0);
	pRigidBodyLowerLegRight->SetMaterial(pMaterialRagdoll);

	// apply gravity force
	//pRigidBodyLowerLegRight->GetForceAccumulator().AddForce(pGravityForceLimb);
	pRigidBodyLowerLegRight->GetForceAccumulator().AddForce(pForceGravity);

	// position lower leg left
	pRigidBodyLowerLegRight->GetKineticProperties().Position = TVector3<Real>(-0.3, -1.25, 0.0);

	// bounding sphere
	pRigidBodyLowerLegRight->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add upper leg right body
	BodyPtr pBodyLowerLegRight(pRigidBodyLowerLegRight);
	p_pSpace->AddBody(pBodyLowerLegRight);

	// add entity for rendering etc.
	entityLimb.Body = pRigidBodyLowerLegRight;
	entityLimb.Model3D = pModel3DLimb;
	p_pEntityList->Add(entityLimb);

	// ADD TORSO BODY LAST FOR CAM FOCUS!!
	p_pSpace->AddBody(pBodyTorso);

	//-- constraints --
	IConstraintFactory* pConstraintFactorySphericalJoint
		= pGravitasEngine->GetConstraintManager()->GetConstraintFactory("SphericalJoint");
	IConstraintFactory* pConstraintFactoryRevoluteJoint
		= pGravitasEngine->GetConstraintManager()->GetConstraintFactory("RevoluteJoint");

	//-- neck constraint --
	ConstraintPtr pConstraintNeck = pConstraintFactorySphericalJoint->CreateConstraint();
	pConstraintNeck->SetBody1(pBodyHead);
	pConstraintNeck->SetBody2(pBodyTorso);
	pConstraintNeck->SetProperty("BindPoint", TVector3<Real>(0.0, 2.375, 0.0));
	pConstraintNeck->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.125);
	pConstraintNeck->Bind();
	p_pConstraintList->Add(pConstraintNeck);

	//-- left shoulder constraint --
	ConstraintPtr pConstraintShoulderLeft = pConstraintFactorySphericalJoint->CreateConstraint();
	pConstraintShoulderLeft->SetBody1(pBodyTorso);
	pConstraintShoulderLeft->SetBody2(pBodyUpperArmLeft);
	pConstraintShoulderLeft->SetProperty("BindPoint", TVector3<Real>(0.75, 2.25, 0.0));
	pConstraintShoulderLeft->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.75);
	pConstraintShoulderLeft->Bind();
	p_pConstraintList->Add(pConstraintShoulderLeft);
	
	//-- right shoulder constraint --
	ConstraintPtr pConstraintShoulderRight = pConstraintFactorySphericalJoint->CreateConstraint();
	pConstraintShoulderRight->SetBody1(pBodyTorso);
	pConstraintShoulderRight->SetBody2(pBodyUpperArmRight);
	pConstraintShoulderRight->SetProperty("BindPoint", TVector3<Real>(-0.75, 2.25, 0.0));
	pConstraintShoulderRight->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.75);
	pConstraintShoulderRight->Bind();
	p_pConstraintList->Add(pConstraintShoulderRight);

	//-- left elbow constraint --
	ConstraintPtr pConstraintElbowLeft = pConstraintFactoryRevoluteJoint->CreateConstraint();
	pConstraintElbowLeft->SetBody1(pBodyUpperArmLeft);
	pConstraintElbowLeft->SetBody2(pBodyLowerArmLeft);
	pConstraintElbowLeft->SetProperty("BindAxisPoint1", TVector3<Real>(2.125, 2.35, 0.0));
	pConstraintElbowLeft->SetProperty("BindAxisPoint2", TVector3<Real>(2.125, 2.15, 0.0));
	pConstraintElbowLeft->Bind();
	p_pConstraintList->Add(pConstraintElbowLeft);

	//-- right elbow constraint --
	ConstraintPtr pConstraintElbowRight = pConstraintFactoryRevoluteJoint->CreateConstraint();
	pConstraintElbowRight->SetBody1(pBodyUpperArmRight);
	pConstraintElbowRight->SetBody2(pBodyLowerArmRight);
	pConstraintElbowRight->SetProperty("BindAxisPoint1", TVector3<Real>(-2.125, 2.35, 0.0));
	pConstraintElbowRight->SetProperty("BindAxisPoint2", TVector3<Real>(-2.125, 2.15, 0.0));
	pConstraintElbowRight->Bind();
	p_pConstraintList->Add(pConstraintElbowRight);

	//-- left hip constraint --
	ConstraintPtr pConstraintHipLeft = pConstraintFactorySphericalJoint->CreateConstraint();
	pConstraintHipLeft->SetBody1(pBodyTorso);
	pConstraintHipLeft->SetBody2(pBodyUpperLegLeft);
	pConstraintHipLeft->SetProperty("BindPoint", TVector3<Real>(0.3, 0.625, 0.0));
	pConstraintHipLeft->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.25);
	pConstraintHipLeft->Bind();
	p_pConstraintList->Add(pConstraintHipLeft);

	//-- right hip constraint --
	ConstraintPtr pConstraintHipRight = pConstraintFactorySphericalJoint->CreateConstraint();
	pConstraintHipRight->SetBody1(pBodyTorso);
	pConstraintHipRight->SetBody2(pBodyUpperLegRight);
	pConstraintHipRight->SetProperty("BindPoint", TVector3<Real>(-0.3, 0.625, 0.0));
	pConstraintHipRight->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.25);
	pConstraintHipRight->Bind();
	p_pConstraintList->Add(pConstraintHipRight);

	//-- left knee constraint --
	ConstraintPtr pConstraintKneeLeft = pConstraintFactoryRevoluteJoint->CreateConstraint();
	pConstraintKneeLeft->SetBody1(pBodyUpperLegLeft);
	pConstraintKneeLeft->SetBody2(pBodyLowerLegLeft);
	pConstraintKneeLeft->SetProperty("BindAxisPoint1", TVector3<Real>(0.55, -0.625, 0.1));
	pConstraintKneeLeft->SetProperty("BindAxisPoint2", TVector3<Real>(0.05, -0.625, 0.1));
	pConstraintKneeLeft->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.5);
	pConstraintKneeLeft->Bind();
	p_pConstraintList->Add(pConstraintKneeLeft);

	//-- right knee constraint --
	ConstraintPtr pConstraintKneeRight = pConstraintFactoryRevoluteJoint->CreateConstraint();
	pConstraintKneeRight->SetBody1(pBodyUpperLegRight);
	pConstraintKneeRight->SetBody2(pBodyLowerLegRight);
	pConstraintKneeRight->SetProperty("BindAxisPoint1", TVector3<Real>(-0.05, -0.625, 0.0));
	pConstraintKneeRight->SetProperty("BindAxisPoint2", TVector3<Real>(-0.55, -0.625, 0.0));
	pConstraintKneeRight->SetProperty("AngleLimit", TMaths<Real>::Pi * 0.5);
	pConstraintKneeRight->Bind();
	p_pConstraintList->Add(pConstraintKneeRight);
}
