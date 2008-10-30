#include "TriangleMeshExample.h"

#include "GrvGravitasEngine.h"

#include "GrvRigidBody.h"

#include "WavefrontModelLoader.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Materials;
using namespace Meson::Gravitas::Space;

void CreateTriangleMesh(SpacePtr p_pSpace, EntityList* p_pEntityList, ConstraintList* p_pConstraintList)
{
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();

	// load wavefront model
	WavefrontModelLoader wavefrontModelLoader;

	VectorArrayList listVertices;
	VectorArrayList listNormals;
	TArrayList< TVector2<Real> > listTextureCoordinates;
	WavefrontFaceArrayList listFaces;

	wavefrontModelLoader.LoadModel(
		"..\\Gravitas\\GravitasTest\\Resources\\Ramp.wavefront",
		listVertices, listNormals, listTextureCoordinates, listFaces);

	// prepare triangle indices for geometry
	TArrayList<int> listFaceVertexIndices;
	for (size_t unIndex = 0; unIndex < listFaces.Size(); unIndex++)
	{
		WavefrontFace& wavefrontFace = listFaces[unIndex];
		listFaceVertexIndices.Add(wavefrontFace.VertexIndex[0]);
		listFaceVertexIndices.Add(wavefrontFace.VertexIndex[1]);
		listFaceVertexIndices.Add(wavefrontFace.VertexIndex[2]);
	}

	// material
	Meson::Gravitas::Materials::MaterialPtr pMaterial(new Meson::Gravitas::Materials::Material());
	pMaterial->RestitutionCoefficient = 0.1;
	pMaterial->StaticFrictionCoefficient = 0.9;
	pMaterial->DynamicFrictionCoefficient = 0.9;

	// prepare body geometry
	Meson::Gravitas::Geometry::GeometryManager *pGeometryManager = pGravitasEngine->GetGeometryManager();
	Meson::Gravitas::Geometry::IGeometryFactory* pGeometryFactoryTriangleMesh
		= pGeometryManager->GetGeometryFactory("TriangleMesh");

	// geometry
	Meson::Gravitas::Geometry::GeometryPtr pGeometryTriangleMesh
		= pGeometryFactoryTriangleMesh->CreateGeometry();
	pGeometryTriangleMesh->SetProperty("Vertices", listVertices);
	pGeometryTriangleMesh->SetProperty("FaceVertexIndices", listFaceVertexIndices);

	// 3D model
	Model3DPtr pModel3DTriangleMesh(new Model3D());

	// 3D model vertices
	for (size_t unIndex = 0; unIndex < listVertices.Size(); unIndex++)
	{
		ModelVertex modelVertex;
		modelVertex.Position.X = (float) listVertices[unIndex].X;
		modelVertex.Position.Y = (float) listVertices[unIndex].Y;
		modelVertex.Position.Z = (float) listVertices[unIndex].Z;
		//modelVertex.Colour = ColourRGBA(0.5, 0.5, 0.5, 1.0);

		modelVertex.Normal = Vector3f::Up;

		modelVertex.Normal.ComputeOrthonormals(modelVertex.Tangent, modelVertex.Binormal);
		modelVertex.TexCoords.X = modelVertex.Position.X * 0.1f;
		modelVertex.TexCoords.Y = (modelVertex.Position.Y + modelVertex.Position.Z) * 0.1f;

		pModel3DTriangleMesh->Vertices.Add(modelVertex);
	}

	// 3d model normals
	for (size_t unIndex = 0; unIndex < listFaces.Size(); unIndex++)
	{
		WavefrontFace& wavefrontFace = listFaces[unIndex];
		pModel3DTriangleMesh->Vertices[wavefrontFace.VertexIndex[0]].Normal
			= listNormals[wavefrontFace.NormalIndex[0]];
		pModel3DTriangleMesh->Vertices[wavefrontFace.VertexIndex[1]].Normal
			= listNormals[wavefrontFace.NormalIndex[1]];
		pModel3DTriangleMesh->Vertices[wavefrontFace.VertexIndex[2]].Normal
			= listNormals[wavefrontFace.NormalIndex[2]];
	}

	// 3D model triangles
	for (size_t unIndex = 0; unIndex < listFaceVertexIndices.Size(); unIndex += 3)
	{
		ModelFace modelFace(
			listFaceVertexIndices[unIndex],
			listFaceVertexIndices[unIndex + 1],
			listFaceVertexIndices[unIndex + 2]);
		pModel3DTriangleMesh->Faces.Add(modelFace);
	}

	pModel3DTriangleMesh->Update();

	// rigid body
	RigidBody* pRigidBody = new RigidBody();

	// set geometry, mass and material
	//pRigidBody->SetGeometryAndMassProperties(pGeometryTriangleMesh, (Real) 1.0);
	pRigidBody->SetGeometry(pGeometryTriangleMesh);
	pRigidBody->GetMassProperties().SetInfiniteMass();
	pRigidBody->SetMaterial(pMaterial);

	// prepare gravity force and apply
	/*ForceGeneratorPtr pGravityForceGenerator(
		new BasicForceGenerator(
			TVector3<Real>::Down * pRigidBody->GetMassProperties().Mass * 5.0));
	pRigidBody->GetForceAccumulator().AttachForceGenerator(pGravityForceGenerator);*/

	// position body
	pRigidBody->GetKineticProperties().Position = TVector3<Real>(0.0, -3.0, 0.0);

	// bounding sphere
	//Meson::Gravitas::Geometry::BoundingSphere* pBoundingSphere = new Meson::Gravitas::Geometry::BoundingSphere();
	//pRigidBody->GetGeometry()->ComputeBoundingVolume(*pBoundingSphere);
	//pRigidBody->SetBoundingVolume(Meson::Gravitas::Geometry::BoundingVolumePtr(pBoundingSphere));

	// add chassis body
	BodyPtr pBody(pRigidBody);
	p_pSpace->AddBody(pBody);

	// add entity for rendering etc.
	Entity entity;
	entity.Body = pBody;
	entity.Model3D = pModel3DTriangleMesh;
	p_pEntityList->Add(entity);

	pGravitasEngine->Logger().Out << "Mesh volume: " << pGeometryTriangleMesh->GetVolume() << "\n";
}
