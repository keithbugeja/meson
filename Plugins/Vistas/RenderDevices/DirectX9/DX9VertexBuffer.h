//----------------------------------------------------------------------------------------------
//	Meson::Vistas::
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "VstVertexBuffer.h"
#include "DX9Definitions.h"

Meson_Vistas_BEGIN
	class DX9VertexBuffer : public VertexBuffer
	{
	public:
		DX9VertexBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, uint p_uiVertexSize, uint p_uiVertexCount, dword p_dwPool, dword p_dwUsage);
		~DX9VertexBuffer(void);

		void Unlock(void);
		void Lock(uint p_uiStart, uint p_uiSize, dword p_dwOptions);
		void Lock(dword p_dwOptions);
		
		void Read(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, byte* p_pbyBuffer);
		void Write(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, const byte* p_pbyBuffer);

	protected:
		LPDIRECT3DDEVICE9 m_pD3DDevice9;
		LPDIRECT3DVERTEXBUFFER9 m_pD3DVertexBuffer9;
		
		byte* m_pbyData;
	};
Meson_Vistas_END
