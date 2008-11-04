//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"

#include "VstTypes.h"
#include "VstTexture.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	// TODO: This is a bare bones texture manager; eventually this should be extended to allow 
	//		 derivation from a ResourceManager and provide resource loading options.
	//----------------------------------------------------------------------------------------------
	class VISTAS_API TextureManager : public Meson::Common::TSingleton<TextureManager>
	{
		friend class Meson::Common::TSingleton<TextureManager>;

	private: 
		TextureManager(void);
		~TextureManager(void);

	public:
		TexturePtr CreateTexture1D(const Meson::Common::Text::String& p_strName, TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureUsage p_eTextureUsage, uint p_uiSize); 
		TexturePtr CreateTexture2D(const Meson::Common::Text::String& p_strName, TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureUsage p_eTextureUsage, uint p_uiWidth, uint p_uiHeight); 
		TexturePtr CreateTexture3D(const Meson::Common::Text::String& p_strName, TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureUsage p_eTextureUsage, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth); 
		TexturePtr CreateTextureCube(const Meson::Common::Text::String& p_strName, TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureUsage p_eTextureUsage, uint p_uiEdgeLength); 

		TexturePtr CreateTextureFromTextureBuffer(const Meson::Common::Text::String& p_strName, TextureBufferPtr p_pTextureBuffer);
		TexturePtr CreateTextureFromFile(const Meson::Common::Text::String& p_strName, const Meson::Common::Text::String& p_strFilename, TextureBuffer::TextureType p_eTextureType=TextureBuffer::TT_2D);
	
		bool ContainsTexture(const Meson::Common::Text::String& p_strName);
		bool ContainsTexture(const Meson::Common::Text::String& p_strName, TexturePtr& p_pTexture);

		TexturePtr GetTexture(const Meson::Common::Text::String& p_strName);

		void ReleaseTexture(TexturePtr p_pTexture);
		void ReleaseTexture(const Meson::Common::Text::String& p_strName);
		void ReleaseAllTextures(void);

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, TexturePtr> m_textureBufferMap;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
