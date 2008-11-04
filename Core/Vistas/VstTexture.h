//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once
#include "Types.h"
#include "VstTypes.h"
#include "VstTextureBuffer.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	// TODO: 1. Extend texture object to allow for an image shadow buffer.
	//		 2. Provide more properties as to what regards the contained texture buffer object.
	//----------------------------------------------------------------------------------------------
	class VISTAS_API Texture : public Meson::Common::TReferenceCounter<Texture>
	{
		friend class TextureManager;

	protected:
		Texture(const Meson::Common::Text::String& p_strName, TextureBufferPtr p_pTextureBuffer);
		~Texture(void);

	public:
		const Meson::Common::Text::String& GetName(void) const;
		TextureBufferPtr GetTextureBuffer(void);

		TextureBuffer::TextureType GetType(void) const;
		TextureBuffer::TextureFormat GetFormat(void) const;

	private:
		Meson::Common::Text::String m_strName;
		TextureBufferPtr m_pTextureBuffer;
	};
	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<Texture> TexturePtr;
	typedef Meson::Common::Collections::TEnumerator<TexturePtr>& TextureEnumerator;
	typedef Meson::Common::Collections::TArrayList<TexturePtr> TextureList;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
