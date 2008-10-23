//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstTransformation.h"

Meson_Vistas_BEGIN

	class IBoundingVolume : public Meson::Common::TReferenceCounter<IBoundingVolume>
	{
	public:
		enum Type 
		{
			BV_SPHERE,
			BV_AXIS_ALIGNED_BOX,
			BV_ORIENTED_BOX,
			
			BV_COUNT
		};

	public:
		virtual Type GetType(void) const = 0;

		virtual Meson::Common::TPointer<IBoundingVolume> Clone(bool p_bShallow = true) = 0;
		virtual Meson::Common::TPointer<IBoundingVolume> CreateInstance(void) = 0;

		virtual void ComputeFromVertices(Vector3ArrayList& p_vectorArrayList) = 0;
		virtual void ComputeFromVolume(const IBoundingVolume& p_boundingVolume) = 0;
		virtual void ComputeFromVolume(const Transformation& p_transformation, const IBoundingVolume& p_boundingVolume) = 0;

		virtual void Transform(const Transformation& p_transformation) = 0;
		virtual void Transform(const Transformation& p_transformation, IBoundingVolume& p_boundingVolume) const = 0;

		virtual bool Intersects(const IBoundingVolume& p_boundingVolume) const = 0;

		virtual float GetRadius(void) const = 0;
		virtual Meson::Common::Maths::Vector3f GetCentre(void) const = 0;

		virtual float GetDistance(const Meson::Common::Maths::Planef& p_plane) const = 0;
		virtual Meson::Common::Maths::Planef::Side GetSide(const Meson::Common::Maths::Planef& p_plane) const = 0;
		virtual Meson::Common::Maths::Vector3f GetClosestPoint(const Meson::Common::Maths::Vector3f& p_vec3Point) const = 0;

		virtual bool Contains(const Meson::Common::Maths::Vector3f& p_vec3Point) const = 0;
		virtual bool Contains(const IBoundingVolume& p_boundingVolume) const = 0;
		virtual void GrowToContain(const IBoundingVolume& p_boundingVolume) = 0;

		virtual IBoundingVolume& operator=(const IBoundingVolume& p_boundingVolume) = 0;
	};

	typedef Meson::Common::TPointer<IBoundingVolume> BoundingVolumePtr;
	typedef Meson::Common::Collections::TArrayList<BoundingVolumePtr> BoundingVolumeList;

Meson_Vistas_END
