#include "Cylinder.h"

#include "Maths.h"
#include "DebugSupport.h"
#include "PolynomialSolver.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Maths::Solvers;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas::Geometry;

Meson_Gravitas_Geometry_BEGIN

const String g_strGeometryTypeCylinder = "Cylinder";

void Cylinder::ProjectToInterval(
	const LineSegment &p_lnsShaft, Real p_rRadius,
	const TVector3<Real>& p_vecAxis, 
	TInterval<Real>& p_interval)
{
	p_interval.MakeEmpty();
	p_interval.Extend(p_lnsShaft.Start.ToVector() * p_vecAxis);
	p_interval.Extend(p_lnsShaft.End.ToVector() * p_vecAxis);

	Real rCosine = p_lnsShaft.Direction() * p_vecAxis;
	// rCosine may be slightly > 1 due to rounding errors!
	Real rCosineSquared = TMaths<Real>::Min((Real) 1.0, rCosine * rCosine);
	Real rSine = TMaths<Real>::Sqrt((Real) 1.0 - rCosineSquared);

	Real rRadiusProjection = p_rRadius * rSine;
	p_interval.Min -= rRadiusProjection;
	p_interval.Max += rRadiusProjection;
}

void Cylinder::EnumerateMaximalVertices(
	const Meson::Gravitas::Geometry::LineSegment &p_lnsShaft, Real p_rRadius,
	const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
	Meson::Gravitas::PointList& p_listVertices)
{
	p_listVertices.Clear();

	TVector3<Real> vecShaftDirection = p_lnsShaft.Direction();
	Real rCosine = vecShaftDirection * p_vecAxis;

	// axis perpendicular to shaft
	if (TMaths<Real>::Equals(rCosine, (Real) 0.0))
	{
		// add two vertices for curve edge
		TVector3<Real> vecRadius = p_vecAxis * p_rRadius;
		p_listVertices.Add(p_lnsShaft.Start + vecRadius);
		p_listVertices.Add(p_lnsShaft.End + vecRadius);
		return;
	}

	// first approximation is one of shaft endpoints
	TPoint3<Real> ptVertex;
	if (rCosine > TMaths<Real>::Epsilon)
		ptVertex = p_lnsShaft.End;
	else
		ptVertex = p_lnsShaft.Start;

	// axis parallel to shaft
	if (TMaths<Real>::Equals(TMaths<Real>::Abs(rCosine), (Real) 1.0))
	{
		// sample 8 vertices on cylinder end
		TVector3<Real> vecNormal1, vecNormal2;
		p_vecAxis.ComputeOrthonormals(vecNormal1, vecNormal2);
		TVector3<Real> vecNormal3 = (vecNormal1 + vecNormal2).NormaliseCopy();
		TVector3<Real> vecNormal4 = (vecNormal1 - vecNormal2).NormaliseCopy();

		p_listVertices.Add(ptVertex + vecNormal1 * p_rRadius);
		p_listVertices.Add(ptVertex - vecNormal1 * p_rRadius);
		p_listVertices.Add(ptVertex + vecNormal2 * p_rRadius);
		p_listVertices.Add(ptVertex - vecNormal2 * p_rRadius);
		p_listVertices.Add(ptVertex + vecNormal3 * p_rRadius);
		p_listVertices.Add(ptVertex - vecNormal3 * p_rRadius);
		p_listVertices.Add(ptVertex + vecNormal4 * p_rRadius);
		p_listVertices.Add(ptVertex - vecNormal4 * p_rRadius);
		return;		
	}

	// arbitrary axis - single maximal vertex on edge between side and one end

	// compute axis component perpendicular to shaft and normalise it
	TVector3<Real> vecPerp = p_vecAxis - vecShaftDirection * rCosine;
	vecPerp.Normalise();

	// add radius component
	ptVertex += vecPerp * p_rRadius;
	p_listVertices.Add(ptVertex);
}

bool Cylinder::IntersectsPoint(
	const Meson::Gravitas::Geometry::LineSegment &p_lnsShaft, Real p_rRadius,
	const Meson::Common::Maths::TPoint3<Real>& p_ptPoint)
{
	TVector3<Real> vecDirection = p_lnsShaft.Direction();
	TVector3<Real> vecPointStartOffset = p_ptPoint - p_lnsShaft.Start;
	Real rProjection = vecPointStartOffset * vecDirection;

	if (rProjection < (Real) 0.0) return false;
	if (rProjection > p_lnsShaft.Length()) return false;

	TVector3<Real> vecPointShaftOffset
		= vecPointStartOffset - vecDirection * rProjection;

	return vecPointShaftOffset.LengthSquared() <= p_rRadius * p_rRadius;
}

Cylinder::Cylinder(void)
	: m_nId(-1)
	, Height((Real) 0.5)
	, HeightSquared((Real) 0.25)
	, Radius((Real) 0.5)
	, RadiusSquared((Real) 0.25)
{
}

Cylinder::Cylinder(const String& p_strId)
	: IGeometry(p_strId)
	, m_nId(-1)
	, Height((Real) 0.5)
	, HeightSquared((Real) 0.25)
	, Radius((Real) 0.5)
	, RadiusSquared((Real) 0.25)
{
}

Cylinder::~Cylinder(void)
{
}

void Cylinder::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties["Height"] = PropertyDescriptor("Height", PropertyType::Real, false);
	p_mapProperties["HeightSquared"] = PropertyDescriptor("HeightSquared", PropertyType::Real, true);
	p_mapProperties["Radius"] = PropertyDescriptor("Radius", PropertyType::Real, false);
	p_mapProperties["RadiusSquared"] = PropertyDescriptor("RadiusSquared", PropertyType::Real, true);
}

void Cylinder::GetProperty(const String& p_strName,
	Real& p_rValue) const
{
	if (p_strName == "Height")
		p_rValue = Height;
	else if (p_strName == "HeightSquared")
		p_rValue = HeightSquared;
	else if (p_strName == "Radius")
		p_rValue = Radius;
	else if (p_strName == "RadiusSquared")
		p_rValue = RadiusSquared;
	else
		MESON_ASSERT(false, "Cylinder geometry: Invalid property specified.")
}

void Cylinder::SetProperty(const String &p_strName, Real p_rValue)
{
	if (p_strName == "Height")
	{
		Height = p_rValue;
		HeightSquared = p_rValue * p_rValue;
	}
	else if (p_strName == "Radius")
	{
		Radius = p_rValue;
		RadiusSquared = p_rValue * p_rValue;
	}
	else
		MESON_ASSERT(false, "Cylinder geometry: Invalid property specified.")
}

const String& Cylinder::GetTypeName(void) const
{
	return g_strGeometryTypeCylinder;
}

short Cylinder::GetTypeId(void) const
{
	return m_nId;
}

void Cylinder::SetTypeId(short p_nId)
{
	m_nId = p_nId;
}

Real Cylinder::GetVolume(void) const
{
	return Height * TMaths<Real>::Pi * RadiusSquared;
}

TMatrix3<Real> Cylinder::GetDistributionTensor(void) const
{
	Real rVolume = GetVolume();

	TMatrix3<Real> matDistributionTensor = TMatrix3<Real>::Zero;


	matDistributionTensor._11 = matDistributionTensor._33
		= (rVolume * ((Real) 3.0 * RadiusSquared + HeightSquared)) / (Real) 12.0; 

	matDistributionTensor._22 = (Real) 0.5 * RadiusSquared * rVolume;

	return matDistributionTensor;
}

bool Cylinder::IsBounded(void) const
{
	return true;
}

void Cylinder::ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const
{
	p_boundingSphere.Centre = TPoint3<Real>::Origin;
	p_boundingSphere.RadiusSquared = HeightSquared * (Real) 0.25 + RadiusSquared;
	p_boundingSphere.Radius = TMaths<Real>::Sqrt(p_boundingSphere.RadiusSquared);
}

void Cylinder::ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	MESON_ASSERT(Height >= (Real) 0.0, "Cylinder height must be positive.");
	MESON_ASSERT(Radius >= (Real) 0.0, "Cylinder radius must be positive.");

	TVector3<Real> vecExtent(Radius, Height * (Real) 0.5, Radius);
	p_boundingAxisAlignedBox.Min = TPoint3<Real>::Origin - vecExtent;
	p_boundingAxisAlignedBox.Max = TPoint3<Real>::Origin + vecExtent;
}

void Cylinder::ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const
{
	MESON_ASSERT(Height >= (Real) 0.0, "Cylinder height must be positive.");
	MESON_ASSERT(Radius >= (Real) 0.0, "Cylinder radius must be positive.");

	TVector3<Real> vecExtent(Radius, Height * (Real) 0.5, Radius);
	p_boundingOrientedBox.Centre = TPoint3<Real>::Origin;
	p_boundingOrientedBox.Extent = vecExtent;
	p_boundingOrientedBox.Axis[0] = TVector3<Real>::Right;
	p_boundingOrientedBox.Axis[1] = TVector3<Real>::Up;
	p_boundingOrientedBox.Axis[2] = TVector3<Real>::Out;
}

bool Cylinder::IntersectsRay(const Ray& p_ray) const
{
	static TPoint3<Real> s_ptIntersction;
	return IntersectsRay(p_ray, s_ptIntersction);
}

bool Cylinder::IntersectsRay(const Ray& p_ray, TPoint3<Real>& p_ptIntersectionPoint) const
{
	Real rTMin((Real) 0.0);
	Real rTMax(TMaths<Real>::Maximum);

	// do slab test marked by end disks
	Real rHalfHeight(Height * (Real) 0.5);
	if (TMaths<Real>::Equals(p_ray.Direction.Y, (Real) 0.0))
	{
		// ray tangential to end disks i.e. definitely no intersection if ray source outside
		if (TMaths<Real>::Abs(p_ray.Source.Y) > rHalfHeight)
				return false;
	}
	else
	{
		// ray not tangential to end disks - compute slab t min / max
		Real rDirectionInverse((Real) 1.0 / p_ray.Direction.Y);
		Real rTMinSlab((-rHalfHeight - p_ray.Source.Y) * rDirectionInverse);
		Real rTMaxSlab(( rHalfHeight - p_ray.Source.Y) * rDirectionInverse);
		if (rTMinSlab > rTMaxSlab)
		{
			Real rTemp(rTMinSlab);
			rTMinSlab = rTMaxSlab;
			rTMaxSlab = rTemp;
		}

		// restrict best t min and max using current slab t min/max
		rTMin = TMaths<Real>::Max(rTMin, rTMinSlab);
		rTMax = TMaths<Real>::Min(rTMax, rTMaxSlab);

		// no intersection if ray looking away from this slab
		if (rTMin > rTMax)
			return false;
	}

	// do slab test for cylinder sides
	if (TMaths<Real>::Equals(p_ray.Direction.X, (Real) 0.0)
		&& TMaths<Real>::Equals(p_ray.Direction.Z, (Real) 0.0))
	{
		// ray parallel to Y axis i.e. definitely no intersection if ray source outside
		if (p_ray.Source.X * p_ray.Source.X + p_ray.Source.Z * p_ray.Source.Z > RadiusSquared)
			return false;
	}
	else
	{
		// find t min / max by solving quadratic equation for circle in XZ plane
		// in conjunction with ray projection on plane
		Real rCoeffSquared(p_ray.Direction.X * p_ray.Direction.X + p_ray.Direction.Z * p_ray.Direction.Z);
		Real rCoeffLinear((Real) 2.0 * (p_ray.Source.X * p_ray.Direction.X + p_ray.Source.Z * p_ray.Direction.Z));
		Real rCoeffConstant(p_ray.Source.X * p_ray.Source.X + p_ray.Source.Z * p_ray.Source.Z - RadiusSquared);

		Real rTMinSlab, rTMaxSlab;
		if (!TPolynomialSolver<Meson::Gravitas::Real>::SolveQuadratic(
			rCoeffSquared, rCoeffLinear, rCoeffConstant, rTMinSlab, rTMaxSlab))
			// no solution implies ray passes outside cylinder
			return false;

		// restrict best t min and max using current slab t min/max
		rTMin = TMaths<Real>::Max(rTMin, rTMinSlab);
		rTMax = TMaths<Real>::Min(rTMax, rTMaxSlab);

		// no intersection if ray looking away from this slab
		if (rTMin > rTMax)
			return false;
	}

	// compute intersection point
	p_ptIntersectionPoint = p_ray.Source + p_ray.Direction * rTMin;

	// if this point reached - intersection occured
	return true;
}

Meson_Gravitas_Geometry_END
