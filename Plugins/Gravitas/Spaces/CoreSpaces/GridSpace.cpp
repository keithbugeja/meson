#include "GridSpace.h"

#include "TreeSet.h"
#include "Sorting.h"
#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Space;

CellKey::CellKey(void)
	: X(0)
	, Y(0)
	, Z(0)
{
}

CellKey::CellKey(const CellKey& p_cellKey)
	: X(p_cellKey.X)
	, Y(p_cellKey.Y)
	, Z(p_cellKey.Z)
{
}

CellKey::CellKey(int p_nX, int p_nY, int p_nZ)
	: X(p_nX)
	, Y(p_nY)
	, Z(p_nZ)
{
}

CellKey& CellKey::operator=(const CellKey& p_cellKey)
{
	X = p_cellKey.X;
	Y = p_cellKey.Y;
	Z = p_cellKey.Z;
	return *this;
}

const bool CellKey::operator==(const CellKey& p_cellKey) const
{
	if (X != p_cellKey.X) return false;
	if (Y != p_cellKey.Y) return false;
	if (Z != p_cellKey.Z) return false;
	return true;
}

Meson::Common::Maths::TVector3<Real> CellKey::ComputePosition(
	const TVector3<Real>& p_vecCellSize) const
{
	return TVector3<Real>(
		X * p_vecCellSize.X,
		Y * p_vecCellSize.Y,
		Z * p_vecCellSize.Z);
}

Meson_Gravitas_Space_BEGIN

ulong HashData(const CellKey& p_cellKey)
{
	return (ulong) p_cellKey.X * 0x8DA6B343 + p_cellKey.Y * 0xD8163841 + p_cellKey.Z * 0xCB1AB31F;
}

Meson_Gravitas_Space_END

GridCell::GridCell(void)
	: Volume()
	, Bodies()
{
}

GridCell::GridCell(const GridCell& p_gridCell)
	: Volume(p_gridCell.Volume)
	, Bodies(p_gridCell.Bodies)
{
}

GridCell& GridCell::operator=(const GridCell& p_gridCell)
{
	Volume = p_gridCell.Volume;
	Bodies = p_gridCell.Bodies;
	return *this;
}

const bool GridCell::operator==(const GridCell& p_gridCell) const
{
	if (Volume != p_gridCell.Volume) return false;
	if (Bodies != p_gridCell.Bodies) return false;
	return true;
}

void GridCell::ComputeCandidateCollisions(
	BodyPairList& p_listCandidateCollisions)
{
	p_listCandidateCollisions.Clear();

	BodyPtr pBody1, pBody2;
	size_t unBodies = Bodies.Size();
	for (size_t unIndex1 = 0; unIndex1 < unBodies; unIndex1++)
	{
		pBody1 = Bodies[unIndex1];
		bool bInfiniteMass1 = pBody1->GetMassProperties().HasInfiniteMass();

		for (size_t unIndex2 = unIndex1 + 1; unIndex2 < unBodies; unIndex2++)
		{
			pBody2 = Bodies[unIndex2];

			// ignore potential collision between bodies both of infinite mass
			if (bInfiniteMass1 && pBody2->GetMassProperties().HasInfiniteMass())
				continue;

			// ignore potential collision between bodies if both inactive
			if (!pBody1->GetKineticProperties().Active
				&& !pBody2->GetKineticProperties().Active)
				continue;

			if (pBody1 < pBody2)
				p_listCandidateCollisions.Add(BodyPair(pBody1, pBody2));
			else
				p_listCandidateCollisions.Add(BodyPair(pBody2, pBody1));
		}
	}
}

void GridCell::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	p_pInstrumentationDevice->DrawBox(Volume.Centre(), Volume.Extent());
}

void GridSpace::GetCellSpan(
	const BoundingVolumePtr p_pBoundingVolume,
	CellKey& p_cellKeyMin, CellKey& p_cellKeyMax)
{
	TInterval<Real> intervalX, intervalY, intervalZ;
	p_pBoundingVolume->ProjectToInterval(TVector3<Real>::Right, intervalX);
	p_pBoundingVolume->ProjectToInterval(TVector3<Real>::Up, intervalY);
	p_pBoundingVolume->ProjectToInterval(TVector3<Real>::Out, intervalZ);

	p_cellKeyMin.X = (int) TMaths<Real>::Floor(intervalX.Min / m_vecCellSize.X);
	p_cellKeyMin.Y = (int) TMaths<Real>::Floor(intervalY.Min / m_vecCellSize.Y);
	p_cellKeyMin.Z = (int) TMaths<Real>::Floor(intervalZ.Min / m_vecCellSize.Z);

	p_cellKeyMax.X = (int) TMaths<Real>::Floor(intervalX.Max / m_vecCellSize.X);
	p_cellKeyMax.Y = (int) TMaths<Real>::Floor(intervalY.Max / m_vecCellSize.Y);
	p_cellKeyMax.Z = (int) TMaths<Real>::Floor(intervalZ.Max / m_vecCellSize.Z);
}

void GridSpace::AddBodyToCells(BodyPtr p_pBody)
{
	BoundingVolumePtr pBoundingVolume(p_pBody->GetBoundingVolume());
	if (pBoundingVolume == NULL) return;

	CellKey cellKeyMin, cellKeyMax;
	GetCellSpan(pBoundingVolume, cellKeyMin, cellKeyMax);

	CellKey cellKey;
	for (int nIndexZ = cellKeyMin.Z; nIndexZ <= cellKeyMax.Z; nIndexZ++)
	{
		cellKey.Z = nIndexZ;
		for (int nIndexY = cellKeyMin.Y; nIndexY <= cellKeyMax.Y; nIndexY++)
		{
			cellKey.Y = nIndexY;
			for (int nIndexX = cellKeyMin.X; nIndexX <= cellKeyMax.X; nIndexX++)
			{
				cellKey.X = nIndexX;
				GridCell& gridCell = m_mapCells[cellKey];

				gridCell.Volume.Min = cellKey.ComputePosition(m_vecCellSize);
				gridCell.Volume.Max = gridCell.Volume.Min + m_vecCellSize;

				if (gridCell.Bodies.IndexOf(p_pBody) < 0)
					gridCell.Bodies.Add(p_pBody);
			}
		}
	}
}

void GridSpace::RemoveBodyFromCells(BodyPtr p_pBody)
{
	TArrayList<CellKey> listEmptyCells;

	for(TEnumerator< TKeyValuePair<CellKey, GridCell> >& enumPairs = m_mapCells.GetEnumerator();
		enumPairs.HasMoreElements();)
	{
		TKeyValuePair<CellKey, GridCell>& keyValuePair = enumPairs.NextElement();
		GridCell& gridCell = keyValuePair.Value;

		gridCell.Bodies.Remove(p_pBody);

		// track cells that have become empty
		if (gridCell.Bodies.Size() == 0)
			listEmptyCells.Add(keyValuePair.Key);
	}

	// remove empty cells
	size_t unEmptyCells = listEmptyCells.Size();
	for (size_t unIndex = 0; unIndex < unEmptyCells; unIndex++)
		m_mapCells.RemoveKey(listEmptyCells(unIndex));
}

void GridSpace::IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray, BodyPtr p_pBody,
	BodyRayIntersectionList& p_listBodyRayIntersections) const
{
	TVector3<Real> vecIntersection;
	Transform trnWorld, trnLocal;

	// prepare world and local transforms
	KineticProperties& kineticProperties = p_pBody->GetKineticProperties();
	trnWorld.Translation = kineticProperties.Position;
	trnWorld.Rotation = kineticProperties.Orientation;
	trnLocal = trnWorld.InvertCopy();

	// transform ray into local geometry space
	Ray rayLocal(p_ray);
	rayLocal.Transform(trnLocal);

	// do local ray cast but skip on failure
	GeometryPtr pGeometry(p_pBody->GetGeometry());
	if (!pGeometry->IntersectsRay(rayLocal, vecIntersection))
		return;
	
	// transform local intersection point to world coords
	trnWorld.Apply(vecIntersection);

	// avoid duplicate due to overlap across cells
	size_t unListCount = p_listBodyRayIntersections.Size();
	for (size_t unListIndex = 0; unListIndex < unListCount; unListIndex++)
		if (p_listBodyRayIntersections[unListIndex].Body == p_pBody)
			return;

	// insert in sorted order
	BodyRayIntersection bodyRayIntersection(p_ray, p_pBody, vecIntersection);
	for (size_t unListIndex = 0; unListIndex < unListCount; unListIndex++)
	{
		// find index for body further away from ray and insert before it
		if (p_listBodyRayIntersections[unListIndex]
			>= bodyRayIntersection)
		{
			p_listBodyRayIntersections.Insert(unListIndex,
				bodyRayIntersection);
			return;
		}
	}

	// if this stage reached, body is furthest from ray
	p_listBodyRayIntersections.Add(
		bodyRayIntersection);
}

void GridSpace::IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray, GridCell& p_gridCell,
	BodyRayIntersectionList& p_listBodyRayIntersections) const
{
	Transform trnWorld, trnLocal;
	size_t unBodyCount = p_gridCell.Bodies.Size();
	for (size_t unBodyIndex = 0; unBodyIndex < unBodyCount; unBodyIndex++)
	{
		BodyPtr pBody = p_gridCell.Bodies[unBodyIndex];

		// if bounding volume available, do quick cull test
		BoundingVolumePtr pBoundingVolume = pBody->GetBoundingVolume();
		if (pBoundingVolume != NULL && !pBoundingVolume->Intersects(p_ray))
			continue;

		IntersectRay(p_ray, pBody, p_listBodyRayIntersections);
	}
}

GridSpace::GridSpace(void)
	: m_listBodies()
	, m_vecCellSize((Real) 1.0, (Real) 1.0, (Real) 1.0)
	, m_listBoundedBodies()
	, m_listUnboundedBodies()
	, m_mapCells()
	, m_stopWatchCellCleaner()
	, m_rCleanInterval((Real) 2.0)
{
	m_stopWatchCellCleaner.Reset();
}

GridSpace::GridSpace(const Meson::Common::Text::String& p_strId)
	: ISpace(p_strId)
	, m_listBodies()
	, m_vecCellSize((Real) 1.0, (Real) 1.0, (Real) 1.0)
	, m_listBoundedBodies()
	, m_listUnboundedBodies()
	, m_mapCells()
	, m_stopWatchCellCleaner()
	, m_rCleanInterval((Real) 2.0)
{
	m_stopWatchCellCleaner.Reset();
}

GridSpace::~GridSpace(void)
{
}

void GridSpace::EnumerateProperties(
	TMap<String, PropertyDescriptor>&
		p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties.Insert("CleanInterval",
		PropertyDescriptor("CleanInterval", PropertyType::Real, false));
}

void GridSpace::GetProperty(const Meson::Common::Text::String& p_strName, Real& p_rValue) const
{
	if (p_strName == "CleanInterval")
		p_rValue = m_rCleanInterval;
	else
		throw new MesonException("Grid Space: Invalid property specified.", __FILE__, __LINE__);
}

void GridSpace::SetProperty(const Meson::Common::Text::String& p_strName, Real p_rValue)
{
	if (p_strName == "CleanInterval")
		m_rCleanInterval = TMaths<Real>::Max(p_rValue, (Real) 0.0);
	else
		throw new MesonException("Grid Space: Invalid property specified.", __FILE__, __LINE__);
}

void GridSpace::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	for(TEnumerator<GridCell>& enumCells = m_mapCells.GetValueEnumerator();
		enumCells.HasMoreElements();)
	{
		GridCell& gridCell = enumCells.NextElement();
		gridCell.RenderInstrumentation(p_pInstrumentationDevice);
	}
}

TEnumerator<BodyPtr>& GridSpace::GetBodies(void)
{
	return m_listBodies.GetEnumerator();
}

BodyPtr GridSpace::GetBody(const String& p_strId)
{
	for (TEnumerator<BodyPtr>& enumBodies = m_listBodies.GetEnumerator();
		enumBodies.HasMoreElements();)
	{
		BodyPtr pBody = enumBodies.NextElement();
		if (pBody->GetId() == p_strId)
			return pBody;
	}

	throw new MesonException(
		"No body with ID '" + p_strId + "' is contained in the space.",
		__FILE__, __LINE__);
}

void GridSpace::AddBody(BodyPtr p_pBody)
{
	AddBodyToCells(p_pBody);

	if (m_listBodies.IndexOf(p_pBody) < 0)
		m_listBodies.Add(p_pBody);
}

void GridSpace::RemoveBody(BodyPtr p_pBody)
{
	m_listBodies.Remove(p_pBody);
	RemoveBodyFromCells(p_pBody);
}

void GridSpace::Rebuild(void)
{
	m_listBoundedBodies.Clear();
	m_listUnboundedBodies.Clear();

	m_mapCells.Clear();

	size_t unBodies = m_listBodies.Size();
	if (unBodies == 0) return;
	
	for (size_t unIndex = 0; unIndex < unBodies; unIndex++)
	{
		BodyPtr pBody = m_listBodies[unIndex];
		if (pBody->GetBoundingVolume() == NULL)
			m_listUnboundedBodies.Add(pBody);
		else
		{
			m_listBoundedBodies.Add(pBody);
			AddBodyToCells(pBody);
		}
	}
}

void GridSpace::Update(void)
{
	size_t unBodies = m_listBodies.Size();
	if (unBodies == 0) return;

	m_listBoundedBodies.Clear();
	m_listUnboundedBodies.Clear();

	// process active bodies
	BodyPtr pBody;
	for (size_t unIndex = 0; unIndex < unBodies; unIndex++)
	{
		pBody = m_listBodies[unIndex];

		if (pBody->GetBoundingVolume() == NULL)
			m_listUnboundedBodies.Add(pBody);
		else
		{
			m_listBoundedBodies.Add(pBody);
			if (pBody->GetKineticProperties().Active)
				AddBodyToCells(pBody);
		}
	}

	if (m_stopWatchCellCleaner.GetElapsedTime().TotalSeconds() > m_rCleanInterval)
	{
		m_stopWatchCellCleaner.Reset();

		TArrayList<CellKey> listEmptyCells;

		// process all grid cells
		BoundingVolumePtr pBoundingVolume;
		for(TEnumerator< TKeyValuePair<CellKey, GridCell> >& enumPairs = m_mapCells.GetEnumerator();
			enumPairs.HasMoreElements();)
		{
			TKeyValuePair<CellKey, GridCell>& keyValuePair = enumPairs.NextElement();
			CellKey& cellKey = keyValuePair.Key;
			GridCell& gridCell = keyValuePair.Value;

			// process all bodies in cell
			size_t unBodies = gridCell.Bodies.Size();
			for (size_t unIndex = 0; unIndex < unBodies;)
			{
				BodyPtr pBody = gridCell.Bodies(unIndex);
				pBoundingVolume = pBody->GetBoundingVolume();

				// if body still in cell, do nothing
				if (pBoundingVolume->Intersects(gridCell.Volume))
				{
					unIndex++;
					continue;
				}

				// otherwise, remove body if no longer in cell
				unBodies--;
				gridCell.Bodies.RemoveAt(unIndex);
			}

			// track cells that have become empty
			if (gridCell.Bodies.Size() == 0)
				listEmptyCells.Add(keyValuePair.Key);
		}

		// remove empty cells
		size_t unEmptyCells = listEmptyCells.Size();
		for (size_t unIndex = 0; unIndex < unEmptyCells; unIndex++)
			m_mapCells.RemoveKey(listEmptyCells(unIndex));
	}
}

void GridSpace::ComputeCandidateCollisions(
	BodyPairList& p_listCandidateCollisions)
{
	p_listCandidateCollisions.Clear();

	if (m_listBodies.Size() == 0) return;

	// compute collisions in cells
	TTreeSet<BodyPair> setBodyPairs;
	BodyPairArrayList p_listCandidateCellCollisions;
	for(TEnumerator<GridCell>& enumCells = m_mapCells.GetValueEnumerator();
		enumCells.HasMoreElements();)
	{
		GridCell& gridCell = enumCells.NextElement();
		if (gridCell.Bodies.Size() == 0) continue;

		gridCell.ComputeCandidateCollisions(p_listCandidateCellCollisions);
		setBodyPairs.InsertEnumerator(p_listCandidateCellCollisions.GetEnumerator());
	}

	for(TEnumerator<BodyPair>& enumPairs = setBodyPairs.GetEnumerator();
		enumPairs.HasMoreElements();)
		p_listCandidateCollisions.Add(enumPairs.NextElement());

	// do nothing else if there are no unbounded bodies
	size_t unUnboundedSize = m_listUnboundedBodies.Size();
	if (unUnboundedSize == 0)
		return;

	// compute collision candidates within unbounded list
	for (size_t unIndex1 = 0; unIndex1 < unUnboundedSize; unIndex1++)
	{
		BodyPtr pBody1 = m_listUnboundedBodies[unIndex1];
		bool bInfiniteMass1 = pBody1->GetMassProperties().HasInfiniteMass();
		bool bActive1(pBody1->GetKineticProperties().Active);
		for (size_t unIndex2 = unIndex1 + 1; unIndex2 < unUnboundedSize; unIndex2++)
		{
			BodyPtr pBody2 = m_listUnboundedBodies[unIndex2];

			if (bInfiniteMass1 && pBody2->GetMassProperties().HasInfiniteMass())
				continue;

			if (!bActive1 && !pBody2->GetKineticProperties().Active)
				continue;

			p_listCandidateCollisions.Add(BodyPair(pBody1, pBody2));
		}
	}

	size_t unBoundedSize = m_listBoundedBodies.Size();
	if (unBoundedSize == 0)
		return;

	// compute bounded with unbounded body collision candidates
	for (size_t unUnboundedIndex = 0; unUnboundedIndex < unUnboundedSize; unUnboundedIndex++)
	{
		BodyPtr pBodyUnbounded = m_listUnboundedBodies[unUnboundedIndex];
		bool bInfiniteMassUnbounded = pBodyUnbounded->GetMassProperties().HasInfiniteMass();
		bool bUnboundedActive(pBodyUnbounded->GetKineticProperties().Active);
		for (size_t unBoundedIndex = 0; unBoundedIndex < unBoundedSize; unBoundedIndex++)
		{
			BodyPtr pBodyBounded = m_listBoundedBodies[unBoundedIndex];

			if (bInfiniteMassUnbounded && pBodyBounded->GetMassProperties().HasInfiniteMass())
				continue;

			if (!bUnboundedActive && !pBodyBounded->GetKineticProperties().Active)
				continue;

			p_listCandidateCollisions.Add(BodyPair(pBodyUnbounded, pBodyBounded));
		}
	}
}

void GridSpace::IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
	BodyRayIntersectionList& p_listBodyRayIntersections) const
{
	p_listBodyRayIntersections.Clear();

	TVector3<Real> vecIntersection;
	Transform trnWorld, trnLocal;
	size_t unBodyCount = m_listBodies.Size();
	for (size_t unBodyIndex = 0; unBodyIndex < unBodyCount; unBodyIndex++)
	{
		BodyPtr pBody = m_listBodies[unBodyIndex];

		// if bounding volume available, do quick cull test
		BoundingVolumePtr pBoundingVolume = pBody->GetBoundingVolume();
		if (pBoundingVolume != NULL && !pBoundingVolume->Intersects(p_ray))
			continue;

		// prepare world and local transforms
		KineticProperties& kineticProperties = pBody->GetKineticProperties();
		trnWorld.Translation = kineticProperties.Position;
		trnWorld.Rotation = kineticProperties.Orientation;
		trnLocal = trnWorld.InvertCopy();

		// transform ray into local geometry space
		Ray rayLocal(p_ray);
		rayLocal.Transform(trnLocal);

		// do local ray cast but skip on failure
		GeometryPtr pGeometry(pBody->GetGeometry());
		if (!pGeometry->IntersectsRay(rayLocal, vecIntersection))
			continue;
		
		// transform local intersection point to world coords
		trnWorld.Apply(vecIntersection);

		// insert in sorted order
		BodyRayIntersection bodyRayIntersection(p_ray, pBody, vecIntersection);
		size_t unListCount = p_listBodyRayIntersections.Size();
		int nInsertIndex = 0;
		for (size_t unListIndex = 0; unListIndex < unListCount; unListIndex++)
		{
			if (p_listBodyRayIntersections[unListIndex]
				>= bodyRayIntersection)
			{
				nInsertIndex = (int) unListIndex;
				break;
			}
		}
		if (nInsertIndex >= 0)
			p_listBodyRayIntersections.Insert(nInsertIndex,
				bodyRayIntersection);
		else
			p_listBodyRayIntersections.Add(
				bodyRayIntersection);
	}
}
