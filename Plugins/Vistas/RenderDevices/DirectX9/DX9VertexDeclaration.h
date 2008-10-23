//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "VstVertexDeclaration.h"

Meson_Vistas_BEGIN
	class DX9VertexDeclaration : public VertexDeclaration
	{
	public:
		DX9VertexDeclaration(LPDIRECT3DDEVICE9 p_pD3DDevice);
		~DX9VertexDeclaration(void);

		LPDIRECT3DVERTEXDECLARATION9 GetVertexDeclaration(bool p_bRegenerate = false);
		
		void AddVertexElement(uint p_uiStream, uint p_uiOffset, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex);
		void AppendVertexElement(uint p_uiStream, VertexDataType p_eVertexDataType, VertexDataFunction p_eVertexDataFunction, uint p_uiUsageIndex);
		void RemoveVertexElement(uint p_uiStream, uint p_uiOffset);
		void RemoveAllVertexElements(void);

	protected:
		void Refresh(void);

		static byte m_byVertexDataType[VDT_COUNT];
		static byte m_byVertexDataFunction[VDF_COUNT];
		
		LPDIRECT3DVERTEXDECLARATION9	m_pD3DVertexDeclaration;
		LPD3DVERTEXELEMENT9				m_pD3DVertexElement;
		LPDIRECT3DDEVICE9				m_pD3DDevice;

		bool m_bIsStale;
	};
Meson_Vistas_END