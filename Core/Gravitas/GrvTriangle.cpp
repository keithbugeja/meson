#include "GrvTriangle.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

Triangle::Triangle(void)
{
}

Triangle::Triangle(const Triangle& p_triangle)
{
	Vertices[0] = p_triangle.Vertices[0];
	Vertices[1] = p_triangle.Vertices[1];
	Vertices[2] = p_triangle.Vertices[2];
}

Triangle::Triangle(
	const TVector3<Real>& p_vecVertex0,
	const TVector3<Real>& p_vecVertex1,
	const TVector3<Real>& p_vecVertex2)
{
	Vertices[0] = p_vecVertex0;
	Vertices[1] = p_vecVertex1;
	Vertices[2] = p_vecVertex2;
}

Real Triangle::Area(void) const
{
	return ((Vertices[1] - Vertices[0]) ^ (Vertices[2] - Vertices[0])).Length() * (Real) 0.5;
}

TVector3<Real> Triangle::Normal(void) const
{
	return ((Vertices[2] - Vertices[0]) ^ (Vertices[1] - Vertices[0])).NormaliseCopy();
}

TVector3<Real> Triangle::Centroid(void) const
{
	return (Vertices[0] + Vertices[1] + Vertices[2]) / (Real) 3.0;
}

TVector3<Real> Triangle::Edge(size_t p_unIndex) const
{
	return Vertices[(p_unIndex + 1) % 3] - Vertices[p_unIndex % 3];
}

Real Triangle::DistanceFromPoint(const TVector3<Real>& p_vecPoint) const
{
	return (p_vecPoint - ClosestPoint(p_vecPoint)).Length();
}

Real Triangle::DistanceFromPoint(
	const TVector3<Real>& p_vecPoint,
	TVector3<Real>& p_vecClosestPoint) const
{
	p_vecClosestPoint = ClosestPoint(p_vecPoint);
	return (p_vecPoint - p_vecClosestPoint).Length();
}

Real Triangle::SignedDistanceFromPlane(const TVector3<Real>& p_vecPoint) const
{
	return (p_vecPoint - Vertices[0]) * Normal();
}

TVector3<Real> Triangle::ClosestPoint(const TVector3<Real>& p_vecPoint) const
{
	const TVector3<Real>& p = p_vecPoint;
	const TVector3<Real>& a = Vertices[0];
	const TVector3<Real>& b = Vertices[1];
	const TVector3<Real>& c = Vertices[2];

	// test if point is vertex 0 or its voronoi region
	TVector3<Real> ab(b - a), ac(c - a), ap(p - a);
	Real d1(ab * ap), d2(ac * ap);
	if (d1 <= (Real) 0.0 && d2 <= (Real) 0.0)
		return a;

	// test if point is vertex 1 or its voronoi region
	TVector3<Real> bp(p - b);
	Real d3(ab * bp), d4(ac * bp);
	if (d3 >= (Real) 0.0 && d4 <= d3)
		return b;

	// test if point is edge 01 or its voronoi region
	Real vc(d1 * d4 - d3 * d2);
	if (vc <= (Real) 0.0 && d1 >= (Real) 0.0 && d3 <= (Real) 0.0)
	{
		Real v(d1 / (d1 - d3));
		return a + v * ab;
	}

	// test if point is vertex 1 or its voronoi region
	TVector3<Real> cp(p - c);
	Real d5(ab * cp), d6(ac * cp);
	if (d6 >= (Real) 0.0 && d5 <= d6)
		return c;

	// test if point is edge 02 or its voronoi region
	Real vb(d5 * d2 - d1 * d6);
	if (vb <= (Real) 0.0 && d2 >= (Real) 0.0 && d6 <= (Real) 0.0)
	{
		Real w(d2 / (d2 - d6));
		return a + w * ac;
	}

	// test if point is edge 12 or its voronoi region
	Real va(d3 * d6 - d5 * d4), d34(d4 - d3), d56(d6 - d5);
	if (va <= (Real) 0.0 && d34 >= (Real) 0.0 && d56 <= (Real) 0.0)
	{
		Real w(d34 / (d34 - d56));
		return b + w * (b - c);
	}

	// point inside
	Real denom((Real) 1.0 / (va+vb+vc)), v(vb * denom), w(vc * denom);
	return a + ab * v + ac * w;
}

bool Triangle::Contains(const TVector3<Real>& p_vecPoint) const
{
	return ClosestPoint(p_vecPoint) == p_vecPoint;
}

bool Triangle::Intersects(const Ray& p_ray) const
{
	static TVector3<Real> s_vecIntersectionPoint;
	return Intersects(p_ray, s_vecIntersectionPoint);
}

bool Triangle::Intersects(const Ray& p_ray,
	const TVector3<Real>& p_vecIntersectionPoint) const
{
	// TODO!!
	return false;
}

bool Triangle::Intersects(const Triangle& p_triangle) const
{
	TInterval<Real> itvProjection1, itvProjection2;

	TVector3<Real> vecAxis(Normal());
	ProjectToInterval(vecAxis, itvProjection1);
	p_triangle.ProjectToInterval(vecAxis, itvProjection2);

	if (!itvProjection1.Intersects(itvProjection2))
		return false;

	vecAxis = p_triangle.Normal();
	ProjectToInterval(vecAxis, itvProjection1);
	p_triangle.ProjectToInterval(vecAxis, itvProjection2);

	if (!itvProjection1.Intersects(itvProjection2))
		return false;

	for (size_t unIndex1 = 0; unIndex1 < 3; unIndex1++)
	{
		TVector3<Real> vecEdge1(Edge(unIndex1));
		for (size_t unIndex2 = 0; unIndex2 < 3; unIndex2++)
		{
			TVector3<Real> vecEdge2(p_triangle.Edge(unIndex2));
			vecAxis = vecEdge1 ^ vecEdge2;
			if (vecAxis.IsZero())
				continue;

			ProjectToInterval(vecAxis, itvProjection1);
			p_triangle.ProjectToInterval(vecAxis, itvProjection2);
			if (!itvProjection1.Intersects(itvProjection2))
				return false;
		}
	}

	return true;
}

bool Triangle::PointOnPlane(const TVector3<Real>& p_vecPoint) const
{
	return TMaths<Real>::Equals((p_vecPoint - Vertices[0]) * Normal(), (Real) 0.0);
}

bool Triangle::PointAbovePlane(const TVector3<Real>& p_vecPoint) const
{
	return (p_vecPoint - Vertices[0]) * Normal() > TMaths<Real>::Epsilon;
}

bool Triangle::PointBelowPlane(const TVector3<Real>& p_vecPoint) const
{
	return (p_vecPoint - Vertices[0]) * Normal() < -TMaths<Real>::Epsilon;
}

bool Triangle::ContainsPointProjection(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const
{
	// edge vectors and point offset
	TVector3<Real> vecEdge02 = Vertices[2] - Vertices[0];
	TVector3<Real> vecEdge01 = Vertices[1] - Vertices[0];
	TVector3<Real> vecPointOffset = p_vecPoint - Vertices[0];

	// dot products
	Real rE02Sqr = vecEdge02.LengthSquared();
	Real rE02E01 = vecEdge02 * vecEdge01;
	Real rE02Point = vecEdge02 * vecPointOffset;
	Real rE01Sqr = vecEdge01.LengthSquared();
	Real rE01Point = vecEdge01 * vecPointOffset;

	// compute barycentric coordinates in terms of edge vectors
	Real rDenominatorInverse = (Real) 1.0 / (rE02Sqr * rE01Sqr - rE02E01 * rE02E01);

	// compute first bary coordinate and ensure non-negative
	Real rBaryU = (rE01Sqr * rE02Point - rE02E01 * rE01Point) * rDenominatorInverse;
	if (rBaryU < (Real) 0.0)
		return false;

	// compute second bary coordinate and ensure non-negative
	Real rBaryV = (rE02Sqr * rE01Point - rE02E01 * rE02Point) * rDenominatorInverse;
	if (rBaryU < (Real) 0.0)
		return false;

	// check coords do not exceed opposite edge
	return rBaryU + rBaryV <= (Real) 1.0;
}

void Triangle::ProjectToInterval(TVector3<Real> p_vecAxis, TInterval<Real>& p_interval) const
{
	p_interval.MakeEmpty();
	p_interval.Extend(Vertices[0] * p_vecAxis);
	p_interval.Extend(Vertices[1] * p_vecAxis);
	p_interval.Extend(Vertices[2] * p_vecAxis);
}

void Triangle::EnumerateMaximalVertices(
	const TVector3<Real>& p_vecDirection,
	VectorList& p_listVertices) const
{
	p_listVertices.Clear();

	Real rVertexProjection0 = Vertices[0] * p_vecDirection;
	Real rVertexProjection1 = Vertices[1] * p_vecDirection;
	Real rVertexProjection2 = Vertices[2] * p_vecDirection;

	if (rVertexProjection1 > rVertexProjection0)
	{
		if (rVertexProjection2 > rVertexProjection1)
			// v2 > v1, v2 > v0
			p_listVertices.Add(Vertices[2]);
		else if (rVertexProjection2 < rVertexProjection1)
			// v1 > v2, v1 > v0
			p_listVertices.Add(Vertices[1]);
		else
		{
			// v2 = v1 > v0
			p_listVertices.Add(Vertices[1]);
			p_listVertices.Add(Vertices[2]);
		}
	}
	else if (rVertexProjection1 < rVertexProjection0)
	{
		if (rVertexProjection2 > rVertexProjection0)
			// v2 > v1, v2 > v0
			p_listVertices.Add(Vertices[2]);
		else if (rVertexProjection2 < rVertexProjection0)
			// v0 > v1, v0 > v2
			p_listVertices.Add(Vertices[0]);
		else
		{
			// v0 = v2 > v1
			p_listVertices.Add(Vertices[0]);
			p_listVertices.Add(Vertices[2]);
		}
	}
	else // proj 0 == proj 1
	{
		if (rVertexProjection2 > rVertexProjection0)
			// v2 > v1, v2 > v0
			p_listVertices.Add(Vertices[2]);
		else if (rVertexProjection2 < rVertexProjection0)
		{
			// v0 = v1 > v2
			p_listVertices.Add(Vertices[0]);
			p_listVertices.Add(Vertices[1]);
		}
		else
		{
			// v0 = v1 = v2
			p_listVertices.Add(Vertices[0]);
			p_listVertices.Add(Vertices[1]);
			p_listVertices.Add(Vertices[2]);
		}
	}
}

void Triangle::Invert(void)
{
	TVector3<Real> vecTemp(Vertices[1]);
	Vertices[1] = Vertices[2];
	Vertices[2] = vecTemp;
}

void Triangle::Transform(const Meson::Gravitas::Geometry::Transform& p_transform)
{
	p_transform.Apply(Vertices[0]);
	p_transform.Apply(Vertices[1]);
	p_transform.Apply(Vertices[2]);
}

Triangle Triangle::TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform) const
{
	return Triangle(
		p_transform.ApplyCopy(Vertices[0]),
		p_transform.ApplyCopy(Vertices[1]),
		p_transform.ApplyCopy(Vertices[2]));
}

Triangle& Triangle::operator=(const Triangle& p_triangle)
{
	Vertices[0] = p_triangle.Vertices[0];
	Vertices[1] = p_triangle.Vertices[1];
	Vertices[2] = p_triangle.Vertices[2];
	return *this;
}

bool Triangle::operator==(const Triangle& p_triangle) const
{
	return Vertices[0] == p_triangle.Vertices[0]
		&& Vertices[1] == p_triangle.Vertices[1]
		&& Vertices[2] == p_triangle.Vertices[2];
}

