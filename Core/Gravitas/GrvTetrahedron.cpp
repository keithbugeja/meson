#include "GrvTetrahedron.h"

#include "GrvTriangle.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

Tetrahedron::Tetrahedron(void)
{
}

Tetrahedron::Tetrahedron(const Tetrahedron& p_tetrahedron)
{
	Vertices[0] = p_tetrahedron.Vertices[0];
	Vertices[1] = p_tetrahedron.Vertices[1];
	Vertices[2] = p_tetrahedron.Vertices[2];
	Vertices[3] = p_tetrahedron.Vertices[3];
}

Tetrahedron::Tetrahedron(
	const TPoint3<Real>& p_ptVertex0,
	const TPoint3<Real>& p_ptVertex1,
	const TPoint3<Real>& p_ptVertex2,
	const TPoint3<Real>& p_ptVertex3)
{
	Vertices[0] = p_ptVertex0;
	Vertices[1] = p_ptVertex1;
	Vertices[2] = p_ptVertex2;
	Vertices[3] = p_ptVertex3;
}

Real Tetrahedron::Volume(void) const
{
	TVector3<Real> vecOffset01(Vertices[1] - Vertices[0]);
	TVector3<Real> vecOffset02(Vertices[2] - Vertices[0]);
	TVector3<Real> vecOffset03(Vertices[3] - Vertices[0]);
	return TMaths<Real>::Abs(vecOffset01 * (vecOffset02 ^ vecOffset03))
		* ((Real) 1.0 / (Real) 6.0);
}

TPoint3<Real> Tetrahedron::Centroid(void) const
{
	TPoint3<Real> ptCentroid(Vertices[0]);
	ptCentroid += Vertices[1].ToVector();
	ptCentroid += Vertices[2].ToVector();
	ptCentroid += Vertices[3].ToVector();
	ptCentroid.ToVector() *= (Real) 0.25;
	return ptCentroid;
}

Real Tetrahedron::DistanceFromPoint(const TPoint3<Real>& p_ptPoint) const
{
	return (p_ptPoint - ClosestPoint(p_ptPoint)).Length();
}

Real Tetrahedron::DistanceFromPoint(
	const TPoint3<Real>& p_ptPoint,
	TPoint3<Real>& p_ptClosestPoint) const
{
	p_ptClosestPoint = ClosestPoint(p_ptPoint);
	return (p_ptPoint - p_ptClosestPoint).Length();
}

TPoint3<Real> Tetrahedron::ClosestPoint(const TPoint3<Real>& p_ptPoint) const
{
	TPoint3<Real> ptCentroid(Centroid());

	// generate side 0 and make sure it points outwards
	Triangle triangle0(Vertices[1], Vertices[2], Vertices[3]);
	if (triangle0.PointAbovePlane(ptCentroid))
		triangle0.Invert();

	// if point above side 0 it is in voronoi of its vertices, edges or face
	if (triangle0.PointAbovePlane(p_ptPoint))
		return triangle0.ClosestPoint(p_ptPoint);

	// generate side 1 and make sure it points outwards
	Triangle triangle1(Vertices[0], Vertices[2], Vertices[3]);
	if (triangle1.PointAbovePlane(ptCentroid))
		triangle1.Invert();

	// if point above side 1 it is in voronoi of its vertices, edges or face
	if (triangle1.PointAbovePlane(p_ptPoint))
		return triangle1.ClosestPoint(p_ptPoint);

	// generate side 2 and make sure it points outwards
	Triangle triangle2(Vertices[0], Vertices[1], Vertices[3]);
	if (!triangle2.PointAbovePlane(ptCentroid))
		triangle2.Invert();

	// if point above side 2 it is in voronoi of its vertices, edges or face
	if (triangle2.PointAbovePlane(p_ptPoint))
		return triangle2.ClosestPoint(p_ptPoint);

	// generate side 3 and make sure it points outwards
	Triangle triangle3(Vertices[0], Vertices[1], Vertices[2]);
	if (triangle3.PointAbovePlane(ptCentroid))
		triangle3.Invert();

	// if point above side 3 it is in voronoi of its vertices, edges or face
	if (triangle3.PointAbovePlane(p_ptPoint))
		return triangle3.ClosestPoint(p_ptPoint);

	// otherwise, it is inside
	return p_ptPoint;
}

bool Tetrahedron::Contains(const TPoint3<Real>& p_ptPoint) const
{
	return ClosestPoint(p_ptPoint) == p_ptPoint;
}

void Tetrahedron::Transform(const Meson::Gravitas::Geometry::Transform& p_transform)
{
	p_transform.Apply(Vertices[0]);
	p_transform.Apply(Vertices[1]);
	p_transform.Apply(Vertices[2]);
	p_transform.Apply(Vertices[3]);
}

Tetrahedron Tetrahedron::TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform) const
{
	return Tetrahedron(
		p_transform.ApplyCopy(Vertices[0]),
		p_transform.ApplyCopy(Vertices[1]),
		p_transform.ApplyCopy(Vertices[2]),
		p_transform.ApplyCopy(Vertices[3]));
}

Tetrahedron& Tetrahedron::operator=(const Tetrahedron& p_tetrahedron)
{
	Vertices[0] = p_tetrahedron.Vertices[0];
	Vertices[1] = p_tetrahedron.Vertices[1];
	Vertices[2] = p_tetrahedron.Vertices[2];
	Vertices[3] = p_tetrahedron.Vertices[3];
	return *this;
}

bool Tetrahedron::operator==(const Tetrahedron& p_tetrahedron) const
{
	return Vertices[0] == p_tetrahedron.Vertices[0]
		&& Vertices[1] == p_tetrahedron.Vertices[1]
		&& Vertices[2] == p_tetrahedron.Vertices[2]
		&& Vertices[3] == p_tetrahedron.Vertices[3];
}

