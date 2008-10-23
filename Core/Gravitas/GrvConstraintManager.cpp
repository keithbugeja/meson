#include "GrvConstraintManager.h"

#include "DebugSupport.h"
#include "HashMap.h"
#include "FileInputStream.h"

#include "GrvLexer.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Gravitas::Constraint;

Meson_Gravitas_Constraint_BEGIN

struct ConstraintManagerState
{
	THashMap<String, IConstraintFactory* > m_mapConstraintFactories;
	THashMap<String, IConstraintSolver* > m_mapConstraintSolvers;

	ConstraintManagerState(void)
		: m_mapConstraintFactories()
		, m_mapConstraintSolvers()
	{}
};

Meson_Gravitas_Constraint_END

ConstraintManager::ConstraintManager()
	: m_pConstraintManagerState(NULL)
{
	m_pConstraintManagerState = new ConstraintManagerState();
}

ConstraintManager::~ConstraintManager()
{
	delete m_pConstraintManagerState;
}

TEnumerator<IConstraintFactory*>& ConstraintManager::GetConstraintFactories(void)
{
	return m_pConstraintManagerState->m_mapConstraintFactories.GetValueEnumerator();
}

IConstraintFactory* ConstraintManager::GetConstraintFactory(const String& p_strName)
{
	IConstraintFactory* pConstraintFactory;
	if (!m_pConstraintManagerState->m_mapConstraintFactories.ContainsKey(p_strName, pConstraintFactory))
		throw new MesonException(
			"Constraint factory '" + p_strName + "' not registered.",
			__FILE__, __LINE__);

	return pConstraintFactory;
}

void ConstraintManager::RegisterConstraintFactory(IConstraintFactory *p_pConstraintFactory)
{
	THashMap<String, IConstraintFactory *> &mapConstraintFactories
		= m_pConstraintManagerState->m_mapConstraintFactories;

	const String &strName = p_pConstraintFactory->GetName();

	if (mapConstraintFactories.ContainsKey(strName))
		throw new MesonException(
			"Constraint factory '" + strName + "' already registered.",
			__FILE__, __LINE__);

	mapConstraintFactories[strName] = p_pConstraintFactory;
}

void ConstraintManager::UnregisterConstraintFactory(IConstraintFactory *p_pConstraintFactory)
{
	THashMap<String, IConstraintFactory *> &mapConstraintFactories
		= m_pConstraintManagerState->m_mapConstraintFactories;

	const String &strName = p_pConstraintFactory->GetName();

	if (!mapConstraintFactories.ContainsKey(strName))
		throw new MesonException(
			"Constraint factory '" + strName + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapConstraintFactories.RemoveKey(strName);
}

TEnumerator<IConstraintSolver*>& ConstraintManager::GetConstraintSolvers(void)
{
	return m_pConstraintManagerState->m_mapConstraintSolvers.GetValueEnumerator();
}

IConstraintSolver* ConstraintManager::GetConstraintSolver(const String& p_strName)
{
	IConstraintSolver* pConstraintSolver;
	if (!m_pConstraintManagerState->m_mapConstraintSolvers.ContainsKey(p_strName, pConstraintSolver))
		throw new MesonException(
			"Constraint solver '" + p_strName + "' not registered.",
			__FILE__, __LINE__);

	return pConstraintSolver;
}

void ConstraintManager::RegisterConstraintSolver(IConstraintSolver* p_pConstraintSolver)
{
	THashMap<String, IConstraintSolver *> &mapConstraintSolvers
		= m_pConstraintManagerState->m_mapConstraintSolvers;

	const String &strName = p_pConstraintSolver->GetName();

	if (mapConstraintSolvers.ContainsKey(strName))
		throw new MesonException(
			"Constraint solver '" + strName + "' already registered.",
			__FILE__, __LINE__);

	mapConstraintSolvers[strName] = p_pConstraintSolver;
}

void ConstraintManager::UnregisterConstraintSolver(IConstraintSolver* p_pConstraintSolver)
{
	THashMap<String, IConstraintSolver *> &mapConstraintSolvers
		= m_pConstraintManagerState->m_mapConstraintSolvers;

	const String &strName = p_pConstraintSolver->GetName();

	if (!mapConstraintSolvers.ContainsKey(strName))
		throw new MesonException(
			"Constraint solver '" + strName + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapConstraintSolvers.RemoveKey(strName);
}

void ConstraintManager::UnregisterAllConstraintSolvers(void)
{
	m_pConstraintManagerState->m_mapConstraintSolvers.Clear();
}

void ConstraintManager::LoadConstraintSolver(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'ConstraintSolver' container identifier
	LexerToken lexerToken = lexer.ReadToken(LexerTokenType::Identifier);

	if (lexerToken.ValueString != "ConstraintSolver")
		throw new MesonException(
			"Parsing Error: Expected identifier 'ConstraintSolver'",
			__FILE__, __LINE__);

	// 'ConstraintSolver' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	// mandatory Type property
	lexerToken = lexer.ReadToken(LexerTokenType::Identifier);
	if (lexerToken.ValueString != "Type")
		throw new MesonException(
			"Parsing Error: Expected identifier 'Type'",
			__FILE__, __LINE__);

	lexer.ReadToken(LexerTokenType::Assignment);

	lexerToken = lexer.ReadToken(LexerTokenType::String);
	String strConstraintSolverType = lexerToken.ValueString;

	// get the constraint solver for the required type
	IConstraintSolver* pConstraintSolver
		= GetConstraintSolver(strConstraintSolverType);

	// get custom properties and configure solver
	lexer.ReadCustomProperties(*pConstraintSolver);
}

void ConstraintManager::LoadConstraintSolvers(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'ConstraintSolvers' container identifier
	LexerToken lexerToken = lexer.ReadToken(LexerTokenType::Identifier);
	if (lexerToken.ValueString != "ConstraintSolvers")
		throw new MesonException(
			"Parsing Error: Expected identifier 'ConstraintSolvers'",
			__FILE__, __LINE__);

	// 'ConstraintSolvers' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	while (true)
	{
		lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			return;

		if (lexerToken.Type != LexerTokenType::Identifier
			|| lexerToken.ValueString != "ConstraintSolver")
			throw new MesonException(
				"Parsing Error: Expected identifier 'ConstraintSolver'",
				__FILE__, __LINE__);

		p_pLookaheadInputStream->Pushback("ConstraintSolver", 16);
		LoadConstraintSolver(p_pLookaheadInputStream);
	}
}

void ConstraintManager::LoadConstraintSolvers(const String& p_strFilename)
{
	InputStreamPtr pInputStream(new FileInputStream(p_strFilename));
	LookaheadInputStreamPtr pLookaheadInputStream(
		new LookaheadInputStream(pInputStream));
	LoadConstraintSolvers(pLookaheadInputStream);
}
