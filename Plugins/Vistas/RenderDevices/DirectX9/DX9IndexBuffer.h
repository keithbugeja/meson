//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9IndexBuffer
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "VstIndexBuffer.h"
#include "DX9Definitions.h"

Meson_Vistas_BEGIN
	class DX9IndexBuffer : public IndexBuffer
	{
	public:
		DX9IndexBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, IndexType p_eIndexType, uint p_uiIndexCount, dword p_dwPool, dword p_dwUsage);
		~DX9IndexBuffer(void);

		void Unlock(void);

		void Lock(uint p_uiStart, uint p_uiSize, dword p_dwOptions);
		void Lock(dword p_dwOptions);
		
		void Read(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, byte* p_pbyBuffer);
		void Write(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, const byte* p_pbyBuffer);

	protected:
		LPDIRECT3DDEVICE9 m_pD3DDevice9;
		LPDIRECT3DINDEXBUFFER9 m_pD3DIndexBuffer9;
		
		uint m_uiIndexSize;
		byte* m_pbyData;
	};
Meson_Vistas_END
