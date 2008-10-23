//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Simulation::SimulationManager
//	Gravitas simulation manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		25/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "GrvSimulationManager.h"

#include "DebugSupport.h"
#include "HashMap.h"

#include "GrvLexer.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Simulation;
using namespace Meson::Gravitas::Space;

Meson_Gravitas_Simulation_BEGIN

struct SimulationManagerState
{
	THashMap<String, ISimulatorFactory *> m_mapSimulatorFactories;
	THashMap<String, SimulatorPtr> m_mapSimulators;
	THashMap<String, EnvironmentPtr> m_mapEnvironments;

	SimulationManagerState(void)
		: m_mapSimulatorFactories()
		, m_mapSimulators()
		, m_mapEnvironments()
	{
	}

	ConstraintPtr LoadConstraint(
		LookaheadInputStreamPtr p_pLookaheadInputStream,
		SpacePtr p_pSpace)
	{
		Lexer lexer(p_pLookaheadInputStream);

		lexer.ReadIdentifier("Constraint");
		lexer.ReadToken(LexerTokenType::OpeningBrace);

		String strConstraintType = lexer.ReadPropertyString("Type");
		String strConstraintId = lexer.ReadPropertyString("Id");

		IConstraintFactory* pConstraintFactory
			= GravitasEngine::GetInstance()->GetConstraintManager()->GetConstraintFactory(
				strConstraintType);

		ConstraintPtr pConstraint = pConstraintFactory->CreateConstraint(strConstraintId);

		String strBody1 = lexer.ReadPropertyString("Body1");
		String strBody2 = lexer.ReadPropertyString("Body2");
		Real rBreakingThreshold = lexer.ReadPropertyReal("BreakingThreshold");

		BodyPtr pBody1 = p_pSpace->GetBody(strBody1);
		BodyPtr pBody2 = p_pSpace->GetBody(strBody2);

		pConstraint->SetBody1(pBody1);
		pConstraint->SetBody2(pBody2);
		pConstraint->SetBreakingThreshold(rBreakingThreshold);

		lexer.ReadCustomProperties(*pConstraint);

		lexer.ReadToken(LexerTokenType::ClosingBrace);

		pConstraint->Bind();

		return pConstraint;
	}

	void LoadConstraints(
		LookaheadInputStreamPtr p_pLookaheadInputStream,
		EnvironmentPtr p_pEnvironment)
	{
		Lexer lexer(p_pLookaheadInputStream);

		lexer.ReadIdentifier("Constraints");

		lexer.ReadToken(LexerTokenType::OpeningBrace);

		while (true)
		{
			LexerToken lexerToken = lexer.ReadToken();

			if (lexerToken.Type == LexerTokenType::ClosingBrace)
				return;

			if (lexerToken.Type != LexerTokenType::Identifier
				|| lexerToken.ValueString != "Constraint")
				throw new MesonException(
					"Parsing Error: Expected identifier 'Constraint'",
					__FILE__, __LINE__);

			p_pLookaheadInputStream->Pushback("Constraint", 10);
			ConstraintPtr pConstraint = LoadConstraint(p_pLookaheadInputStream, p_pEnvironment->Space);
			p_pEnvironment->Constraints->Add(pConstraint);
		}
	}
};

Meson_Gravitas_Simulation_END

SimulationManager::SimulationManager()
	: m_pSimulationManagerState(NULL)
{
	m_pSimulationManagerState = new SimulationManagerState();
}

SimulationManager::~SimulationManager()
{
	delete m_pSimulationManagerState;
}

TEnumerator<ISimulatorFactory*>& SimulationManager::GetSimulatorFactories(void)
{
	return m_pSimulationManagerState->m_mapSimulatorFactories.GetValueEnumerator();
}

ISimulatorFactory* SimulationManager::GetSimulatorFactory(const String& p_strName)
{
	return m_pSimulationManagerState->m_mapSimulatorFactories[p_strName];
}

void SimulationManager::RegisterSimulatorFactory(ISimulatorFactory *p_pSimulatorFactory)
{
	THashMap<String, ISimulatorFactory *> &mapSimulatorFactories
		= m_pSimulationManagerState->m_mapSimulatorFactories;

	const String &strName = p_pSimulatorFactory->GetName();

	if (mapSimulatorFactories.ContainsKey(strName))
		throw new MesonException(
			"Simulator Factory '" + strName + "' already registered.",
			__FILE__, __LINE__);

	mapSimulatorFactories[strName] = p_pSimulatorFactory;
}

void SimulationManager::UnregisterSimulatorFactory(ISimulatorFactory *p_pSimulatorFactory)
{
	THashMap<String, ISimulatorFactory *> &mapSimulatorFactories
		= m_pSimulationManagerState->m_mapSimulatorFactories;

	const String &strName = p_pSimulatorFactory->GetName();

	if (!mapSimulatorFactories.ContainsKey(strName))
		throw new MesonException(
			"Simulator Factory '" + strName + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapSimulatorFactories.RemoveKey(strName);
}

void SimulationManager::UnregisterAllSimulatorFactories(void)
{
	m_pSimulationManagerState->m_mapSimulatorFactories.Clear();
}

TEnumerator<SimulatorPtr>& SimulationManager::GetSimulators(void)
{
	return m_pSimulationManagerState->m_mapSimulators.GetValueEnumerator();
}

SimulatorPtr SimulationManager::GetSimulator(const String& p_strId)
{
	return m_pSimulationManagerState->m_mapSimulators[p_strId];
}

void SimulationManager::RegisterSimulator(SimulatorPtr p_pSimulator)
{
	THashMap<String, SimulatorPtr>& mapSimulators
		= m_pSimulationManagerState->m_mapSimulators;

	const String& strId = p_pSimulator->GetId();

	if (mapSimulators.ContainsKey(strId))
		throw new MesonException(
			"Simulator instance '" + strId + "' already registered.",
			__FILE__, __LINE__);

	mapSimulators[strId] = p_pSimulator;
}

void SimulationManager::UnregisterSimulator(SimulatorPtr p_pSimulator)
{
	THashMap<String, SimulatorPtr>& mapSimulators
		= m_pSimulationManagerState->m_mapSimulators;

	const String& strId = p_pSimulator->GetId();

	if (!mapSimulators.ContainsKey(strId))
		throw new MesonException(
			"Simulator instance '" + strId + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapSimulators.RemoveKey(strId);
}

void SimulationManager::UnregisterAllSimulators(void)
{
	m_pSimulationManagerState->m_mapSimulators.Clear();
}

SimulatorPtr SimulationManager::LoadSimulator(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	lexer.ReadIdentifier("Simulator");

	lexer.ReadToken(LexerTokenType::OpeningBrace);

	String strSimulatorType = lexer.ReadPropertyString("Type");
	ISimulatorFactory* pSimulatorFactory = GetSimulatorFactory(strSimulatorType);

	String strSimulatorId = lexer.ReadPropertyString("Id");

	SimulatorPtr pSimulator = pSimulatorFactory->CreateSimulator(strSimulatorId);

	lexer.ReadCustomProperties(*pSimulator);

	lexer.ReadToken(LexerTokenType::ClosingBrace);

	RegisterSimulator(pSimulator);

	return pSimulator;
}

TEnumerator<EnvironmentPtr>& SimulationManager::GetEnvironments(void)
{
	return m_pSimulationManagerState->m_mapEnvironments.GetValueEnumerator();
}

EnvironmentPtr SimulationManager::GetEnvironment(const String& p_strId)
{
	return m_pSimulationManagerState->m_mapEnvironments[p_strId];
}

void SimulationManager::RegisterEnvironment(EnvironmentPtr p_pEnvironment)
{
	THashMap<String, EnvironmentPtr>& mapEnvironments
		= m_pSimulationManagerState->m_mapEnvironments;

	const String& strId = p_pEnvironment->Id;

	if (mapEnvironments.ContainsKey(strId))
		throw new MesonException(
			"Environment instance '" + strId + "' already registered.",
			__FILE__, __LINE__);

	mapEnvironments[strId] = p_pEnvironment;
}

void SimulationManager::UnregisterEnvironment(EnvironmentPtr p_pEnvironment)
{
	THashMap<String, EnvironmentPtr>& mapEnvironments
		= m_pSimulationManagerState->m_mapEnvironments;

	const String& strId = p_pEnvironment->Id;

	if (!mapEnvironments.ContainsKey(strId))
		throw new MesonException(
			"Environment instance '" + strId + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapEnvironments.RemoveKey(strId);
}

void SimulationManager::UnregisterAllEnvironments(void)
{
	m_pSimulationManagerState->m_mapEnvironments.Clear();
}

EnvironmentPtr SimulationManager::LoadEnvironment(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	lexer.ReadIdentifier("Environment");
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	String strEnvironmentId = lexer.ReadPropertyString("Id");

	EnvironmentPtr pEnvironment(new Environment(strEnvironmentId));

	// space and bodies
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();
	SpacePtr pSpace = pGravitasEngine->GetSpaceManager()->LoadSpace(
		p_pLookaheadInputStream);
	pEnvironment->Space = pSpace;

	// constraints
	pEnvironment->Constraints = new ConstraintArrayList();
	m_pSimulationManagerState->LoadConstraints(p_pLookaheadInputStream, pEnvironment);

	// kinetic integrator
	String strKineticIntegratorId = lexer.ReadPropertyString("KineticIntegrator");
	pEnvironment->KineticIntegrator
		= pGravitasEngine->GetKineticsManager()->GetKineticIntegrator(
			strKineticIntegratorId);

	// constraint solver
	String strConstraintSolverId = lexer.ReadPropertyString("ConstraintSolver");
	pEnvironment->ConstraintSolver
		= pGravitasEngine->GetConstraintManager()->GetConstraintSolver(
			strConstraintSolverId);

	lexer.ReadToken(LexerTokenType::ClosingBrace);

	RegisterEnvironment(pEnvironment);

	return pEnvironment;
}
