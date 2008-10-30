#include "PolyhedronFace.h"

#include "Maths.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;

Meson_Gravitas_Geometry_BEGIN

	PolyhedronFace::PolyhedronFace(void)
		: VertexList(NULL)
		, Normal()
	{
		VertexIndex[0] = 0;
		VertexIndex[1] = 0;
		VertexIndex[2] = 0;
		FaceIndex[0] = 0;
		FaceIndex[1] = 0;
		FaceIndex[2] = 0;
	}

	PolyhedronFace::PolyhedronFace(const PolyhedronFace& p_polyhedronFace)
		: VertexList(p_polyhedronFace.VertexList)
		, Normal(p_polyhedronFace.Normal)
	{
		VertexIndex[0] = p_polyhedronFace.VertexIndex[0];
		VertexIndex[1] = p_polyhedronFace.VertexIndex[1];
		VertexIndex[2] = p_polyhedronFace.VertexIndex[2];
		FaceIndex[0] = p_polyhedronFace.FaceIndex[0];
		FaceIndex[1] = p_polyhedronFace.FaceIndex[1];
		FaceIndex[2] = p_polyhedronFace.FaceIndex[2];
	}

	PolyhedronFace::PolyhedronFace(
		const VectorList* p_listVertices,
		size_t p_unVertexIndex0, size_t p_unVertexIndex1, size_t p_unVertexIndex2)
		: VertexList((VectorList*) p_listVertices)
		, Normal()
	{
		VertexIndex[0] = p_unVertexIndex0;
		VertexIndex[1] = p_unVertexIndex1;
		VertexIndex[2] = p_unVertexIndex2;
		FaceIndex[0] = FaceIndex[1] = FaceIndex[2] = 0;
		UpdateNormal();
	}

	PolyhedronFace::PolyhedronFace(
		const VectorList* p_listVertices,
		const size_t* p_punIndices)
		: VertexList((VectorList *) p_listVertices)
		, Normal()
	{
		VertexIndex[0] = *p_punIndices;
		VertexIndex[1] = *++p_punIndices;
		VertexIndex[2] = *++p_punIndices;
		FaceIndex[0] = FaceIndex[1] = FaceIndex[2] = 0;
		UpdateNormal();
	}

	void PolyhedronFace::UpdateNormal(void)
	{
		TVector3<Real>& vecVertex0 = (*VertexList)[VertexIndex[0]];
		TVector3<Real>& vecVertex1 = (*VertexList)[VertexIndex[1]];
		TVector3<Real>& vecVertex2 = (*VertexList)[VertexIndex[2]];
		Normal = (vecVertex1 - vecVertex0) ^ (vecVertex2 - vecVertex0);
		Normal.Normalise();
	}

	void PolyhedronFace::Invert(void)
	{
		size_t unTemp = VertexIndex[0];
		VertexIndex[0] = VertexIndex[1];
		VertexIndex[1] = unTemp;
		Normal = -Normal;
	}

	PolyhedronFace &PolyhedronFace::operator=(const PolyhedronFace& p_polyhedronFace)
	{
		VertexList = p_polyhedronFace.VertexList;
		VertexIndex[0] = p_polyhedronFace.VertexIndex[0];
		VertexIndex[1] = p_polyhedronFace.VertexIndex[1];
		VertexIndex[2] = p_polyhedronFace.VertexIndex[2];
		FaceIndex[0] = p_polyhedronFace.FaceIndex[0];
		FaceIndex[1] = p_polyhedronFace.FaceIndex[1];
		FaceIndex[2] = p_polyhedronFace.FaceIndex[2];
		Normal = p_polyhedronFace.Normal;
		return *this;
	}

	bool PolyhedronFace::operator==(const PolyhedronFace& p_polyhedronFace) const
	{
		if (VertexList != p_polyhedronFace.VertexList) return false;
		if (VertexIndex[0] != p_polyhedronFace.VertexIndex[0]) return false;
		if (VertexIndex[1] != p_polyhedronFace.VertexIndex[1]) return false;
		if (VertexIndex[2] != p_polyhedronFace.VertexIndex[2]) return false;
		return true;
	}

	bool PolyhedronFace::operator<(const PolyhedronFace& p_polyhedronFace) const
	{
		if (VertexIndex[0] > p_polyhedronFace.VertexIndex[0]) return false;
		if (VertexIndex[0] < p_polyhedronFace.VertexIndex[0]) return true;
		if (VertexIndex[1] > p_polyhedronFace.VertexIndex[1]) return false;
		if (VertexIndex[1] < p_polyhedronFace.VertexIndex[1]) return true;
		return VertexIndex[2] < p_polyhedronFace.VertexIndex[2];
	}

	bool PolyhedronFace::HasVertexIndex(size_t p_unIndex) const
	{
		return (VertexIndex[0] == p_unIndex)
			|| (VertexIndex[1] == p_unIndex)
			|| (VertexIndex[2] == p_unIndex);
	}

	Real PolyhedronFace::SignedDistanceFromPlane(const TVector3<Real>& p_vecPoint) const
	{
		TVector3<Real>& vecVertex0 = (*VertexList)[VertexIndex[0]];
		return (Real) ((p_vecPoint - vecVertex0) * Normal);
	}

	bool PolyhedronFace::IsOutside(const TVector3<Real>& p_vecPoint) const
	{
		return SignedDistanceFromPlane(p_vecPoint) >= TMaths<Real>::Epsilon;
	}

	bool PolyhedronFace::IsInside(const TVector3<Real>& p_vecPoint) const
	{
		return SignedDistanceFromPlane(p_vecPoint) <= -TMaths<Real>::Epsilon;
	}

	bool PolyhedronFace::IsInPlane(const TVector3<Real>& p_vecPoint) const
	{
		return TMaths<Real>::Equals(SignedDistanceFromPlane(p_vecPoint),	(Real) 0.0 );
	}

	// does quick point in face test, ignoring distance from tangent plane
	bool PolyhedronFace::IntersectsQuick(const TVector3<Real>& p_vecPoint) const
	{
		// get face vertices and align to point as origin
		TVector3<Real> vecVertex0 = (*VertexList)(VertexIndex[0]) - p_vecPoint;
		TVector3<Real> vecVertex1 = (*VertexList)(VertexIndex[1]) - p_vecPoint;
		TVector3<Real> vecVertex2 = (*VertexList)(VertexIndex[2]) - p_vecPoint;
		
		// compute dot product terms (Lagrange identity) to avoid cross products
		Real rDot01 = vecVertex0 * vecVertex1;
		Real rDot02 = vecVertex0 * vecVertex2;
		Real rDot12 = vecVertex1 * vecVertex2;
		Real rDot22 = vecVertex2.LengthSquared();

		// if normals for 1st and 2nd triangles formed with point are in opposite
		// directions, then point must be outside face
		if (rDot12 * rDot02 - rDot22 * rDot01 < 0.0f) return false;

		// compute additional dot product term for test with 3rd triangle
		Real rDot11 = vecVertex1.LengthSquared();

		// if normal for 3rd triangle is in opposite direction, then
		// point must be outside face
		return rDot01 * rDot12 - rDot02 * rDot11 >= 0.0f;
	}

	bool PolyhedronFace::Intersects(const TVector3<Real>& p_vecPoint) const
	{
		// get distance from tangent plane
		Real rDistance = SignedDistanceFromPlane(p_vecPoint);

		// if not on plane, no intersection
		if (!TMaths<Real>::Equals(rDistance, (Real) 0.0))
			return false;

		// otherwise, do quick point test
		return IntersectsQuick(p_vecPoint);
	}

	bool PolyhedronFace::Intersects(const LineSegment& p_lineSegment) const
	{
		// get distance of starting point from tangent plane
		Real rDistanceStart = SignedDistanceFromPlane(p_lineSegment.Start);

		// if distance of starting point is zero, test starting point only
		if (Meson::Common::Maths::TMaths<Real>::Equals(rDistanceStart, (Real) 0.0)
			&& IntersectsQuick(p_lineSegment.Start))
			return true;

		// get distance of ending point from tangent plane
		Real rDistanceEnd = SignedDistanceFromPlane(p_lineSegment.End);

		// if distance of ending point is zero, test ending point only
		if (Meson::Common::Maths::TMaths<Real>::Equals(rDistanceEnd, (Real) 0.0)
			&& IntersectsQuick(p_lineSegment.End))
			return true;

		// if distances both positive or negative, segment does not
		// even intersect tangent plane
		if (rDistanceStart * rDistanceEnd > (Real) 0.0) return false;

		// otherwise, segment intersects plane, so find point on plane
		TVector3<Real> vecPointOnPlane
			= p_lineSegment.Start - Normal * rDistanceStart;

		// do quick test using point on plane
		return IntersectsQuick(vecPointOnPlane);
	}

	bool PolyhedronFace::Intersects(const LineSegment& p_lineSegment,
		TVector3<Real>& p_vecIntersectionPoint) const
	{
		// get distance of starting point from tangent plane
		Real rDistanceStart = SignedDistanceFromPlane(p_lineSegment.Start);

		// if distance of starting point is zero, test starting point only
		if (Meson::Common::Maths::TMaths<Real>::Equals(rDistanceStart, (Real) 0.0)
			&& IntersectsQuick(p_lineSegment.Start))
		{
			p_vecIntersectionPoint = p_lineSegment.Start;
			return true;
		}

		// get distance of ending point from tangent plane
		Real rDistanceEnd = SignedDistanceFromPlane(p_lineSegment.End);

		// if distance of ending point is zero, test ending point only
		if (Meson::Common::Maths::TMaths<Real>::Equals(rDistanceEnd, (Real) 0.0)
			&& IntersectsQuick(p_lineSegment.End))
		{
			p_vecIntersectionPoint = p_lineSegment.End;
			return true;
		}

		// if distances both positive or negative, segment does not
		// even intersect tangent plane
		if (rDistanceStart * rDistanceEnd > (Real) 0.0) return false;

		// otherwise, segment intersects plane, so find point on plane
		TVector3<Real> vecPointOnPlane
			= p_lineSegment.Start - Normal * rDistanceStart;

		// do quick test using point on plane
		if (!IntersectsQuick(vecPointOnPlane)) return false;

		// if point in plane test ok, intersection is projected point
		p_vecIntersectionPoint = vecPointOnPlane;
		return true;
	}

	TVector3<Real> PolyhedronFace::ClosestPointTo(const TVector3<Real>& p_vecPoint) const
	{
		// check if projected point in voronoi region of first vertex

		// get face vertices and align to point as origin
		TVector3<Real>& vecVertex0 = (*VertexList)(VertexIndex[0]);
		TVector3<Real>& vecVertex1 = (*VertexList)(VertexIndex[1]);
		TVector3<Real>& vecVertex2 = (*VertexList)(VertexIndex[2]);

		// compute edge vectors and offset from first vertex
		TVector3<Real> vecEdge01 = vecVertex1 - vecVertex0;
		TVector3<Real> vecEdge02 = vecVertex2 - vecVertex0;
		TVector3<Real> vecOffsetToVertex0 = p_vecPoint - vecVertex0;

		Real rDot1 = vecEdge01 * vecOffsetToVertex0;
		Real rDot2 = vecEdge02 * vecOffsetToVertex0;

		// closest point is first vertex
		if (rDot1 <= (Real) 0.0 && rDot2 <= (Real) 0.0)
			return vecVertex0;

		// check if projected point in voronoi region of second vertex
		
		TVector3<Real> vecOffsetToVertex1 = p_vecPoint - vecVertex1;
		Real rDot3 = vecEdge01 * vecOffsetToVertex1;
		Real rDot4 = vecEdge02 * vecOffsetToVertex1;

		// closest point is second vertex
		if (rDot3 >= (Real) 0.0 && rDot4 <= rDot3)
			return vecVertex1;

		// check if projected point in voronoi region of first edge

		Real rBary2 = rDot1 * rDot4 - rDot3 * rDot2;

		// check if on bary co-ords (1-v, v, 0)
		if (rBary2 <= (Real) 0.0 && rDot1 >= (Real) 0.0 && rDot3 <= (Real) 0.0)
			// closest point on first edge
			return vecVertex0 + vecEdge01 * (rDot1 / (rDot1 - rDot3));

		// check if projected point in voronoi region of third vertex

		TVector3<Real> vecOffsetToVertex2 = p_vecPoint - vecVertex2;
		Real rDot5 = vecEdge01 * vecOffsetToVertex2;
		Real rDot6 = vecEdge02 * vecOffsetToVertex2;

		// closest point is third vertex
		if (rDot6 >= (Real) 0.0 && rDot5 <= rDot6)
			return vecVertex2;

		// check if projected point in voronoi region of second edge

		Real rBary1 = rDot5 * rDot2 - rDot1 * rDot6;

		// check if on bary co-ords (1-w, 0, w)
		if (rBary1 <= (Real) 0.0 && rDot2 >= (Real) 0.0 && rDot6 <= (Real) 0.0)
			// closest point on first edge
			return vecVertex0 + vecEdge02 * (rDot2 / (rDot2 - rDot6));

		// check if projected point in voronoi region of third edge

		Real rBary0 = rDot3 * rDot6 - rDot5 * rDot4;

		// check if on bary co-ords (0, 1-w, w)
		Real rDot43 = rDot4 - rDot3;
		Real rDot56 = rDot5 - rDot6;
		if (rBary0 <= (Real) 0.0 && rDot43 >= (Real) 0.0 && rDot56 >= (Real) 0.0)
			// closest point on third edge
			return vecVertex1 + (vecVertex2 - vecVertex1) * (rDot43 / (rDot43 + rDot56));

		// otherwise, point is inside face region

		Real rDenominator = (Real) 1.0 / (rBary0 + rBary1 + rBary2);
		Real rBaryV = rBary1 * rDenominator;
		Real rBaryW = rBary2 * rDenominator;

		// compute point from barycentric co-ords
		return vecVertex0 + vecEdge01 * rBaryV + vecEdge02 * rBaryW;
	}

Meson_Gravitas_Geometry_END
