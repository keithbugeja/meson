//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9RendererPlugIn
//	PlugIn interface for Vistas Direct3D9 renderer.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "DX9RendererPlugIn.h"
#include "VstVistasEngine.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
///	DX9RendererPlugIn Constructor
//----------------------------------------------------------------------------------------------
DX9RendererPlugIn::DX9RendererPlugIn(void) 
	: m_pName("DirectX9 PlugIn"), 
	  m_version(0,0,1), 
	  m_pRenderDevice(NULL)
{
}

//----------------------------------------------------------------------------------------------
///	DX9RendererPlugIn Destructor
//----------------------------------------------------------------------------------------------
DX9RendererPlugIn::~DX9RendererPlugIn(void)
{
}

//----------------------------------------------------------------------------------------------
/// Returns the name of the Plug-In
///	\return Plug-In name
//----------------------------------------------------------------------------------------------
const String& DX9RendererPlugIn::GetName(void) const
{
	return m_pName;
}

//----------------------------------------------------------------------------------------------
/// Returns the version number of the Plug-In
/// \return Version number
//----------------------------------------------------------------------------------------------
const Version& DX9RendererPlugIn::GetVersion(void) const
{
	return m_version;
}

//----------------------------------------------------------------------------------------------
/// Registers the Plug-In with the Engine subsystem
//----------------------------------------------------------------------------------------------
void DX9RendererPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	VistasEngine* pVistasEngine = (VistasEngine*)m_pSharedObjectMap->GetRegisteredObject("VistasEngine");
	MESON_ASSERT(pVistasEngine != NULL, "VistasEngine is not a registered shared object.");

	VistasEngine::SetInstance(pVistasEngine);
	pVistasEngine->Logger().Out << "Registering Direct3D9 Renderer...\n";

	if (m_pRenderDevice == NULL)
	{
		m_pRenderDevice = new DX9RenderDevice();
		pVistasEngine->GetRenderDeviceManager()->RegisterRenderDevice(m_pRenderDevice);
		pVistasEngine->Logger().Out << "Registered Direct3D9 Renderer.\n";
	}
}

//----------------------------------------------------------------------------------------------
/// Initialises the Plug-In
//----------------------------------------------------------------------------------------------
void DX9RendererPlugIn::Initialise(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Shuts the Plug-In down
//----------------------------------------------------------------------------------------------
void DX9RendererPlugIn::Shutdown(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Unregisters the Plug-In from the engine subsystem.
//----------------------------------------------------------------------------------------------
void DX9RendererPlugIn::Unregister(void) 
{ 
	VistasEngine* pVistasEngine = VistasEngine::GetInstance();
	pVistasEngine->GetInstance()->Logger().Out << "Unregistered Direct3D9 Renderer...\n";

	if (m_pRenderDevice != NULL)
	{
		//pVistasEngine->RemoveRenderDevice( m_pRenderDevice );
		pVistasEngine->GetRenderDeviceManager()->UnregisterRenderDevice(m_pRenderDevice);
		delete m_pRenderDevice;
		m_pRenderDevice = NULL;
	}

	VistasEngine::SetInstance(NULL);
}

Meson_Vistas_END
