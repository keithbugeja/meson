#include "GrvForceManager.h"

#include "DebugSupport.h"
#include "HashMap.h"
#include "FileInputStream.h"

#include "GrvLexer.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Collections;
using namespace Meson::Gravitas::Force;
using namespace Meson::Common::Text;

Meson_Gravitas_Force_BEGIN

struct ForceManagerState
{
	THashMap<String, IForceFactory* > m_mapForceFactories;
	THashMap<String, ForcePtr> m_mapForces;

	ForceManagerState(void)
		: m_mapForceFactories()
		, m_mapForces() {}
};

Meson_Gravitas_Force_END

ForceManager::ForceManager()
	: m_pForceManagerState(NULL)
{
	m_pForceManagerState = new ForceManagerState();
}

ForceManager::~ForceManager()
{
	delete m_pForceManagerState;
}

TEnumerator<IForceFactory*>& ForceManager::GetForceFactories(void)
{
	return m_pForceManagerState->m_mapForceFactories.GetValueEnumerator();
}

IForceFactory* ForceManager::GetForceFactory(const Meson::Common::Text::String& p_strName)
{
	IForceFactory* pForceFactory;
	if (!m_pForceManagerState->m_mapForceFactories.ContainsKey(p_strName, pForceFactory))
		throw new MesonException(
			"Force factory '" + p_strName + "' not registered.",
			__FILE__, __LINE__);

	return pForceFactory;
}

void ForceManager::RegisterForceFactory(IForceFactory* p_pForceFactory)
{
	THashMap<String, IForceFactory *>& mapForceFactories
		= m_pForceManagerState->m_mapForceFactories;

	const String &strName = p_pForceFactory->GetName();

	if (mapForceFactories.ContainsKey(strName))
		throw new MesonException(
			"Force Factory '" + strName + "' already registered.",
			__FILE__, __LINE__);

	mapForceFactories.Insert(strName, p_pForceFactory);
}

void ForceManager::UnregisterForceFactory(IForceFactory* p_pForceFactory)
{
	THashMap<String, IForceFactory* >& mapForceFactories
		= m_pForceManagerState->m_mapForceFactories;

	const String& strName = p_pForceFactory->GetName();

	if (!mapForceFactories.ContainsKey(strName))
		throw new MesonException(
			"Force Factory '" + strName + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapForceFactories.RemoveKey(strName);
}


TEnumerator<ForcePtr>& ForceManager::GetForces(void)
{
	return m_pForceManagerState->m_mapForces.GetValueEnumerator();
}

ForcePtr ForceManager::GetForce(const String& p_strId)
{
	THashMap<String, ForcePtr>& mapForces
		= m_pForceManagerState->m_mapForces;

	ForcePtr pForce;
	if (!mapForces.ContainsKey(p_strId, pForce))
		throw new MesonException(
			"Force instance '" + p_strId + "' is not registered.",
			__FILE__, __LINE__);

	return pForce;
}

void ForceManager::RegisterForce(ForcePtr p_pForce)
{
	THashMap<String, ForcePtr>& mapForces
		= m_pForceManagerState->m_mapForces;

	const String& strId = p_pForce->GetId();

	if (mapForces.ContainsKey(strId))
		throw new MesonException(
			"Force instance '" + strId + "' already registered.",
			__FILE__, __LINE__);

	mapForces.Insert(strId, p_pForce);
}

void ForceManager::UnregisterForce(ForcePtr p_pForce)
{
	THashMap<String, ForcePtr>& mapForces
		= m_pForceManagerState->m_mapForces;

	const String& strId = p_pForce->GetId();

	if (!mapForces.ContainsKey(strId))
		throw new MesonException(
			"Force instance '" + strId + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapForces.RemoveKey(strId);
}

void ForceManager::UnregisterAllForces(void)
{
	m_pForceManagerState->m_mapForces.Clear();
}


void ForceManager::LoadForce(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'Force' container identifier
	lexer.ReadIdentifier("Force");

	// 'Force' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	// mandatory Type property
	String strForceType = lexer.ReadPropertyString("Type");

	// mandatory ID property
	String strForceId = lexer.ReadPropertyString("Id");

	// get the force factory for the required type
	IForceFactory* pForceFactory
		= GetForceFactory(strForceType);

	// create force with given id from the specified factory
	ForcePtr pForce = pForceFactory->CreateForce(strForceId);

	// get custom properties
	lexer.ReadCustomProperties(*pForce);

	// 'Force' container closing brace
	lexer.ReadToken(LexerTokenType::ClosingBrace);

	// register force
	RegisterForce(pForce);
}

void ForceManager::LoadForces(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'Forces' container identifier
	lexer.ReadIdentifier("Forces");

	// 'Forces' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			return;

		if (lexerToken.Type != LexerTokenType::Identifier
			|| lexerToken.ValueString != "Force")
			throw new MesonException(
				"Parsing Error: Expected identifier 'Force'",
				__FILE__, __LINE__);

		p_pLookaheadInputStream->Pushback("Force", 5);
		LoadForce(p_pLookaheadInputStream);
	}
}

void ForceManager::LoadForces(const String& p_strFilename)
{
	InputStreamPtr pInputStream(new FileInputStream(p_strFilename));
	LookaheadInputStreamPtr pLookaheadInputStream(
		new LookaheadInputStream(pInputStream));
	LoadForces(pLookaheadInputStream);
}