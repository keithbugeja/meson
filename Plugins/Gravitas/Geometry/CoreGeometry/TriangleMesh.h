#pragma once

#include "GrvGeometry.h"

#include "TriangleMeshNode.h"

Meson_Gravitas_Geometry_BEGIN

class TriangleMesh :
	public IGeometry
{
private: // private variables
	short m_nId;
	uint m_unMaximumBVHDepth;
	uint m_unMinimumTrianglesPerNode;
	Meson::Common::Text::String m_strWavefrontModelFilename;

private: // private methods
	TriangleMeshNode* PartitionTriangles(
		uint p_unLevel,
		const PointList* p_listVertices,
		const IndexedTriangleList& p_listTriangles) const;

public: // public variables
	PointArrayList Vertices;
	TriangleMeshNode* Root;

public: // public methods
	TriangleMesh(void);
	TriangleMesh(const Meson::Common::Text::String& p_strId);
	~TriangleMesh(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String,
		PropertyDescriptor>& p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, int& p_nValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Text::String& p_strValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList<int>& p_listValues) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList< Meson::Common::Maths::TPoint3<Real> >& p_listValues) const;
	void SetProperty(const Meson::Common::Text::String& p_strName, int p_nValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Text::String& p_strValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList<int>& p_listValues);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList< Meson::Common::Maths::TPoint3<Real> >& p_listValues);

	void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice);

	// IGeometry methods
	const Meson::Common::Text::String& GetTypeName(void) const;
	short GetTypeId(void) const;
	void SetTypeId(short p_nId);
	Real GetVolume(void) const;
	Meson::Common::Maths::TMatrix3<Real> GetDistributionTensor(void) const;

	bool IsBounded(void) const;
	void ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const;
	void ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const;
	void ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const;

	bool IntersectsRay(const Ray& p_ray) const;
	bool IntersectsRay(const Ray& p_ray,
		Meson::Common::Maths::TPoint3<Real>& p_ptIntersectionPoint) const;

	// custom interface
	Meson::Common::Maths::TPoint3<Real> GetCentroid(void) const;
};

Meson_Gravitas_Geometry_END
