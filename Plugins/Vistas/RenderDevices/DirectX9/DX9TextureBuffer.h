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
		// Constructor for raw texture
		DX9TextureBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, TextureBuffer::TextureFormat p_eTextureFormat, 
			TextureBuffer::TextureType p_eTextureType, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, 
			uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage);

		// Constructor for file-based texture
		DX9TextureBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, const Meson::Common::Text::String& p_strFilename, 
			TextureBuffer::TextureType p_eTextureType);
		
		// Destructor
		~DX9TextureBuffer(void);

		// Locking and unlocking methods
		void Unlock(void);
		void Lock(dword p_dwOptions);
		void Lock(uint p_uiLeft, uint p_uiTop, uint p_uiWidth, uint p_uiHeight, uint p_uiLevel, dword p_dwLockOptions);
		
		// Data access methods
		void Read(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, byte* p_pbyBuffer);
		void Write(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, const byte* p_pbyBuffer);

		// Texture handle accessors
		LPDIRECT3DTEXTURE9			GetTexture2D(void) const;
		LPDIRECT3DCUBETEXTURE9		GetTextureCube(void) const;
		LPDIRECT3DVOLUMETEXTURE9	GetTextureVolume(void) const;

		LPDIRECT3DBASETEXTURE9		GetTexture(void) const;

		// Convert texture format from Direct3D9 to Vistas
		static TextureFormat ConvertTextureFormat(D3DFORMAT p_d3dFormat);

	protected:
		uint m_uiLockLevel;
		byte* m_pbyData;

		LPDIRECT3DDEVICE9			m_pD3DDevice9;
		LPDIRECT3DBASETEXTURE9		m_pD3DTexture9;
		
		LPDIRECT3DTEXTURE9			m_pD3DTexture2D;
		LPDIRECT3DCUBETEXTURE9		m_pD3DTextureCube;
		LPDIRECT3DVOLUMETEXTURE9	m_pD3DTextureVolume;
		
		D3DLOCKED_RECT				m_d3dLockedRect;
		
		static D3DFORMAT m_d3dTextureFormat[TextureBuffer::TF_COUNT];
	};
Meson_Vistas_END
