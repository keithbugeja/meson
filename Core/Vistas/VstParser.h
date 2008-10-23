//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Converter.h"

#include "VstTypes.h"
#include "VstLexer.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class IParser : public ILexer 
	{
	public:
		IParser(Meson::Common::IO::IInputStream* p_pInputStream)
			: ILexer(p_pInputStream) {}

		virtual ~IParser(void) {}

		virtual void Initialise(void) = 0;
		virtual void Shutdown(void) = 0;

		virtual void Parse(void)
		{
			m_tokenStream.Clear();

			while (true)
			{
				Token& token = ReadToken();
				m_tokenStream.Add(token);
				if (token.IsEOS())
					break;
			}
		}

		virtual Token ReadToken(void) = 0;

		virtual TokenStream GetTokenStream(void) const
		{
			return m_tokenStream;
		}

		protected:
			TokenStream m_tokenStream;

	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END