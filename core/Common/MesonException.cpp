#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

#ifndef NULL
	#define NULL 0
#endif

MesonException::MesonException(
	const String& p_strMessage,
	const String& p_strSourceFile,
	int p_nSourceLine,
	const MesonException* p_pMesonExceptionInner)
	: m_strMessage(p_strMessage)
	, m_strSourceFile(p_strSourceFile)
	, m_nSourceLine(p_nSourceLine)
	, m_pMesonExceptionInner((MesonException*) p_pMesonExceptionInner)
{
}

MesonException::MesonException(
	const String& p_strMessage,
	const String& p_strSourceFile,
	int p_nSourceLine)
	: m_strMessage(p_strMessage)
	, m_strSourceFile(p_strSourceFile)
	, m_nSourceLine(p_nSourceLine)
	, m_pMesonExceptionInner(NULL)
{
}

MesonException::MesonException(
	const String& p_strMessage,
	const MesonException* p_pMesonExceptionInner)
	: m_strMessage(p_strMessage)
	, m_strSourceFile(__FILE__)
	, m_nSourceLine(__LINE__)
	, m_pMesonExceptionInner((MesonException*) p_pMesonExceptionInner)
{
}

MesonException::MesonException(
	const String& p_strMessage)
	: m_strMessage(p_strMessage)
	, m_strSourceFile(__FILE__)
	, m_nSourceLine(__LINE__)
	, m_pMesonExceptionInner(NULL)
{
}

MesonException::MesonException(
	const char* p_szMessage)
	: m_strMessage(p_szMessage)
	, m_strSourceFile(__FILE__)
	, m_nSourceLine(__LINE__)
	, m_pMesonExceptionInner(NULL)
{
}

MesonException::MesonException(void)
	: m_strMessage("")
{
}

MesonException::~MesonException(void)
{
	if (m_pMesonExceptionInner != NULL)
		delete m_pMesonExceptionInner;
}

const String& MesonException::GetMessage(void) const
{
	return m_strMessage;
}

const String& MesonException::GetSourceFile(void) const
{
	return m_strSourceFile;
}

int MesonException::GetSourceLine(void) const
{
	return m_nSourceLine;
}

MesonException* MesonException::GetInnerException(void) const
{
	return m_pMesonExceptionInner;
}

String MesonException::GetMessageTrace(void) const
{
	String strMessage = m_strMessage + "(" + "Source: "
		+ m_strSourceFile + " Line: " + m_nSourceLine + ")";
	
	if (m_pMesonExceptionInner == NULL)
		return strMessage;
	else
		return strMessage + " Inner Exception: "
			+ m_pMesonExceptionInner->GetMessageTrace();
}

