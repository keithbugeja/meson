#include "StopWatch.h"

#include "Platform.h"

#ifdef PLATFORM_WIN32
	#include <Windows.h>
#else
	#include <time.h>
#endif

Meson_Common_System_BEGIN

#ifdef PLATFORM_WIN32 // Windows-specific implementation
	
	struct StopWatchState
	{
		LARGE_INTEGER m_liTicksPerSecond;
		LARGE_INTEGER m_liTicks;
		ElapsedTime m_elapsedTime;

		StopWatchState(void)
			: m_elapsedTime()
		{ }
	};

	StopWatch::StopWatch(void)
		: m_pStopWatchState(NULL)
	{
		m_pStopWatchState = new StopWatchState();
		QueryPerformanceFrequency(&m_pStopWatchState->m_liTicksPerSecond);
		Reset();
	}

	StopWatch::~StopWatch(void)
	{
		delete m_pStopWatchState;
	}

	void StopWatch::Reset(void)
	{
		QueryPerformanceCounter(&m_pStopWatchState->m_liTicks);
	}

	ElapsedTime& StopWatch::GetElapsedTime(void)
	{
		LARGE_INTEGER liTicks;
		QueryPerformanceCounter(&liTicks);
		double dTotalSeconds
			= (double) (liTicks.QuadPart - m_pStopWatchState->m_liTicks.QuadPart)
				/ (double) m_pStopWatchState->m_liTicksPerSecond.QuadPart;
	
		m_pStopWatchState->m_elapsedTime = ElapsedTime(dTotalSeconds);
		return m_pStopWatchState->m_elapsedTime;
	}

#else // generic implementation

	struct StopWatchState
	{
		clock_t m_clock;
	};

	StopWatch::StopWatch(void)
		: m_pStopWatchState(NULL)
	{
		m_pStopWatchState = new StopWatchState();
		Reset();
	}

	StopWatch::~StopWatch(void)
	{
		delete m_pStopWatchState;
	}

	void StopWatch::Reset(void)
	{
		m_pStopWatchState->m_clock = clock();
	}

	double StopWatch::GetElapsedTime(void)
	{
		return (double)
			(clock() - m_pStopWatchState->m_clock)
			/ CLOCKS_PER_SEC;
	}

#endif

Meson_Common_System_END
