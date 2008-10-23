#pragma once

#include "ArrayList.h"
#include "Matrix4.h"
#include "Interval.h"

#include "GrvGeometry.h"
#include "GrvTransform.h"
#include "PolyhedronFace.h"
#include "PolyhedronEdge.h"

Meson_Gravitas_Geometry_BEGIN

class Polyhedron
	: public IGeometry
{
private: // private variables
	short m_nId;

public: // public variables
	Meson::Common::Collections::TArrayList<Meson::Common::Maths::TPoint3<Real>> m_listVertices;
	Meson::Common::Collections::TArrayList<PolyhedronFace> m_listFaces;
	Meson::Common::Collections::TArrayList<PolyhedronEdge> m_listEdges;

private: // private methods
	bool IsConvex(void) const;
	void UpdateNormals(void);
	void UpdateFacesAndEdges(void);
	void ComputeMassPropertySubExpressions(
		Real p_rW0, Real p_rW1, Real p_rW2,
		Real &p_rF1, Real &p_rF2, Real &p_rF3,
		Real &p_rG0, Real &p_rG1, Real &p_rG2) const;

public: // public methods
	Polyhedron(void);
	Polyhedron(const Polyhedron &p_polyhedron);
	Polyhedron &operator=(const Polyhedron &p_polyhedron);

	// IGravitasEngine interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>
			&p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, size_t p_unIndex,
		int &p_nValue);
	void GetProperty(const Meson::Common::Text::String& p_strName, size_t p_unIndex,
		Meson::Common::Maths::TPoint3<Real>& p_ptValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList<int>& p_listValues) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList< Meson::Common::Maths::TPoint3<Real> > &p_listValues) const;
	void SetProperty(const Meson::Common::Text::String& p_strName,
		size_t p_unIndex, int p_nValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		size_t p_unIndex,
		const Meson::Common::Maths::TPoint3<Real>& p_ptValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList<int>& p_listValues);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList< Meson::Common::Maths::TPoint3<Real> >& p_listValues);

	// IGeoemtry interface
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

	void AddFace(size_t p_unVertexIndex0, size_t p_unVertexIndex1, size_t p_unVertexIndex2);
	void AddFace(const size_t *p_unVertexIndices);

	void MakeCube(Real p_rWidth, Real p_rHeight, Real p_rDepth);
	void MakeConvexHull(const Meson::Common::Collections::TList<Meson::Common::Maths::TPoint3<Real>> &p_listPoints, bool p_bCleanup = true);
	void MakeMinkowskiDifference(const Polyhedron &p_polyhedron1, const Polyhedron &p_polyhedron2, bool p_bCleanup = true);

	void EliminateUnusedVertices(void);
	void AlignCentroid(void);

	void Transform(const Meson::Common::Maths::TMatrix4<Real> &p_matTransform);
	Polyhedron TransformCopy(const Meson::Common::Maths::TMatrix4<Real> &p_matTransform) const;
	void Transform(const Meson::Common::Maths::TVector3<Real> &p_vecOffset);
	Polyhedron TransformCopy(const Meson::Common::Maths::TVector3<Real> &p_vecOffset) const;
	void Transform(const Meson::Common::Maths::TQuaternion<Real> &p_qtnRotation);
	Polyhedron TransformCopy(const Meson::Common::Maths::TQuaternion<Real> &p_qtnRotation) const;
	void Transform(const Meson::Gravitas::Geometry::Transform& p_transform);
	Polyhedron TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform) const;

	void ProjectToInterval(
		const Meson::Common::Maths::TVector3<Real> &p_vecAxis,
		Meson::Common::Maths::TInterval<Real> &p_interval) const;

	bool Intersects(const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;
	bool Intersects(const LineSegment &p_lineSegment) const;
	bool IntersectsBoundary(const LineSegment &p_lineSegment) const;
	bool IntersectsBoundary(const LineSegment &p_lineSegment,
		size_t &p_unOccurences,
		Meson::Common::Maths::TPoint3<Real> &p_ptIntersectionPointNear,
		Meson::Common::Maths::TPoint3<Real> &p_ptIntersectionPointFar) const;

	Meson::Common::Maths::TPoint3<Real> ClosestPointTo(
		const Meson::Common::Maths::TPoint3<Real> &p_ptPoint) const;
	Meson::Common::Maths::TPoint3<Real> ClosestPointTo(
		const Meson::Common::Maths::TPoint3<Real> &p_ptPoint,
		size_t &p_unClosestFaceIndex) const;

};

Meson_Gravitas_Geometry_END
