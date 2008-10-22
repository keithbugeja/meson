#include "Timer.h"

Meson_Common_System_BEGIN

	Timer::Timer(double p_dInterval)
		: m_dInterval(p_dInterval)
		, m_stopWatch()
		, m_bRequestStop(false)
		, m_listListeners()
	{
	}

	Timer::~Timer(void)
	{
	}

	void Timer::AddListener(TimerListener *pTimerListener)
	{
		for (size_t unIndex = 0; unIndex < m_listListeners.Size(); unIndex++)
			if (m_listListeners[unIndex] == pTimerListener) return;

		m_listListeners.Add(pTimerListener);
	}

	void Timer::RemoveListener(TimerListener *pTimerListener)
	{
		for (size_t unIndex = 0; unIndex < m_listListeners.Size(); unIndex++)
		{
			if (m_listListeners[unIndex] == pTimerListener)
			{
				m_listListeners.RemoveAt(unIndex);
				return;
			}
		}
	}

	void Timer::Stop(void)
	{
		m_bRequestStop = true;
		while (m_bRequestStop) Sleep(0);
	}

	void Timer::Execute()
	{
		m_stopWatch.Reset();

		while (true)
		{
			if (m_bRequestStop)
				break;

			if (m_stopWatch.GetElapsedTime().TotalSeconds() > m_dInterval)
			{
				m_stopWatch.Reset();
				
				for (size_t unIndex = 0; unIndex < m_listListeners.Size(); unIndex++)
					m_listListeners[unIndex]->OnTimerEvent(this);
			}

			Sleep(0.001);
		}
		m_bRequestStop = false;
	}

Meson_Common_System_END
