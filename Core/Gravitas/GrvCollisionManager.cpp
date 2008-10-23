#include "GrvCollisionManager.h"

#include "DebugSupport.h"
#include "HashMap.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Collision;

Meson_Gravitas_Collision_BEGIN

struct CollisionManagerState
{
	static const short MAX_TYPES = 32;
	ICollisionDetector* m_collisionDetectorMatrix[MAX_TYPES][MAX_TYPES];
	GeometryManager* m_pGeometryManager;
	THashMap<String, ICollisionDetector *> m_mapCollisionDetectors;

	CollisionManagerState(void)
		: m_pGeometryManager(NULL)
		, m_mapCollisionDetectors()
	{
		for (short nIndex1 = 0; nIndex1 < MAX_TYPES; nIndex1++)
			for (short nIndex2 = 0; nIndex2 < MAX_TYPES; nIndex2++)
				m_collisionDetectorMatrix[nIndex1][nIndex2] = NULL;
	}

	String MakeKey(ICollisionDetector* p_pCollisionDetector)
	{
		return p_pCollisionDetector->GetGeometryType1()
			+ "-" + p_pCollisionDetector->GetGeometryType2();
	}
};

Meson_Gravitas_Collision_END

CollisionManager::CollisionManager()
	: m_pCollisionManagerState(NULL)
{
	m_pCollisionManagerState = new CollisionManagerState();
}

CollisionManager::~CollisionManager()
{
	delete m_pCollisionManagerState;
}

TEnumerator<ICollisionDetector*>& CollisionManager::GetCollisionDetectors(void)
{
	return m_pCollisionManagerState->m_mapCollisionDetectors.GetValueEnumerator();
}

bool CollisionManager::HasCollisionDetector(
	IGeometry* p_pGeometry1, IGeometry* p_pGeometry2)
{
	return m_pCollisionManagerState->m_collisionDetectorMatrix[p_pGeometry1->GetTypeId()][p_pGeometry2->GetTypeId()] != NULL;
}

ICollisionDetector* CollisionManager::GetCollisionDetector(
	IGeometry* p_pGeometry1, IGeometry* p_pGeometry2)
{
	return m_pCollisionManagerState->m_collisionDetectorMatrix[p_pGeometry1->GetTypeId()][p_pGeometry2->GetTypeId()];
}

void CollisionManager::RegisterCollisionDetector(ICollisionDetector* p_pCollisionDetector)
{
	if (m_pCollisionManagerState->m_pGeometryManager == NULL)
		m_pCollisionManagerState->m_pGeometryManager
			= GravitasEngine::GetInstance()->GetGeometryManager();

	GeometryManager* pGeometryManager
		= m_pCollisionManagerState->m_pGeometryManager;

	short nGeometryTypeId1 = pGeometryManager->GetGeometryTypeId(
		p_pCollisionDetector->GetGeometryType1());

	short nGeometryTypeId2 = pGeometryManager->GetGeometryTypeId(
		p_pCollisionDetector->GetGeometryType2());

	String strKey = m_pCollisionManagerState->MakeKey(p_pCollisionDetector);
	if (m_pCollisionManagerState->m_collisionDetectorMatrix[nGeometryTypeId1][nGeometryTypeId2] != NULL)
	{
		throw new MesonException(
			"Collision detector '" + strKey + "' already registered.",
			__FILE__, __LINE__);
	}

	// store on map for enumeration
	m_pCollisionManagerState->m_collisionDetectorMatrix[nGeometryTypeId1][nGeometryTypeId2]
		= p_pCollisionDetector;

	THashMap<String, ICollisionDetector*>& mapCollisionDetectors
		= m_pCollisionManagerState->m_mapCollisionDetectors;

	mapCollisionDetectors[strKey] = p_pCollisionDetector;
}

void CollisionManager::UnregisterCollisionDetector(ICollisionDetector* p_pCollisionDetector)
{
	if (m_pCollisionManagerState->m_pGeometryManager == NULL)
		m_pCollisionManagerState->m_pGeometryManager
			= GravitasEngine::GetInstance()->GetGeometryManager();

	GeometryManager* pGeometryManager
		= m_pCollisionManagerState->m_pGeometryManager;

	short nGeometryTypeId1 = pGeometryManager->GetGeometryTypeId(
		p_pCollisionDetector->GetGeometryType1());

	short nGeometryTypeId2 = pGeometryManager->GetGeometryTypeId(
		p_pCollisionDetector->GetGeometryType2());

	String strKey = m_pCollisionManagerState->MakeKey(p_pCollisionDetector);
	if (m_pCollisionManagerState->m_collisionDetectorMatrix[nGeometryTypeId1][nGeometryTypeId2] == NULL)
	{
		throw new MesonException(
			"Collision detector '" + strKey + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);
	}

	m_pCollisionManagerState->m_collisionDetectorMatrix[nGeometryTypeId1][nGeometryTypeId2]
		= NULL;

	// remove from map
	THashMap<String, ICollisionDetector*>& mapCollisionDetectors
		= m_pCollisionManagerState->m_mapCollisionDetectors;
	mapCollisionDetectors.RemoveKey(strKey);
}
