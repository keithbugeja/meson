#include "PolyhedronExample.h"

#include "GrvGravitasEngine.h"

#include "GrvRigidBody.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Materials;
using namespace Meson::Gravitas::Space;

void CreatePolyhedron(SpacePtr p_pSpace, EntityList* p_pEntityList, ConstraintList* p_pConstraintList)
{
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();

	// prepare body geometry
	Meson::Gravitas::Geometry::GeometryManager *pGeometryManager = pGravitasEngine->GetGeometryManager();
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryConvexPolyhedron = pGeometryManager->GetGeometryFactory("ConvexPolyhedron");

	// material
	Meson::Gravitas::Materials::MaterialPtr pMaterial(new Meson::Gravitas::Materials::Material());
	pMaterial->RestitutionCoefficient = 0.9;
	pMaterial->StaticFrictionCoefficient = 0.9;
	pMaterial->DynamicFrictionCoefficient = 0.3;

	// geometry
	Meson::Gravitas::Geometry::GeometryPtr pGeometryConvexPolyhedron = pGeometryFactoryConvexPolyhedron->CreateGeometry();

	pGeometryConvexPolyhedron->SetProperty("Vertex", 0, TPoint3<Real>(-0.5, -0.5, -0.5));
	pGeometryConvexPolyhedron->SetProperty("Vertex", 1, TPoint3<Real>( 0.5, -0.3, -0.3));
	pGeometryConvexPolyhedron->SetProperty("Vertex", 2, TPoint3<Real>(-0.5,  0.5, -0.5));
	pGeometryConvexPolyhedron->SetProperty("Vertex", 3, TPoint3<Real>( 0.5,  0.3, -0.3));
	pGeometryConvexPolyhedron->SetProperty("Vertex", 4, TPoint3<Real>(-0.5, -0.5,  0.5));
	pGeometryConvexPolyhedron->SetProperty("Vertex", 5, TPoint3<Real>( 0.5, -0.3,  0.3));
	pGeometryConvexPolyhedron->SetProperty("Vertex", 6, TPoint3<Real>(-0.5,  0.5,  0.5));
	pGeometryConvexPolyhedron->SetProperty("Vertex", 7, TPoint3<Real>( 0.5,  0.3,  0.3));

	TArrayList<int> listVertices;

	// back
	listVertices.Add(2);
	listVertices.Add(3);
	listVertices.Add(1);
	listVertices.Add(0);
	pGeometryConvexPolyhedron->SetProperty("FaceIndex", 0);
	pGeometryConvexPolyhedron->SetProperty("FaceVertices", listVertices);

	// front
	listVertices[0] = 4;
	listVertices[1] = 5;
	listVertices[2] = 7;
	listVertices[3] = 6;
	pGeometryConvexPolyhedron->SetProperty("FaceIndex", 1);
	pGeometryConvexPolyhedron->SetProperty("FaceVertices", listVertices);

	// left
	listVertices[0] = 4;
	listVertices[1] = 6;
	listVertices[2] = 2;
	listVertices[3] = 0;
	pGeometryConvexPolyhedron->SetProperty("FaceIndex", 2);
	pGeometryConvexPolyhedron->SetProperty("FaceVertices", listVertices);

	// right
	listVertices[0] = 1;
	listVertices[1] = 3;
	listVertices[2] = 7;
	listVertices[3] = 5;
	pGeometryConvexPolyhedron->SetProperty("FaceIndex", 3);
	pGeometryConvexPolyhedron->SetProperty("FaceVertices", listVertices);

	// bottom
	listVertices[0] = 0;
	listVertices[1] = 1;
	listVertices[2] = 5;
	listVertices[3] = 4;
	pGeometryConvexPolyhedron->SetProperty("FaceIndex", 4);
	pGeometryConvexPolyhedron->SetProperty("FaceVertices", listVertices);

	// top
	listVertices[0] = 6;
	listVertices[1] = 7;
	listVertices[2] = 3;
	listVertices[3] = 2;
	pGeometryConvexPolyhedron->SetProperty("FaceIndex", 5);
	pGeometryConvexPolyhedron->SetProperty("FaceVertices", listVertices);

	pGeometryConvexPolyhedron->SetProperty("Update", true);

	// 3D model
	Model3DPtr pModel3DConvexPolyhedron(new Model3D());
	pModel3DConvexPolyhedron->MakeCube(TVector3<Real>(0.5f, 0.5f, 0.5f), 1.0);
	// taper the body in line with poly geom
	for (size_t unIndex = 0; unIndex < pModel3DConvexPolyhedron->Vertices.Size(); unIndex++)
	{
		ModelVertex& modelVertex = pModel3DConvexPolyhedron->Vertices[unIndex];
		if (modelVertex.Position.X > 0.0f)
		{
			modelVertex.Position.Y *= 0.6f;
			modelVertex.Position.Z *= 0.6f;
		}
		modelVertex.Position.X += 0.0816f;
	}
	pModel3DConvexPolyhedron->Update();

	// rigid body
	RigidBody* pRigidBodyConvexPolyhedron = new RigidBody();

	// set geometry, mass and material
	pRigidBodyConvexPolyhedron->SetGeometryAndMassProperties(pGeometryConvexPolyhedron, (Real) 1.0);
	pRigidBodyConvexPolyhedron->SetMaterial(pMaterial);

	// prepare gravity force and apply
	/*
	ForcePtr pGravityForce(
		new BasicForce(
			TVector3<Real>::Down * pRigidBodyConvexPolyhedron->GetMassProperties().Mass * 5.0));
	pRigidBodyConvexPolyhedron->GetForceAccumulator().AddForce(pGravityForce);
	*/

	// prepare gravity force
	ForceManager* pForceManager = pGravitasEngine->GetForceManager();
	IForceFactory* pForceFectoryGravity = pForceManager->GetForceFactory("Gravity");
	ForcePtr pForceGravity = pForceFectoryGravity->CreateForce();
	pForceGravity->SetProperty("Acceleration", TVector3<Real>((Real) 0.0, (Real) -5.0, (Real) 0.0));
	pRigidBodyConvexPolyhedron->GetForceAccumulator().AddForce(pForceGravity);

	// position body
	pRigidBodyConvexPolyhedron->GetKineticProperties().Position = TPoint3<Real>(0.0, 10.0, 0.0);
	pRigidBodyConvexPolyhedron->GetKineticProperties().AngularVelocity = TVector3<Real>::Out * TMaths<Real>::Pi * 1.0;

	// bounding sphere
	Meson::Gravitas::Geometry::BoundingSphere* pBoundingSphere = new Meson::Gravitas::Geometry::BoundingSphere();
	pRigidBodyConvexPolyhedron->GetGeometry()->ComputeBoundingVolume(*pBoundingSphere);
	pRigidBodyConvexPolyhedron->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add chassis body
	BodyPtr pBodyConvexPolyhedron(pRigidBodyConvexPolyhedron);
	p_pSpace->AddBody(pBodyConvexPolyhedron);

	// add entity for rendering etc.
	Entity entity;
	entity.Body = pBodyConvexPolyhedron;
	entity.Model3D = pModel3DConvexPolyhedron;
	p_pEntityList->Add(entity);
}
