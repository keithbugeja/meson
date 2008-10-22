#pragma once

#include "Namespaces.h"

#include "Thread.h"
#include "StopWatch.h"
#include "ArrayList.h"
#include "TimerListener.h"

Meson_Common_System_BEGIN

	class Timer
		: public Thread
	{
	private:
		double m_dInterval;
		StopWatch m_stopWatch;
		bool m_bRequestStop;
		Meson::Common::Collections::TArrayList<TimerListener *> m_listListeners;

	public:
		Timer(double p_dInterval);
		~Timer(void);
		void AddListener(TimerListener *pTimerListener);
		void RemoveListener(TimerListener *pTimerListener);
		void Stop(void);
		virtual void Execute(void);
	};

Meson_Common_System_END