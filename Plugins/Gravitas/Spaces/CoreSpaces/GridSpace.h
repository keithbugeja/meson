#pragma once

#include "ArrayList.h"
#include "HashMap.h"
#include "StopWatch.h"

#include "GrvNamespaces.h"
#include "GrvSpace.h"
#include "GrvBoundingAxisAlignedBox.h"

Meson_Gravitas_Space_BEGIN

struct CellKey
{
	int X, Y, Z;
	CellKey(void);
	CellKey(const CellKey& p_cellKey);
	CellKey(int p_nX, int p_nY, int p_nZ);
	CellKey& operator=(const CellKey& p_cellKey);
	const bool operator==(const CellKey& p_cellKey) const;
	Meson::Common::Maths::TPoint3<Real> ComputePosition(
		const Meson::Common::Maths::TVector3<Real>& p_vecCellSize) const;
};

ulong HashData(const CellKey& p_cellKey);

struct GridCell
{
	Meson::Gravitas::Geometry::BoundingAxisAlignedBox Volume;
	Meson::Gravitas::Body::BodyArrayList Bodies;

	GridCell(void);
	GridCell(const GridCell& gridCell);
	GridCell& operator=(const GridCell& p_gridCell);
	const bool operator==(const GridCell& p_gridCell) const;
	void ComputeCandidateCollisions(
		Meson::Gravitas::Body::BodyPairList& p_listCandidateCollisions);
	const bool operator<(const GridCell& p_gridCell) const;

	void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice);
};

class GridSpace
	: public Meson::Gravitas::Space::ISpace
{
private: // private variables
	Meson::Gravitas::Body::BodyArrayList m_listBodies;
	Meson::Common::Maths::TVector3<Real> m_vecCellSize;
	Meson::Gravitas::Body::BodyArrayList m_listUnboundedBodies;
	Meson::Gravitas::Body::BodyArrayList m_listBoundedBodies;
	Meson::Common::Collections::THashMap<CellKey, GridCell> m_mapCells;
	Meson::Common::System::StopWatch m_stopWatchCellCleaner;
	Real m_rCleanInterval;

private: // private methods
	void GetCellSpan(
		const Meson::Gravitas::Geometry::BoundingVolumePtr p_pBoundingVolume,
		CellKey& p_cellKeyMin, CellKey& p_cellKeyMax);
	void AddBodyToCells(Meson::Gravitas::Body::BodyPtr p_pBody);
	void RemoveBodyFromCells(Meson::Gravitas::Body::BodyPtr p_pBody);
	void IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray, Meson::Gravitas::Body::BodyPtr p_pBody,
		BodyRayIntersectionList& p_listBodyRayIntersections) const;
	void IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray, GridCell& p_gridCell,
		BodyRayIntersectionList& p_listBodyRayIntersections) const;

public: // public methods
	GridSpace(void);
	GridSpace(const Meson::Common::Text::String& p_strId);
	~GridSpace(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>
			&p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, Real& p_rValue) const;
	void SetProperty(const Meson::Common::Text::String& p_strName, Real p_rValue);
	void RenderInstrumentation(
		Meson::Gravitas::Instrumentation::IInstrumentationDevice* p_pInstrumentationDevice);

	// ISpace interface
	Meson::Common::Collections::TEnumerator<Meson::Gravitas::Body::BodyPtr>&
		GetBodies(void);
	Meson::Gravitas::Body::BodyPtr GetBody(
		const Meson::Common::Text::String& p_strId);
	void AddBody(Meson::Gravitas::Body::BodyPtr p_pBody);
	void RemoveBody(Meson::Gravitas::Body::BodyPtr p_pBody);

	void Rebuild(void);
	void Update(void);

	void ComputeCandidateCollisions(
		Meson::Gravitas::Body::BodyPairList& p_listCandidateCollisions);

	void IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
		BodyRayIntersectionList& p_listBodyRayIntersections) const;
};

Meson_Gravitas_Space_END
