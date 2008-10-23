//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9AdapterManager
//	Contains an adapter-bound Direct3D device for specific display metrics.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "DX9AdapterManager.h"
#include "VstVistasEngine.h"
#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
DX9AdapterManager::DX9AdapterManager( LPDIRECT3D9 p_pD3D9 ) 
	: m_pD3D9(p_pD3D9)
{
	if (m_pD3D9 == NULL)
		throw new MesonException("Invalid Direct3D9 object: cannot instantiate DX9AdapterManager.", __FILE__, __LINE__);

	DX9AdapterDevice* pAdapterDevice = NULL;

	for (uint uiAdapter = 0; uiAdapter < m_pD3D9->GetAdapterCount(); uiAdapter++)
	{
		pAdapterDevice = new DX9AdapterDevice(p_pD3D9, uiAdapter);  		
		m_adapterDeviceList.Add(pAdapterDevice);
	}
}
//----------------------------------------------------------------------------------------------
DX9AdapterManager::~DX9AdapterManager(void)
{
	for (uint uiIndex = 0; uiIndex < m_adapterDeviceList.Size(); uiIndex++)
		delete m_adapterDeviceList[uiIndex];

	m_adapterDeviceList.Clear();
}

//----------------------------------------------------------------------------------------------
LPDIRECT3D9 DX9AdapterManager::GetDirect3D(void)
{
	return m_pD3D9;
}
//----------------------------------------------------------------------------------------------
DX9AdapterDeviceEnumerator DX9AdapterManager::GetAdapterEnumerator(void)
{
	return m_adapterDeviceList.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
uint DX9AdapterManager::GetAdapterCount(void)
{
	return (uint)m_adapterDeviceList.Size();
}
//----------------------------------------------------------------------------------------------
DX9AdapterDevice* DX9AdapterManager::GetAdapterByDeviceID(uint p_uiDeviceID)
{
	if (p_uiDeviceID < m_adapterDeviceList.Size())
		return m_adapterDeviceList[p_uiDeviceID];

	throw new MesonException("Cannot return DX9AdapterDevice: requested device id is invalid.", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
DX9AdapterDevice* DX9AdapterManager::GetAdapterByDeviceName(String p_strDeviceName)
{
	DX9AdapterDevice* pAdapterDevice = NULL;
	DX9AdapterDeviceEnumerator adapterEnumerator = GetAdapterEnumerator();

	while(adapterEnumerator.HasMoreElements())
	{
		pAdapterDevice = adapterEnumerator.NextElement();
		if (!pAdapterDevice->GetDeviceName().CompareIgnoreCase(p_strDeviceName))
			return pAdapterDevice;
	}

	return NULL;
}
//----------------------------------------------------------------------------------------------
DX9AdapterDevice* DX9AdapterManager::GetDefaultAdapter(void)
{
	return m_adapterDeviceList[0];
}

Meson_Vistas_END
