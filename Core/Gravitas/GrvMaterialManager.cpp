#include "GrvMaterialManager.h"

#include "DebugSupport.h"
#include "HashMap.h"
#include "FileInputStream.h"

#include "GrvLexer.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;
using namespace Meson::Gravitas::Materials;

Meson_Gravitas_Materials_BEGIN

struct MaterialManagerState
{
	THashMap<String, MaterialPtr> m_mapMaterials;

	MaterialManagerState(void) : m_mapMaterials() {}
};

Meson_Gravitas_Materials_END

MaterialManager::MaterialManager()
	: m_pMaterialManagerState(NULL)
{
	m_pMaterialManagerState = new MaterialManagerState();
}

MaterialManager::~MaterialManager()
{
	delete m_pMaterialManagerState;
}

TEnumerator<MaterialPtr>& MaterialManager::GetMaterials(void)
{
	return m_pMaterialManagerState->m_mapMaterials.GetValueEnumerator();
}

MaterialPtr MaterialManager::GetMaterial(const String& p_strId)
{
	MaterialPtr pMaterial;
	if (!m_pMaterialManagerState->m_mapMaterials.ContainsKey(p_strId, pMaterial))
		throw new MesonException(
			"Material '" + p_strId + "' not registered.",
			__FILE__, __LINE__);

	return pMaterial;
}

void MaterialManager::RegisterMaterial(MaterialPtr p_pMaterial)
{
	THashMap<String, MaterialPtr>& mapMaterials
		= m_pMaterialManagerState->m_mapMaterials;

	const String& strId = p_pMaterial->GetId();

	if (mapMaterials.ContainsKey(strId))
		throw new MesonException(
			"Material '" + strId + "' already registered.",
			__FILE__, __LINE__);

	mapMaterials.Insert(strId, p_pMaterial);
}

void MaterialManager::UnregisterMaterial(MaterialPtr p_pMaterial)
{
	THashMap<String, MaterialPtr>& mapMaterials
		= m_pMaterialManagerState->m_mapMaterials;

	const String& strId = p_pMaterial->GetId();

	if (!mapMaterials.ContainsKey(strId))
		throw new MesonException(
			"Material '" + strId + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapMaterials.RemoveKey(strId);
}

void MaterialManager::UnregisterAllMaterials(void)
{
	m_pMaterialManagerState->m_mapMaterials.Clear();
}

void MaterialManager::LoadMaterial(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'material' container identifier
	lexer.ReadIdentifier("Material");

	// 'material' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	// mandatory ID property
	String strMaterialId = lexer.ReadPropertyString("Id");

	// create material with given id and default properties
	MaterialPtr pMaterial(new Material(strMaterialId));

	// optional properties
	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		// closing brace for 'Material' container
		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			break;

		if (lexerToken.Type != LexerTokenType::Identifier)
			throw new MesonException(
				"Parsing Error: Opening brace or material property expected.",
				__FILE__, __LINE__);

		// assignment
		lexer.ReadToken(LexerTokenType::Assignment);

		// value
		LexerToken lexerTokenValue = lexer.ReadToken(LexerTokenType::Real);

		if (lexerToken.ValueString == "RestitutionCoefficient")
			pMaterial->RestitutionCoefficient = lexerTokenValue.ValueReal;
		else if (lexerToken.ValueString == "StaticFrictionCoefficient")
			pMaterial->StaticFrictionCoefficient = lexerTokenValue.ValueReal;
		else if (lexerToken.ValueString == "DynamicFrictionCoefficient")
			pMaterial->DynamicFrictionCoefficient = lexerTokenValue.ValueReal;
		else if (lexerToken.ValueString == "SurfacePerturbationCoefficient")
			pMaterial->SurfacePerturbationCoefficient = lexerTokenValue.ValueReal;
		else
			throw new MesonException(
				"Parsing Error: Invalid material property '" + lexerToken.ValueString + "'.",
				__FILE__, __LINE__);
	}

	// register matieral
	RegisterMaterial(pMaterial);
}


void MaterialManager::LoadMaterials(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'materials' container identifier
	lexer.ReadIdentifier("Materials");

	// 'materials' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			return;

		if (lexerToken.Type != LexerTokenType::Identifier
			|| lexerToken.ValueString != "Material")
			throw new MesonException(
				"Parsing Error: Expected identifier 'Material'",
				__FILE__, __LINE__);

		p_pLookaheadInputStream->Pushback("Material", 8);
		LoadMaterial(p_pLookaheadInputStream);
	}
}

void MaterialManager::LoadMaterials(const String& p_strFilename)
{
	InputStreamPtr pInputStream(new FileInputStream(p_strFilename));
	LookaheadInputStreamPtr pLookaheadInputStream(
		new LookaheadInputStream(pInputStream));
	LoadMaterials(pLookaheadInputStream);
}