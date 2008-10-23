#include "Polyhedron.h"

#include "Maths.h"
#include "TreeSet.h"
#include "MesonException.h"
#include "Sorting.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;

Meson_Gravitas_Geometry_BEGIN
	
// Polyhedron methods

const String g_strGeometryTypePolyhedron = "Polyhedron";

bool Polyhedron::IsConvex(void) const
{
	// check all faces
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);
		// check all vertices against each face
		for (size_t unVertexIndex = 0; unVertexIndex < m_listVertices.Size(); unVertexIndex++)
		{
			// ignore vertices referenced by face
			if (polyhedronFace.HasVertexIndex(unVertexIndex))
				continue;
			// not convex if any vertex is on outer side of face plane
			if (polyhedronFace.IsOutside(m_listVertices(unVertexIndex)))
				return false;
		}
	}

	// otherwise, convex
	return true;
}

void Polyhedron::UpdateNormals(void)
{
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
		m_listFaces(unFaceIndex).UpdateNormal();
}

void Polyhedron::UpdateFacesAndEdges(void)
{
	// clear edge list
	m_listEdges.Clear();

	for (size_t unFaceIndex1 = 0; unFaceIndex1 < m_listFaces.Size(); unFaceIndex1++)
	{
		PolyhedronFace &polyhedronFace1 = m_listFaces(unFaceIndex1);
		size_t (&unIndices1)[3] = m_listFaces(unFaceIndex1).VertexIndex;

		for (size_t unFaceIndex2 = unFaceIndex1 + 1; unFaceIndex2 < m_listFaces.Size(); unFaceIndex2++)
		{
			PolyhedronFace &polyhedronFace2 = m_listFaces(unFaceIndex2);
			size_t (&unIndices2)[3] = m_listFaces(unFaceIndex2).VertexIndex;

			// incidence tests against face 1, edge 0

			if ((unIndices1[0] == unIndices2[1]) && (unIndices1[1] == unIndices2[0]))
			{
				// faces both incident on edge 0
				polyhedronFace1.FaceIndex[0] = unFaceIndex2;
				polyhedronFace2.FaceIndex[0] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[0], unIndices1[1]));
			}

			if ((unIndices1[0] == unIndices2[2]) && (unIndices1[1] == unIndices2[1]))
			{
				// face 1 edge 0 incident to face 2 edge 1
				polyhedronFace1.FaceIndex[0] = unFaceIndex2;
				polyhedronFace2.FaceIndex[1] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[0], unIndices1[1]));
			}

			if ((unIndices1[0] == unIndices2[0]) && (unIndices1[1] == unIndices2[2]))
			{
				// face 1 edge 0 incident to face 2 edge 2
				polyhedronFace1.FaceIndex[0] = unFaceIndex2;
				polyhedronFace2.FaceIndex[2] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[0], unIndices1[1]));
			}

			// incidence tests against face 1, edge 1

			if ((unIndices1[1] == unIndices2[1]) && (unIndices1[2] == unIndices2[0]))
			{
				// face 1 edge 1 incident to face 2 edge 0
				polyhedronFace1.FaceIndex[1] = unFaceIndex2;
				polyhedronFace2.FaceIndex[0] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[1], unIndices1[2]));
			}

			if ((unIndices1[1] == unIndices2[2]) && (unIndices1[2] == unIndices2[1]))
			{
				// face 1 edge 1 incident to face 2 edge 1
				polyhedronFace1.FaceIndex[1] = unFaceIndex2;
				polyhedronFace2.FaceIndex[1] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[1], unIndices1[2]));
			}

			if ((unIndices1[1] == unIndices2[0]) && (unIndices1[2] == unIndices2[2]))
			{
				// face 1 edge 1 incident to face 2 edge 2
				polyhedronFace1.FaceIndex[1] = unFaceIndex2;
				polyhedronFace2.FaceIndex[2] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[1], unIndices1[2]));
			}

			// incidence tests against face 1, edge 2

			if ((unIndices1[2] == unIndices2[1]) && (unIndices1[0] == unIndices2[0]))
			{
				// face 1 edge 2 incident to face 2 edge 0
				polyhedronFace1.FaceIndex[2] = unFaceIndex2;
				polyhedronFace2.FaceIndex[0] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[2], unIndices1[0]));
			}

			if ((unIndices1[2] == unIndices2[2]) && (unIndices1[0] == unIndices2[1]))
			{
				// face 1 edge 2 incident to face 2 edge 1
				polyhedronFace1.FaceIndex[2] = unFaceIndex2;
				polyhedronFace2.FaceIndex[1] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[2], unIndices1[0]));
			}

			if ((unIndices1[2] == unIndices2[0]) && (unIndices1[0] == unIndices2[2]))
			{
				// face 1 edge 2 incident to face 2 edge 2
				polyhedronFace1.FaceIndex[2] = unFaceIndex2;
				polyhedronFace2.FaceIndex[2] = unFaceIndex1;

				// add edge
				m_listEdges.Add(PolyhedronEdge(unIndices1[2], unIndices1[0]));
			}
		}
	}
}

void Polyhedron::ComputeMassPropertySubExpressions(
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

/*
bool Polyhedron::Intercepts(
	const Particle &p_particle,
	const TVector3<Real> &p_vecOffset,
	const TVector3<Real> &p_vecVelocity,
	const TQuaternion<Real> &p_qtrOrientation,
	Real p_rMaxTime, Real &p_rTime) const
{
	// compute position relative to polyhedron at origin
	TPoint3<Real> ptParticle = TPoint3<Real>::Origin + p_vecOffset;

	// if point in polygon, collision is now
	if (Intersects(ptParticle))
	{
		p_rTime = (Real) 0.0;
		return true;
	}

	// otherwise, particle is outside but may collide

	// if velocity is zero, particle will never collide
	if (p_vecVelocity.IsZero())
		return false;

	// sweep particle motion as line segment
	TPoint3<Real> ptStart = TPoint3<Real>::Origin + p_vecOffset;
	LineSegment lineSegment(ptParticle,
		ptParticle + p_vecVelocity * p_rMaxTime);

	// prepare for line segment intersection
	size_t unOccurences = 0;
	TPoint3<Real> ptIntersectionPointNear, ptIntersectionPointFar;

	// if no intersection, no intercept
	if (!IntersectsBoundary(lineSegment, unOccurences, ptIntersectionPointNear, ptIntersectionPointFar))
		return false;

	// otherwise, determine time from collision point in sweep

	// get vector distance of collision point from particle
	TVector3<Real> vecOffset = ptStart - ptIntersectionPointNear;

	// determine time of impact by dividing distance by speed
	p_rTime = vecOffset.Length() / p_vecVelocity.Length();

	// ignore if time greater than max
	if (p_rTime > p_rMaxTime) return false;

	// otherwise intersection occurs within max time
	return true;
}*/

Polyhedron::Polyhedron(void)
	: m_nId(-1)
	, m_listVertices()
	, m_listFaces()
{
}

Polyhedron::Polyhedron(const Polyhedron& p_polyhedron)
	: m_listVertices(p_polyhedron.m_listVertices)
	, m_listFaces(p_polyhedron.m_listFaces)
{
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
		m_listFaces(unFaceIndex).VertexList = &m_listVertices;
}

Polyhedron &Polyhedron::operator=(const Polyhedron& p_polyhedron)
{
	m_listVertices = p_polyhedron.m_listVertices;
	m_listFaces = p_polyhedron.m_listFaces;
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
		m_listFaces(unFaceIndex).VertexList = &m_listVertices;
	return *this;
}

void Polyhedron::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties["Vertices"] = PropertyDescriptor("Vertices", PropertyType::PointList, false);
	p_mapProperties["Vertex"] = PropertyDescriptor("Vertex", PropertyType::IndexedPoint, false);
	p_mapProperties["FaceVertexIndices"] = PropertyDescriptor("FaceVertexIndices", PropertyType::IntegerList, false);
	p_mapProperties["FaceVertexIndex"] = PropertyDescriptor("FaceVertexIndex", PropertyType::IndexedInteger, false);
	p_mapProperties["EdgeVertexIndices"] = PropertyDescriptor("EdgeVertexIndices", PropertyType::IntegerList, true);
	p_mapProperties["EdgeVertexIndex"] = PropertyDescriptor("EdgeVertexIndex", PropertyType::IndexedInteger, true);
}

void Polyhedron::GetProperty(const String& p_strName, size_t p_unIndex, int& p_nValue)
{
	if (p_strName == "FaceVertexIndex")
		p_nValue = (int) m_listFaces[p_unIndex / 3].VertexIndex[p_unIndex % 3];
	else if (p_strName == "EdgeVertexIndex")
		p_nValue = (int) m_listEdges[p_unIndex / 2].VertexIndex[p_unIndex % 2];
	else
		MESON_ASSERT("false", "Polyhedron geometry: Unsupported property.");
}

void Polyhedron::GetProperty(const String& p_strName, size_t p_unIndex,
	TPoint3<Real>& p_ptValue) const
{
	if (p_strName == "Vertex")
		p_ptValue = m_listVertices[p_unIndex];
	else
		MESON_ASSERT("false", "Polyhedron geometry: Unsupported property.");
}

void Polyhedron::GetProperty(const String& p_strName,
	TList<int>& p_listValues) const
{
	if (p_strName == "FaceVertexIndices")
	{
		p_listValues.Clear();
		TEnumerator<PolyhedronFace>& enumFaces 
			= ((TArrayList<PolyhedronFace>& ) m_listFaces).GetEnumerator();
		while(enumFaces.HasMoreElements())
		{
			PolyhedronFace &polyhedronFace = enumFaces.NextElement();
			p_listValues.Add((int) polyhedronFace.VertexIndex[0]);
			p_listValues.Add((int) polyhedronFace.VertexIndex[1]);
			p_listValues.Add((int) polyhedronFace.VertexIndex[2]);
		}
	}
	else if (p_strName == "EdgeVertexIndices")
	{
		p_listValues.Clear();
		TEnumerator<PolyhedronEdge> &enumEdges
			= ((TArrayList<PolyhedronEdge>& ) m_listEdges).GetEnumerator();
		while(enumEdges.HasMoreElements())
		{
			PolyhedronEdge &polyhedronEdge = enumEdges.NextElement();
			p_listValues.Add((int) polyhedronEdge.VertexIndex[0]);
			p_listValues.Add((int) polyhedronEdge.VertexIndex[1]);
		}
	}
	else
		MESON_ASSERT("false", "Polyhedron geometry: Unsupported property.");
}

void Polyhedron::GetProperty(const String& p_strName,
	PointList& p_listValues) const
{
	if (p_strName == "Vertices")
		p_listValues = m_listVertices;
	else
		MESON_ASSERT("false", "Polyhedron geometry: Unsupported property.");
}

void Polyhedron::SetProperty(const String& p_strName, size_t p_unIndex, int p_nValue)
{
	if (p_strName == "FaceVertexIndex")
	{
		size_t unFaceIndex = p_unIndex / 3;
		while (m_listFaces.Size() <= unFaceIndex)
			m_listFaces.Add(PolyhedronFace(&m_listVertices, 0, 0, 0));
		m_listFaces[unFaceIndex].VertexIndex[p_nValue % 3] = p_nValue;

		UpdateFacesAndEdges();
		AlignCentroid();
	}
	else
		MESON_ASSERT("false", "Polyhedron geometry: Unsupported property.");
}

void Polyhedron::SetProperty(const String& p_strName, size_t p_unIndex, const TPoint3<Real>& p_ptValue)
{
	if (p_strName == "Vertex")
	{
		while (m_listVertices.Size() <= p_unIndex)
			m_listVertices.Add(TPoint3<Real>((Real) 0.0, (Real) 0.0, (Real) 0.0));
		m_listVertices[p_unIndex] = p_ptValue;

		AlignCentroid();
	}
	else
		MESON_ASSERT("false", "Polyhedron geometry: Unsupported property.");
}

void Polyhedron::SetProperty(const String& p_strName, const TList<int>& p_listValues)
{
	if (p_strName == "FaceVertexIndices")
	{
		MESON_ASSERT(p_listValues.Size() % 3 == 0,
			"FaceVertexIndices list size must be a multiple of three.");

		m_listFaces.Clear();
		TEnumerator<int> &enumValues = ((TList<int> &) p_listValues).GetEnumerator();
		while(enumValues.HasMoreElements())
			m_listFaces.Add(PolyhedronFace(&m_listVertices,
				enumValues.NextElement(),
				enumValues.NextElement(),
				enumValues.NextElement()));

		UpdateFacesAndEdges();
		AlignCentroid();
	}
	else
		MESON_ASSERT("false", "Polyhedron volume: Unsupported property.");
}

void Polyhedron::SetProperty(const String& p_strName, const PointList& p_listValues)
{
	if (p_strName == "Vertices")
	{
		m_listFaces.Clear();
		m_listVertices = p_listValues;
	}
	else
		MESON_ASSERT("false", "Polyhedron geometry: Unsupported property.");
}

TPoint3<Real> Polyhedron::GetCentroid(void) const
{
	// initialise centroid integrals
	Real rVolumeIntegral = (Real) 0.0;
	Real rCentroidIntegralX = (Real) 0.0;
	Real rCentroidIntegralY = (Real) 0.0;
	Real rCentroidIntegralZ = (Real) 0.0;

	// process all faces
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);

		// get face vertices
		TVector3<Real> &vecVertex0 =  m_listVertices(polyhedronFace.VertexIndex[0]);
		TVector3<Real> &vecVertex1 =  m_listVertices(polyhedronFace.VertexIndex[1]);
		TVector3<Real> &vecVertex2 =  m_listVertices(polyhedronFace.VertexIndex[2]);

		// compute cross product from edges (same direction as normal)
		TVector3<Real> vecD = (vecVertex1 - vecVertex0) ^ (vecVertex2 - vecVertex0);

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
		rVolumeIntegral += vecD.X * rF1x;

		rCentroidIntegralX += vecD.X * rF2x;
		rCentroidIntegralY += vecD.Y * rF2y;
		rCentroidIntegralZ += vecD.Z * rF2z;
	}

	// centroid
	Real rMultiplier = (Real) 0.25 / rVolumeIntegral;
	TPoint3<Real> ptCentroid;
	ptCentroid.X = rCentroidIntegralX * rMultiplier;
	ptCentroid.Y = rCentroidIntegralY * rMultiplier;
	ptCentroid.Z = rCentroidIntegralZ * rMultiplier;

	return ptCentroid;
}

void Polyhedron::AddFace(size_t p_unVertexIndex0, size_t p_unVertexIndex1, size_t p_unVertexIndex2)
{
	m_listFaces.Add(PolyhedronFace(&m_listVertices,
		p_unVertexIndex0, p_unVertexIndex1, p_unVertexIndex2));
}

void Polyhedron::AddFace(const size_t* p_unVertexIndices)
{
	m_listFaces.Add(PolyhedronFace(&m_listVertices, p_unVertexIndices));
}

void Polyhedron::MakeCube(Real p_rWidth, Real p_rHeight, Real p_rDepth)
{
	m_listFaces.Clear();
	m_listVertices.Clear();
	Real rHalfWidth = p_rWidth * (Real) 0.5;
	Real rHalfHeight = p_rHeight * (Real) 0.5;
	Real rHalfDepth = p_rDepth * (Real) 0.5;
	m_listVertices.Add(TPoint3<Real>(-rHalfWidth, -rHalfHeight, +rHalfDepth));
	m_listVertices.Add(TPoint3<Real>(+rHalfWidth, -rHalfHeight, +rHalfDepth));
	m_listVertices.Add(TPoint3<Real>(+rHalfWidth, +rHalfHeight, +rHalfDepth));
	m_listVertices.Add(TPoint3<Real>(-rHalfWidth, +rHalfHeight, +rHalfDepth));
	m_listVertices.Add(TPoint3<Real>(-rHalfWidth, -rHalfHeight, -rHalfDepth));
	m_listVertices.Add(TPoint3<Real>(+rHalfWidth, -rHalfHeight, -rHalfDepth));
	m_listVertices.Add(TPoint3<Real>(+rHalfWidth, +rHalfHeight, -rHalfDepth));
	m_listVertices.Add(TPoint3<Real>(-rHalfWidth, +rHalfHeight, -rHalfDepth));

	// front
	AddFace(0, 1, 2);
	AddFace(2, 3, 0);

	// back
	AddFace(4, 6, 5);
	AddFace(6, 4, 7);

	// left
	AddFace(1, 5, 6);
	AddFace(6, 2, 1);

	// right
	AddFace(0, 3, 7);
	AddFace(7, 4, 0);

	// bottom
	AddFace(1, 0, 4);
	AddFace(4, 5, 1);

	// top
	AddFace(3, 2, 6);
	AddFace(6, 7, 3);

	UpdateFacesAndEdges();
}

void Polyhedron::MakeConvexHull(
	const TList<TPoint3<Real>> &p_listPoints, bool p_bCleanUp)
{
	// minimum points required for 3D simplex
	if (p_listPoints.Size() < 4) return;

	// clear vertex and face lists
	m_listVertices.Clear();
	m_listFaces.Clear();

	// prepare list of pending points to process
	TArrayList<TPoint3<Real>> listPendingPoints = p_listPoints;

	// select 4 extremal points for initial simplex

	// left-most point
	size_t unIndexSelected = 0;
	for (size_t unIndex = 0; unIndex < listPendingPoints.Size(); unIndex++)
		if (listPendingPoints(unIndexSelected).X > listPendingPoints(unIndex).X)
			unIndexSelected = unIndex;
	m_listVertices.Add(listPendingPoints(unIndexSelected));
	listPendingPoints.RemoveAt(unIndexSelected);

	// right-most point
	unIndexSelected = 0;
	for (size_t unIndex = 0; unIndex < listPendingPoints.Size(); unIndex++)
		if (listPendingPoints(unIndexSelected).X < listPendingPoints(unIndex).X)
			unIndexSelected = unIndex;
	m_listVertices.Add(listPendingPoints(unIndexSelected));
	listPendingPoints.RemoveAt(unIndexSelected);

	// top-most point
	unIndexSelected = 0;
	for (size_t unIndex = 0; unIndex < listPendingPoints.Size(); unIndex++)
		if (listPendingPoints(unIndexSelected).Y < listPendingPoints(unIndex).Y)
			unIndexSelected = unIndex;
	m_listVertices.Add(listPendingPoints(unIndexSelected));
	listPendingPoints.RemoveAt(unIndexSelected);

	// construct initial face for simplex
	AddFace(0, 1, 2);
	PolyhedronFace &polyhedronFaceFirst = m_listFaces(0);

	// select 4th point for simplex
	for (size_t unIndex = 0; unIndex < listPendingPoints.Size(); unIndex++)
	{
		TPoint3<Real> &ptPoint = listPendingPoints(unIndex);
		Real rDistance = polyhedronFaceFirst.SignedDistanceFromPlane(ptPoint);

		// skip coplanar points
		if (TMaths<Real>::Equals(rDistance, (Real) 0.0)) continue;

		// add point
		m_listVertices.Add(ptPoint);
		listPendingPoints.RemoveAt(unIndex);

		// add 3 more faces
		AddFace(0, 1, 3);
		AddFace(1, 2, 3);
		AddFace(2, 0, 3);

		// done
		break;
	}

	if (m_listVertices.Size() < 4)
		throw new Meson::Common::MesonException("All points are coplanar.", __FILE__, __LINE__);

	// ensure simplex faces are properly oriented
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);
		for (size_t unVertexIndex = 0; unVertexIndex < m_listVertices.Size(); unVertexIndex++)
		{
			if (polyhedronFace.HasVertexIndex(unVertexIndex))
				continue;
			if (polyhedronFace.IsOutside(m_listVertices(unVertexIndex)))
				polyhedronFace.Invert();
		}
	}

	// update face neighbours
	UpdateFacesAndEdges();

	// prepare working containers
	int x = sizeof(PolyhedronFace);
	TArrayList<size_t> listDiscoveredFaceIndices;
	TTreeSet<size_t> setProcessedFaceIndices;
	TTreeSet<size_t> setVisibleFaceIndices;
	TArrayList<PolyhedronEdge> listHorizonEdges;

	// grow hull until all pending points processed
	while (listPendingPoints.Size() > 0)
	{
		// prune all points already inside polyhedron
		for (int nPointIndex = (int) listPendingPoints.Size() - 1; nPointIndex >= 0 ; nPointIndex--)
			if (Intersects(listPendingPoints((size_t) nPointIndex)))
				listPendingPoints.RemoveAt(nPointIndex);

		// end if no more pending points
		if (listPendingPoints.Size() == 0) break;

		// find face with outside points
		int nFurthestIndex = -1; Real rFurthestDistance = (Real) 0.0;
		int nSelectedFace = -1;
		for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
		{
			PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);

			//determine furthest 'outside' point from supporting plane
			for (size_t unPointIndex = 0; unPointIndex < listPendingPoints.Size(); unPointIndex++)
			{
				// get signed distance from plane
				Real rCurrentDistance = polyhedronFace.SignedDistanceFromPlane(
					listPendingPoints(unPointIndex));

				// ignore point if negative distance i.e. it is on other side
				if (rCurrentDistance < (Real) 0.0) continue;

				// ignore if less than current furthest distance
				if (rFurthestDistance >= rCurrentDistance) continue;

				// make current point the furthest
				rFurthestDistance = rCurrentDistance;
				nFurthestIndex = (int) unPointIndex;
			}

			// found first face with at least one outside point (furthest)
			if (nFurthestIndex >= 0)
			{
				nSelectedFace = (int) unFaceIndex;
				break;
			}
		}

		// stop if no faces found with outside points
		if (nFurthestIndex == -1) return;

		// get furthest point
		TPoint3<Real> ptFurthestPoint = listPendingPoints(nFurthestIndex);

		// eliminate furthest point from pending list
		listPendingPoints.RemoveAt(nFurthestIndex);

		// add new point
		m_listVertices.Add(ptFurthestPoint);
		size_t unFurthestPointIndex = m_listVertices.Size() - 1;

		// determine all faces visible to this point
		// also, determine horizon edges
		setProcessedFaceIndices.Clear();
		setVisibleFaceIndices.Clear();
		listDiscoveredFaceIndices.Clear();
		listHorizonEdges.Clear();
		setVisibleFaceIndices.Insert(nSelectedFace);
		bool bMoreFaces = true;
		while (bMoreFaces)
		{
			bMoreFaces = false;

			TEnumerator<size_t> &enumFaceIndices = setVisibleFaceIndices.GetEnumerator();
			while (enumFaceIndices.HasMoreElements())
			{
				size_t unFaceIndex = enumFaceIndices.NextElement();
				PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);

				// skip this face if already processed
				if (setProcessedFaceIndices.ContainsElement(unFaceIndex))
					continue;

				// otherwise, at least one more face found
				bMoreFaces = true;

				// test neighbouring faces
				for (size_t unNeighbour = 0; unNeighbour < 3; unNeighbour++)
				{
					size_t unNeighbourFaceIndex
						= polyhedronFace.FaceIndex[unNeighbour];
					PolyhedronFace &polyhedronFaceNeighbour
						= m_listFaces(unNeighbourFaceIndex);

					// skip if already processed
					if (setProcessedFaceIndices.ContainsElement(unNeighbourFaceIndex))
						continue;

					// check if face beyond horizon
					if (!polyhedronFaceNeighbour.IsOutside(ptFurthestPoint))
						// face is beyond horizon, keep corresponding horizon edge
						listHorizonEdges.Add(
							PolyhedronEdge(
								polyhedronFace.VertexIndex[unNeighbour],
								polyhedronFace.VertexIndex[(unNeighbour + 1) % 3]));
					else
						// add to discovered visible faces
						listDiscoveredFaceIndices.Add(polyhedronFace.FaceIndex[unNeighbour]);
				}

				// add current face to processed set
				setProcessedFaceIndices.Insert(unFaceIndex);
			}

			// add newly discovered faces to visible set
			for (size_t unIndex = 0; unIndex < listDiscoveredFaceIndices.Size(); unIndex++)
				setVisibleFaceIndices.Insert(listDiscoveredFaceIndices(unIndex));
			listDiscoveredFaceIndices.Clear();
		}

		// build faces
		for (size_t unHorizonEdgeIndex  = 0; unHorizonEdgeIndex < listHorizonEdges.Size(); unHorizonEdgeIndex++)
		{
			PolyhedronEdge &polyhedronEdge = listHorizonEdges(unHorizonEdgeIndex);

			PolyhedronFace polyhedronFace(&m_listVertices,
				polyhedronEdge.VertexIndex[0],
				polyhedronEdge.VertexIndex[1],
				unFurthestPointIndex);

			m_listFaces.Add(polyhedronFace);
		}

		// eliminate old faces
		TArrayList<size_t> listFacesToRemove;
		TEnumerator<size_t> &enumFaceIndices = setVisibleFaceIndices.GetEnumerator();
		while (enumFaceIndices.HasMoreElements())
			listFacesToRemove.Add(enumFaceIndices.NextElement());
		TSorting<size_t>::QuickSort(listFacesToRemove);
		for (int nIndexToRemove = (int) listFacesToRemove.Size() - 1; nIndexToRemove >= 0; nIndexToRemove--)
			m_listFaces.RemoveAt(listFacesToRemove(nIndexToRemove));

		// update face neighbours
		UpdateFacesAndEdges();

		// check for early out
		if (listPendingPoints.Size() == 0) break;
	}

	if (p_bCleanUp)
		EliminateUnusedVertices();
}

void Polyhedron::MakeMinkowskiDifference(const Polyhedron &p_polyhedron1, const Polyhedron &p_polyhedron2, bool p_bCleanUp)
{
	TArrayList<TPoint3<Real>> listPoints;
	for (size_t unIndex1 = 0; unIndex1 < p_polyhedron1.m_listVertices.Size(); unIndex1++)
	{
		TPoint3<Real> &ptPoint1 = p_polyhedron1.m_listVertices(unIndex1);
		for (size_t unIndex2 = 0; unIndex2 < p_polyhedron2.m_listVertices.Size(); unIndex2++)
			listPoints.Add(ptPoint1 - p_polyhedron2.m_listVertices(unIndex2).ToVector());
	}

	MakeConvexHull(listPoints, p_bCleanUp);
}

void Polyhedron::EliminateUnusedVertices(void)
{
	TTreeMap<size_t, size_t> mapVertices;
	TArrayList<TPoint3<Real>> listNewVertices;

	size_t unNextVertexIndex = 0;
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);

		for (size_t unVertexIndex = 0; unVertexIndex < 3; unVertexIndex++)
		{
			size_t unOldVertex = polyhedronFace.VertexIndex[unVertexIndex];
			if (!mapVertices.ContainsKey(unOldVertex))
			{
				mapVertices[unOldVertex] = unNextVertexIndex++;
				listNewVertices.Add(m_listVertices(unOldVertex));
			}
		}
	}

	// clear and re-populate vertex list
	m_listVertices = listNewVertices;

	// map old vertex indices to new
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);

		for (size_t unVertexIndex = 0; unVertexIndex < 3; unVertexIndex++)
			polyhedronFace.VertexIndex[unVertexIndex] = mapVertices(polyhedronFace.VertexIndex[unVertexIndex]);
	}
}

void Polyhedron::AlignCentroid(void)
{
	TPoint3<Real> ptCentroid = GetCentroid();
	Transform(-ptCentroid.ToVector());
}

void Polyhedron::Transform(const TMatrix4<Real>& p_matTransform)
{
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
	{
		TVector3<Real> vecVertex = m_listVertices(unIndex).ToVector();
		TVector4<Real> vecVertexNew = p_matTransform * vecVertex;

		m_listVertices(unIndex).X = vecVertexNew.X;
		m_listVertices(unIndex).Y = vecVertexNew.Y;
		m_listVertices(unIndex).Z = vecVertexNew.Z;
	}

	UpdateNormals();
}

Polyhedron Polyhedron::TransformCopy(const TMatrix4<Real>& p_matTransform) const
{
	Polyhedron polyhedron;
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
	{
		TVector3<Real> vecVertex = m_listVertices(unIndex).ToVector();
		TVector4<Real> vecVertexNew = p_matTransform * vecVertex;

		polyhedron.m_listVertices.Add(
			TPoint3<Real>(vecVertexNew.X, vecVertexNew.Y, vecVertexNew.Z));
	}

	polyhedron.m_listFaces = m_listFaces;
	for (size_t unFaceIndex = 0; unFaceIndex < polyhedron.m_listFaces.Size(); unFaceIndex++)
		polyhedron.m_listFaces(unFaceIndex).VertexList = &polyhedron.m_listVertices;

	polyhedron.UpdateNormals();

	return polyhedron;
}

void Polyhedron::Transform(const Meson::Common::Maths::TVector3<Real>& p_vecOffset)
{
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
		m_listVertices(unIndex) += p_vecOffset;
}

Polyhedron Polyhedron::TransformCopy(const Meson::Common::Maths::TVector3<Real>& p_vecOffset) const
{
	Polyhedron polyhedron;
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
	{
		TVector3<Real> vecVertex = m_listVertices(unIndex).ToVector();
		TVector4<Real> vecVertexNew = vecVertex + p_vecOffset;

		polyhedron.m_listVertices.Add(
			TPoint3<Real>(vecVertexNew.X, vecVertexNew.Y, vecVertexNew.Z));
	}

	polyhedron.m_listFaces = m_listFaces;
	for (size_t unFaceIndex = 0; unFaceIndex < polyhedron.m_listFaces.Size(); unFaceIndex++)
		polyhedron.m_listFaces(unFaceIndex).VertexList = &polyhedron.m_listVertices;

	return polyhedron;
}

void Polyhedron::Transform(const TQuaternion<Real>& p_qtnRotation)
{
	TQuaternion<Real> qtnConjugate = p_qtnRotation.ConjugateCopy();
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
	{
		TQuaternion<Real> qtnVertex = TQuaternion<Real>(m_listVertices(unIndex).ToVector());
		TVector3<Real> vecVertexNew = (p_qtnRotation * qtnVertex * qtnConjugate).Vector;

		m_listVertices(unIndex).X = vecVertexNew.X;
		m_listVertices(unIndex).Y = vecVertexNew.Y;
		m_listVertices(unIndex).Z = vecVertexNew.Z;
	}

	UpdateNormals();
}

Polyhedron Polyhedron::TransformCopy(const TQuaternion<Real>& p_qtnRotation) const
{
	Polyhedron polyhedron;
	TQuaternion<Real> qtnConjugate = p_qtnRotation.ConjugateCopy();
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
	{
		TQuaternion<Real> qtnVertex = TQuaternion<Real>(m_listVertices(unIndex).ToVector());
		TVector3<Real> vecVertexNew = (p_qtnRotation * qtnVertex * qtnConjugate).Vector;

		polyhedron.m_listVertices.Add(
			TPoint3<Real>(vecVertexNew.X, vecVertexNew.Y, vecVertexNew.Z));
	}

	polyhedron.m_listFaces = m_listFaces;
	for (size_t unFaceIndex = 0; unFaceIndex < polyhedron.m_listFaces.Size(); unFaceIndex++)
		polyhedron.m_listFaces(unFaceIndex).VertexList = &polyhedron.m_listVertices;

	polyhedron.UpdateNormals();

	return polyhedron;
}

void Polyhedron::Transform(const Meson::Gravitas::Geometry::Transform& p_transform)
{
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
		m_listVertices(unIndex) = p_transform(m_listVertices(unIndex));

	UpdateNormals();
}

Polyhedron Polyhedron::TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform) const
{
	Polyhedron polyhedron;
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
		polyhedron.m_listVertices.Add(p_transform(m_listVertices(unIndex)));

	polyhedron.m_listFaces = m_listFaces;
	for (size_t unFaceIndex = 0; unFaceIndex < polyhedron.m_listFaces.Size(); unFaceIndex++)
		polyhedron.m_listFaces(unFaceIndex).VertexList = &polyhedron.m_listVertices;

	polyhedron.UpdateNormals();

	return polyhedron;
}

void Polyhedron::ProjectToInterval(
	const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
	Meson::Common::Maths::TInterval<Real>& p_interval) const
{
	p_interval.MakeEmpty();
	for (size_t unVertexIndex = 0; unVertexIndex < m_listVertices.Size(); unVertexIndex++)
		p_interval.Extend(m_listVertices(unVertexIndex).ToVector() * p_vecAxis);
}

bool Polyhedron::Intersects(const TPoint3<Real>& p_ptPoint) const
{
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
		if (m_listFaces(unFaceIndex).IsOutside(p_ptPoint))
			return false;
	return true;
}

bool Polyhedron::Intersects(const LineSegment& p_lineSegment) const
{
	// do quick test with starting point
	if (Intersects(p_lineSegment.Start)) return true;

	// do quick test with ending point
	if (Intersects(p_lineSegment.End)) return true;

	// segment endpoints both outside but may still intersect
	// at boundaries
	return IntersectsBoundary(p_lineSegment);
}

bool Polyhedron::IntersectsBoundary(const LineSegment& p_lineSegment) const
{
	// do line segment test with faces and report first positive test
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
		if (m_listFaces(unFaceIndex).Intersects(p_lineSegment)) return true;

	// no intersection if none of the faces tested positive
	return false;
}

bool Polyhedron::IntersectsBoundary(
	const LineSegment &p_lineSegment,
	size_t& p_unOccurences,
	TPoint3<Real>& p_ptIntersectionPointNear,
	TPoint3<Real>& p_ptIntersectionPointFar) const
{
	// initialise occurences
	p_unOccurences = 0;

	// process all faces
	TPoint3<Real> ptIntersectionPoint;
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		// skip faces that do not intersect with the segment
		if (!m_listFaces(unFaceIndex).Intersects(p_lineSegment, ptIntersectionPoint))
			continue;

		// intersection occured - increment count
		++p_unOccurences;

		if (p_unOccurences == 1)
			// first occurence - assume near point
			p_ptIntersectionPointNear = ptIntersectionPoint;
		else
		{
			// second occurence - check if nearer than first or otherwise
			if ((ptIntersectionPoint - p_lineSegment.Start).LengthSquared()
				< (p_ptIntersectionPointNear - p_lineSegment.Start).LengthSquared())
			{
				// second occurence is closer
				p_ptIntersectionPointFar = p_ptIntersectionPointNear;
				p_ptIntersectionPointNear = ptIntersectionPoint;
			}
			else
				// first occurence is closer
				p_ptIntersectionPointFar = ptIntersectionPoint;

			return true;
		}
	}

	// otherwise, test positive if at least one occurence
	return p_unOccurences > 0;
}

TPoint3<Real> Polyhedron::ClosestPointTo(const TPoint3<Real>& p_ptPoint) const
{
	size_t unClosestFaceIndex = 0;
	// ignore use closest face index variant but ignore index
	return ClosestPointTo(p_ptPoint, unClosestFaceIndex);
}

TPoint3<Real> Polyhedron::ClosestPointTo(const TPoint3<Real>& p_ptPoint,
	size_t& p_unClosestFaceIndex) const
{
	// if point intersects polyhedron, point itself is closest
	if (Intersects(p_ptPoint))
		return p_ptPoint;

	// otherwise, process all faces
	Real rMinDistanceSquared = TMaths<Real>::Maximum,
		rCurrentDistanceSquared = (Real) 0.0;
	TPoint3<Real> ptPointClosest, ptPointCurrent;
	size_t unClosestFaceIndex = 0;
	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		// get current face
		PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);

		// ignore back faces
		if (polyhedronFace.IsInside(p_ptPoint))
			continue;

		// get closest point on face
		ptPointCurrent = polyhedronFace.ClosestPointTo(p_ptPoint);

		// keep track of ovarall closest point 
		rCurrentDistanceSquared = (ptPointCurrent - p_ptPoint).LengthSquared();
		if (rMinDistanceSquared > rCurrentDistanceSquared)
		{
			rMinDistanceSquared = rCurrentDistanceSquared;
			ptPointClosest = ptPointCurrent;
			p_unClosestFaceIndex = unFaceIndex;
		}
	}

	return ptPointClosest;
}

const String& Polyhedron::GetTypeName(void) const
{
	return g_strGeometryTypePolyhedron;
}

short Polyhedron::GetTypeId(void) const
{
	return m_nId;
}

void Polyhedron::SetTypeId(short p_nId)
{
	m_nId = p_nId;
}

Real Polyhedron::GetVolume(void) const
{
	Real rVolumeIntegral = (Real) 0.0;

	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);

		// get face vertices
		TVector3<Real> &vecVertex0 =  m_listVertices(polyhedronFace.VertexIndex[0]);
		TVector3<Real> &vecVertex1 =  m_listVertices(polyhedronFace.VertexIndex[1]);
		TVector3<Real> &vecVertex2 =  m_listVertices(polyhedronFace.VertexIndex[2]);

		// compute cross product from edge vectors (same direction as normal)
		TVector3<Real> vecD = (vecVertex1 - vecVertex0) ^ (vecVertex2 - vecVertex0);

		// update integral
		rVolumeIntegral += vecD.X * (vecVertex0.X + vecVertex1.X + vecVertex2.X);
	}

	// volume
	return rVolumeIntegral / (Real) 6.0;
}

TMatrix3<Real> Polyhedron::GetDistributionTensor(void) const
{
	// initialise tensor integrals
	Real rTensorIntegralXX = (Real) 0.0;
	Real rTensorIntegralYY = (Real) 0.0;
	Real rTensorIntegralZZ = (Real) 0.0;
	Real rTensorIntegralXY = (Real) 0.0;
	Real rTensorIntegralYZ = (Real) 0.0;
	Real rTensorIntegralZX = (Real) 0.0;

	for (size_t unFaceIndex = 0; unFaceIndex < m_listFaces.Size(); unFaceIndex++)
	{
		PolyhedronFace &polyhedronFace = m_listFaces(unFaceIndex);

		// get face vertices
		TVector3<Real> &vecVertex0 =  m_listVertices(polyhedronFace.VertexIndex[0]);
		TVector3<Real> &vecVertex1 =  m_listVertices(polyhedronFace.VertexIndex[1]);
		TVector3<Real> &vecVertex2 =  m_listVertices(polyhedronFace.VertexIndex[2]);

		// compute cross product form edges (same direction as normal)
		TVector3<Real> vecD = (vecVertex1 - vecVertex0) ^ (vecVertex2 - vecVertex0);

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
		rTensorIntegralXX += vecD.X * rF3x;
		rTensorIntegralYY += vecD.Y * rF3y;
		rTensorIntegralZZ += vecD.Z * rF3z;
		rTensorIntegralXY += vecD.X * (vecVertex0.Y * rG0x + vecVertex1.Y * rG1x + vecVertex2.Y * rG2x);
		rTensorIntegralYZ += vecD.Y * (vecVertex0.Z * rG0y + vecVertex1.Z * rG1y + vecVertex2.Z * rG2y);
		rTensorIntegralZX += vecD.Z * (vecVertex0.X * rG0z + vecVertex1.X * rG1z + vecVertex2.X * rG2z);
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

bool Polyhedron::IsBounded(void) const
{
	return true;
}

void Polyhedron::ComputeBoundingVolume(BoundingSphere& p_boundingSphere) const
{
	if (m_listVertices.Size() == 0)
	{
		p_boundingSphere.Centre = TPoint3<Real>::Origin;
		p_boundingSphere.Radius = TMaths<Real>::Epsilon;
		p_boundingSphere.RadiusSquared = TMaths<Real>::Epsilon;
		return;
	}

	// compute vertex median
	TPoint3<Real> ptMin, ptMax, ptMedian;
	ptMin = ptMax = m_listVertices(0);
	for (size_t unIndex = 1; unIndex < m_listVertices.Size(); unIndex++)
	{
		TPoint3<Real> &ptVertex = m_listVertices(unIndex);
		ptMin.X = TMaths<Real>::Min(ptMin.X, ptVertex.X); 
		ptMin.Y = TMaths<Real>::Min(ptMin.Y, ptVertex.Y); 
		ptMin.Z = TMaths<Real>::Min(ptMin.Z, ptVertex.Z); 
		ptMax.X = TMaths<Real>::Max(ptMax.X, ptVertex.X); 
		ptMax.Y = TMaths<Real>::Max(ptMax.Y, ptVertex.Y); 
		ptMax.Z = TMaths<Real>::Max(ptMax.Z, ptVertex.Z); 
	}
	ptMedian = ptMin + (ptMax - ptMin) * (Real) 0.5;

	// compute greatest radius
	Real rRadiusSquaredMax = 0;
	for (size_t unIndex = 0; unIndex < m_listVertices.Size(); unIndex++)
	{
		Real rRadiusSquaredCurrent
			= (m_listVertices(unIndex) - ptMedian).LengthSquared();
		if (rRadiusSquaredMax < rRadiusSquaredCurrent)
			rRadiusSquaredMax = rRadiusSquaredCurrent;
	}

	p_boundingSphere.Centre = ptMedian;
	p_boundingSphere.RadiusSquared = rRadiusSquaredMax;
	p_boundingSphere.Radius = TMaths<Real>::Sqrt(rRadiusSquaredMax);
}

void Polyhedron::ComputeBoundingVolume(BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const
{
	// handle empty case
	if (m_listVertices.Size() == 0)
	{
		p_boundingAxisAlignedBox.Min = p_boundingAxisAlignedBox.Max
			= TPoint3<Real>::Origin;
		return;
	}

	// initialise bounds to first vertex
	p_boundingAxisAlignedBox.Min = p_boundingAxisAlignedBox.Max
		= m_listVertices(0);

	// extend box to other vertices
	for (size_t unIndex = 1; unIndex < m_listVertices.Size(); unIndex++)
		p_boundingAxisAlignedBox.ExtendToPoint(m_listVertices(unIndex));
}

void Polyhedron::ComputeBoundingVolume(BoundingOrientedBox& p_boundingOrientedBox) const
{
	// note: uses brute-force approach to fit an approximately good OBB

	Real rMinVolume = TMaths<Real>::Maximum;

	// process all faces to find first axis
	for (size_t unIndex1 = 0; unIndex1 < m_listFaces.Size(); unIndex1++)
	{
		// align first axis with a face normal
		TInterval<Real> interval1;
		TVector3<Real> vecAxis1 = m_listFaces(unIndex1).Normal;

		// compute interval corresponding to first axis
		ProjectToInterval(vecAxis1, interval1);

		// process all faces again to find second axis
		for (size_t unIndex2 = 0; unIndex2 < m_listFaces.Size(); unIndex2++)
		{
			// skip face used for first axis
			if (unIndex2 == unIndex1) continue;

			// start by using second face normal as second axis
			TVector3<Real> vecAxis2 = m_listFaces(unIndex2).Normal;

			// remove second axis's component in directon of first axis
			// and normalise to make perpendicular
			vecAxis2 -= (vecAxis2 * vecAxis1) * vecAxis1;
			vecAxis2.Normalise();	

			// skip if result is zero (i.e. 2nd axis originally parallel to 1st)
			if (TMaths<Real>::Equals(vecAxis2.LengthSquared(), (Real) 0.0))
				continue;

			// compute third axis by cross product
			TVector3<Real> vecAxis3 = vecAxis1 ^ vecAxis2;
		
			// compute intervals corresponding to second and third axes
			TInterval<Real> interval2, interval3;
			ProjectToInterval(vecAxis1, interval1);

			// compute current volume
			Real rSpan1 = interval1.Span();
			Real rSpan2 = interval2.Span();
			Real rSpan3 = interval3.Span();
			Real rCurrentVolume = interval1.Span() * interval2.Span() * interval3.Span();

			// skip these axes if current min volume is smaller
			if (rMinVolume < rCurrentVolume) continue;

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

bool Polyhedron::IntersectsRay(const Ray& p_ray) const
{
	return false;
}

bool Polyhedron::IntersectsRay(const Ray& p_ray, TPoint3<Real>& p_ptIntersectionPoint) const
{
	return false;
}

Meson_Gravitas_Geometry_END
