//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "VstTextureBuffer.h"
#include "DX9Definitions.h"

Meson_Vistas_BEGIN
	class DX9TextureBuffer : public TextureBuffer
	{
	public:
		DX9TextureBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureType p_eTextureType, 
			uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage);

		DX9TextureBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, const Meson::Common::Text::String& p_strFilename, TextureBuffer::TextureType p_eTextureType);
		~DX9TextureBuffer(void);

		void Unlock(void);
		void Lock(dword p_dwOptions);
		void Lock(uint p_uiLeft, uint p_uiTop, uint p_uiWidth, uint p_uiHeight, uint p_uiLevel, dword p_dwLockOptions);
		
		void Read(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, byte* p_pbyBuffer);
		void Write(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, const byte* p_pbyBuffer);

		LPDIRECT3DBASETEXTURE9 GetTexture(void) const;

		LPDIRECT3DTEXTURE9 GetTexture2D(void) const;
		LPDIRECT3DCUBETEXTURE9 GetTextureCube(void) const;
		LPDIRECT3DVOLUMETEXTURE9 GetTextureVolume(void) const;

	protected:
		LPDIRECT3DDEVICE9			m_pD3DDevice9;

		LPDIRECT3DBASETEXTURE9		m_pD3DTexture9;
		
		LPDIRECT3DTEXTURE9			m_pD3DTexture2D;
		LPDIRECT3DCUBETEXTURE9		m_pD3DTextureCube;
		LPDIRECT3DVOLUMETEXTURE9	m_pD3DTextureVolume;
		
		D3DLOCKED_RECT		m_d3dLockedRect;
		
		uint m_uiLockLevel;
		byte* m_pbyData;

		static D3DFORMAT m_d3dTextureFormat[TextureBuffer::TF_COUNT];
	};
Meson_Vistas_END
