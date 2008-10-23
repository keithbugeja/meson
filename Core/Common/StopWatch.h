#pragma once

#include "Namespaces.h"
#include "ElapsedTime.h"

Meson_Common_System_BEGIN

struct StopWatchState;

class StopWatch
{
private:
	StopWatchState *m_pStopWatchState;

public:
	StopWatch(void);
	~StopWatch(void);
	void Reset(void);
	ElapsedTime& GetElapsedTime(void);
};

Meson_Common_System_END
