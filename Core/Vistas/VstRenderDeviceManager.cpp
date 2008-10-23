//----------------------------------------------------------------------------------------------
//	Meson::Vistas::VistasEngine
//	Vistas 3D visualisation engine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstRenderDeviceManager.h"
#include "VstRenderDevice.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Config;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
RenderDeviceManager::RenderDeviceManager(void)
	: m_pActiveRenderDevice(NULL)
{
}
//----------------------------------------------------------------------------------------------
RenderDeviceManager::~RenderDeviceManager(void)
{
	if (m_pActiveRenderDevice != NULL)
		m_pActiveRenderDevice->Shutdown();
}
//----------------------------------------------------------------------------------------------
void RenderDeviceManager::RegisterRenderDevice(Meson::Vistas::IRenderDevice* p_pRenderDevice)
{
	IRenderDevice* pRenderDevice;
	
	if (m_renderDeviceMap.ContainsKey(p_pRenderDevice->GetName(), pRenderDevice))
	{
		if (GetActive() == pRenderDevice)
			throw new MesonException("Cannot replace active render device! Stop active device before providing a replacement.", __FILE__, __LINE__);
	}

	m_renderDeviceMap[p_pRenderDevice->GetName()] = p_pRenderDevice;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceManager::UnregisterRenderDevice(Meson::Vistas::IRenderDevice* p_pRenderDevice)
{
	IRenderDevice* pRenderDevice;
	
	if (m_renderDeviceMap.ContainsKey(p_pRenderDevice->GetName(), pRenderDevice))
	{
		if (GetActive() == pRenderDevice)
			throw new MesonException("Cannot unload an active render device! Stop device before unregistering.", __FILE__, __LINE__);

		m_renderDeviceMap.RemoveKey(p_pRenderDevice->GetName());
	}
}
//----------------------------------------------------------------------------------------------
IRenderDevice* RenderDeviceManager::GetActive(void) const
{
	return m_pActiveRenderDevice;
}
//----------------------------------------------------------------------------------------------
bool RenderDeviceManager::IsAvailable(const Meson::Common::Text::String& p_strRenderDeviceName)
{
	return m_renderDeviceMap.ContainsKey(p_strRenderDeviceName);
}
//----------------------------------------------------------------------------------------------
Configuration* RenderDeviceManager::GetConfiguration(const Meson::Common::Text::String& p_strRenderDeviceName)
{
	IRenderDevice* pRenderDevice;
	
	if (m_renderDeviceMap.ContainsKey(p_strRenderDeviceName, pRenderDevice))
		return pRenderDevice->GetConfiguration();

	return NULL;
}
//----------------------------------------------------------------------------------------------
WindowRenderTarget* RenderDeviceManager::Startup(const Meson::Common::Text::String& p_strRenderDeviceName)
{
	if (m_pActiveRenderDevice != NULL)
		throw new MesonException("Cannot start a render device while another is already active.", __FILE__, __LINE__);

	WindowRenderTarget* pWindowRenderTarget = NULL;
	IRenderDevice* pRenderDevice;
	
	if (m_renderDeviceMap.ContainsKey(p_strRenderDeviceName, pRenderDevice))
	{
		pWindowRenderTarget = pRenderDevice->Startup();
		m_pActiveRenderDevice = pRenderDevice;
	}

	return pWindowRenderTarget;
}
//----------------------------------------------------------------------------------------------
void RenderDeviceManager::Shutdown(void)
{
	if (m_pActiveRenderDevice != NULL)
	{
		m_pActiveRenderDevice->Shutdown();
		m_pActiveRenderDevice = NULL;
	}
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
