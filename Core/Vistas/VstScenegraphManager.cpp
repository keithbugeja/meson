#include "MesonException.h"
#include "FileInputStream.h"

#include "VstScenegraphManager.h"
#include "VstPropertySet.h"
#include "VstLexer.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace	Meson::Common::IO;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
ScenegraphManager::ScenegraphManager(void)
	: m_scenegraphMap()
{
}
//----------------------------------------------------------------------------------------------
ScenegraphManager::~ScenegraphManager(void)
{
	ScenegraphEnumerator scenegraphEnumerator = m_scenegraphMap.GetValueEnumerator();

	while(scenegraphEnumerator.HasMoreElements())
		scenegraphEnumerator.NextElement() = NULL;

	m_scenegraphMap.Clear();
}
//----------------------------------------------------------------------------------------------
ScenegraphPtr ScenegraphManager::CreateScenegraph(const Meson::Common::Text::String& p_strId)
{
	if (ContainsScenegraph(p_strId))
		throw new MesonException("Another scenegraph object already exists with the given name", __FILE__, __LINE__);

	ScenegraphPtr pScenegraph(new Scenegraph(p_strId));
	m_scenegraphMap.Insert(p_strId, pScenegraph);

	return pScenegraph;
}
//----------------------------------------------------------------------------------------------
bool ScenegraphManager::ContainsScenegraph(const Meson::Common::Text::String& p_strId)
{
	return m_scenegraphMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool ScenegraphManager::ContainsScenegraph(const Meson::Common::Text::String& p_strId, ScenegraphPtr& p_pScenegraph)
{
	return m_scenegraphMap.ContainsKey(p_strId, p_pScenegraph);
}
//----------------------------------------------------------------------------------------------
ScenegraphPtr ScenegraphManager::GetScenegraph(const Meson::Common::Text::String& p_strId)
{
	return m_scenegraphMap[p_strId];
}
//----------------------------------------------------------------------------------------------
ScenegraphEnumerator ScenegraphManager::GetScenegraphEnumerator(void)
{
	return m_scenegraphMap.GetValueEnumerator();
}
//----------------------------------------------------------------------------------------------
void ScenegraphManager::RegisterScenegraph(ScenegraphPtr p_pScenegraph, bool p_bReplace)
{
	if (!p_bReplace)
	{
		if (ContainsScenegraph(p_pScenegraph->GetId()))
			throw new MesonException("Another scenegraph object already exists with the given name", __FILE__, __LINE__);

		m_scenegraphMap.Insert(p_pScenegraph->GetId(), p_pScenegraph);
	}
	else
		m_scenegraphMap[p_pScenegraph->GetId()] = p_pScenegraph;
}
//----------------------------------------------------------------------------------------------
void ScenegraphManager::UnregisterScenegraph(ScenegraphPtr p_pScenegraph)
{
	if (ContainsScenegraph(p_pScenegraph->GetId()))
		m_scenegraphMap.RemoveKey(p_pScenegraph->GetId());
}
//----------------------------------------------------------------------------------------------
void ScenegraphManager::UnregisterScenegraph(const Meson::Common::Text::String& p_strId)
{
	if (ContainsScenegraph(p_strId))
		m_scenegraphMap.RemoveKey(p_strId);
}
//----------------------------------------------------------------------------------------------
void ScenegraphManager::UnregisterAllScenegraphs(void)
{
	m_scenegraphMap.Clear();
}
//----------------------------------------------------------------------------------------------
void ScenegraphManager::LoadScenegraph(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Scenegraph' container identifier
	lexer.ReadKeyword("Scenegraph");

	// 'Scenegraph' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory ID property
	String strScenegraphId = lexer.ReadPropertyString("Id");
	ScenegraphPtr pScenegraph = CreateScenegraph(strScenegraphId);

	// Load nodes
	pScenegraph->LoadNodes(p_pInputStream);

	// 'Scenegraph' container closing brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// Register scenegraph object
	RegisterScenegraph(pScenegraph);
}
//----------------------------------------------------------------------------------------------
void ScenegraphManager::LoadScenegraphs(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Scenegraphs' container identifier
	lexer.ReadKeyword("Scenegraphs");

	// 'Scenegraphs' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD
			|| lexerToken.StringValue != "Scenegraph")
			throw new MesonException(
				"Parsing Error: Expected keyword 'Scenegraph'",
				__FILE__, __LINE__);

		p_pInputStream->Pushback("Scenegraph", 10);
		LoadScenegraph(p_pInputStream);
	}
}
//----------------------------------------------------------------------------------------------
void ScenegraphManager::LoadScenegraphs(const Meson::Common::Text::String& p_strResourceName)
{
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourceName, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	LoadScenegraphs(pLookaheadInputStream);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
