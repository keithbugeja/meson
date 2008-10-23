#include "GrvKineticsManager.h"

#include "DebugSupport.h"
#include "HashMap.h"
#include "FileInputStream.h"

#include "GrvLexer.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas::Kinetics;

Meson_Gravitas_Kinetics_BEGIN

struct KineticsManagerState
{
	THashMap<String, KineticIntegratorPtr> m_mapKineticIntegrators;

	KineticsManagerState(void) : m_mapKineticIntegrators() {}
};

Meson_Gravitas_Kinetics_END

KineticsManager::KineticsManager()
	: m_pKineticsManagerState(NULL)
{
	m_pKineticsManagerState = new KineticsManagerState();
}

KineticsManager::~KineticsManager()
{
	delete m_pKineticsManagerState;
}

TEnumerator<KineticIntegratorPtr>& KineticsManager::GetKineticIntegrators(void)
{
	return m_pKineticsManagerState->m_mapKineticIntegrators.GetValueEnumerator();
}

KineticIntegratorPtr KineticsManager::GetKineticIntegrator(const String& p_strName)
{
	KineticIntegratorPtr pKineticIntegrator;
	if (!m_pKineticsManagerState->m_mapKineticIntegrators.ContainsKey(p_strName, pKineticIntegrator))
		throw new MesonException("Kinetic integrator '" + p_strName + "' not registered.",
			__FILE__, __LINE__);
	return pKineticIntegrator;
}

void KineticsManager::RegisterKineticIntegrator(KineticIntegratorPtr p_pKineticIntegrator)
{
	THashMap<String, KineticIntegratorPtr> &mapKineticIntegrators
		= m_pKineticsManagerState->m_mapKineticIntegrators;

	const String &strName = p_pKineticIntegrator->GetName();

	if (mapKineticIntegrators.ContainsKey(strName))
		throw new MesonException(
			"Kinetic integrator '" + strName + "' already registered.",
			__FILE__, __LINE__);

	mapKineticIntegrators[strName] = p_pKineticIntegrator;
}

void KineticsManager::UnregisterKineticIntegrator(KineticIntegratorPtr p_pKineticIntegrator)
{
	THashMap<String, KineticIntegratorPtr> &mapKineticIntegrators
		= m_pKineticsManagerState->m_mapKineticIntegrators;

	const String &strName = p_pKineticIntegrator->GetName();

	if (!mapKineticIntegrators.ContainsKey(strName))
		throw new MesonException(
			"Kinetic integrator '" + strName + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapKineticIntegrators.RemoveKey(strName);
}

void KineticsManager::UnregisterAllKineticIntegrators(void)
{
	m_pKineticsManagerState->m_mapKineticIntegrators.Clear();
}

void KineticsManager::LoadKineticIntegrator(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'KineticIntegrator' container identifier
	LexerToken lexerToken = lexer.ReadToken(LexerTokenType::Identifier);

	if (lexerToken.ValueString != "KineticIntegrator")
		throw new MesonException(
			"Parsing Error: Expected identifier 'KineticIntegrator'",
			__FILE__, __LINE__);

	// 'KineticIntegrator' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	// mandatory Type property
	lexerToken = lexer.ReadToken(LexerTokenType::Identifier);
	if (lexerToken.ValueString != "Type")
		throw new MesonException(
			"Parsing Error: Expected identifier 'Type'",
			__FILE__, __LINE__);

	lexer.ReadToken(LexerTokenType::Assignment);

	lexerToken = lexer.ReadToken(LexerTokenType::String);
	String strKineticIntegratorType = lexerToken.ValueString;

	// get the kinetic integrator for the required type
	IKineticIntegrator* pKineticIntegrator
		= GetKineticIntegrator(strKineticIntegratorType);

	// get custom properties and configure kinetic integrator
	lexer.ReadCustomProperties(*pKineticIntegrator);
}

void KineticsManager::LoadKineticIntegrators(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'KineticIntegrators' container identifier
	LexerToken lexerToken = lexer.ReadToken(LexerTokenType::Identifier);
	if (lexerToken.ValueString != "KineticIntegrators")
		throw new MesonException(
			"Parsing Error: Expected identifier 'KineticIntegrators'",
			__FILE__, __LINE__);

	// 'KineticIntegrators' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	while (true)
	{
		lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			return;

		if (lexerToken.Type != LexerTokenType::Identifier
			|| lexerToken.ValueString != "KineticIntegrator")
			throw new MesonException(
				"Parsing Error: Expected identifier 'KineticIntegrator'",
				__FILE__, __LINE__);

		p_pLookaheadInputStream->Pushback("KineticIntegrator", 17);
		LoadKineticIntegrator(p_pLookaheadInputStream);
	}
}

void KineticsManager::LoadKineticIntegrators(const String& p_strFilename)
{
	InputStreamPtr pInputStream(new FileInputStream(p_strFilename));
	LookaheadInputStreamPtr pLookaheadInputStream(
		new LookaheadInputStream(pInputStream));
	LoadKineticIntegrators(pLookaheadInputStream);
}
