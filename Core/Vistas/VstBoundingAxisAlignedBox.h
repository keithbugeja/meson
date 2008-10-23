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

	class BoundingAxisAlignedBox : public IBoundingVolume
	{
	public:
		BoundingAxisAlignedBox();
		BoundingAxisAlignedBox(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox);
		BoundingAxisAlignedBox(const Meson::Common::Maths::Vector3f& p_vec3MinExtent, const Meson::Common::Maths::Vector3f& p_vec3MaxExtent);

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
		Meson::Common::Maths::Vector3f GetExtent(void) const;

		float GetDistance(const Meson::Common::Maths::Planef& p_plane) const;
		Meson::Common::Maths::Vector3f GetClosestPoint(const Meson::Common::Maths::Vector3f& p_vec3Point) const;
		Meson::Common::Maths::Planef::Side GetSide(const Meson::Common::Maths::Planef& p_plane) const;

		bool Contains(const Meson::Common::Maths::Vector3f& p_vec3Point) const;
		bool Contains(const IBoundingVolume& p_boundingVolume) const;
		bool Contains(const BoundingSphere& p_boundingSphere) const;
		bool Contains(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const;

		void GrowToContain(const IBoundingVolume& p_boundingVolume);
		void GrowToContain(const BoundingSphere& p_boundingSphere);
		void GrowToContain(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox);

		BoundingAxisAlignedBox& operator=(const BoundingAxisAlignedBox& p_boundingAxisAlignedBox);
		BoundingAxisAlignedBox& operator=(const BoundingSphere& p_boundingSphere);
		IBoundingVolume& operator=(const IBoundingVolume& p_boundingVolume);
	
	public:
		Meson::Common::Maths::Vector3f MinExtent, 
									   MaxExtent;
	};

Meson_Vistas_END
