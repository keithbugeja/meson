#include "MesonException.h"
#include "FileInputStream.h"

#include "VstCameraManager.h"
#include "VstPropertySet.h"
#include "VstLexer.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace	Meson::Common::IO;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
CameraManager::CameraManager(void)
	: m_cameraMap() 
{
}
//----------------------------------------------------------------------------------------------
CameraManager::~CameraManager(void)
{
	CameraEnumerator cameraEnumerator = m_cameraMap.GetValueEnumerator();

	while(cameraEnumerator.HasMoreElements())
		cameraEnumerator.NextElement() = NULL;

	m_cameraMap.Clear();
}
//----------------------------------------------------------------------------------------------
CameraPtr CameraManager::CreateCamera(const Meson::Common::Text::String& p_strId)
{
	if (ContainsCamera(p_strId))
		throw new MesonException("Another camera object already exists with the given name", __FILE__, __LINE__);

	CameraPtr pCamera(new Camera(p_strId));
	m_cameraMap.Insert(p_strId, pCamera);

	return pCamera;
}
//----------------------------------------------------------------------------------------------
bool CameraManager::ContainsCamera(const Meson::Common::Text::String& p_strId)
{
	return m_cameraMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool CameraManager::ContainsCamera(const Meson::Common::Text::String& p_strId, CameraPtr& p_pCamera)
{
	return m_cameraMap.ContainsKey(p_strId, p_pCamera);
}
//----------------------------------------------------------------------------------------------
CameraPtr CameraManager::GetCamera(const Meson::Common::Text::String& p_strId)
{
	return m_cameraMap[p_strId];
}
//----------------------------------------------------------------------------------------------
void CameraManager::RegisterCamera(CameraPtr p_pCamera, bool p_bReplace)
{
	if (!p_bReplace)
	{
		if (ContainsCamera(p_pCamera->GetId()))
			throw new MesonException("Another camera object already exists with the given name", __FILE__, __LINE__);

		m_cameraMap.Insert(p_pCamera->GetId(), p_pCamera);
	}
	else
		m_cameraMap[p_pCamera->GetId()] = p_pCamera;
}
//----------------------------------------------------------------------------------------------
void CameraManager::UnregisterCamera(CameraPtr p_pCamera)
{
	if (ContainsCamera(p_pCamera->GetId()))
		m_cameraMap.RemoveKey(p_pCamera->GetId());
}
//----------------------------------------------------------------------------------------------
void CameraManager::UnregisterCamera(const Meson::Common::Text::String& p_strId)
{
	if (ContainsCamera(p_strId))
		m_cameraMap.RemoveKey(p_strId);
}
//----------------------------------------------------------------------------------------------
void CameraManager::UnregisterAllCameras(void)
{
	m_cameraMap.Clear();
}
//----------------------------------------------------------------------------------------------
void CameraManager::LoadCamera(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Camera' container identifier
	lexer.ReadKeyword("Camera");

	// 'Camera' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory ID property
	String strCameraId = lexer.ReadPropertyString("Id");

	// mandatory Type property
	String strCameraType = lexer.ReadPropertyString("Type");

	CameraPtr pCamera = CreateCamera(strCameraId);
	pCamera->Reset();
	
	// Default types : substitute abstract factories for the time being.
	if (!strCameraType.CompareIgnoreCase("perspective"))
		pCamera->SetProjection(Camera::CP_PERSPECTIVE);
	else if (!strCameraType.CompareIgnoreCase("orthographic"))
		pCamera->SetProjection(Camera::CP_ORTHOGRAPHIC);

	// get properties for object
	IPropertySet* pPropertySet = (IPropertySet*)(Camera*)pCamera;

	// Read properties into set
	lexer.ReadCustomProperties(*pPropertySet);

	// 'Light' container closing brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// Register light object
	RegisterCamera(pCamera);
}
//----------------------------------------------------------------------------------------------
void CameraManager::LoadCameras(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Cameras' container identifier
	lexer.ReadKeyword("Cameras");

	// 'Cameras' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD
			|| lexerToken.StringValue != "Camera")
			throw new MesonException(
				"Parsing Error: Expected keyword 'Camera'",
				__FILE__, __LINE__);

		p_pInputStream->Pushback("Camera", 6);
		LoadCamera(p_pInputStream);
	}
}
//----------------------------------------------------------------------------------------------
void CameraManager::LoadCameras(const Meson::Common::Text::String& p_strResourceName)
{
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourceName, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	LoadCameras(pLookaheadInputStream);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
