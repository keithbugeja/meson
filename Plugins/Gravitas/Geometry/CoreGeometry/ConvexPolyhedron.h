#pragma once

#include "ArrayList.h"
#include "Point3.h"
#include "Interval.h"

#include "GrvGeometry.h"

#include "ConvexPolyhedronEdge.h"
#include "ConvexPolyhedronFace.h"

Meson_Gravitas_Geometry_BEGIN

class ConvexPolyhedron :
	public IGeometry
{
private: // private variables
	static Meson::Common::Text::String s_strTypeName;
	short m_nTypeId;
	short m_nFaceIndex;
	Meson::Common::Text::String m_strWavefrontModelFilename;

private: // private methods
	void ComputeMassPropertySubExpressions(
		Real p_rW0, Real p_rW1, Real p_rW2,
		Real &p_rF1, Real &p_rF2, Real &p_rF3,
		Real &p_rG0, Real &p_rG1, Real &p_rG2) const;
	bool IsConvex(Real p_rCoplanarTollerance) const;

public: // public variables
	PointArrayList Vertices;
	Meson::Common::Collections::TArrayList<ConvexPolyhedronEdge> Edges;
	Meson::Common::Collections::TArrayList<ConvexPolyhedronFace> Faces;

public: // public methods
	ConvexPolyhedron(void);
	ConvexPolyhedron(const Meson::Common::Text::String& p_strId);
	ConvexPolyhedron(const ConvexPolyhedron& p_convexPolyhedron);
	~ConvexPolyhedron(void);

	// IConfigurable methods
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>
			&p_mapProperties) const;

	void GetProperty(const Meson::Common::Text::String& p_strName, bool& p_bValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, int& p_nValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Text::String& p_strValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, size_t p_unIndex,
		Meson::Common::Maths::TPoint3<Real>& p_ptValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Collections::TList<int> &p_listValues) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		PointList& p_listValues) const;

	void SetProperty(const Meson::Common::Text::String& p_strName, bool p_bValue);
	void SetProperty(const Meson::Common::Text::String& p_strName, int p_nValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Text::String& p_strValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		size_t p_unIndex,
		const Meson::Common::Maths::TPoint3<Real>& p_ptValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Collections::TList<int>& p_listValues);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const PointList& p_listValues);

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

	// custom methods
	void UpdateDerivativeData(bool p_bAlignCentroid = false);
	Meson::Common::Maths::TPoint3<Real> GetCentroid(void) const;
	void AlignCentroid(void);

	void ProjectToInterval(
		const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
		Meson::Common::Maths::TInterval<Real>& p_interval) const;
	void EnumerateMaximalVertices(
		const Meson::Common::Maths::TVector3<Real>& p_vecDirection,
		Meson::Gravitas::PointList& p_listVertices) const;

	bool ContainsPoint(const Meson::Common::Maths::TPoint3<Real>& ptPoint) const;
	Meson::Common::Maths::TPoint3<Real> GetClosestPoint(
		const Meson::Common::Maths::TPoint3<Real>& ptPoint) const;

	ConvexPolyhedron TransformCopy(
		const Meson::Gravitas::Geometry::Transform& p_transform) const;
	void TransformCopy(
		const Meson::Gravitas::Geometry::Transform& p_transform,
		ConvexPolyhedron& p_convexPolyhedron) const;

	ConvexPolyhedron& operator=(const ConvexPolyhedron& p_convexPolyhedron);
};

Meson_Gravitas_Geometry_END
