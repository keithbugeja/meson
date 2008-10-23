#include "Thread.h"

#include "Platform.h"

#include "MesonException.h"

#ifdef PLATFORM_WIN32
	#include <windows.h>
#else
#endif

Meson_Common_System_BEGIN

#ifdef PLATFORM_WIN32

	struct ThreadState
	{
		ThreadFunction m_threadFunction;
		void *m_pContext;
		bool m_bRunning;
	};

	long WINAPI ThreadProcess(void *p_pContext)
	{
		Thread *p_pThread = (Thread *) p_pContext;
		ThreadState *pThreadState = p_pThread->m_pThreadState;

		if (pThreadState->m_threadFunction != NULL)
			pThreadState->m_threadFunction(pThreadState->m_pContext);
		else
			p_pThread->Execute();

		pThreadState->m_bRunning = false;
		return 0;
	}

	Thread::Thread(void)
		: m_pThreadState(NULL)
	{
		m_pThreadState = new ThreadState();
		m_pThreadState->m_threadFunction = NULL;
		m_pThreadState->m_pContext = NULL;
		m_pThreadState->m_bRunning = false;
	}

	Thread::Thread(ThreadFunction p_threadFunction, void *p_pContext)
	{
		m_pThreadState = new ThreadState();
		m_pThreadState->m_threadFunction = p_threadFunction;
		m_pThreadState->m_pContext = p_pContext;
		m_pThreadState->m_bRunning = false;
	}

	Thread::~Thread(void)
	{
		while (m_pThreadState->m_bRunning)
			::Sleep(0);

		delete m_pThreadState;
	}

	void Thread::Start(void)
	{
		if (IsRunning())
			throw new MesonException(
				"Thread is already running", __FILE__, __LINE__);

		m_pThreadState->m_bRunning = true;

		HANDLE handle = ::CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE) ThreadProcess, (void *)this, 0, 0);

		if (handle == NULL)
			throw new MesonException(
				"Error while creating Thread", __FILE__, __LINE__);
	}

	void Thread::Sleep(double dInterval)
	{
		DWORD dwIntervalMS = (DWORD) (dInterval * 1000.0f);
		::Sleep(dwIntervalMS);
	}

#else

	// generic implementation
	#ifndef NULL
		#define NULL 0
	#endif

	struct ThreadState
	{
		bool m_bRunning;
	};

	Thread::Thread(void)
		: m_pThreadState(NULL)
	{
		throw new MesonException(
			"No generic Thread implementation available.",
			__FILE__, __LINE__);
	}

	Thread::Thread(ThreadFunction p_threadFunction, void *p_pContext)
	{
		throw new MesonException(
			"No generic Thread implementation available.",
			__FILE__, __LINE__);
	}

	Thread::~Thread(void)
	{
		delete m_pThreadState;
	}

	void Thread::Start(void)
	{
		throw new MesonException(
			"No generic Thread implementation available.",
			__FILE__, __LINE__);
	}

#endif

	bool Thread::IsRunning(void)
	{
		return m_pThreadState->m_bRunning;
	}

	void Thread::Execute(void)
	{
		throw new MesonException(
			"Thread Execute() method was not overridden.",
			__FILE__, __LINE__);
	}

Meson_Common_System_END
