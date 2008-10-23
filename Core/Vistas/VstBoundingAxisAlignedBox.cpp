//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstBoundingAxisAlignedBox.h"
#include "VstBoundingSphere.h"

using namespace Meson::Common::Maths;
using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
BoundingAxisAlignedBox::BoundingAxisAlignedBox()
	: MinExtent(-1.0f),
	  MaxExtent(1.0f)
{
}
//----------------------------------------------------------------------------------------------
BoundingAxisAlignedBox::BoundingAxisAlignedBox(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox)
	: MinExtent(p_boundingAxisAlignedBox.MinExtent),
	  MaxExtent(p_boundingAxisAlignedBox.MaxExtent)
{
}
//----------------------------------------------------------------------------------------------
BoundingAxisAlignedBox::BoundingAxisAlignedBox(const Vector3f& p_vec3MinExtent, const Vector3f& p_vec3MaxExtent)
{
	if (p_vec3MinExtent.X > p_vec3MaxExtent.X) {
		MinExtent.X = p_vec3MaxExtent.X;
		MaxExtent.X = p_vec3MinExtent.X;
	} else {
		MinExtent.X = p_vec3MinExtent.X;
		MaxExtent.X = p_vec3MaxExtent.X;
	}

	if (p_vec3MinExtent.Y > p_vec3MaxExtent.Y) {
		MinExtent.Y = p_vec3MaxExtent.Y;
		MaxExtent.Y = p_vec3MinExtent.Y;
	} else {
		MinExtent.Y = p_vec3MinExtent.Y;
		MaxExtent.Y = p_vec3MaxExtent.Y;
	}

	if (p_vec3MinExtent.Z > p_vec3MaxExtent.Z) {
		MinExtent.Z = p_vec3MaxExtent.Z;
		MaxExtent.Z = p_vec3MinExtent.Z;
	} else {
		MinExtent.Z = p_vec3MinExtent.Z;
		MaxExtent.Z = p_vec3MaxExtent.Z;
	}
}
//----------------------------------------------------------------------------------------------
IBoundingVolume::Type BoundingAxisAlignedBox::GetType(void) const 
{
	return BV_AXIS_ALIGNED_BOX;
}
//----------------------------------------------------------------------------------------------
BoundingVolumePtr BoundingAxisAlignedBox::Clone(bool p_bShallow)
{
	return BoundingVolumePtr(new BoundingAxisAlignedBox(*this));
}
//----------------------------------------------------------------------------------------------
BoundingVolumePtr BoundingAxisAlignedBox::CreateInstance(void)
{
	return BoundingVolumePtr(new BoundingAxisAlignedBox());
}
//----------------------------------------------------------------------------------------------
void BoundingAxisAlignedBox::ComputeFromVertices(Vector3ArrayList& p_vectorArrayList)
{
	Vector3f vec3MinExtent(Mathf::Maximum), 
			 vec3MaxExtent(Mathf::Minimum);
	
	Vector3Enumerator vec3Enumerator = p_vectorArrayList.GetEnumerator();
	
	while(vec3Enumerator.HasMoreElements())
	{
		Vector3f& vec3Vertex = vec3Enumerator.NextElement();

		vec3MinExtent.X = Mathf::Min(vec3Vertex.X, vec3MinExtent.X);
		vec3MinExtent.Y = Mathf::Min(vec3Vertex.Y, vec3MinExtent.Y);
		vec3MinExtent.Z = Mathf::Min(vec3Vertex.Z, vec3MinExtent.Z);

		vec3MaxExtent.X = Mathf::Max(vec3Vertex.X, vec3MaxExtent.X);
		vec3MaxExtent.Y = Mathf::Max(vec3Vertex.Y, vec3MaxExtent.Y);
		vec3MaxExtent.Z = Mathf::Max(vec3Vertex.Z, vec3MaxExtent.Z);
	}

	MinExtent = vec3MinExtent;
	MaxExtent = vec3MaxExtent;
}
//----------------------------------------------------------------------------------------------
void BoundingAxisAlignedBox::ComputeFromVolume(const Transformation& p_transformation, const IBoundingVolume& p_boundingVolume)
{
	*this = p_boundingVolume;
	Transform(p_transformation);
}
//----------------------------------------------------------------------------------------------
void BoundingAxisAlignedBox::ComputeFromVolume(const IBoundingVolume& p_boundingVolume)
{
	*this = p_boundingVolume;
}
//----------------------------------------------------------------------------------------------
void BoundingAxisAlignedBox::Transform(const Transformation& p_transformation)
{
	Transformation& transformation = (const_cast<Transformation&>(p_transformation));
	Matrix3f& mat3Rotation = (const_cast<Matrix3f&>(transformation.GetRotationInMatrixForm()));

	Vector3f vec3Centre = transformation.ApplyCopy(GetCentre());
	Vector3f vec3Extent = GetExtent();

	Vector3f vec3HalfSize(
		Mathf::Abs(mat3Rotation._11) * vec3Extent.X + Mathf::Abs(mat3Rotation._12) * vec3Extent.Y + Mathf::Abs(mat3Rotation._13) * vec3Extent.Z,
		Mathf::Abs(mat3Rotation._21) * vec3Extent.X + Mathf::Abs(mat3Rotation._22) * vec3Extent.Y + Mathf::Abs(mat3Rotation._23) * vec3Extent.Z,
		Mathf::Abs(mat3Rotation._31) * vec3Extent.X + Mathf::Abs(mat3Rotation._32) * vec3Extent.Y + Mathf::Abs(mat3Rotation._33) * vec3Extent.Z
	);	

	if (p_transformation.HasScalingComponent())
		vec3HalfSize.ComponentProduct(p_transformation.GetScaling());

	MinExtent = vec3Centre - vec3HalfSize;
	MaxExtent = vec3Centre + vec3HalfSize;
}
//----------------------------------------------------------------------------------------------
void BoundingAxisAlignedBox::Transform(const Transformation& p_transformation, IBoundingVolume& p_boundingVolume) const
{
	MESON_ASSERT(p_boundingVolume.GetType() == IBoundingVolume::BV_AXIS_ALIGNED_BOX, "Incompatible type information : cannot transform.");

	p_boundingVolume = *this;
	p_boundingVolume.Transform(p_transformation);
}
//----------------------------------------------------------------------------------------------
bool BoundingAxisAlignedBox::Intersects(const IBoundingVolume& p_boundingVolume) const
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
bool BoundingAxisAlignedBox::Intersects(const BoundingSphere& p_boundingSphere) const
{
	/** If the AABB's closest point to sphere is within radius, then we have a collision.
	 */
	Vector3f vec3ClosestPoint = GetClosestPoint(p_boundingSphere.Centre);
	vec3ClosestPoint -= p_boundingSphere.Centre;
	
	return (vec3ClosestPoint.LengthSquared() < p_boundingSphere.RadiusSquared); 
}
//----------------------------------------------------------------------------------------------
bool BoundingAxisAlignedBox::Intersects(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const
{
	return (p_boundingAxisAlignedBox.MinExtent.X < MaxExtent.X && p_boundingAxisAlignedBox.MaxExtent.X > MinExtent.X &&
			p_boundingAxisAlignedBox.MinExtent.Y < MaxExtent.Y && p_boundingAxisAlignedBox.MaxExtent.Y > MinExtent.Y &&
			p_boundingAxisAlignedBox.MinExtent.Z < MaxExtent.Z && p_boundingAxisAlignedBox.MaxExtent.Z > MinExtent.Z);
}

//----------------------------------------------------------------------------------------------
float BoundingAxisAlignedBox::GetRadius(void) const
{
	return GetExtent().Length();
}
//----------------------------------------------------------------------------------------------
Vector3f BoundingAxisAlignedBox::GetCentre(void) const
{
	return MinExtent + (MaxExtent - MinExtent) * 0.5f;
}
//----------------------------------------------------------------------------------------------
Vector3f BoundingAxisAlignedBox::GetExtent(void) const
{
	return (MaxExtent - MinExtent) * 0.5f;
}
//----------------------------------------------------------------------------------------------
Planef::Side BoundingAxisAlignedBox::GetSide(const Planef& p_plane) const
{
	Vector3f vec3Extent = GetExtent();
	Vector3f vec3Centre = GetCentre();
	float fDistance = p_plane.GetDisplacement(vec3Centre);

	float fAbsDistance = Mathf::Abs(p_plane.Normal.X * vec3Extent.X) + Mathf::Abs(p_plane.Normal.Y * vec3Extent.Y) + Mathf::Abs(p_plane.Normal.Z * vec3Extent.Z); 

	if (fDistance < -fAbsDistance)
		return Planef::SIDE_NEGATIVE;

	if (fDistance >  fAbsDistance)
		return Planef::SIDE_POSITIVE;

	return Planef::SIDE_BOTH;
}
//----------------------------------------------------------------------------------------------
float BoundingAxisAlignedBox::GetDistance(const Meson::Common::Maths::Planef& p_plane) const
{
	Vector3f vec3PointOnPlane = GetCentre() + p_plane.GetDisplacementVector(GetCentre());
	Vector3f vec3ClosestPoint = GetClosestPoint(vec3PointOnPlane);
	
	return p_plane.GetDistance(vec3ClosestPoint);
}
//----------------------------------------------------------------------------------------------
Meson::Common::Maths::Vector3f BoundingAxisAlignedBox::GetClosestPoint(const Meson::Common::Maths::Vector3f& p_vec3Point) const
{
	Vector3f vec3ClosestPoint;

	// No need to use Math::Min/Max
	vec3ClosestPoint.X = MIN(MAX(MinExtent.X, p_vec3Point.X), MaxExtent.X);
	vec3ClosestPoint.Y = MIN(MAX(MinExtent.Y, p_vec3Point.Y), MaxExtent.Y);
	vec3ClosestPoint.Z = MIN(MAX(MinExtent.Z, p_vec3Point.Z), MaxExtent.Z);

	return vec3ClosestPoint;
}
//----------------------------------------------------------------------------------------------
bool BoundingAxisAlignedBox::Contains(const IBoundingVolume& p_boundingVolume) const
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
bool BoundingAxisAlignedBox::Contains(const BoundingSphere& p_boundingSphere) const
{
	return (MaxExtent.X - p_boundingSphere.Centre.X >= p_boundingSphere.Radius &&
		    p_boundingSphere.Centre.X - MinExtent.X >= p_boundingSphere.Radius &&
			MaxExtent.Y - p_boundingSphere.Centre.Y >= p_boundingSphere.Radius &&
		    p_boundingSphere.Centre.Y - MinExtent.Y >= p_boundingSphere.Radius &&
			MaxExtent.Z - p_boundingSphere.Centre.Z >= p_boundingSphere.Radius &&
		    p_boundingSphere.Centre.Z - MinExtent.Z >= p_boundingSphere.Radius);
}
//----------------------------------------------------------------------------------------------
bool BoundingAxisAlignedBox::Contains(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const
{
	return (MinExtent.X < p_boundingAxisAlignedBox.MinExtent.X &&
			MaxExtent.X > p_boundingAxisAlignedBox.MaxExtent.X &&
			MinExtent.Y < p_boundingAxisAlignedBox.MinExtent.Y &&
			MaxExtent.Y > p_boundingAxisAlignedBox.MaxExtent.Y &&
			MinExtent.Z < p_boundingAxisAlignedBox.MinExtent.Z &&
			MaxExtent.Z > p_boundingAxisAlignedBox.MaxExtent.Z);
}
//----------------------------------------------------------------------------------------------
bool BoundingAxisAlignedBox::Contains(const Vector3f& p_vec3Point) const
{
	return (MinExtent.X < p_vec3Point.X &&
			MaxExtent.X > p_vec3Point.X &&
			MinExtent.Y < p_vec3Point.Y &&
			MaxExtent.Y > p_vec3Point.Y &&
			MinExtent.Z < p_vec3Point.Z &&
			MaxExtent.Z > p_vec3Point.Z);
}
//----------------------------------------------------------------------------------------------
void BoundingAxisAlignedBox::GrowToContain(const IBoundingVolume& p_boundingVolume)
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
void BoundingAxisAlignedBox::GrowToContain(const BoundingSphere& p_boundingSphere)
{
	MinExtent.X = MIN(MinExtent.X, p_boundingSphere.Centre.X - p_boundingSphere.Radius);
	MinExtent.Y = MIN(MinExtent.Y, p_boundingSphere.Centre.Y - p_boundingSphere.Radius);
	MinExtent.Z = MIN(MinExtent.Z, p_boundingSphere.Centre.Z - p_boundingSphere.Radius);
	
	MaxExtent.X = MAX(MaxExtent.X, p_boundingSphere.Centre.X + p_boundingSphere.Radius);
	MaxExtent.Y = MAX(MaxExtent.Y, p_boundingSphere.Centre.Y + p_boundingSphere.Radius);
	MaxExtent.Z = MAX(MaxExtent.Z, p_boundingSphere.Centre.Z + p_boundingSphere.Radius);
}
//----------------------------------------------------------------------------------------------
void BoundingAxisAlignedBox::GrowToContain(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox)
{
	MinExtent.X = MIN(MinExtent.X, p_boundingAxisAlignedBox.MinExtent.X);
	MinExtent.Y = MIN(MinExtent.Y, p_boundingAxisAlignedBox.MinExtent.Y);
	MinExtent.Z = MIN(MinExtent.Z, p_boundingAxisAlignedBox.MinExtent.Z);
	
	MaxExtent.X = MAX(MaxExtent.X, p_boundingAxisAlignedBox.MaxExtent.X);
	MaxExtent.Y = MAX(MaxExtent.Y, p_boundingAxisAlignedBox.MaxExtent.Y);
	MaxExtent.Z = MAX(MaxExtent.Z, p_boundingAxisAlignedBox.MaxExtent.Z);
}
//----------------------------------------------------------------------------------------------
BoundingAxisAlignedBox& BoundingAxisAlignedBox::operator=(const BoundingSphere& p_boundingSphere)
{
	MinExtent.X = p_boundingSphere.Centre.X - p_boundingSphere.Radius;
	MinExtent.Y = p_boundingSphere.Centre.Y - p_boundingSphere.Radius;
	MinExtent.Z = p_boundingSphere.Centre.Z - p_boundingSphere.Radius;

	MaxExtent.X = p_boundingSphere.Centre.X + p_boundingSphere.Radius;
	MaxExtent.Y = p_boundingSphere.Centre.Y + p_boundingSphere.Radius;
	MaxExtent.Z = p_boundingSphere.Centre.Z + p_boundingSphere.Radius;

	return *this;
}
//----------------------------------------------------------------------------------------------
BoundingAxisAlignedBox& BoundingAxisAlignedBox::operator=(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox)
{
	MinExtent = p_boundingAxisAlignedBox.MinExtent;
	MaxExtent = p_boundingAxisAlignedBox.MaxExtent;

	return *this;
}
//----------------------------------------------------------------------------------------------
IBoundingVolume& BoundingAxisAlignedBox::operator=(const IBoundingVolume& p_boundingVolume)
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

Meson_Vistas_END