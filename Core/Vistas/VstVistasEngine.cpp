//----------------------------------------------------------------------------------------------
//	Meson::Vistas::VistasEngine
//	Vistas 3D visualisation engine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"

#include "VstRenderDevice.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
struct VistasEngineState
{
	String m_strName;
	Version m_version;

	VisibleEntitySetManager* m_pVisibleEntitySetManager;
	WindowMessageManager* m_pWindowMessageManager;
	RenderDeviceManager* m_pRenderDeviceManager;
	RepositoryManager* m_pRepositoryManager;
	ScenegraphManager* m_pScenegraphManager;
	GeometryManager* m_pGeometryManager;
	RendererManager* m_pRendererManager;
	MaterialManager* m_pMaterialManager;
	TextureManager* m_pTextureManager;
	EffectManager* m_pEffectManager;
	CullerManager* m_pCullerManager;
	CameraManager* m_pCameraManager;
	LightManager* m_pLightManager;

	VistasEngineState(void)
		: m_strName("VistasEngine")
		, m_version(0,0,1)
		, m_pWindowMessageManager(WindowMessageManager::GetInstance())
		, m_pRenderDeviceManager(RenderDeviceManager::GetInstance())
		, m_pRepositoryManager(RepositoryManager::GetInstance())
		, m_pRendererManager(RendererManager::GetInstance())
		, m_pCullerManager(CullerManager::GetInstance())
		, m_pGeometryManager(GeometryManager::GetInstance())
		, m_pMaterialManager(MaterialManager::GetInstance())
		, m_pTextureManager(TextureManager::GetInstance())
		, m_pEffectManager(EffectManager::GetInstance())
		, m_pCameraManager(CameraManager::GetInstance())
		, m_pLightManager(LightManager::GetInstance())
		, m_pScenegraphManager(ScenegraphManager::GetInstance())
		, m_pVisibleEntitySetManager(VisibleEntitySetManager::GetInstance())
	{
	}

	~VistasEngineState(void)
	{
		m_pVisibleEntitySetManager->Delete();
		m_pEffectManager->Delete();
		m_pScenegraphManager->Delete();
		m_pLightManager->Delete();
		m_pCameraManager->Delete();
		m_pTextureManager->Delete();
		m_pMaterialManager->Delete();
		m_pGeometryManager->Delete();
		m_pCullerManager->Delete();
		m_pRendererManager->Delete();
		m_pRepositoryManager->Delete();
		m_pRenderDeviceManager->Delete();
		m_pWindowMessageManager->Delete();
	}
};
//----------------------------------------------------------------------------------------------
VistasEngine::VistasEngine(void) 
	: m_pVistasEngineState(new VistasEngineState())
{
}
//----------------------------------------------------------------------------------------------
VistasEngine::~VistasEngine() 
{
	delete m_pVistasEngineState;
}
//----------------------------------------------------------------------------------------------
const String& VistasEngine::GetName(void)
{
	return m_pVistasEngineState->m_strName;
}
//----------------------------------------------------------------------------------------------
const Version& VistasEngine::GetVersion(void)
{
	return m_pVistasEngineState->m_version;
}
//----------------------------------------------------------------------------------------------
ScenegraphManager* VistasEngine::GetScenegraphManager(void) const
{
	return m_pVistasEngineState->m_pScenegraphManager;
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetManager* VistasEngine::GetVisibleEntitySetManager(void) const
{
	return m_pVistasEngineState->m_pVisibleEntitySetManager;
}
//----------------------------------------------------------------------------------------------
EffectManager* VistasEngine::GetEffectManager(void) const
{
	return m_pVistasEngineState->m_pEffectManager;
}
//----------------------------------------------------------------------------------------------
CameraManager* VistasEngine::GetCameraManager(void) const
{
	return m_pVistasEngineState->m_pCameraManager;
}
//----------------------------------------------------------------------------------------------
LightManager* VistasEngine::GetLightManager(void) const
{
	return m_pVistasEngineState->m_pLightManager;
}
//----------------------------------------------------------------------------------------------
MaterialManager* VistasEngine::GetMaterialManager(void) const
{
	return m_pVistasEngineState->m_pMaterialManager;
}
//----------------------------------------------------------------------------------------------
CullerManager* VistasEngine::GetCullerManager(void) const
{
	return m_pVistasEngineState->m_pCullerManager;
}
//----------------------------------------------------------------------------------------------
RendererManager* VistasEngine::GetRendererManager(void) const
{
	return m_pVistasEngineState->m_pRendererManager;
}
//----------------------------------------------------------------------------------------------
TextureManager* VistasEngine::GetTextureManager(void) const
{
	return m_pVistasEngineState->m_pTextureManager;
}
//----------------------------------------------------------------------------------------------
RepositoryManager* VistasEngine::GetRepositoryManager(void) const
{
	return m_pVistasEngineState->m_pRepositoryManager;
}
//----------------------------------------------------------------------------------------------
GeometryManager* VistasEngine::GetGeometryManager(void) const
{
	return m_pVistasEngineState->m_pGeometryManager;
}
//----------------------------------------------------------------------------------------------
WindowMessageManager* VistasEngine::GetWindowMessageManager(void) const
{
	return m_pVistasEngineState->m_pWindowMessageManager;
}
//----------------------------------------------------------------------------------------------
HardwareResourceManager* VistasEngine::GetHardwareResourceManager(void) const
{
	return m_pVistasEngineState->m_pRenderDeviceManager->GetActive()->GetHardwareResourceManager();
}
//----------------------------------------------------------------------------------------------
RenderDeviceManager* VistasEngine::GetRenderDeviceManager(void) const
{
	return m_pVistasEngineState->m_pRenderDeviceManager;
}

Meson_Vistas_END