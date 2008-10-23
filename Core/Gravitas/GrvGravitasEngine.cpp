//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::GravitasEngine
//	Gravitas physics engine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		09/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Memory;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Event;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Materials;
using namespace Meson::Gravitas::Simulation;
using namespace Meson::Gravitas::Space;

Meson_Gravitas_BEGIN

MemoryManager* GetGravitasMemoryManager(void)
{
	MemoryManager* pMemoryManagerRoot = GetRootMemoryManager();
	static MemoryManager s_memoryManagerGravitas
		= MemoryManager("Gravitas", pMemoryManagerRoot, 0, 0);
	return &s_memoryManagerGravitas;
}

struct GravitasEngineState
{
	String m_strName;
	Version m_version;
	CollisionManager* m_pCollisionManager;
	ConstraintManager* m_pConstraintManager;
	EventManager* m_pEventManager;
	ForceManager* m_pForceManager;
	GeometryManager* m_pGeometryManager;
	InstrumentationManager* m_pInstrumentationManager;
	KineticsManager* m_pKineticsManager;
	MaterialManager* m_pMaterialManager;
	SimulationManager* m_pSimulationManager;
	SpaceManager* m_pSpaceManager;

	GravitasEngineState(void)
		: m_strName("GravitasEngine")
		, m_version(0,0,1)
		, m_pCollisionManager(NULL)
		, m_pConstraintManager(NULL)
		, m_pEventManager(NULL)
		, m_pForceManager(NULL)
		, m_pGeometryManager(NULL)
		, m_pInstrumentationManager(NULL)
		, m_pKineticsManager(NULL)
		, m_pMaterialManager(NULL)
		, m_pSimulationManager(NULL)
		, m_pSpaceManager(NULL)
	{
		m_pCollisionManager = CollisionManager::GetInstance();
		m_pConstraintManager = ConstraintManager::GetInstance();
		m_pEventManager = EventManager::GetInstance();
		m_pForceManager = ForceManager::GetInstance();
		m_pGeometryManager = GeometryManager::GetInstance();
		m_pInstrumentationManager = InstrumentationManager::GetInstance();
		m_pKineticsManager = KineticsManager::GetInstance();
		m_pMaterialManager = MaterialManager::GetInstance();
		m_pSimulationManager = SimulationManager::GetInstance();
		m_pSpaceManager = SpaceManager::GetInstance();
	}

	~GravitasEngineState(void)
	{
		m_pSpaceManager->Delete();
		m_pSimulationManager->Delete();
		m_pMaterialManager->Delete();
		m_pKineticsManager->Delete();
		m_pInstrumentationManager->Delete();
		m_pGeometryManager->Delete();
		m_pForceManager->Delete();
		m_pEventManager->Delete();
		m_pConstraintManager->Delete();
		m_pCollisionManager->Delete();
	}
};

Meson_Gravitas_END

GravitasEngine::GravitasEngine()
	: m_pGravitasEngineState(NULL)
{
	m_pGravitasEngineState = new GravitasEngineState();
}

GravitasEngine::~GravitasEngine() 
{
	delete m_pGravitasEngineState;
}

const String &GravitasEngine::GetName(void)
{
	return m_pGravitasEngineState->m_strName;
}

const Version &GravitasEngine::GetVersion(void)
{
	return m_pGravitasEngineState->m_version;
}

CollisionManager* GravitasEngine::GetCollisionManager(void)
{
	return m_pGravitasEngineState->m_pCollisionManager;
}

ConstraintManager* GravitasEngine::GetConstraintManager(void)
{
	return m_pGravitasEngineState->m_pConstraintManager;
}

EventManager* GravitasEngine::GetEventManager(void)
{
	return m_pGravitasEngineState->m_pEventManager;
}

ForceManager* GravitasEngine::GetForceManager(void)
{
	return m_pGravitasEngineState->m_pForceManager;
}

GeometryManager* GravitasEngine::GetGeometryManager(void)
{
	return m_pGravitasEngineState->m_pGeometryManager;
}

InstrumentationManager* GravitasEngine::GetInstrumentationManager(void)
{
	return m_pGravitasEngineState->m_pInstrumentationManager;
}

KineticsManager* GravitasEngine::GetKineticsManager(void)
{
	return m_pGravitasEngineState->m_pKineticsManager;
}

MaterialManager* GravitasEngine::GetMaterialManager(void)
{
	return m_pGravitasEngineState->m_pMaterialManager;
}

SimulationManager* GravitasEngine::GetSimulationManager(void)
{
	return m_pGravitasEngineState->m_pSimulationManager;
}

SpaceManager* GravitasEngine::GetSpaceManager(void)
{
	return m_pGravitasEngineState->m_pSpaceManager;
}
