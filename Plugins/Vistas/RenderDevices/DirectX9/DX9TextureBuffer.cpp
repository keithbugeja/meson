//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include <d3dx9tex.h>
#include "MesonException.h"
#include "VstVistasEngine.h"
#include "VstHardwareResourceManager.h"
#include "DX9TextureBuffer.h"

using namespace Meson::Common::Text;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
D3DFORMAT DX9TextureBuffer::m_d3dTextureFormat[TextureBuffer::TF_COUNT] = 
{
	D3DFMT_A2R10G10B10,			//TF_A2R10G10B10
	D3DFMT_A8R8G8B8,			//TF_A8R8G8B8
	D3DFMT_X8R8G8B8,			//TF_X8R8G8B8,
	D3DFMT_A1R5G5B5,			//TF_A1R5G5B5,
	D3DFMT_X1R5G5B5,			//TF_X1R5G5B5,
	D3DFMT_R5G6B5,				//TF_R5G6B5,

	D3DFMT_D16,					//TF_D16,
	D3DFMT_D32,					//TF_D32,
	D3DFMT_D15S1,				//TF_D15S1,
	D3DFMT_D24S8,				//TF_D24S8,
	D3DFMT_D24X8,				//TF_D24X8,
	D3DFMT_D24X4S4,				//TF_D24X4S4,

	D3DFMT_DXT1,				//TF_DXT1,
	D3DFMT_DXT2,				//TF_DXT2,
	D3DFMT_DXT3,				//TF_DXT3,
	D3DFMT_DXT4,				//TF_DXT4,
	D3DFMT_DXT5,				//TF_DXT5,

	D3DFMT_R16F,				//TF_R16F,
	D3DFMT_G16R16F,				//TF_G16R16F,
	D3DFMT_A16B16G16R16F,		//TF_A16B16G16R16F,

	D3DFMT_R32F,				//TF_R32F,
	D3DFMT_G32R32F,				//TF_G32R32F,
	D3DFMT_A32B32G32R32F		//TF_A32B32G32R32F
};
//----------------------------------------------------------------------------------------------
DX9TextureBuffer::DX9TextureBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureType p_eTextureType, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage)
	: TextureBuffer(p_eTextureFormat, p_eTextureType, p_uiWidth, p_uiHeight, p_uiDepth, p_uiMipmapLevels, p_dwPool, p_dwUsage )
	, m_pD3DDevice9(p_pD3DDevice9)
	, m_pD3DTextureVolume(NULL)
	, m_pD3DTextureCube(NULL)
	, m_pD3DTexture2D(NULL)
	, m_pD3DTexture9(NULL)
	, m_pbyData(NULL)
{
	D3DPOOL d3dPool;
	DWORD dwUsage = 0;
	D3DFORMAT d3dFormat = m_d3dTextureFormat[p_eTextureFormat];

	m_bIsLocked = false;

	// Texture usage
	if (p_dwUsage & TextureBuffer::TU_AUTOGEN_MIPMAP)
		dwUsage |= D3DUSAGE_AUTOGENMIPMAP;

	/** If texutre is used as a render target, the memory pool 
		must be set to D3DPOOL_DEFAULT.
	 */
	if (p_dwUsage & TextureBuffer::TU_RENDERTARGET)
	{
		dwUsage |= D3DUSAGE_RENDERTARGET;
		d3dPool = D3DPOOL_DEFAULT;
	}
	else
	{
		switch( p_dwPool )
		{
			case HardwareResource::MP_DEFAULT:
			default:
				d3dPool = D3DPOOL_MANAGED;
		}
	}

	// Determine texture type
	switch (p_eTextureType)
	{
		case TT_2D:
		{
			if (D3D_OK != p_pD3DDevice9->CreateTexture(p_uiWidth, p_uiHeight, p_uiMipmapLevels, dwUsage, d3dFormat, d3dPool, &m_pD3DTexture2D, NULL))
				throw new Meson::Common::MesonException("Failed creating texture buffer.", __FILE__, __LINE__ );

			m_pD3DTexture9 = m_pD3DTexture2D;
			break;
		}

		case TT_3D:
		{
			if (D3D_OK != p_pD3DDevice9->CreateVolumeTexture(p_uiWidth, p_uiHeight, p_uiWidth, p_uiMipmapLevels, dwUsage, d3dFormat, d3dPool, &m_pD3DTextureVolume, NULL))
				throw new Meson::Common::MesonException("Failed creating texture buffer.", __FILE__, __LINE__ );

			m_pD3DTexture9 = m_pD3DTextureVolume;
			break;
		}

		case TT_CUBEMAP:
		{
			if (D3D_OK != p_pD3DDevice9->CreateCubeTexture(p_uiWidth, p_uiMipmapLevels, dwUsage, d3dFormat, d3dPool, &m_pD3DTextureCube, NULL))
				throw new Meson::Common::MesonException("Failed creating texture buffer.", __FILE__, __LINE__ );

			m_pD3DTexture9 = m_pD3DTextureCube;
			break;
		}
	}

	m_resourceHandle = (void*)m_pD3DTexture9;
}
//----------------------------------------------------------------------------------------------
DX9TextureBuffer::DX9TextureBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, const String& p_strFilename, TextureBuffer::TextureType p_eTextureType)
	: TextureBuffer(TextureBuffer::TF_A8R8G8B8, p_eTextureType, 0, 0, 0, 0, 0, 0)
	, m_pD3DDevice9(p_pD3DDevice9)
	, m_pD3DTextureVolume(NULL)
	, m_pD3DTextureCube(NULL)
	, m_pD3DTexture2D(NULL)
	, m_pD3DTexture9(NULL)
	, m_pbyData(NULL)
{
	switch(p_eTextureType)
	{
		case TT_1D:
		case TT_2D:
		{
			if (D3D_OK != D3DXCreateTextureFromFile(m_pD3DDevice9, p_strFilename, &m_pD3DTexture2D ))
				throw new Meson::Common::MesonException("Failed creating texture buffer from file : make sure file exists.", __FILE__, __LINE__ );
		
			m_pD3DTexture9 = m_pD3DTexture2D;
			break;
		}
		
		case TT_3D:
		{
			if (D3D_OK != D3DXCreateVolumeTextureFromFile(m_pD3DDevice9, p_strFilename, &m_pD3DTextureVolume))
				throw new Meson::Common::MesonException("Failed creating texture buffer from file : make sure file exists.", __FILE__, __LINE__ );
		
			m_pD3DTexture9 = m_pD3DTextureVolume;
			break;
		}

		case TT_CUBEMAP:
		{
			if (D3D_OK != D3DXCreateCubeTextureFromFile(m_pD3DDevice9, p_strFilename, &m_pD3DTextureCube))
				throw new Meson::Common::MesonException("Failed creating texture buffer from file : make sure file exists.", __FILE__, __LINE__ );
		
			m_pD3DTexture9 = m_pD3DTextureCube;
			break;
		}
	}

	m_resourceHandle = (void*)m_pD3DTexture9;
}
//----------------------------------------------------------------------------------------------
DX9TextureBuffer::~DX9TextureBuffer(void)
{
	Unlock();

	switch(m_eTextureType)
	{
		case TT_1D:
		case TT_2D:
			SAFE_RELEASE(m_pD3DTexture2D);
			break;
		
		case TT_3D:
			SAFE_RELEASE(m_pD3DTextureVolume);
			break;

		case TT_CUBEMAP:
			SAFE_RELEASE(m_pD3DTextureCube);
			break;
	}
}
//----------------------------------------------------------------------------------------------
void DX9TextureBuffer::Unlock(void) 
{
	if (m_bIsLocked)
	{
		switch(m_eTextureType)
		{
			case TT_2D:
			{
				m_pD3DTexture2D->UnlockRect(m_uiLockLevel);
				m_bIsLocked = false;
				
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
void DX9TextureBuffer::Lock(uint p_uiLeft, uint p_uiTop, uint p_uiWidth, uint p_uiHeight, uint p_uiLevel, dword p_dwLockOptions)
{
	if (!m_bIsLocked)
	{
		switch(m_eTextureType)
		{
			case TT_2D:
			{
				RECT lockRect = { p_uiLeft, p_uiTop, p_uiWidth + p_uiLeft, p_uiTop + p_uiHeight };
				m_uiLockLevel = p_uiLevel;
			
				if (D3D_OK != m_pD3DTexture2D->LockRect( m_uiLockLevel, &m_d3dLockedRect, &lockRect, p_dwLockOptions))
					throw new Meson::Common::MesonException("Unable to lock texture buffer.", __FILE__, __LINE__ );

				m_bIsLocked = true;
				
				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
void DX9TextureBuffer::Lock(dword p_dwLockOptions) 
{
	if (!m_bIsLocked)
	{
		switch(m_eTextureType)
		{
			case TT_2D:
			{
				m_uiLockLevel = 0;
			
				if (D3D_OK != m_pD3DTexture2D->LockRect( m_uiLockLevel, &m_d3dLockedRect, NULL, p_dwLockOptions))
					throw new Meson::Common::MesonException("Unable to lock texture buffer.", __FILE__, __LINE__ );

				m_bIsLocked = true;

				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
void DX9TextureBuffer::Read(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, byte* p_pbyBuffer)
{
	memcpy(p_pbyBuffer + p_nDestinationOffset, (const byte*)m_d3dLockedRect.pBits + p_nSourceOffset, p_nSize); 
}
//----------------------------------------------------------------------------------------------
void DX9TextureBuffer::Write(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, const byte* p_pbyBuffer)
{
	memcpy((byte*)m_d3dLockedRect.pBits + p_nDestinationOffset, p_pbyBuffer + p_nSourceOffset, p_nSize);
}
//----------------------------------------------------------------------------------------------
LPDIRECT3DBASETEXTURE9 DX9TextureBuffer::GetTexture(void) const
{
	return m_pD3DTexture9;
}
//----------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 DX9TextureBuffer::GetTexture2D(void) const
{
	return m_pD3DTexture2D;
}
//----------------------------------------------------------------------------------------------
LPDIRECT3DCUBETEXTURE9 DX9TextureBuffer::GetTextureCube(void) const
{
	return m_pD3DTextureCube;
}
//----------------------------------------------------------------------------------------------
LPDIRECT3DVOLUMETEXTURE9 DX9TextureBuffer::GetTextureVolume(void) const
{
	return m_pD3DTextureVolume;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
