#include "TriangleMesh.h"

// Common headers
#include "Maths.h"

// Gravitas headers
#include "GrvGravitasEngine.h"

// local headers
#include "GeometryLoader.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;

const String g_strGeometryTypeTriangleMesh = "TriangleMesh";

TriangleMeshNode* TriangleMesh::PartitionTriangles(
	uint p_unLevel,
	const VectorList* p_pListVertices, 
	const IndexedTriangleList& p_listTriangles) const
{
	if (p_listTriangles.Size() == 0)
		return NULL;

	TriangleMeshNode* pTriangleMeshNode = new TriangleMeshNode();
	pTriangleMeshNode->Vertices = p_pListVertices;

	// determine list of used vertices
	VectorArrayList listVertices;
	for (size_t unIndexTriangles = 0; unIndexTriangles < p_listTriangles.Size(); unIndexTriangles++)
	{
		const IndexedTriangle& indexedTriangle = p_listTriangles[unIndexTriangles];
		for (size_t unIndexVertex = 0; unIndexVertex < 3; unIndexVertex++)
			listVertices.Add((*p_pListVertices)[indexedTriangle.VertexIndices[unIndexVertex]]);
	}

	// initialise bounds to first vertex
	BoundingAxisAlignedBox& boundingAxisAlignedBox
		= pTriangleMeshNode->BoundingVolume;
	boundingAxisAlignedBox.Min = boundingAxisAlignedBox.Max
		= listVertices[0];

	// extend box to other vertices
	for (size_t unIndex = 1; unIndex < listVertices.Size(); unIndex++)
		boundingAxisAlignedBox.ExtendToPoint(listVertices[unIndex]);

	// check termination conditions
	if (p_unLevel >= m_unMaximumBVHDepth
		|| p_listTriangles.Size() <= m_unMinimumTrianglesPerNode)
	{
		pTriangleMeshNode->Triangles = p_listTriangles;
		return pTriangleMeshNode;
	}

	// otherwise partition triangles in further nodes

	// determine largest axial span and prepare AABBs accordingly
	BoundingAxisAlignedBox boundingAxisAlignedBoxNegative(boundingAxisAlignedBox);
	BoundingAxisAlignedBox boundingAxisAlignedBoxPositive(boundingAxisAlignedBox);

	TVector3<Real> vecCentre = boundingAxisAlignedBox.Centre();

	TVector3<Real> vecExtent = boundingAxisAlignedBox.Extent();
	if (vecExtent.X >= vecExtent.Y && vecExtent.X >= vecExtent.Z)
	{
		boundingAxisAlignedBoxNegative.Max.X = vecCentre.X;
		boundingAxisAlignedBoxPositive.Min.X = vecCentre.X;
	}
	else if (vecExtent.Y >= vecExtent.X && vecExtent.Y >= vecExtent.Z)
	{
		boundingAxisAlignedBoxNegative.Max.Y = vecCentre.Y;
		boundingAxisAlignedBoxPositive.Min.Y = vecCentre.Y;
	}
	else
	{
		boundingAxisAlignedBoxNegative.Max.Z = vecCentre.Z;
		boundingAxisAlignedBoxPositive.Min.Z = vecCentre.Z;
	}

	// prepare triangle lists for child bounds
	IndexedTriangleArrayList listTrianglesPositive, listTrianglesNegative;

	// process triangles
	for (size_t unIndex = 0; unIndex < p_listTriangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = p_listTriangles[unIndex];
	
		if (boundingAxisAlignedBoxPositive.Contains(indexedTriangle.GetCentroid(&Vertices)))
				listTrianglesPositive.Add(indexedTriangle);
			else
				listTrianglesNegative.Add(indexedTriangle);
	}

	// if partitioning did not occur, this is a leaf node
	if (listTrianglesNegative.Size() == 0 || listTrianglesPositive.Size() == 0)
	{
		pTriangleMeshNode->Triangles = p_listTriangles;
		return pTriangleMeshNode;
	}

	// recursively partition triangle lists
	if (listTrianglesNegative.Size() > 0)
		pTriangleMeshNode->Child[0] = PartitionTriangles(
			p_unLevel + 1, p_pListVertices, listTrianglesNegative);

	if (listTrianglesPositive.Size() > 0)
		pTriangleMeshNode->Child[1] = PartitionTriangles(
			p_unLevel + 1, p_pListVertices, listTrianglesPositive);

	return pTriangleMeshNode;
}

TriangleMesh::TriangleMesh(void)
	: m_nId(-1)
	, Vertices()
	, Root(NULL)
	, m_unMaximumBVHDepth(16)
	, m_unMinimumTrianglesPerNode(4)
	, m_strWavefrontModelFilename()
{
}

TriangleMesh::TriangleMesh(const String& p_strId)
	: IGeometry(p_strId)
	, m_nId(-1)
	, Vertices()
	, Root(NULL)
	, m_unMaximumBVHDepth(16)
	, m_unMinimumTrianglesPerNode(10)
{
}

TriangleMesh::~TriangleMesh(void)
{
}

void TriangleMesh::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties.Insert("MaximumBVHDepth",
		PropertyDescriptor("MaximumBVHDepth", PropertyType::Integer, false));
	p_mapProperties.Insert("MinimumTrianglesPerNode",
		PropertyDescriptor("MinimumTrianglesPerNode", PropertyType::Integer, false));
	p_mapProperties.Insert("WavefrontModelFilename",
		PropertyDescriptor("WavefrontModelFilename", PropertyType::String, false));
	p_mapProperties.Insert("Vertices",
		PropertyDescriptor("Vertices", PropertyType::VectorList, false));
	p_mapProperties.Insert("FaceVertexIndices",
		PropertyDescriptor("FaceVertexIndices", PropertyType::IntegerList, false));
}

void TriangleMesh::GetProperty(const String& p_strName, int& p_nValue) const
{
	if (p_strName == "MaximumBVHDepth")
	{
		p_nValue = m_unMaximumBVHDepth;
	}
	else if (p_strName == "MinimumTrianglesPerNode")
	{
		p_nValue = m_unMinimumTrianglesPerNode;
	}
	else
		MESON_ASSERT(false, "TriangleMesh geometry: Unsupported property.");
}

void TriangleMesh::GetProperty(const String& p_strName, String& p_strValue) const
{
	if (p_strName == "WavefrontModelFilename")
	{
		p_strValue = m_strWavefrontModelFilename;
	}
	else
		MESON_ASSERT(false, "TriangleMesh geometry: Unsupported property.");
}

void TriangleMesh::GetProperty(const String& p_strName,
	TList<int>& p_listValues) const
{
	if (p_strName == "FaceVertexIndices")
	{
		p_listValues.Clear();
	}
	else
		MESON_ASSERT(false, "TriangleMesh geometry: Unsupported property.");
}

void TriangleMesh::GetProperty(const String& p_strName,
	VectorList& p_listValues) const
{
	if (p_strName == "Vertices")
		p_listValues = Vertices;
	else
		MESON_ASSERT(false, "TriangleMesh geometry: Unsupported property.");
}

void TriangleMesh::SetProperty(const String& p_strName, int p_nValue)
{
	if (p_strName == "MaximumBVHDepth")
	{
		m_unMaximumBVHDepth = (uint) TMaths<Real>::Max(1, p_nValue);
	}
	else if (p_strName == "MinimumTrianglesPerNode")
	{
		m_unMinimumTrianglesPerNode = (uint) TMaths<Real>::Max(1, p_nValue);
	}
	else
		MESON_ASSERT(false, "TriangleMesh geometry: Unsupported property.");
}

void TriangleMesh::SetProperty(const String& p_strName, const String& p_strValue)
{
	if (p_strName == "WavefrontModelFilename")
	{
		m_strWavefrontModelFilename = p_strValue;
		
		// load geometry
		GeometryLoader geometryLoader;
		GeometryFaceArrayList listGeometryFaces;
		geometryLoader.LoadWavefrontGeometry(m_strWavefrontModelFilename,
			true, Vertices, listGeometryFaces);

		// delete BVH
		if (Root != NULL)
			delete Root;

		// convert geometry faces to triangle mesh faces
		IndexedTriangleArrayList listIndexedTriangles;
		for (size_t unIndex = 0; unIndex < listGeometryFaces.Size(); unIndex++)
		{
			GeometryFace& geometryFace = listGeometryFaces[unIndex];

			IndexedTriangle indexedTriangle;
			indexedTriangle.VertexIndices[0] = geometryFace.VertexIndices[0];
			indexedTriangle.VertexIndices[1] = geometryFace.VertexIndices[1];
			indexedTriangle.VertexIndices[2] = geometryFace.VertexIndices[2];
			indexedTriangle.UpdateNormal(&Vertices);

			listIndexedTriangles.Add(indexedTriangle);
		}

		// rebuild BVH
		Root = PartitionTriangles(0, &Vertices, listIndexedTriangles);

		GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();

		pGravitasEngine->Logger().Out
			<< "Triangle Mesh loaded from Wavefront model file: "
			<< m_strWavefrontModelFilename << "\n";
		pGravitasEngine->Logger().Out
			<< "Triangle Mesh Vertices: " << (int) Vertices.Size() << "\n";
		pGravitasEngine->Logger().Out
			<< "Triangle Mesh Faces: " << (int) listIndexedTriangles.Size() << "\n";

		pGravitasEngine->Logger().Out
			<< "Triangle Mesh BVH tree depth: " << (int) Root->GetTreeDepth() << "\n";
		pGravitasEngine->Logger().Out
			<< "Triangle Mesh BVH node count: " << (int) Root->GetNodeCount() << "\n";
	}
	else
		MESON_ASSERT(false, "TriangleMesh geometry: Unsupported property.");
}

void TriangleMesh::SetProperty(const String& p_strName, const TList<int>& p_listValues)
{
	if (p_strName == "FaceVertexIndices")
	{
		MESON_ASSERT(p_listValues.Size() % 3 == 0,
			"FaceVertexIndices list size must be a multiple of three.");

		m_strWavefrontModelFilename = "";

		if (Root != NULL)
			delete Root;

		IndexedTriangleArrayList listIndexedTriangles;

		TEnumerator<int> &enumValues = ((TList<int> &) p_listValues).GetEnumerator();
		while(enumValues.HasMoreElements())
		{
			IndexedTriangle indexedTriangle;

			indexedTriangle.VertexIndices[0] = enumValues.NextElement();
			indexedTriangle.VertexIndices[1] = enumValues.NextElement();
			indexedTriangle.VertexIndices[2] = enumValues.NextElement();

			indexedTriangle.UpdateNormal(&Vertices);

			listIndexedTriangles.Add(indexedTriangle);
		}

		Root = PartitionTriangles(0, &Vertices, listIndexedTriangles);

		GravitasEngine::GetInstance()->Logger().Out << "Triangle Mesh Faces: " << (int) listIndexedTriangles.Size() << "\n";
	}
	else
		MESON_ASSERT(false, "TriangleMesh volume: Unsupported property.");
}

void TriangleMesh::SetProperty(const String& p_strName, const VectorList& p_listValues)
{
	if (p_strName == "Vertices")
	{
		if (Root != NULL)
		{
			delete Root;
			Root = NULL;
		}

		m_strWavefrontModelFilename = "";

		Vertices = p_listValues;
		GravitasEngine::GetInstance()->Logger().Out << "Triangle Mesh Vertices: " << (int) Vertices.Size() << "\n";
	}
	else
		MESON_ASSERT(false, "TriangleMesh geometry: Unsupported property.");
}

void TriangleMesh::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	if (Root != NULL)
		Root->RenderInstrumentation(p_pInstrumentationDevice);
}

const String& TriangleMesh::GetTypeName(void) const
{
	return g_strGeometryTypeTriangleMesh;
}

short TriangleMesh::GetTypeId(void) const
{
	return m_nId;
}

void TriangleMesh::SetTypeId(short p_nId)
{
	m_nId = p_nId;
}

Real TriangleMesh::GetVolume(void) const
{
	// calculate as area
	if (Root == NULL)
		return (Real) 0.0;
	else
		return Root->GetVolume();
}

TMatrix3<Real> TriangleMesh::GetDistributionTensor(void) const
{
	TMatrix3<Real> matDistributionTensor = TMatrix3<Real>::Zero;
	if (Root != NULL)
	{
		Real rMomentOfInertiaX
			= Root->GetMomentOfInertia(TVector3<Real>::Right);
		Real rMomentOfInertiaY
			= Root->GetMomentOfInertia(TVector3<Real>::Up);
		Real rMomentOfInertiaZ
			= Root->GetMomentOfInertia(TVector3<Real>::Out);

		matDistributionTensor._11
			= rMomentOfInertiaY + rMomentOfInertiaZ;
		matDistributionTensor._22
			= rMomentOfInertiaX + rMomentOfInertiaZ;
		matDistributionTensor._33
			= rMomentOfInertiaX + rMomentOfInertiaY;
	}

	return matDistributionTensor;
}

bool TriangleMesh::IsBounded(void) const
{
	return true;
}

void TriangleMesh::ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const
{
	p_boundingSphere.Centre = TVector3<Real>::Zero;
	p_boundingSphere.Radius =  p_boundingSphere.RadiusSquared =(Real) 0.0;

	if (Vertices.Size() == 0)
		return;

	// compute greatest radius
	Real rRadiusSquaredMax = 0;
	for (size_t unIndex = 0; unIndex < Vertices.Size(); unIndex++)
	{
		Real rRadiusSquaredCurrent
			= Vertices[unIndex].LengthSquared();
		if (rRadiusSquaredMax < rRadiusSquaredCurrent)
			rRadiusSquaredMax = rRadiusSquaredCurrent;
	}

	p_boundingSphere.RadiusSquared = rRadiusSquaredMax;
	p_boundingSphere.Radius = TMaths<Real>::Sqrt(rRadiusSquaredMax);
}

void TriangleMesh::ComputeBoundingVolume(BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const
{
	// handle empty case
	if (Vertices.Size() == 0)
	{
		p_boundingAxisAlignedBox.Min = p_boundingAxisAlignedBox.Max
			= TVector3<Real>::Zero;
		return;
	}

	// initialise bounds to first vertex
	p_boundingAxisAlignedBox.Min = p_boundingAxisAlignedBox.Max
		= Vertices[0];

	// extend box to other vertices
	for (size_t unIndex = 1; unIndex < Vertices.Size(); unIndex++)
		p_boundingAxisAlignedBox.ExtendToPoint(Vertices[unIndex]);
}

void TriangleMesh::ComputeBoundingVolume(BoundingOrientedBox& p_boundingOrientedBox) const
{
	// quick and dirty - same as AABB
	BoundingAxisAlignedBox boundingAxisAlignedBox;
	ComputeBoundingVolume(boundingAxisAlignedBox);
	p_boundingOrientedBox = boundingAxisAlignedBox;
}

bool TriangleMesh::IntersectsRay(const Ray& p_ray) const
{
	if (Root == NULL)
		return false;

	return Root->IntersectsRay(p_ray);
}

bool TriangleMesh::IntersectsRay(const Ray& p_ray, TVector3<Real>& p_vecIntersectionPoint) const
{
	if (Root == NULL)
		return false;

	return Root->IntersectsRay(p_ray, p_vecIntersectionPoint);
}
		
TVector3<Real> TriangleMesh::GetCentroid(void) const
{
	return TVector3<Real>::Zero;
}

