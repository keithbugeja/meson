#include "ConvexPolyhedron.h"

// Gravitas headers
#include "GrvLineSegment.h"
#include "GrvTriangle.h"
#include "GrvTetrahedron.h"
#include "GrvGravitasEngine.h"

// local headers
#include "GeometryLoader.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

String ConvexPolyhedron::s_strTypeName("ConvexPolyhedron");

void ConvexPolyhedron::ComputeMassPropertySubExpressions(
	Real p_rW0, Real p_rW1, Real p_rW2,
	Real &p_rF1, Real &p_rF2, Real &p_rF3,
	Real &p_rG0, Real &p_rG1, Real &p_rG2) const
{
	Real rTemp0 = p_rW0 + p_rW1;
	Real rTemp1 = p_rW0 * p_rW0;
	Real rTemp2 = rTemp1 + p_rW1 * rTemp0;

	p_rF1 = rTemp0 + p_rW2;
	p_rF2 = rTemp2 + p_rW2 * p_rF1;
	p_rF3 = p_rW0 * rTemp1 + p_rW1 * rTemp2 + p_rW2 * p_rF2;

	p_rG0 = p_rF2 + p_rW0 * (p_rF1 + p_rW0);
	p_rG1 = p_rF2 + p_rW1 * (p_rF1 + p_rW1);
	p_rG2 = p_rF2 + p_rW2 * (p_rF1 + p_rW2);
}

bool ConvexPolyhedron::IsConvex(Real p_rCoplanarTollerance) const
{
	// check all faces
	for (size_t unFaceIndex = 0; unFaceIndex < Faces.Size(); unFaceIndex++)
	{
		ConvexPolyhedronFace& convexPolyhedronFace = Faces[unFaceIndex];

		// check all vertices against each face
		for (ushort usVertexIndex = 0; usVertexIndex < Vertices.Size(); usVertexIndex++)
		{
			// ignore vertices referenced by face
			if (convexPolyhedronFace.HasVertexIndex(usVertexIndex))
				continue;

			// not convex if any vertex is on outer side of face plane
			Real rSignedDistanceFromPlane = convexPolyhedronFace.GetSignedDistanceFromPlane(Vertices[usVertexIndex]);
			if (rSignedDistanceFromPlane > p_rCoplanarTollerance)
				return false;
		}
	}

	// otherwise, convex
	return true;
}

ConvexPolyhedron::ConvexPolyhedron(void)
	: m_nTypeId(-1)
	, m_nFaceIndex(-1)
	, m_strWavefrontModelFilename()
	, Vertices()
	, Edges()
	, Faces()
{
}

ConvexPolyhedron::ConvexPolyhedron(const Meson::Common::Text::String& p_strId)
	: IGeometry(p_strId)
	, m_nTypeId(-1)
	, m_nFaceIndex(-1)
	, m_strWavefrontModelFilename()
	, Vertices()
	, Edges()
	, Faces()
{
}

ConvexPolyhedron::ConvexPolyhedron(const ConvexPolyhedron& p_convexPolyhedron)
	: m_nTypeId(p_convexPolyhedron.m_nTypeId)
	, m_nFaceIndex(-1)
	, m_strWavefrontModelFilename()
	, Vertices(p_convexPolyhedron.Vertices)
	, Edges()
	, Faces(p_convexPolyhedron.Faces)
{
	UpdateDerivativeData();
}

ConvexPolyhedron::~ConvexPolyhedron(void)
{
}

void ConvexPolyhedron::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties.Insert("Vertices",
		PropertyDescriptor("Vertices", PropertyType::PointList, false));
	p_mapProperties.Insert("Vertex",
		PropertyDescriptor("Vertex", PropertyType::IndexedPoint, false));
	p_mapProperties.Insert("FaceIndex",
		PropertyDescriptor("FaceIndex", PropertyType::Integer, false));
	p_mapProperties.Insert("FaceVertices",
		PropertyDescriptor("FaceVertices", PropertyType::IntegerList, false));
	p_mapProperties.Insert("WavefrontModelFilename",
		PropertyDescriptor("WavefrontModelFilename", PropertyType::String, false));
	p_mapProperties.Insert("Update",
		PropertyDescriptor("Update", PropertyType::Boolean, false));
}

void ConvexPolyhedron::GetProperty(const String& p_strName, bool& p_bValue) const
{
	if (p_strName == "Update")
		p_bValue = false;
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::GetProperty(const String& p_strName, int& p_nValue) const
{
	if (p_strName == "FaceIndex")
		p_nValue = m_nFaceIndex;
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::GetProperty(const String& p_strName, String& p_strValue) const
{
	if (p_strName == "WavefrontModelFilename")
	{
		p_strValue = m_strWavefrontModelFilename;
	}
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::GetProperty(const String& p_strName, size_t p_unIndex,
	TPoint3<Real>& p_ptValue) const
{
	if (p_strName == "Vertex")
		p_ptValue = Vertices[p_unIndex];
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::GetProperty(const String& p_strName, TList<int>& p_listValues) const
{
	if (p_strName == "FaceVertices")
	{
		if (m_nFaceIndex < 0 )
			throw new MesonException("FaceIndex not set.", __FILE__, __LINE__);

		ConvexPolyhedronFace& face = Faces[m_nFaceIndex];
		p_listValues.Clear();
		for (size_t unIndex = 0; unIndex < face.VertexIndices.Size(); unIndex++)
			p_listValues.Add(face.VertexIndices[unIndex]);
	}
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::GetProperty(const String& p_strName,
	PointList& p_listValues) const
{
	if (p_strName == "Vertices")
		p_listValues = Vertices;
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::SetProperty(const String& p_strName, bool p_bValue)
{
	if (p_strName == "Update")
		UpdateDerivativeData();
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::SetProperty(const String& p_strName, int p_nValue)
{
	if (p_strName == "FaceIndex")
	{
		if (p_nValue < 0)
			throw new MesonException("Invalid FaceIndex.", __FILE__, __LINE__);

		while (Faces.Size() <= (size_t) p_nValue)
			Faces.Add(ConvexPolyhedronFace(&Vertices));

		m_nFaceIndex = p_nValue;
	}
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::SetProperty(const String& p_strName, const String& p_strValue)
{
	if (p_strName == "WavefrontModelFilename")
	{
		m_strWavefrontModelFilename = p_strValue;
		
		// load geometry
		GeometryLoader geometryLoader;
		GeometryFaceArrayList listGeometryFaces;
		geometryLoader.LoadWavefrontGeometry(m_strWavefrontModelFilename,
			true, Vertices, listGeometryFaces);

		// convert geometry faces topolyhedron faces
		Edges.Clear();
		Faces.Clear();
		ConvexPolyhedronFace convexPolyhedronFace(&Vertices);
		for (size_t unFaceIndex = 0; unFaceIndex < listGeometryFaces.Size(); unFaceIndex++)
		{
			GeometryFace& geometryFace = listGeometryFaces[unFaceIndex];

			convexPolyhedronFace.VertexIndices.Clear();
			for (size_t unVertexIndex = 0;
				unVertexIndex <geometryFace.VertexIndices.Size(); unVertexIndex++)
				convexPolyhedronFace.VertexIndices.Add(
					geometryFace.VertexIndices[unVertexIndex]);

			Faces.Add(convexPolyhedronFace);
		}

		UpdateDerivativeData();

		Real rCoplanarTollerance = (Real) 0.01;
		if (!IsConvex(rCoplanarTollerance))
			throw new MesonException(
				"The Wavefront nodel file '" + m_strWavefrontModelFilename
				+ "' does not contain a convex model.",
				__FILE__, __LINE__);

		GravitasEngine::GetInstance()->Logger().Out
			<< "Convex Polyhedron loaded from Wavefront model file: "
			<< m_strWavefrontModelFilename << "\n";
		GravitasEngine::GetInstance()->Logger().Out
			<< "Convex Polyhedron Vertices: " << (int) Vertices.Size() << "\n";
		GravitasEngine::GetInstance()->Logger().Out
			<< "Convex Polyhedron Faces: " << (int) Faces.Size() << "\n";
	}
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::SetProperty(const String& p_strName, size_t p_unIndex, const TPoint3<Real>& p_ptValue)
{
	if (p_strName == "Vertex")
	{
		while (Vertices.Size() <= p_unIndex)
			Vertices.Add(TPoint3<Real>((Real) 0.0, (Real) 0.0, (Real) 0.0));
		Vertices[p_unIndex] = p_ptValue;
		m_strWavefrontModelFilename = "";
	}
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::SetProperty(const String& p_strName, const TList<int>& p_listValues)
{
	if (p_strName == "FaceVertices")
	{
		if (m_nFaceIndex < 0 )
			throw new MesonException("FaceIndex not set.", __FILE__, __LINE__);

		ConvexPolyhedronFace& face = Faces[m_nFaceIndex];
		face.VertexIndices.Clear();
		for (size_t unIndex = 0; unIndex < p_listValues.Size(); unIndex++)
			face.VertexIndices.Add(p_listValues[unIndex]);
		m_strWavefrontModelFilename = "";
	}
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

void ConvexPolyhedron::SetProperty(const String& p_strName, const PointList& p_listValues)
{
	if (p_strName == "Vertices")
	{
		Vertices = p_listValues;
		Faces.Clear();
		Edges.Clear();
		m_strWavefrontModelFilename = "";
	}
	else
		throw new MesonException(
			"ConvexPolyhedron geometry: Unsupported property: " + p_strName,
			__FILE__, __LINE__);
}

const String& ConvexPolyhedron::GetTypeName(void) const
{
	return s_strTypeName;
}

short ConvexPolyhedron::GetTypeId(void) const
{
	return m_nTypeId;
}

void ConvexPolyhedron::SetTypeId(short p_nId)
{
	m_nTypeId = p_nId;
}

Real ConvexPolyhedron::GetVolume(void) const
{
	Real rVolume((Real) 0.0);
	size_t unCount(Faces.Size());
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
		rVolume += Faces[unIndex].ComputeSignedVolume(TPoint3<Real>::Origin);

	return rVolume;
}

TMatrix3<Real> ConvexPolyhedron::GetDistributionTensor(void) const
{
	// initialise tensor integrals
	Real rTensorIntegralXX((Real) 0.0);
	Real rTensorIntegralYY((Real) 0.0);
	Real rTensorIntegralZZ((Real) 0.0);
	Real rTensorIntegralXY((Real) 0.0);
	Real rTensorIntegralYZ((Real) 0.0);
	Real rTensorIntegralZX((Real) 0.0);

	// process all faces
	size_t unFaceCount(Faces.Size());
	for (size_t unFaceIndex = 0; unFaceIndex < unFaceCount; unFaceIndex++)
	{
		ConvexPolyhedronFace &face = Faces(unFaceIndex);

		// process face as tessellated triangles
		size_t unVertexIndex0(0);
		size_t unFaceVertexCount(face.VertexIndices.Size());
		for (size_t unVertexIndex1 = 1, unVertexIndex2 = 2; unVertexIndex1 < unFaceVertexCount - 1;
			unVertexIndex1++, unVertexIndex2++)
		{
			// get triangle vertices
			TVector3<Real>& vecVertex0 =  Vertices[face.VertexIndices[unVertexIndex0]];
			TVector3<Real>& vecVertex1 =  Vertices[face.VertexIndices[unVertexIndex1]];
			TVector3<Real>& vecVertex2 =  Vertices[face.VertexIndices[unVertexIndex2]];

			// compute cross product from edges (same direction as normal)
			TVector3<Real> vecDoubleArea((vecVertex2 - vecVertex0) ^ (vecVertex1 - vecVertex0));

			// compute integral terms
			Real rF1x, rF2x, rF3x, rG0x, rG1x, rG2x;
			ComputeMassPropertySubExpressions(vecVertex0.X, vecVertex1.X, vecVertex2.X,
				rF1x, rF2x, rF3x, rG0x, rG1x, rG2x);

			Real rF1y, rF2y, rF3y, rG0y, rG1y, rG2y;
			ComputeMassPropertySubExpressions(vecVertex0.Y, vecVertex1.Y, vecVertex2.Y,
				rF1y, rF2y, rF3y, rG0y, rG1y, rG2y);

			Real rF1z, rF2z, rF3z, rG0z, rG1z, rG2z;
			ComputeMassPropertySubExpressions(vecVertex0.Z, vecVertex1.Z, vecVertex2.Z,
				rF1z, rF2z, rF3z, rG0z, rG1z, rG2z);

			// update integrals
			rTensorIntegralXX += vecDoubleArea.X * rF3x;
			rTensorIntegralYY += vecDoubleArea.Y * rF3y;
			rTensorIntegralZZ += vecDoubleArea.Z * rF3z;
			rTensorIntegralXY += vecDoubleArea.X * (vecVertex0.Y * rG0x + vecVertex1.Y * rG1x + vecVertex2.Y * rG2x);
			rTensorIntegralYZ += vecDoubleArea.Y * (vecVertex0.Z * rG0y + vecVertex1.Z * rG1y + vecVertex2.Z * rG2y);
			rTensorIntegralZX += vecDoubleArea.Z * (vecVertex0.X * rG0z + vecVertex1.X * rG1z + vecVertex2.X * rG2z);
		}
	}

	// apply integral multipliers
	Real rTensorDiagonalMultiplier = (Real) 1.0 / (Real) 60.0;
	rTensorIntegralXX *= rTensorDiagonalMultiplier;
	rTensorIntegralYY *= rTensorDiagonalMultiplier;
	rTensorIntegralZZ *= rTensorDiagonalMultiplier;

	Real rTensorUpperMultiplier = (Real) 1.0 / (Real) 120.0;
	rTensorIntegralXY *= rTensorUpperMultiplier;
	rTensorIntegralYZ *= rTensorUpperMultiplier;
	rTensorIntegralZX *= rTensorUpperMultiplier;

	// distribution tensor
	TMatrix3<Real> matDistributionTensor;

	// diagonals
	matDistributionTensor._11 = rTensorIntegralYY + rTensorIntegralZZ;
	matDistributionTensor._22 = rTensorIntegralXX + rTensorIntegralZZ;
	matDistributionTensor._33 = rTensorIntegralXX + rTensorIntegralYY;

	// upper terms
	matDistributionTensor._12 = -rTensorIntegralXY;
	matDistributionTensor._23 = -rTensorIntegralYZ;
	matDistributionTensor._13 = -rTensorIntegralZX;

	// lower terms (symmetric about diagonal)
	matDistributionTensor._21 = matDistributionTensor._12;
	matDistributionTensor._32 = matDistributionTensor._23;
	matDistributionTensor._31 = matDistributionTensor._13;

	return matDistributionTensor;
}

bool ConvexPolyhedron::IsBounded(void) const
{
	return true;
}

void ConvexPolyhedron::ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const
{
	TPoint3<Real> ptMin(TPoint3<Real>::Origin), ptMax(TPoint3<Real>::Origin);

	size_t unCount(Vertices.Size());
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		TPoint3<Real>& ptVertex = Vertices[unIndex];
		ptMin.X = TMaths<Real>::Min(ptMin.X, ptVertex.X);
		ptMin.Y = TMaths<Real>::Min(ptMin.Y, ptVertex.Y);
		ptMin.Z = TMaths<Real>::Min(ptMin.Z, ptVertex.Z);

		ptMax.X = TMaths<Real>::Max(ptMax.X, ptVertex.X);
		ptMax.Y = TMaths<Real>::Max(ptMax.Y, ptVertex.Y);
		ptMax.Z = TMaths<Real>::Max(ptMax.Z, ptVertex.Z);
	}

	p_boundingSphere.Centre = ptMin + (ptMax - ptMin) * (Real) 0.5;
	p_boundingSphere.RadiusSquared = (Real) 0.0;
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		TPoint3<Real>& ptVertex = Vertices[unIndex];
		p_boundingSphere.RadiusSquared = TMaths<Real>::Max(
			p_boundingSphere.RadiusSquared,
			(ptVertex - p_boundingSphere.Centre).LengthSquared());
	}
	p_boundingSphere.Radius = TMaths<Real>::Sqrt(p_boundingSphere.RadiusSquared);
}

void ConvexPolyhedron::ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	p_boundingAxisAlignedBox.Min = p_boundingAxisAlignedBox.Max = TPoint3<Real>::Origin;

	size_t unCount(Vertices.Size());
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		TPoint3<Real>& ptVertex = Vertices[unIndex];
		p_boundingAxisAlignedBox.Min.X = TMaths<Real>::Min(p_boundingAxisAlignedBox.Min.X, ptVertex.X);
		p_boundingAxisAlignedBox.Min.Y = TMaths<Real>::Min(p_boundingAxisAlignedBox.Min.Y, ptVertex.Y);
		p_boundingAxisAlignedBox.Min.Z = TMaths<Real>::Min(p_boundingAxisAlignedBox.Min.Z, ptVertex.Z);

		p_boundingAxisAlignedBox.Max.X = TMaths<Real>::Max(p_boundingAxisAlignedBox.Max.X, ptVertex.X);
		p_boundingAxisAlignedBox.Max.Y = TMaths<Real>::Max(p_boundingAxisAlignedBox.Max.Y, ptVertex.Y);
		p_boundingAxisAlignedBox.Max.Z = TMaths<Real>::Max(p_boundingAxisAlignedBox.Max.Z, ptVertex.Z);
	}
}

void ConvexPolyhedron::ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const
{
	// note: uses brute-force approach to fit an approximately good OBB

	Real rMinVolume(TMaths<Real>::Maximum);

	// process all faces to find first axis
	size_t unCount(Faces.Size());
	for (size_t unIndex1 = 0; unIndex1 < unCount; unIndex1++)
	{
		// align first axis with a face normal
		TInterval<Real> interval1;
		TVector3<Real> vecAxis1 = Faces[unIndex1].Normal;

		// compute interval corresponding to first axis
		ProjectToInterval(vecAxis1, interval1);

		// process all faces again to find second axis
		for (size_t unIndex2 = 0; unIndex2 < unCount; unIndex2++)
		{
			// skip face used for first axis
			if (unIndex2 == unIndex1) continue;

			// start by using second face normal as second axis
			TVector3<Real> vecAxis2(Faces[unIndex2].Normal);

			// remove second axis's component in directon of first axis
			// and normalise to make perpendicular
			vecAxis2 -= (vecAxis2 * vecAxis1) * vecAxis1;

			// skip if axis2 parallel to axis1
			if (vecAxis2.IsZero())
				continue;

			// otherwise normalise and use axis2
			vecAxis2.Normalise();	

			// compute third axis by cross product
			TVector3<Real> vecAxis3(vecAxis1 ^ vecAxis2);
		
			// compute intervals corresponding to second and third axes
			TInterval<Real> interval2, interval3;
		
			// compute current volume
			Real rSpan1(interval1.Span());
			Real rSpan2(interval2.Span());
			Real rSpan3(interval3.Span());
			Real rCurrentVolume(rSpan1 * rSpan2 * rSpan3);

			// skip these axes if current min volume is smaller
			if (rMinVolume < rCurrentVolume)
				continue;

			// otherwise compute new OBB

			// axes
			p_boundingOrientedBox.Axis[0] = vecAxis1;
			p_boundingOrientedBox.Axis[1] = vecAxis2;
			p_boundingOrientedBox.Axis[2] = vecAxis3;
			// extents
			p_boundingOrientedBox.Extent.X = rSpan1 * (Real) 0.5;
			p_boundingOrientedBox.Extent.Y = rSpan2 * (Real) 0.5;
			p_boundingOrientedBox.Extent.Z = rSpan3 * (Real) 0.5;
			// centre
			p_boundingOrientedBox.Centre = TPoint3<Real>::Origin
				+ vecAxis1 * interval1.Median()
				+ vecAxis2 * interval2.Median()
				+ vecAxis3 * interval3.Median();

			// update current best volume
			rMinVolume = rCurrentVolume;
		}
	}
}

bool ConvexPolyhedron::IntersectsRay(const Ray& p_ray) const
{
	static TPoint3<Real> s_ptIntersectionPoint;
	return IntersectsRay(p_ray, s_ptIntersectionPoint);
}

bool ConvexPolyhedron::IntersectsRay(const Ray& p_ray,
	TPoint3<Real>& p_ptIntersectionPoint) const
{
	// if ray source itself is inside, it is intersection point
	if (ContainsPoint(p_ray.Source))
	{
		p_ptIntersectionPoint = p_ray.Source;
		return true;
	}

	// otherwise, do face tests
	for (size_t unIndex = 0; unIndex < Faces.Size(); unIndex++)
	{
		ConvexPolyhedronFace& face = Faces[unIndex];

		// ignore faces facing away from ray source
		if (face.IsPointBelowPlane(p_ray.Source))
			continue;

		if (face.IntersectsRay(p_ray, p_ptIntersectionPoint))
			return true;
	}

	// otherwise, no intersection
	return false;
}

void ConvexPolyhedron::UpdateDerivativeData(bool p_bAlignCentroid)
{
	// recompute edges from faces
	Edges.Clear();

	for (size_t unFaceIndex = 0; unFaceIndex < Faces.Size(); unFaceIndex++)
	{
		ConvexPolyhedronFace& face = Faces[unFaceIndex];

		// update face
		face.Vertices = &Vertices;
		face.UpdateDerivativeData();

		size_t unFaceVertexIndex0(face.VertexIndices.Size() - 1);
		for (size_t unFaceVertexIndex1 = 0;
			unFaceVertexIndex1 < face.VertexIndices.Size(); unFaceVertexIndex1++)
		{
			short nVertex0 = face.VertexIndices[unFaceVertexIndex0];
			short nVertex1 = face.VertexIndices[unFaceVertexIndex1];

			// normalise edge vertex order to detect duplicates
			if (nVertex0 > nVertex1)
			{
				short nTemp(nVertex0);
				nVertex0 = nVertex1;
				nVertex1 = nTemp;
			}

			// create edge
			ConvexPolyhedronEdge edge(&Vertices, nVertex0, nVertex1);
			edge.UpdateDerivativeData();

			// do not insert duplicate edges
			if (Edges.IndexOf(edge) < 0)
				Edges.Add(edge);

			unFaceVertexIndex0 = unFaceVertexIndex1;
		}
	}

	// align centroid to origin
	if (p_bAlignCentroid)
		AlignCentroid();
}

TPoint3<Real> ConvexPolyhedron::GetCentroid(void) const
{
	size_t unFaceCount(Faces.Size());
	if (unFaceCount == 0)
		return TPoint3<Real>::Origin;

	// initialise centroid integrals
	Real rVolumeIntegral = (Real) 0.0;
	Real rCentroidIntegralX = (Real) 0.0;
	Real rCentroidIntegralY = (Real) 0.0;
	Real rCentroidIntegralZ = (Real) 0.0;

	// process all faces
	for (size_t unFaceIndex = 0; unFaceIndex < unFaceCount; unFaceIndex++)
	{
		ConvexPolyhedronFace &face = Faces(unFaceIndex);

		// process face as tessellated triangles
		size_t unVertexIndex0(0);
		size_t unFaceVertexCount(face.VertexIndices.Size());
		for (size_t unVertexIndex1 = 1, unVertexIndex2 = 2; unVertexIndex1 < unFaceVertexCount - 1;
			unVertexIndex1++, unVertexIndex2++)
		{
			// get triangle vertices
			TVector3<Real>& vecVertex0 =  Vertices[face.VertexIndices[unVertexIndex0]];
			TVector3<Real>& vecVertex1 =  Vertices[face.VertexIndices[unVertexIndex1]];
			TVector3<Real>& vecVertex2 =  Vertices[face.VertexIndices[unVertexIndex2]];

			// compute cross product form edges (same direction as normal)
			TVector3<Real> vecDoubleArea((vecVertex1 - vecVertex0) ^ (vecVertex2 - vecVertex0));

			// compute integral terms
			Real rF1x, rF2x, rF3x, rG0x, rG1x, rG2x;
			ComputeMassPropertySubExpressions(vecVertex0.X, vecVertex1.X, vecVertex2.X,
				rF1x, rF2x, rF3x, rG0x, rG1x, rG2x);

			Real rF1y, rF2y, rF3y, rG0y, rG1y, rG2y;
			ComputeMassPropertySubExpressions(vecVertex0.Y, vecVertex1.Y, vecVertex2.Y,
				rF1y, rF2y, rF3y, rG0y, rG1y, rG2y);

			Real rF1z, rF2z, rF3z, rG0z, rG1z, rG2z;
			ComputeMassPropertySubExpressions(vecVertex0.Z, vecVertex1.Z, vecVertex2.Z,
				rF1z, rF2z, rF3z, rG0z, rG1z, rG2z);

			// update integrals
			rVolumeIntegral += vecDoubleArea.X * rF1x;

			rCentroidIntegralX += vecDoubleArea.X * rF2x;
			rCentroidIntegralY += vecDoubleArea.Y * rF2y;
			rCentroidIntegralZ += vecDoubleArea.Z * rF2z;
		}
	}

	// centroid
	TPoint3<Real> ptCentroid(TPoint3<Real>::Origin);

	if (TMaths<Real>::Equals(rVolumeIntegral, (Real) 0.0))
		return ptCentroid;

	Real rMultiplier((Real) 0.25 / rVolumeIntegral);
	ptCentroid.X = rCentroidIntegralX * rMultiplier;
	ptCentroid.Y = rCentroidIntegralY * rMultiplier;
	ptCentroid.Z = rCentroidIntegralZ * rMultiplier;

	return ptCentroid;
}

void ConvexPolyhedron::AlignCentroid(void)
{
	TVector3<Real> vecOffset(TPoint3<Real>::Origin - GetCentroid());
	for (size_t unIndex = 0; unIndex < Vertices.Size(); unIndex++)
		Vertices[unIndex] += vecOffset;
}

void ConvexPolyhedron::ProjectToInterval(
	const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
	Meson::Common::Maths::TInterval<Real>& p_interval) const
{
	p_interval.MakeEmpty();
	for (size_t unVertexIndex = 0; unVertexIndex < Vertices.Size(); unVertexIndex++)
		p_interval.Extend(Vertices[unVertexIndex].ToVector() * p_vecAxis);
}

void ConvexPolyhedron::EnumerateMaximalVertices(
	const TVector3<Real>& p_vecDirection, PointList& p_listVertices) const
{
	// clear vertex list
	p_listVertices.Clear();

	Real rMaximumProjection(-TMaths<Real>::Maximum);

	// determine maximal vertices in terms of projection
	for (size_t unIndex = 0; unIndex < Vertices.Size(); unIndex++)
	{
		TPoint3<Real>& ptVertex = Vertices[unIndex];
		Real rProjection(p_vecDirection * ptVertex.ToVector());

		// ignore vertices that are definitely less extreme than current
		if (rProjection + TMaths<Real>::Epsilon < rMaximumProjection) continue;

		// if new projection definitely greater than current maximum, discard
		// current maximal vertices
		if (rProjection - TMaths<Real>::Epsilon > rMaximumProjection)
			p_listVertices.Clear();

		rMaximumProjection = rProjection;
		p_listVertices.Add(ptVertex);
	}
}

bool ConvexPolyhedron::ContainsPoint(const TPoint3<Real>& ptPoint) const
{
	for (size_t unIndex = 0; unIndex < Faces.Size(); unIndex++)
		if (Faces[unIndex].IsPointAbovePlane(ptPoint))
			return false;

	return true;
}

TPoint3<Real> ConvexPolyhedron::GetClosestPoint(
	const Meson::Common::Maths::TPoint3<Real>& ptPoint) const
{
	// if no vertices, return origin
	if (Vertices.Size() == 0)
		return TPoint3<Real>::Origin;

	// prepare variables
	static PointArrayList listSimplexVertices, listMaximalVertices;
	TPoint3<Real> ptClosest;
	LineSegment lineSegment;
	Triangle triangle;
	Tetrahedron tetrahedron;

	// start with 0D simplex
	listSimplexVertices.Clear();
	listSimplexVertices.Add(Vertices[0]);

	for (size_t unIterations = 0; unIterations < Vertices.Size(); unIterations++)
	{
		// find norm in current simplex
		switch (listSimplexVertices.Size())
		{
		case 1:
			// closest of 0D simplex is point itself
			ptClosest = listSimplexVertices[0];
			break;
		case 2:
			// closest of 1D simplex is closest to line segment
			lineSegment.Start = listSimplexVertices[0];
			lineSegment.End = listSimplexVertices[1];
			ptClosest = lineSegment.ClosestPoint(ptPoint);
			break;
		case 3:
			// closest of 2D simplex is closest to triangle
			triangle.Vertices[0] = listSimplexVertices[0];
			triangle.Vertices[1] = listSimplexVertices[1];
			triangle.Vertices[2] = listSimplexVertices[2];
			ptClosest = triangle.ClosestPoint(ptPoint);
			break;
		case 4:
			// closest of 2D simplex is closest to tetrahedron
			tetrahedron.Vertices[0] = listSimplexVertices[0];
			tetrahedron.Vertices[1] = listSimplexVertices[1];
			tetrahedron.Vertices[2] = listSimplexVertices[2];
			tetrahedron.Vertices[3] = listSimplexVertices[3];
			ptClosest = tetrahedron.ClosestPoint(ptPoint);
			break;
		default:
			MESON_ASSERT(false, "GJK point containment algorithm error.")
			break;
		}

		// if current simplex contains point, return it as closest
		// (contained inside)
		if (ptClosest == ptPoint)
			return ptPoint;

		// reduce current simplex by eliminating redundant vertices
		TVector3<Real> vecPointOffset(ptPoint - ptClosest);
		for (size_t unIndex = 0; unIndex < listSimplexVertices.Size();)
		{
			TVector3<Real> vecVertexOffset(listSimplexVertices[unIndex] - ptClosest);
			if (vecPointOffset * vecVertexOffset < -TMaths<Real>::Epsilon)
				listSimplexVertices.RemoveAt(unIndex);
			else
				++unIndex;
		}

		// find new max vertex in direction of point (chose one if many)
		EnumerateMaximalVertices(vecPointOffset, listMaximalVertices);
		TPoint3<Real>& ptNewVertex = listMaximalVertices[0];

		// if new vertex is already part of simplex, cannot find closer point
		// (point is external and closest is on boundary)
		for (size_t unIndex = 0; unIndex < listSimplexVertices.Size(); unIndex++)
			if (listSimplexVertices[unIndex] == ptNewVertex)
				return ptClosest;

		// otherwise, add new vertex to simplex
		listSimplexVertices.Add(listMaximalVertices[0]);
	}

	return ptClosest;
}

ConvexPolyhedron ConvexPolyhedron::TransformCopy(
	const Meson::Gravitas::Geometry::Transform& p_transform) const
{
	ConvexPolyhedron convexPolyhedron(*this);

	// transform vertices
	p_transform.Apply(convexPolyhedron.Vertices);
	
	// rotate face normals
	TQuaternion<Real> qtnConjugate(p_transform.Rotation.ConjugateCopy());
	size_t unCount(convexPolyhedron.Faces.Size());
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		TVector3<Real>& vecNormal = convexPolyhedron.Faces[unIndex].Normal;
		vecNormal = (p_transform.Rotation * vecNormal * qtnConjugate).Vector;
	}

	return convexPolyhedron;
}

void ConvexPolyhedron::TransformCopy(
	const Meson::Gravitas::Geometry::Transform& p_transform,
	ConvexPolyhedron& p_convexPolyhedron) const
{
	p_convexPolyhedron.Vertices = Vertices;
	p_convexPolyhedron.Faces = Faces;
	p_convexPolyhedron.Edges = Edges;

	// transform vertices
	p_transform.Apply(p_convexPolyhedron.Vertices);

	TQuaternion<Real> qtnConjugate(p_transform.Rotation.ConjugateCopy());

	// update face vertex list pointers and rotate face normals
	size_t unCount(p_convexPolyhedron.Faces.Size());
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		ConvexPolyhedronFace& face = p_convexPolyhedron.Faces[unIndex];
		face.Vertices = &Vertices;
		face.Normal = (p_transform.Rotation * face.Normal * qtnConjugate).Vector;
	}

	// update edge vertex list pointers and rotate displacements
	unCount = p_convexPolyhedron.Edges.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		ConvexPolyhedronEdge& edge = p_convexPolyhedron.Edges[unIndex];
		edge.Vertices = &Vertices;
		edge.Displacement = (p_transform.Rotation * edge.Displacement * qtnConjugate).Vector;
	}
}

ConvexPolyhedron& ConvexPolyhedron::operator=(const ConvexPolyhedron& p_convexPolyhedron)
{
	Vertices = p_convexPolyhedron.Vertices;
	Faces = p_convexPolyhedron.Faces;
	UpdateDerivativeData();
	return *this;
}
