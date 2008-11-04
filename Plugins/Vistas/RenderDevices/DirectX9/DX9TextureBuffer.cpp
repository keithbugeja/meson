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

using namespace Meson::Common;
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
	DWORD dwUsage = 0;
	D3DPOOL d3dPool = D3DPOOL_DEFAULT;
	D3DFORMAT d3dFormat = m_d3dTextureFormat[p_eTextureFormat];

	// Texture is unlocked
	m_bIsLocked = false;

	// Texture usage
	if (p_dwUsage & TextureBuffer::TU_AUTOGEN_MIPMAP)
		dwUsage |= D3DUSAGE_AUTOGENMIPMAP;

	// If texutre is used as a render target, the memory pool 
	// must be set to D3DPOOL_DEFAULT.
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
		// 1D and 2D textures
		case TT_1D:
		case TT_2D:
		{
			if (D3D_OK != p_pD3DDevice9->CreateTexture(p_uiWidth, p_uiHeight, p_uiMipmapLevels, dwUsage, d3dFormat, d3dPool, &m_pD3DTexture2D, NULL))
				throw new Meson::Common::MesonException("Failed creating texture buffer.", __FILE__, __LINE__ );

			m_pD3DTexture9 = m_pD3DTexture2D;
			break;
		}

		// 3D (volume) textures
		case TT_3D:
		{
			if (D3D_OK != p_pD3DDevice9->CreateVolumeTexture(p_uiWidth, p_uiHeight, p_uiWidth, p_uiMipmapLevels, dwUsage, d3dFormat, d3dPool, &m_pD3DTextureVolume, NULL))
				throw new Meson::Common::MesonException("Failed creating texture buffer.", __FILE__, __LINE__ );

			m_pD3DTexture9 = m_pD3DTextureVolume;
			break;
		}

		// Cubemaps (6x2D) textures
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
			// Load texture from file
			if (D3D_OK != D3DXCreateTextureFromFile(m_pD3DDevice9, p_strFilename, &m_pD3DTexture2D ))
				throw new Meson::Common::MesonException("Failed creating texture buffer from file : make sure file exists.", __FILE__, __LINE__ );
		
			// Set generic texture pointer
			m_pD3DTexture9 = m_pD3DTexture2D;
			
			// Read width, height and depth from loaded texture
			//m_uiWidth = m_pD3DTexture2D->Width;
			//m_uiHeight = m_pD3DTexture2D->Height;
			//m_uiDepth = 1;
			//
			//// Get mipmap levels and texture format
			//m_uiMipMapLevels = m_pD3DTexture2D->Levels;
			//m_eTextureFormat = ConvertTextureFormat(m_pD3DTexture2D->Format);

			break;
		}
		
		case TT_3D:
		{
			// Load texture from file
			if (D3D_OK != D3DXCreateVolumeTextureFromFile(m_pD3DDevice9, p_strFilename, &m_pD3DTextureVolume))
				throw new Meson::Common::MesonException("Failed creating texture buffer from file : make sure file exists.", __FILE__, __LINE__ );
		
			// Set generic texture pointer
			m_pD3DTexture9 = m_pD3DTextureVolume;

			// Read width, height and depth from loaded texture
			//m_uiWidth = m_pD3DTextureVolume->Width;
			//m_uiHeight = m_pD3DTextureVolume->Height;
			//m_uiDepth = m_pD3DTextureVolume->Depth;

			//// Get mipmap levels and texture format
			//m_uiMipMapLevels = m_pD3DTextureVolume->Levels;
			//m_eTextureFormat = ConvertTextureFormat(m_pD3DTextureVolume->Format);

			break;
		}

		case TT_CUBEMAP:
		{
			// Load texture from file
			if (D3D_OK != D3DXCreateCubeTextureFromFile(m_pD3DDevice9, p_strFilename, &m_pD3DTextureCube))
				throw new Meson::Common::MesonException("Failed creating texture buffer from file : make sure file exists.", __FILE__, __LINE__ );
		
			// Set generic texture pointer
			m_pD3DTexture9 = m_pD3DTextureCube;

			// Read width, height and depth from loaded texture
			//m_uiWidth = m_pD3DTextureCube->Width;
			//m_uiHeight = m_pD3DTextureCube->Height;
			//m_uiDepth = 1;
			//
			//// Get mipmap levels and texture format
			//m_uiMipMapLevels = m_pD3DTextureCube->Levels;
			//m_eTextureFormat = ConvertTextureFormat(m_pD3DTextureCube->Format);
			
			break;
		}
	}

	// Update resource handle
	m_resourceHandle = (void*)m_pD3DTexture9;

	// Debug information
	VistasEngine::GetInstance()->Logger().Out<<"Texture loaded : ['"<<m_strId<<"', "<<m_eTextureFormat<<", "<<m_uiWidth<<"x"<<m_uiHeight<<"x"<<m_uiDepth<<" : "<<m_uiMipMapLevels<<"]\n";
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
TextureBuffer::TextureFormat DX9TextureBuffer::ConvertTextureFormat(D3DFORMAT p_d3dFormat)
{
	// Note that this could be replaced with a map for reverse lookup, although
	// hashing could prove to be more expensive than iterating and comparing
	for (uint uiIndex = 0; uiIndex < TF_COUNT; uiIndex++)
		if (m_d3dTextureFormat[uiIndex] == p_d3dFormat)
			return (TextureBuffer::TextureFormat)uiIndex;

	// Throw exception if the format is unknown
	throw new Meson::Common::MesonException("Unknown texture format : unable to perform conversion", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
