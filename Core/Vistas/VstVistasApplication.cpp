#include "MesonException.h"

#include "VstVistasApplication.h"
#include "VstSceneNode.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Config;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
VistasApplication::VistasApplication(const String& p_strDeviceName, const String& p_strTitle, uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, bool p_bInstrumentationDriver)
	: WindowMessageHandler()
	, m_pVistasEngine(VistasEngine::GetInstance())
	, m_strTitle(p_strTitle)
	, m_uiWidth(p_uiWidth)
	, m_uiHeight(p_uiHeight)
	, m_bIsFullScreen(p_bFullScreen)
	, m_bIsInstrumentationDriver(p_bInstrumentationDriver)
	, m_strDeviceName(p_strDeviceName)
{
}
//----------------------------------------------------------------------------------------------
VistasApplication::~VistasApplication(void)
{
	m_pVistasEngine->Delete();
}
//----------------------------------------------------------------------------------------------
void VistasApplication::Run(void)
{
	OnLoadPlugIns();
	OnInitialise();

	Dispatch();

	OnShutdown();
	OnUnloadPlugIns();
}
//----------------------------------------------------------------------------------------------
void VistasApplication::OnIdle(void) { }
//----------------------------------------------------------------------------------------------
void VistasApplication::OnLoadPlugIns(void) { }
//----------------------------------------------------------------------------------------------
void VistasApplication::OnUnloadPlugIns(void) { }
//----------------------------------------------------------------------------------------------
void VistasApplication::OnInitialise(void)
{
	// Verify availability of Direct3D device
	if (!m_pVistasEngine->GetRenderDeviceManager()->IsAvailable(m_strDeviceName))
		throw new MesonException(m_strDeviceName + " render device is not available!", __FILE__, __LINE__);

	// Configure device
	Configuration *pDeviceConfig = m_pVistasEngine->GetRenderDeviceManager()->GetConfiguration(m_strDeviceName);
	pDeviceConfig->Set("Initialise.CreateWindow.Title", m_strTitle);
	pDeviceConfig->SetLong("Initialise.CreateWindow.Width", m_uiWidth);
	pDeviceConfig->SetLong("Initialise.CreateWindow.Height", m_uiHeight);
	if (m_bIsFullScreen) pDeviceConfig->SetBool("Initialise.CreateWindow.FullScreen", m_bIsFullScreen);
	if (m_bIsInstrumentationDriver) pDeviceConfig->SetBool("Adapter.PerformanceHud.Enabled", m_bIsInstrumentationDriver);

	// Startup and alias active renderdevice
	WindowMessageHandler::Hook(m_pVistasEngine->GetRenderDeviceManager()->Startup(m_strDeviceName));
	m_pRenderDevice = m_pVistasEngine->GetRenderDeviceManager()->GetActive();
}
//----------------------------------------------------------------------------------------------
void VistasApplication::OnShutdown(void)
{
	// Remove registered nodes
	DestroyRegisteredNodes();

	// Terminate window
	WindowMessageHandler::Terminate();
	
	// Release renderer pointer
	m_pRenderDevice = NULL;

	// Shutdown renderer
	m_pVistasEngine->GetRenderDeviceManager()->Shutdown();
}
//----------------------------------------------------------------------------------------------
void VistasApplication::RegisterNode(ISceneNode* p_pNode)
{
	if (m_nodeMap.ContainsKey(p_pNode->GetId()))
		throw Meson::Common::MesonException("A node with the same id has already been registered.", __FILE__, __LINE__);

	m_nodeMap.Insert(p_pNode->GetId(), p_pNode);
	m_nodeList.Add(p_pNode);
}
//----------------------------------------------------------------------------------------------
ISceneNode* VistasApplication::GetNode(const Meson::Common::Text::String& p_strId)
{
	ISceneNode* pSceneNode = NULL;

	m_nodeMap.ContainsKey(p_strId, pSceneNode);

	return pSceneNode;
}
//----------------------------------------------------------------------------------------------
void VistasApplication::UnregisterNode(ISceneNode* p_pNode)
{
	if (m_nodeMap.ContainsKey(p_pNode->GetId()))
	{
		m_nodeMap.RemoveKey(p_pNode->GetId());
		m_nodeList.Remove(p_pNode);
	}
}
//----------------------------------------------------------------------------------------------
void VistasApplication::UnregisterNode(const Meson::Common::Text::String& p_strId)
{
	ISceneNode* pSceneNode = NULL;

	if (m_nodeMap.ContainsKey(p_strId, pSceneNode))
	{
		m_nodeMap.RemoveKey(p_strId);
		m_nodeList.Remove(pSceneNode);
	}
}
//----------------------------------------------------------------------------------------------
void VistasApplication::DestroyRegisteredNodes(void)
{
	Meson::Common::Collections::TEnumerator<ISceneNode*>& nodeEnumerator = m_nodeList.GetEnumerator();

	while(nodeEnumerator.HasMoreElements())
		delete nodeEnumerator.NextElement();

	m_nodeMap.Clear();
	m_nodeList.Clear();
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
