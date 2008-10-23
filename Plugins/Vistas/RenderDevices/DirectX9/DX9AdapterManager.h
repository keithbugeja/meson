//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "String8.h"
#include "ArrayList.h"
#include "Enumerator.h"
#include "VstNamespaces.h"
#include "DX9AdapterDevice.h"

Meson_Vistas_BEGIN
	typedef Meson::Common::Collections::TEnumerator<DX9AdapterDevice*>& DX9AdapterDeviceEnumerator;

	class DX9AdapterManager
	{
	public:
		DX9AdapterManager( LPDIRECT3D9 p_pD3D9 );
		~DX9AdapterManager(void);

		LPDIRECT3D9 GetDirect3D(void);
		
		DX9AdapterDeviceEnumerator GetAdapterEnumerator(void);
		uint GetAdapterCount(void);

		DX9AdapterDevice* GetAdapterByDeviceID(uint p_uiDeviceID);
		DX9AdapterDevice* GetAdapterByDeviceName(Meson::Common::Text::String p_strDeviceName);
		DX9AdapterDevice* GetDefaultAdapter(void);

	protected:
		LPDIRECT3D9	m_pD3D9;

		Meson::Common::Collections::TArrayList<DX9AdapterDevice*> m_adapterDeviceList;
	};
Meson_Vistas_END
