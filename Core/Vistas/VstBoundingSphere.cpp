//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstBoundingSphere.h"
#include "VstBoundingAxisAlignedBox.h"

using namespace Meson::Common::Maths;

using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
BoundingSphere::BoundingSphere()
	: Radius(1.0f),
	  RadiusSquared(1.0f),
	  Centre(0.0f)
{
}
//----------------------------------------------------------------------------------------------
BoundingSphere::BoundingSphere(const BoundingSphere& p_boundingSphere)
	: Radius(p_boundingSphere.Radius),
	  RadiusSquared(p_boundingSphere.RadiusSquared),
	  Centre(p_boundingSphere.Centre)
{
}
//----------------------------------------------------------------------------------------------
BoundingSphere::BoundingSphere(const Vector3f& p_vec3Centre, float p_fRadius)
	: Radius(p_fRadius),
	  RadiusSquared(p_fRadius * p_fRadius),
	  Centre(p_vec3Centre)
{
}
//----------------------------------------------------------------------------------------------
IBoundingVolume::Type BoundingSphere::GetType(void) const 
{
	return BV_SPHERE;
}
//----------------------------------------------------------------------------------------------
BoundingVolumePtr BoundingSphere::Clone(bool p_bShallow)
{
	return BoundingVolumePtr(new BoundingSphere(*this));
}
//----------------------------------------------------------------------------------------------
BoundingVolumePtr BoundingSphere::CreateInstance(void)
{
	return BoundingVolumePtr(new BoundingSphere());
}
//----------------------------------------------------------------------------------------------
// TODO: Quick and dirty - replace with a mean-based method
void BoundingSphere::ComputeFromVertices(Vector3ArrayList& p_vectorArrayList)
{
	Vector3f vec3MinExtent(Mathf::Maximum),
		vec3MaxExtent(Mathf::Minimum);

	Vector3Enumerator vec3Enumerator = p_vectorArrayList.GetEnumerator();
	
	while(vec3Enumerator.HasMoreElements())
	{
		Vector3f& vec3Vertex = vec3Enumerator.NextElement();

		vec3MaxExtent.X = Mathf::Max(vec3Vertex.X, vec3MaxExtent.X);
		vec3MaxExtent.Y = Mathf::Max(vec3Vertex.Y, vec3MaxExtent.Y);
		vec3MaxExtent.Z = Mathf::Max(vec3Vertex.Z, vec3MaxExtent.Z);

		vec3MinExtent.X = Mathf::Min(vec3Vertex.X, vec3MinExtent.X);
		vec3MinExtent.Y = Mathf::Min(vec3Vertex.Y, vec3MinExtent.Y);
		vec3MinExtent.Z = Mathf::Min(vec3Vertex.Z, vec3MinExtent.Z);
	}

	Vector3f &vec3Distance = vec3MaxExtent - vec3MinExtent;
	Radius = vec3Distance.Length() * 0.5f;
	RadiusSquared = Radius * Radius;

	Vector3f &vec3DistanceUnit = vec3Distance.NormaliseCopy();
	Centre = vec3MinExtent + vec3DistanceUnit * Radius;
}
//----------------------------------------------------------------------------------------------
void BoundingSphere::ComputeFromVolume(const Transformation& p_transformation, const IBoundingVolume& p_boundingVolume)
{
	*this = p_boundingVolume;
	Transform(p_transformation);
}
//----------------------------------------------------------------------------------------------
void BoundingSphere::ComputeFromVolume(const IBoundingVolume& p_boundingVolume)
{
	*this = p_boundingVolume;
}
//----------------------------------------------------------------------------------------------
void BoundingSphere::Transform(const Transformation& p_transformation)
{
	Matrix3f matRotation(p_transformation.GetRotationInMatrixForm());

	float a = matRotation._11 * matRotation._11 + matRotation._12 * matRotation._12 + matRotation._13 * matRotation._13;
	float b = matRotation._21 * matRotation._21 + matRotation._22 * matRotation._22 + matRotation._23 * matRotation._23;
	float c = matRotation._31 * matRotation._31 + matRotation._32 * matRotation._32 + matRotation._33 * matRotation._33;

	float d = Mathf::Max(Mathf::Max(a, b), c);
	float e = Mathf::Min(Mathf::Min(a, b), c);

	if (d > 1.0f + Mathf::Epsilon)
	{
		d = Mathf::Sqrt(d);
		Radius *= d;
	}
	else if (e < 1.0f - Mathf::Epsilon)
	{
		e = Mathf::Sqrt(e);
		Radius *= e;
	} 

	if (p_transformation.HasScalingComponent())
	{
		Vector3f vec3Scaling(p_transformation.GetScaling());
		vec3Scaling = vec3Scaling * Radius;

		Radius = Mathf::Max(Mathf::Max(vec3Scaling.X, vec3Scaling.Y), vec3Scaling.Z);
		RadiusSquared = Radius * Radius;
	}

	p_transformation.Apply(Centre);
}
//----------------------------------------------------------------------------------------------
void BoundingSphere::Transform(const Transformation& p_transformation, IBoundingVolume& p_boundingVolume) const
{
	MESON_ASSERT(p_boundingVolume.GetType() == IBoundingVolume::BV_SPHERE, "Incompatible type information : cannot transform.");
	
	p_boundingVolume = *this;
	p_boundingVolume.Transform(p_transformation);
}
//----------------------------------------------------------------------------------------------
bool BoundingSphere::Intersects(const IBoundingVolume& p_boundingVolume) const
{
	switch (p_boundingVolume.GetType())
	{
		case IBoundingVolume::BV_SPHERE:
			return Intersects(static_cast<const BoundingSphere&>(p_boundingVolume));

		case IBoundingVolume::BV_AXIS_ALIGNED_BOX:
			return Intersects(static_cast<const BoundingAxisAlignedBox&>(p_boundingVolume));
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool BoundingSphere::Intersects(const BoundingSphere& p_boundingSphere) const
{
	Vector3f vec3Distance = p_boundingSphere.Centre - Centre;
	return (vec3Distance.LengthSquared() < RadiusSquared + p_boundingSphere.RadiusSquared);
}
//----------------------------------------------------------------------------------------------
bool BoundingSphere::Intersects(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const
{
	/** If the AABB's closest point to sphere is within radius, then we have a collision.
	 */
	Vector3f vec3ClosestPoint = p_boundingAxisAlignedBox.GetClosestPoint(Centre);
	vec3ClosestPoint -= Centre;
	
	return (vec3ClosestPoint.LengthSquared() < RadiusSquared); 
}
//----------------------------------------------------------------------------------------------
float BoundingSphere::GetRadius(void) const
{
	return Radius;
}
//----------------------------------------------------------------------------------------------
Vector3f BoundingSphere::GetCentre(void) const
{
	return Centre;
}
//----------------------------------------------------------------------------------------------
Planef::Side BoundingSphere::GetSide(const Planef& p_plane) const
{
	return p_plane.GetSide(Centre, p_plane.Normal * Radius);
}
//----------------------------------------------------------------------------------------------
float BoundingSphere::GetDistance(const Meson::Common::Maths::Planef& p_plane) const
{
	return p_plane.GetDistance(Centre) - Radius;
}
//----------------------------------------------------------------------------------------------
Vector3f BoundingSphere::GetClosestPoint(const Meson::Common::Maths::Vector3f& p_vec3Point) const
{
	Vector3f vec3ClosestPoint(p_vec3Point - Centre);

	if (vec3ClosestPoint.LengthSquared() > Radius)
		vec3ClosestPoint = p_vec3Point;
	else
	{
		vec3ClosestPoint.Normalise();
		vec3ClosestPoint *= Radius;
	}

	return vec3ClosestPoint;
}
//----------------------------------------------------------------------------------------------
bool BoundingSphere::Contains(const IBoundingVolume& p_boundingVolume) const
{
	switch (p_boundingVolume.GetType())
	{
		case IBoundingVolume::BV_SPHERE:
			return Contains(static_cast<const BoundingSphere&>(p_boundingVolume));

		case IBoundingVolume::BV_AXIS_ALIGNED_BOX:
			return Contains(static_cast<const BoundingAxisAlignedBox&>(p_boundingVolume));
	}

	return false;
}
//----------------------------------------------------------------------------------------------
bool BoundingSphere::Contains(const BoundingSphere& p_boundingSphere) const
{
	Vector3f vec3Distance = p_boundingSphere.Centre - Centre;
	return (vec3Distance.LengthSquared() + p_boundingSphere.RadiusSquared < RadiusSquared);
}
//----------------------------------------------------------------------------------------------
bool BoundingSphere::Contains(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const
{
	return ((Centre - p_boundingAxisAlignedBox.MinExtent).LengthSquared() < RadiusSquared &&
		    (p_boundingAxisAlignedBox.MaxExtent - Centre).LengthSquared() < RadiusSquared);
}
//----------------------------------------------------------------------------------------------
bool BoundingSphere::Contains(const Vector3f& p_vec3Point) const
{
	Vector3f vec3Distance = Centre - p_vec3Point;
	return (vec3Distance.LengthSquared() < RadiusSquared);
}
//----------------------------------------------------------------------------------------------
void BoundingSphere::GrowToContain(const IBoundingVolume& p_boundingVolume)
{
	switch (p_boundingVolume.GetType())
	{
		case IBoundingVolume::BV_SPHERE:
			GrowToContain(static_cast<const BoundingSphere&>(p_boundingVolume));
			break;

		case IBoundingVolume::BV_AXIS_ALIGNED_BOX:
			GrowToContain(static_cast<const BoundingAxisAlignedBox&>(p_boundingVolume));
			break;
	}
}
//----------------------------------------------------------------------------------------------
void BoundingSphere::GrowToContain(const BoundingSphere& p_boundingSphere)
{
	Vector3f vec3Distance = p_boundingSphere.Centre - Centre;
	float fDistance = vec3Distance.Length();
	float fRadius = Mathf::Max(Radius, p_boundingSphere.Radius);

	if (fDistance > fRadius)
	{
		Centre += vec3Distance * 0.5f;
		Radius = fRadius + fDistance * 0.5f;
	}
}
//----------------------------------------------------------------------------------------------
void BoundingSphere::GrowToContain(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox)
{
	GrowToContain(BoundingSphere(p_boundingAxisAlignedBox.GetCentre(), p_boundingAxisAlignedBox.GetRadius()));
}
//----------------------------------------------------------------------------------------------
IBoundingVolume& BoundingSphere::operator=(const IBoundingVolume& p_boundingVolume)
{
	switch (p_boundingVolume.GetType())
	{
		case IBoundingVolume::BV_SPHERE:
		{
			*this = static_cast<const BoundingSphere&>(p_boundingVolume);
			return *this;
		}

		case IBoundingVolume::BV_AXIS_ALIGNED_BOX:
		{
			*this = static_cast<const BoundingAxisAlignedBox&>(p_boundingVolume);
			return *this;
		}
	}

	return *this;
}
//----------------------------------------------------------------------------------------------
BoundingSphere& BoundingSphere::operator=(const BoundingSphere& p_boundingSphere)
{
	Radius = p_boundingSphere.Radius;
	RadiusSquared = p_boundingSphere.RadiusSquared;
	Centre = p_boundingSphere.Centre;

	return *this;
}
//----------------------------------------------------------------------------------------------
BoundingSphere& BoundingSphere::operator=(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox)
{
	Radius = p_boundingAxisAlignedBox.GetRadius();
	RadiusSquared = Radius * Radius;
	Centre = p_boundingAxisAlignedBox.GetCentre();

	return *this;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END