#pragma once

#include "Namespaces.h"

Meson_Common_System_BEGIN

	typedef void (* ThreadFunction)(void *p_pContext);

	struct ThreadState;

	class Thread
	{
	public:
		ThreadState *m_pThreadState;

	public:
		Thread(void);
		Thread(ThreadFunction p_threadFunction, void *p_pContext);
		~Thread(void);
		bool IsRunning(void);
		void Start(void);
		void Sleep(double dInterval);
		virtual void Execute(void);
	};

Meson_Common_System_END
