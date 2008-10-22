#include "Mutex.h"

#include "Platform.h"

#ifdef PLATFORM_WIN32
	#include <windows.h>
#endif

Meson_Common_System_BEGIN

#ifdef PLATFORM_WIN32

	struct MutexState
	{
		CRITICAL_SECTION *m_pCriticalSection;
		bool m_bContextOwner;
		bool m_bLocked;
	};

	Mutex::Mutex(void)
		: m_pMutexState(NULL)
	{
		m_pMutexState = new MutexState();
		m_pMutexState->m_pCriticalSection = new CRITICAL_SECTION();
		m_pMutexState->m_bContextOwner = true;
		m_pMutexState->m_bLocked = false;
		::InitializeCriticalSection(m_pMutexState->m_pCriticalSection);
	}

	Mutex::Mutex(uint *p_punContext)
		: m_pMutexState(NULL)
	{
		m_pMutexState = new MutexState();
#pragma warning(disable: 4312) 
		m_pMutexState->m_pCriticalSection = (CRITICAL_SECTION *) *p_punContext;
		m_pMutexState->m_bContextOwner = true;
		m_pMutexState->m_bLocked = false;
		if (m_pMutexState->m_pCriticalSection == NULL)
		{
			m_pMutexState->m_pCriticalSection = new CRITICAL_SECTION();
			::InitializeCriticalSection(m_pMutexState->m_pCriticalSection);
		}
	}

	Mutex::~Mutex(void)
	{
		if (m_pMutexState->m_bLocked)
			Unlock();

		if (m_pMutexState->m_bContextOwner)
		{
			::DeleteCriticalSection(m_pMutexState->m_pCriticalSection);
			delete m_pMutexState->m_pCriticalSection;
		}

		delete m_pMutexState;
	}

	void Mutex::Lock(void)
	{
		::EnterCriticalSection(m_pMutexState->m_pCriticalSection);
		m_pMutexState->m_bLocked = true;
	}

	void Mutex::Unlock(void)
	{
		::LeaveCriticalSection(m_pMutexState->m_pCriticalSection);
		m_pMutexState->m_bLocked = false;
	}

#else
	#ifndef NULL
		#define NULL 0
	#endif

	struct MutexState
	{
		union
		{
			uint *m_punContext;
			uint m_unContext;
		} Context;
		bool m_bContextOwner;
		bool m_bLocked;
	};

	Mutex::Mutex(void)
		: m_pMutexState(NULL)
	{
		m_pMutexState = new MutexState();
		m_pMutexState->Context.m_unContext = 0;
		m_pMutexState->m_bContextOwner = true;
		m_pMutexState->m_bLocked = false;
	}

	Mutex::Mutex(uint *p_punContext)
		: m_pMutexState(NULL)
	{
		m_pMutexState = new MutexState();
		m_pMutexState->Context.m_punContext = p_punContext;
		m_pMutexState->m_bContextOwner = false;
		m_pMutexState->m_bLocked = *p_punContext == 0;
	}

	Mutex::~Mutex(void)
	{
		if (m_pMutexState->m_bLocked)
			Unlock();
		delete m_pMutexState;
	}

	void Mutex::Lock(void)
	{
		uint *punContext = m_pMutexState->m_bContextOwner
			? &m_pMutexState->Context.m_unContext
			: m_pMutexState->Context.m_punContext;

		while (true)
		{
			// wait until clear
			while (*punContext > 0);

			// attempt to increment
			++*punContext;

			// if exactly 1, exclusive lock
			if (*punContext == 1) break;

			// otherwise, concurrent lock attempt - back off
			--*punContext;
		}
		m_pMutexState->m_bLocked = true;
	}

	void Mutex::Unlock(void)
	{
		uint *punContext = m_pMutexState->m_bContextOwner
			? &m_pMutexState->Context.m_unContext
			: m_pMutexState->Context.m_punContext;

		m_pMutexState->m_bLocked = false;
		*punContext = 0;
	}
#endif

Meson_Common_System_END
