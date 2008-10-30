#pragma once

#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

class TriangleMesh;

struct IndexedTriangle
{
	size_t VertexIndices[3];
	Meson::Common::Maths::TVector3<Real> Normal;

	bool operator==(const IndexedTriangle& p_indexedTriangle) const;

	Real GetArea(const VectorList* p_plistVertices) const;
	Meson::Common::Maths::TVector3<Real> GetCentroid(const VectorList* p_plistVertices) const;

	bool Intersects(
		const VectorList* p_plistVertices,
		const BoundingAxisAlignedBox& boundingAxisAlignedBox) const;
	bool IntersectsRay(
		const VectorList* p_plistVertices, const Ray& p_ray) const;
	bool IntersectsRay(
		const VectorList* p_plistVertices, const Ray& p_ray,
		Meson::Common::Maths::TVector3<Real>& p_ptIntersectionPoint) const;

	void UpdateNormal(const VectorList* p_plistVertices);
};

typedef Meson::Common::Collections::TList<IndexedTriangle> IndexedTriangleList;
typedef Meson::Common::Collections::TArrayList<IndexedTriangle> IndexedTriangleArrayList;

struct TriangleMeshNode
{
	const VectorList* Vertices;
	BoundingAxisAlignedBox BoundingVolume;
	IndexedTriangleArrayList Triangles;
	TriangleMeshNode* Child[2];

	TriangleMeshNode(void);
	~TriangleMeshNode(void);

	size_t GetTreeDepth(void) const;
	size_t GetNodeCount(void) const;
	size_t GetTriangleCount(void) const;

	Real GetVolume(void) const;
	Real GetMomentOfInertia(const Meson::Common::Maths::TVector3<Real>& p_vecAxis) const;
	Meson::Common::Maths::TVector3<Real> GetCentroid(void) const;

	bool IntersectsRay(const Ray& p_ray) const;
	bool IntersectsRay(const Ray& p_ray,
		Meson::Common::Maths::TVector3<Real>& p_vecIntersectionPoint) const;

	void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice);
};

Meson_Gravitas_Geometry_END
