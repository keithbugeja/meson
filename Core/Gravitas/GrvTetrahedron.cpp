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
	const TVector3<Real>& p_vecVertex0,
	const TVector3<Real>& p_vecVertex1,
	const TVector3<Real>& p_vecVertex2,
	const TVector3<Real>& p_vecVertex3)
{
	Vertices[0] = p_vecVertex0;
	Vertices[1] = p_vecVertex1;
	Vertices[2] = p_vecVertex2;
	Vertices[3] = p_vecVertex3;
}

Real Tetrahedron::Volume(void) const
{
	TVector3<Real> vecOffset01(Vertices[1] - Vertices[0]);
	TVector3<Real> vecOffset02(Vertices[2] - Vertices[0]);
	TVector3<Real> vecOffset03(Vertices[3] - Vertices[0]);
	return TMaths<Real>::Abs(vecOffset01 * (vecOffset02 ^ vecOffset03))
		* ((Real) 1.0 / (Real) 6.0);
}

TVector3<Real> Tetrahedron::Centroid(void) const
{
	return (Vertices[0] + Vertices[1] + Vertices[2] + Vertices[3]) * (Real) 0.25;
}

Real Tetrahedron::DistanceFromPoint(const TVector3<Real>& p_vecPoint) const
{
	return (p_vecPoint - ClosestPoint(p_vecPoint)).Length();
}

Real Tetrahedron::DistanceFromPoint(
	const TVector3<Real>& p_vecPoint,
	TVector3<Real>& p_vecClosestPoint) const
{
	p_vecClosestPoint = ClosestPoint(p_vecPoint);
	return (p_vecPoint - p_vecClosestPoint).Length();
}

TVector3<Real> Tetrahedron::ClosestPoint(const TVector3<Real>& p_vecPoint) const
{
	TVector3<Real> vecCentroid(Centroid());

	// generate side 0 and make sure it points outwards
	Triangle triangle0(Vertices[1], Vertices[2], Vertices[3]);
	if (triangle0.PointAbovePlane(vecCentroid))
		triangle0.Invert();

	// if point above side 0 it is in voronoi of its vertices, edges or face
	if (triangle0.PointAbovePlane(p_vecPoint))
		return triangle0.ClosestPoint(p_vecPoint);

	// generate side 1 and make sure it points outwards
	Triangle triangle1(Vertices[0], Vertices[2], Vertices[3]);
	if (triangle1.PointAbovePlane(vecCentroid))
		triangle1.Invert();

	// if point above side 1 it is in voronoi of its vertices, edges or face
	if (triangle1.PointAbovePlane(p_vecPoint))
		return triangle1.ClosestPoint(p_vecPoint);

	// generate side 2 and make sure it points outwards
	Triangle triangle2(Vertices[0], Vertices[1], Vertices[3]);
	if (!triangle2.PointAbovePlane(vecCentroid))
		triangle2.Invert();

	// if point above side 2 it is in voronoi of its vertices, edges or face
	if (triangle2.PointAbovePlane(p_vecPoint))
		return triangle2.ClosestPoint(p_vecPoint);

	// generate side 3 and make sure it points outwards
	Triangle triangle3(Vertices[0], Vertices[1], Vertices[2]);
	if (triangle3.PointAbovePlane(vecCentroid))
		triangle3.Invert();

	// if point above side 3 it is in voronoi of its vertices, edges or face
	if (triangle3.PointAbovePlane(p_vecPoint))
		return triangle3.ClosestPoint(p_vecPoint);

	// otherwise, it is inside
	return p_vecPoint;
}

bool Tetrahedron::Contains(const TVector3<Real>& p_vecPoint) const
{
	return ClosestPoint(p_vecPoint) == p_vecPoint;
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

