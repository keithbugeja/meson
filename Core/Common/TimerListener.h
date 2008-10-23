#pragma once

#include "Namespaces.h"

Meson_Common_System_BEGIN

	class Timer;

	class TimerListener
	{
	public:
		virtual ~TimerListener(void) { };
		virtual void OnTimerEvent(Timer *p_pTimer) = 0;
	};

Meson_Common_System_END