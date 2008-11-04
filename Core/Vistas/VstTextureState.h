//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstTexture.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class VISTAS_API TextureState : public Meson::Common::TReferenceCounter<TextureState>
	{
	public:
		enum TextureType
		{
			TT_SIMPLE,
			TT_COMPLEX,
			TT_COMPLEX_ALIAS,
			TT_COUNT
		};
		
	public:
		TextureState(TexturePtr& p_pTexture);
		TextureState(TextureBufferPtr p_pTextureBuffer);
		TextureState(const Meson::Common::Text::String& p_strTextureCatalogue, const Meson::Common::Text::String& p_strTextureName);
		TextureState(const TextureState& p_textureState);

		~TextureState(void);

		TextureType GetType() const;

		void SetTexture(TexturePtr p_pTexture);
		TexturePtr GetTexture(void);

		void SetTextureBuffer(TextureBufferPtr p_pTextureBuffer);
		TextureBufferPtr GetTextureBuffer(void);

		void SetAlias(const Meson::Common::Text::String& p_strTextureCatalogue, const Meson::Common::Text::String& p_strTextureName);
		bool GetAlias(Meson::Common::Text::String& p_strTextureCatalogue, Meson::Common::Text::String& p_strTextureName) const;

		const TextureState& operator= (const TextureState& p_textureState);

	protected:
		TextureType m_eTextureType;

		TexturePtr m_pTexture;
		TextureBufferPtr m_pTextureBuffer;
		
		Meson::Common::Text::String m_strAlias, 
									m_strAliasCatalogue;
	};

	typedef Meson::Common::TPointer<TextureState> TextureStatePtr;

	//----------------------------------------------------------------------------------------------
Meson_Vistas_END
