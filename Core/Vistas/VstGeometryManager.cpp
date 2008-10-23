//----------------------------------------------------------------------------------------------
#include "MesonException.h"
#include "FileInputStream.h"

#include "VstGeometryManager.h"
#include "VstPropertySet.h"
#include "VstLexer.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace	Meson::Common::IO;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
GeometryManager::GeometryManager(void)
	: m_geometryFactoryMap()
	, m_geometryMap()
{
}

//----------------------------------------------------------------------------------------------
GeometryManager::~GeometryManager(void)
{
	GeometryEnumerator geometryEnumerator = m_geometryMap.GetValueEnumerator();

	while(geometryEnumerator.HasMoreElements())
		geometryEnumerator.NextElement() = NULL;

	m_geometryMap.Clear();
}
//----------------------------------------------------------------------------------------------
bool GeometryManager::ContainsGeometry(const Meson::Common::Text::String& p_strId)
{
	return m_geometryMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool GeometryManager::ContainsGeometry(const Meson::Common::Text::String& p_strId, GeometryPtr& p_pGeometry)
{
	return m_geometryMap.ContainsKey(p_strId, p_pGeometry);
}
//----------------------------------------------------------------------------------------------
GeometryPtr GeometryManager::GetGeometry(const Meson::Common::Text::String& p_strId)
{
	return m_geometryMap[p_strId];
}
//----------------------------------------------------------------------------------------------
void GeometryManager::RegisterGeometry(GeometryPtr p_pGeometry, bool p_bReplace)
{
	m_geometryMap.Insert(p_pGeometry->GetId(), p_pGeometry);
}
//----------------------------------------------------------------------------------------------
void GeometryManager::UnregisterGeometry(GeometryPtr p_pGeometry)
{
	m_geometryMap.RemoveValue(p_pGeometry);
}
//----------------------------------------------------------------------------------------------
void GeometryManager::UnregisterGeometry(const Meson::Common::Text::String& p_strId)
{
	m_geometryMap.RemoveKey(p_strId);
}
//----------------------------------------------------------------------------------------------
void GeometryManager::UnregisterAllGeometry(void)
{
	m_geometryMap.Clear();
}
//----------------------------------------------------------------------------------------------
GeometryFactoryEnumerator GeometryManager::GetGeometryFactories(void)
{
	return (GeometryFactoryEnumerator)m_geometryFactoryMap.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
IGeometryFactory* GeometryManager::GetGeometryFactory(const Meson::Common::Text::String& p_strName)
{
	IGeometryFactory* pGeometryFactory;

	if (m_geometryFactoryMap.ContainsKey(p_strName, pGeometryFactory))
		return pGeometryFactory;

	throw new MesonException("Specified geometry factory is not registered.", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void GeometryManager::RegisterGeometryFactory(Meson::Vistas::IGeometryFactory *p_pGeometryFactory)
{
	if (m_geometryFactoryMap.ContainsKey(p_pGeometryFactory->GetName()))
		throw new MesonException("Geometry factory '" + p_pGeometryFactory->GetName() + "' has already been registered.", __FILE__, __LINE__);

	m_geometryFactoryMap.Insert(p_pGeometryFactory->GetName(), p_pGeometryFactory);
}
//----------------------------------------------------------------------------------------------
void GeometryManager::UnregisterGeometryFactory(Meson::Vistas::IGeometryFactory *p_pGeometryFactory)
{
	if (!m_geometryFactoryMap.ContainsKey(p_pGeometryFactory->GetName()))
		throw new MesonException("Geometry factory '" + p_pGeometryFactory->GetName() + "' is not registered and cannot be removed.", __FILE__, __LINE__);

	m_geometryFactoryMap.RemoveKey(p_pGeometryFactory->GetName());
}
//----------------------------------------------------------------------------------------------
void GeometryManager::LoadGeometry(LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Geometry' container identifier
	lexer.ReadKeyword("Geometry");

	// 'Geometry' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory ID property
	String strGeometryId = lexer.ReadPropertyString("Id");

	// mandatory Type property
	String strGeometryType = lexer.ReadPropertyString("Type");

	// get the geometry factory for the required type
	IGeometryFactory* pGeometryFactory = GetGeometryFactory(strGeometryType);

	// get properties for factory
	IPropertySet* pPropertySet = pGeometryFactory->GetPropertySet();

	// Read properties into set
	lexer.ReadCustomProperties(*pPropertySet);

	// 'Geometry' container closing brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// create geometry with given id from the specified factory
	GeometryPtr pGeometry = pGeometryFactory->CreateGeometry(strGeometryId);

	// register geometry
	RegisterGeometry(pGeometry);
}
//----------------------------------------------------------------------------------------------
void GeometryManager::LoadGeometries(LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Geometries' container identifier
	lexer.ReadKeyword("Geometries");

	// 'Geometries' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD
			|| lexerToken.StringValue != "Geometry")
			throw new MesonException(
				"Parsing Error: Expected keyword 'Geometry'",
				__FILE__, __LINE__);

		p_pInputStream->Pushback("Geometry", 8);
		LoadGeometry(p_pInputStream);
	}
}
//----------------------------------------------------------------------------------------------
void GeometryManager::LoadGeometries(const String& p_strResourceName)
{
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourceName, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	LoadGeometries(pLookaheadInputStream);
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
