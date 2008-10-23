#include "MesonException.h"
#include "FileInputStream.h"

#include "VstLightManager.h"
#include "VstPropertySet.h"
#include "VstLexer.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace	Meson::Common::IO;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
LightManager::LightManager(void)
	: m_lightMap()	
{
}
//----------------------------------------------------------------------------------------------
LightManager::~LightManager(void)
{
	LightEnumerator lightEnumerator = m_lightMap.GetValueEnumerator();

	while(lightEnumerator.HasMoreElements())
		lightEnumerator.NextElement() = NULL;

	m_lightMap.Clear();
}
//----------------------------------------------------------------------------------------------
LightPtr LightManager::CreateLight(const Meson::Common::Text::String& p_strId)
{
	if (ContainsLight(p_strId))
		throw new MesonException("Another light object already exists with the given name", __FILE__, __LINE__);

	LightPtr pLight(new Light(p_strId));
	m_lightMap.Insert(p_strId, pLight);

	return pLight;
}
//----------------------------------------------------------------------------------------------
bool LightManager::ContainsLight(const Meson::Common::Text::String& p_strId)
{
	return m_lightMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool LightManager::ContainsLight(const Meson::Common::Text::String& p_strId, LightPtr& p_pLight)
{
	return m_lightMap.ContainsKey(p_strId, p_pLight);
}
//----------------------------------------------------------------------------------------------
LightPtr LightManager::GetLight(const Meson::Common::Text::String& p_strId)
{
	return m_lightMap[p_strId];
}
//----------------------------------------------------------------------------------------------
void LightManager::RegisterLight(LightPtr p_pLight, bool p_bReplace)
{
	if (!p_bReplace)
	{
		if (ContainsLight(p_pLight->GetId()))
			throw new MesonException("Another light object already exists with the given name", __FILE__, __LINE__);

		m_lightMap.Insert(p_pLight->GetId(), p_pLight);
	}
	else
		m_lightMap[p_pLight->GetId()] = p_pLight;
}
//----------------------------------------------------------------------------------------------
void LightManager::UnregisterLight(LightPtr p_pLight)
{
	if (ContainsLight(p_pLight->GetId()))
		m_lightMap.RemoveKey(p_pLight->GetId());
}
//----------------------------------------------------------------------------------------------
void LightManager::UnregisterLight(const Meson::Common::Text::String& p_strId)
{
	if (ContainsLight(p_strId))
		m_lightMap.RemoveKey(p_strId);
}
//----------------------------------------------------------------------------------------------
void LightManager::UnregisterAllLights(void)
{
	m_lightMap.Clear();
}
//----------------------------------------------------------------------------------------------
void LightManager::LoadLight(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Light' container identifier
	lexer.ReadKeyword("Light");

	// 'Light' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory ID property
	String strLightId = lexer.ReadPropertyString("Id");

	// mandatory Type property
	String strLightType = lexer.ReadPropertyString("Type");

	LightPtr pLight = CreateLight(strLightId);
	pLight->Reset();
	
	// Default types : substitute abstract factories for the time being.
	if (!strLightType.CompareIgnoreCase("spot"))
		pLight->SetType(Light::LT_SPOT);
	else if (!strLightType.CompareIgnoreCase("point"))
		pLight->SetType(Light::LT_SPOT);
	else if (!strLightType.CompareIgnoreCase("directional"))
		pLight->SetType(Light::LT_DIRECTIONAL);

	// get properties for object
	IPropertySet* pPropertySet = (IPropertySet*)(Light*)pLight;

	// Read properties into set
	lexer.ReadCustomProperties(*pPropertySet);

	// 'Light' container closing brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// Register light object
	RegisterLight(pLight);
}
//----------------------------------------------------------------------------------------------
void LightManager::LoadLights(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Lights' container identifier
	lexer.ReadKeyword("Lights");

	// 'Lights' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD
			|| lexerToken.StringValue != "Light")
			throw new MesonException(
				"Parsing Error: Expected keyword 'Light'",
				__FILE__, __LINE__);

		p_pInputStream->Pushback("Light", 5);
		LoadLight(p_pInputStream);
	}
}
//----------------------------------------------------------------------------------------------
void LightManager::LoadLights(const Meson::Common::Text::String& p_strResourceName)
{
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourceName, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	LoadLights(pLookaheadInputStream);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
