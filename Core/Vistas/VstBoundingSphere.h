//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstBoundingVolume.h"

Meson_Vistas_BEGIN

	class BoundingSphere : public IBoundingVolume
	{
	public:
		BoundingSphere();
		BoundingSphere(const BoundingSphere& p_boundingSphere);
		BoundingSphere(const Meson::Common::Maths::Vector3f& p_vec3Centre, float p_fRadius);

		Type GetType(void) const;

		BoundingVolumePtr Clone(bool p_bShallow);
		BoundingVolumePtr CreateInstance(void);

		void ComputeFromVertices(Vector3ArrayList& p_vectorArrayList);
		void ComputeFromVolume(const IBoundingVolume& p_boundingVolume);
		void ComputeFromVolume(const Transformation& p_transformation, const IBoundingVolume& p_boundingVolume);

		void Transform(const Transformation& p_transformation);
		void Transform(const Transformation& p_transformation, IBoundingVolume& p_boundingVolume) const;

		bool Intersects(const IBoundingVolume& p_boundingVolume) const;
		bool Intersects(const BoundingSphere& p_boundingSphere) const;
		bool Intersects(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const;

		float GetRadius(void) const;
		Meson::Common::Maths::Vector3f GetCentre(void) const;

		float GetDistance(const Meson::Common::Maths::Planef& p_plane) const;
		Meson::Common::Maths::Planef::Side GetSide(const Meson::Common::Maths::Planef& p_plane) const;
		Meson::Common::Maths::Vector3f GetClosestPoint(const Meson::Common::Maths::Vector3f& p_vec3Point) const;

		bool Contains(const Meson::Common::Maths::Vector3f& p_vec3Point) const;
		bool Contains(const IBoundingVolume& p_boundingVolume) const;
		bool Contains(const BoundingSphere& p_boundingSphere) const;
		bool Contains(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const;

		void GrowToContain(const IBoundingVolume& p_boundingVolume);
		void GrowToContain(const BoundingSphere& p_boundingSphere);
		void GrowToContain(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox);

		IBoundingVolume& operator=(const IBoundingVolume& p_boundingVolume);
		BoundingSphere& operator=(const BoundingSphere& p_boundingSphere);
		BoundingSphere& operator=(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox);
	
	public:
		float Radius,
			  RadiusSquared;
		
		Meson::Common::Maths::Vector3f Centre;
	};

Meson_Vistas_END
