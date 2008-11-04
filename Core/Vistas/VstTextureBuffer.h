//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstHardwareResource.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class VISTAS_API TextureBuffer : public HardwareResource
	{
		VISTAS_IMPLEMENT_RTTI(TextureBuffer, HardwareResource);

    public:
		enum TextureFormat
		{
			TF_A2R10G10B10,
			TF_A8R8G8B8,
			TF_X8R8G8B8,
			TF_A1R5G5B5,
			TF_X1R5G5B5,
			TF_R5G6B5, 

			TF_D16,
			TF_D32,
			TF_D15S1,
			TF_D24S8,
			TF_D24X8,
			TF_D24X4S4,

			TF_DXT1,
			TF_DXT2,
			TF_DXT3,
			TF_DXT4,
			TF_DXT5,

			TF_R16F,
			TF_G16R16F,
			TF_A16B16G16R16F,

			TF_R32F,
			TF_G32R32F,
			TF_A32B32G32R32F,

			TF_COUNT
		};

		enum TextureType
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_CUBEMAP,

			TT_COUNT
		};

		enum TextureUsage
		{
			TU_AUTOGEN_MIPMAP	= 0x01,
			TU_RENDERTARGET		= 0x02,
			TU_RESERVED			= 0x8000,
			TU_DEFAULT			= TU_RESERVED | TU_AUTOGEN_MIPMAP
		};

	public:
		TextureBuffer(TextureFormat p_eTextureFormat, TextureType p_eTextureType, 
					  uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage);

		virtual void Lock(dword p_dwLockOptions) = 0;
		virtual void Lock(uint p_uiLeft, uint p_uiTop, uint p_uiWidth, uint p_uiHeight, uint p_uiLevel, dword p_dwLockOptions) = 0;

		uint GetWidth(void) const;
		uint GetHeight(void) const;
		uint GetDepth(void) const;
		uint GetMipMapLevels(void) const;

		TextureFormat GetTextureFormat(void) const;
		TextureType GetTextureType(void) const;

	protected:
		TextureFormat m_eTextureFormat;
		TextureType	m_eTextureType;

		uint m_uiWidth, 
			 m_uiHeight,
			 m_uiDepth,
			 m_uiMipMapLevels;
	};
	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<TextureBuffer> TextureBufferPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END