#pragma once

#include "Namespaces.h"

#include "String8.h"

Meson_Common_BEGIN

	class MesonException
	{
	private: // private variables
		Meson::Common::Text::String m_strMessage;
		Meson::Common::Text::String m_strSourceFile;
		int m_nSourceLine;
		MesonException* m_pMesonExceptionInner;

	public: // public methods
		MesonException(const Meson::Common::Text::String& p_strMessage,
			const Meson::Common::Text::String& p_strSourceFile,
			int p_nSourceLine,
			const MesonException* p_pMesonExceptionInner);
		MesonException(const Meson::Common::Text::String& p_strMessage,
			const Meson::Common::Text::String& p_strSourceFile,
			int p_nSourceLine);
		MesonException(const Meson::Common::Text::String& p_strMessage,
			const MesonException *p_pMesonExceptionInner);
		MesonException(const Meson::Common::Text::String& p_strMessage);
		
		MesonException::MesonException( const char* p_szMessage );

		MesonException(void);
		virtual ~MesonException(void);
		const Meson::Common::Text::String& GetMessage(void) const;
		const Meson::Common::Text::String& GetSourceFile(void) const;
		int GetSourceLine(void) const;
		MesonException* GetInnerException(void) const;
		Meson::Common::Text::String GetMessageTrace(void) const;
	};

Meson_Common_END
