//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "VstHardwareResourceFactory.h"
#include "DX9Definitions.h"
#include "DX9VertexDeclaration.h"
#include "DX9TextureBuffer.h"
#include "DX9VertexBuffer.h"
#include "DX9IndexBuffer.h"

Meson_Vistas_BEGIN
	class DX9HardwareResourceFactory : public IHardwareResourceFactory
	{
	public:
		//----------------------------------------------------------------------------------------------
		DX9HardwareResourceFactory(LPDIRECT3DDEVICE9 p_pD3DDevice) :
		  m_pD3DDevice(p_pD3DDevice)
		{
		}
		//----------------------------------------------------------------------------------------------
		virtual TextureBuffer* CreateTextureBuffer2D(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiWidth, uint p_uiHeight, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage)
		{
			return new DX9TextureBuffer(m_pD3DDevice, p_eTextureFormat, TextureBuffer::TT_2D, p_uiWidth, p_uiHeight, 0, p_uiMipmapLevels, p_dwPool, p_dwUsage);
		}
		//----------------------------------------------------------------------------------------------
		virtual TextureBuffer* CreateTextureBuffer3D(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage)
		{
			return new DX9TextureBuffer(m_pD3DDevice, p_eTextureFormat, TextureBuffer::TT_3D, p_uiWidth, p_uiHeight, p_uiDepth, p_uiMipmapLevels, p_dwPool, p_dwUsage);
		}
		//----------------------------------------------------------------------------------------------
		virtual TextureBuffer* CreateTextureBufferCube(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiEdgeLength, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage)
		{
			return new DX9TextureBuffer(m_pD3DDevice, p_eTextureFormat, TextureBuffer::TT_CUBEMAP, p_uiEdgeLength, 0, 0, p_uiMipmapLevels, p_dwPool, p_dwUsage);
		}
		//----------------------------------------------------------------------------------------------
		virtual TextureBuffer* CreateTextureBuffer(TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureType p_eTextureType, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage)
		{
			return new DX9TextureBuffer(m_pD3DDevice, p_eTextureFormat, p_eTextureType, p_uiWidth, p_uiHeight, p_uiDepth, p_uiMipmapLevels, p_dwPool, p_dwUsage);
		}
		//----------------------------------------------------------------------------------------------
		virtual TextureBuffer* CreateTextureBufferFromFile(const Meson::Common::Text::String& p_strFilename, TextureBuffer::TextureType p_eTextureType)
		{
			return new DX9TextureBuffer(m_pD3DDevice, p_strFilename, p_eTextureType);
		}
		//----------------------------------------------------------------------------------------------
		virtual void DestroyTextureBuffer(TextureBuffer* p_pTextureBuffer)
		{
			SAFE_DELETE(p_pTextureBuffer);
		}
		//----------------------------------------------------------------------------------------------
		virtual IndexBuffer* CreateIndexBuffer(IndexBuffer::IndexType p_eIndexType, uint p_uiIndexCount, dword p_dwPool, dword p_dwUsage)
		{
			return new DX9IndexBuffer(m_pD3DDevice, p_eIndexType, p_uiIndexCount, p_dwPool, p_dwUsage);
		}
		//----------------------------------------------------------------------------------------------
		virtual void DestroyIndexBuffer(IndexBuffer* p_pIndexBuffer)
		{
			SAFE_DELETE(p_pIndexBuffer);
		}
		//----------------------------------------------------------------------------------------------
		VertexBuffer* CreateVertexBuffer(uint p_uiVertexSize, uint p_uiVertexCount, dword p_dwPool, dword p_dwUsage) 
		{
			return new DX9VertexBuffer(m_pD3DDevice, p_uiVertexSize, p_uiVertexCount, p_dwPool, p_dwUsage);
		}
		//----------------------------------------------------------------------------------------------
		void DestroyVertexBuffer(VertexBuffer* p_pVertexBuffer) 
		{
			SAFE_DELETE(p_pVertexBuffer);
		}
		//----------------------------------------------------------------------------------------------
		VertexDeclaration* CreateVertexDeclaration(void)
		{
			return new DX9VertexDeclaration(m_pD3DDevice);
		}
		//----------------------------------------------------------------------------------------------
		void DestroyVertexDeclaration(VertexDeclaration* p_pVertexDeclaration)
		{
			SAFE_DELETE(p_pVertexDeclaration);
		}
		//----------------------------------------------------------------------------------------------
	protected:
		LPDIRECT3DDEVICE9 m_pD3DDevice;
	};
Meson_Vistas_END
