#include "GrvGeometryManager.h"

#include "DebugSupport.h"
#include "ArrayList.h"
#include "HashMap.h"
#include "FileInputStream.h"

#include "GrvGravitasEngine.h"
#include "GrvLexer.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

Meson_Gravitas_Geometry_BEGIN

struct GeometryManagerState
{
	TArrayList<String> m_listTypeRegistry;
	THashMap<String, IGeometryFactory *> m_mapGeometryFactories;
	THashMap<String, GeometryPtr> m_mapGeometry;

	GeometryManagerState(void)
		: m_listTypeRegistry()
		, m_mapGeometryFactories()
		, m_mapGeometry()
	{}
};

Meson_Gravitas_Geometry_END

GeometryManager::GeometryManager()
	: m_pGeometryManagerState(NULL)
{
	m_pGeometryManagerState = new GeometryManagerState();
}

GeometryManager::~GeometryManager()
{
	delete m_pGeometryManagerState;
}

short GeometryManager::GetGeometryTypeId(const String& p_strTypeName)
{
	size_t unRegistrySize = m_pGeometryManagerState->m_listTypeRegistry.Size();
	for (size_t unIndex = 0; unIndex < unRegistrySize; unIndex++)
		if (m_pGeometryManagerState->m_listTypeRegistry[unIndex] == p_strTypeName)
			return (short) unIndex;

	m_pGeometryManagerState->m_listTypeRegistry.Add(p_strTypeName);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Geometry type '" << p_strTypeName << "' mapped to ID: " << (short) unRegistrySize << "\n";
	return (short) unRegistrySize;
}

TEnumerator<IGeometryFactory*>& GeometryManager::GetGeometryFactories(void)
{
	return m_pGeometryManagerState->m_mapGeometryFactories.GetValueEnumerator();
}

IGeometryFactory* GeometryManager::GetGeometryFactory(const String& p_strName)
{
	THashMap<String, IGeometryFactory *>& mapGeometryFactories
		= m_pGeometryManagerState->m_mapGeometryFactories;

	IGeometryFactory* pGeometryFactory;
	if (!mapGeometryFactories.ContainsKey(p_strName, pGeometryFactory))
		throw new MesonException(
			"Geometry Factory '" + p_strName + "' is not registered.",
			__FILE__, __LINE__);

	return pGeometryFactory;
}

void GeometryManager::RegisterGeometryFactory(IGeometryFactory* p_pGeometryFactory)
{
	THashMap<String, IGeometryFactory *> &mapGeometryFactories
		= m_pGeometryManagerState->m_mapGeometryFactories;

	const String& strName = p_pGeometryFactory->GetName();

	if (mapGeometryFactories.ContainsKey(strName))
		throw new MesonException(
			"Geometry Factory '" + strName + "' already registered.",
			__FILE__, __LINE__);

	mapGeometryFactories[strName] = p_pGeometryFactory;

	// trigger type id assignment
	GetGeometryTypeId(strName);
}

void GeometryManager::UnregisterGeometryFactory(IGeometryFactory* p_pGeometryFactory)
{
	THashMap<String, IGeometryFactory *> &mapGeometryFactories
		= m_pGeometryManagerState->m_mapGeometryFactories;

	const String& strName = p_pGeometryFactory->GetName();

	if (!mapGeometryFactories.ContainsKey(strName))
		throw new MesonException(
			"Geometry Factory '" + strName + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapGeometryFactories.RemoveKey(strName);
}

TEnumerator<GeometryPtr>& GeometryManager::GetGeometries(void)
{
	return m_pGeometryManagerState->m_mapGeometry.GetValueEnumerator();
}

GeometryPtr GeometryManager::GetGeometry(const String& p_strId)
{
	THashMap<String, GeometryPtr>& mapGeometry
		= m_pGeometryManagerState->m_mapGeometry;

	GeometryPtr pGeometry;
	if (!mapGeometry.ContainsKey(p_strId, pGeometry))
		throw new MesonException(
			"Geometry instance '" + p_strId + "' is not registered.",
			__FILE__, __LINE__);

	return pGeometry;
}

void GeometryManager::RegisterGeometry(GeometryPtr p_pGeometry)
{
	THashMap<String, GeometryPtr>& mapGeometry
		= m_pGeometryManagerState->m_mapGeometry;

	const String& strId = p_pGeometry->GetId();

	if (mapGeometry.ContainsKey(strId))
		throw new MesonException(
			"Geometry instance '" + strId + "' already registered.",
			__FILE__, __LINE__);

	mapGeometry.Insert(strId, p_pGeometry);
}

void GeometryManager::UnregisterGeometry(GeometryPtr p_pGeometry)
{
	THashMap<String, GeometryPtr>& mapGeometry
		= m_pGeometryManagerState->m_mapGeometry;

	const String& strId = p_pGeometry->GetId();

	if (!mapGeometry.ContainsKey(strId))
		throw new MesonException(
			"Geometry instance '" + strId + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapGeometry.RemoveKey(strId);
}

void GeometryManager::UnregisterAllGeometries(void)
{
	m_pGeometryManagerState->m_mapGeometry.Clear();
}

void GeometryManager::LoadGeometry(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'Geometry' container identifier
	lexer.ReadIdentifier("Geometry");

	// 'Geometry' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	// mandatory Type property
	String strGeometryType = lexer.ReadPropertyString("Type");

	// mandatory ID property
	String strGeometryId = lexer.ReadPropertyString("Id");

	// get the geometry factor for the required type
	IGeometryFactory* pGeometryFactory
		= GetGeometryFactory(strGeometryType);

	// create geometry with given id from the specified factory
	GeometryPtr pGeometry = pGeometryFactory->CreateGeometry(strGeometryId);

	// get custom properties
	lexer.ReadCustomProperties(*pGeometry);

	// 'Geometry' container closing brace
	lexer.ReadToken(LexerTokenType::ClosingBrace);

	// register geometry
	RegisterGeometry(pGeometry);
}

void GeometryManager::LoadGeometries(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'Geometries' container identifier
	lexer.ReadIdentifier("Geometries");

	// 'Geometries' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			return;

		if (lexerToken.Type != LexerTokenType::Identifier
			|| lexerToken.ValueString != "Geometry")
			throw new MesonException(
				"Parsing Error: Expected identifier 'Geometry'",
				__FILE__, __LINE__);

		p_pLookaheadInputStream->Pushback("Geometry", 8);
		LoadGeometry(p_pLookaheadInputStream);
	}
}

void GeometryManager::LoadGeometries(const String& p_strFilename)
{
	InputStreamPtr pInputStream(new FileInputStream(p_strFilename));
	LookaheadInputStreamPtr pLookaheadInputStream(
		new LookaheadInputStream(pInputStream));
	LoadGeometries(pLookaheadInputStream);
}