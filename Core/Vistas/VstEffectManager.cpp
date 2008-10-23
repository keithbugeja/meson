#include "MesonException.h"
#include "FileInputStream.h"

#include "VstEffectManager.h"
#include "VstPropertySet.h"
#include "VstLexer.h"

#include "VstReflectionEffect.h"
#include "VstRefractionEffect.h"
#include "VstShadowEffect.h"
#include "VstSkyEffect.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace	Meson::Common::IO;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
EffectManager::EffectManager(void)
	: m_effectMap()
{
}
//----------------------------------------------------------------------------------------------
EffectManager::~EffectManager(void)
{
	DestroyAllEffects();
}
//----------------------------------------------------------------------------------------------
void EffectManager::DestroyAllEffects(void)
{
	IRenderDevice* pRenderDevice = VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();

	if (pRenderDevice)
	{
		EffectEnumerator effectEnumerator = m_effectMap.GetValueEnumerator();

		while(effectEnumerator.HasMoreElements())
		{
			EffectPtr& pEffect = effectEnumerator.NextElement();
			pEffect->Dispose(pRenderDevice);
			pEffect = NULL;
		}
	}

	m_effectMap.Clear();
}
//----------------------------------------------------------------------------------------------
bool EffectManager::ContainsEffect(const Meson::Common::Text::String& p_strId)
{
	return m_effectMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool EffectManager::ContainsEffect(const Meson::Common::Text::String& p_strId, EffectPtr& p_pEffect)
{
	return m_effectMap.ContainsKey(p_strId, p_pEffect);
}
//----------------------------------------------------------------------------------------------
EffectPtr EffectManager::GetEffect(const Meson::Common::Text::String& p_strId)
{
	return m_effectMap[p_strId];
}
//----------------------------------------------------------------------------------------------
void EffectManager::RegisterEffect(EffectPtr p_pEffect, bool p_bReplace)
{
	if (!p_bReplace)
	{
		if (ContainsEffect(p_pEffect->GetId()))
			throw new MesonException("Another effect object already exists with the given name", __FILE__, __LINE__);

		m_effectMap.Insert(p_pEffect->GetId(), p_pEffect);
	}
	else
		m_effectMap[p_pEffect->GetId()] = p_pEffect;
}
//----------------------------------------------------------------------------------------------
void EffectManager::UnregisterEffect(EffectPtr p_pEffect)
{
	if (ContainsEffect(p_pEffect->GetId()))
		m_effectMap.RemoveKey(p_pEffect->GetId());
}
//----------------------------------------------------------------------------------------------
void EffectManager::UnregisterEffect(const Meson::Common::Text::String& p_strId)
{
	if (ContainsEffect(p_strId))
		m_effectMap.RemoveKey(p_strId);
}
//----------------------------------------------------------------------------------------------
void EffectManager::UnregisterAllEffects(void)
{
	m_effectMap.Clear();
}
//----------------------------------------------------------------------------------------------
void EffectManager::LoadEffect(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Effect' container identifier
	lexer.ReadKeyword("Effect");

	// 'Effect' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory ID property
	String strEffectId = lexer.ReadPropertyString("Id");

	// mandatory Type property
	String strEffectType = lexer.ReadPropertyString("Type");

	// 'CustomProperties' container keyword
	lexer.ReadKeyword("CustomProperties");

	// 'CustomProperties' container left brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	EffectPtr pEffect(NULL);

	if (!strEffectType.CompareIgnoreCase("shadow"))
	{
		String strRenderer = lexer.ReadPropertyString("Renderer");
		String strTextureId = lexer.ReadPropertyString("TextureId");
		int nWidth = lexer.ReadPropertyInteger("Width");
		int nHeight = lexer.ReadPropertyInteger("Height");
		String strProjectorId = lexer.ReadPropertyString("ProjectorId");
		String strShadowCasterSetId = lexer.ReadPropertyString("ShadowCasterSetId");

		RendererPtr pRenderer = VistasEngine::GetInstance()->GetRendererManager()->GetRendererFactory(strRenderer)->CreateRenderer();
		CameraPtr pCamera = VistasEngine::GetInstance()->GetCameraManager()->GetCamera(strProjectorId);
		ShadowEffect* pShadowEffect = new ShadowEffect(strTextureId, pCamera, (uint)nWidth, (uint)nHeight);
		pShadowEffect->SetShadowCasterSet(VistasEngine::GetInstance()->GetVisibleEntitySetManager()->GetVisibleEntitySet(strShadowCasterSetId));
		pShadowEffect->SetRenderer(pRenderer);
		pEffect = pShadowEffect;
	}
	else if (!strEffectType.CompareIgnoreCase("reflection"))
	{
		String strTextureId = lexer.ReadPropertyString("TextureId");
		float fReflectivity = lexer.ReadPropertyFloat("Reflectivity");
		TexturePtr pTexture = VistasEngine::GetInstance()->GetTextureManager()->GetTexture(strTextureId);

		ReflectionEffect* pReflectionEffect = new ReflectionEffect(pTexture, fReflectivity);
		pEffect = pReflectionEffect;
	}
	else if (!strEffectType.CompareIgnoreCase("refraction"))
	{
		String strTextureId = lexer.ReadPropertyString("TextureId");
		float fRefractivity = lexer.ReadPropertyFloat("Refractivity");
		TexturePtr pTexture = VistasEngine::GetInstance()->GetTextureManager()->GetTexture(strTextureId);

		RefractiveEffect* pRefractiveEffect = new RefractiveEffect(pTexture, fRefractivity);
		pEffect = pRefractiveEffect;
	}
	else if (!strEffectType.CompareIgnoreCase("sky"))
	{
		String strTextureId = lexer.ReadPropertyString("TextureId");
		TexturePtr pTexture = VistasEngine::GetInstance()->GetTextureManager()->GetTexture(strTextureId);

		SkyEffect* pSkyEffect = new SkyEffect(pTexture);
		pEffect = pSkyEffect;
	}

	pEffect->SetId(strEffectId);
	pEffect->Initialise(VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive());

	// 'CustomProperties' container right brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// 'Effect' container right brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// Register effect object
	RegisterEffect(pEffect);
}
//----------------------------------------------------------------------------------------------
void EffectManager::LoadEffects(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Effects' container identifier
	lexer.ReadKeyword("Effects");

	// 'Effects' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD
			|| lexerToken.StringValue != "Effect")
			throw new MesonException(
				"Parsing Error: Expected keyword 'Effect'",
				__FILE__, __LINE__);

		p_pInputStream->Pushback("Effect", 6);
		LoadEffect(p_pInputStream);
	}
}
//----------------------------------------------------------------------------------------------
void EffectManager::LoadEffects(const Meson::Common::Text::String& p_strResourceName)
{
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourceName, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	LoadEffects(pLookaheadInputStream);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
