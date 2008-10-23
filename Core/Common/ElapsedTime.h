#pragma once

#include "Namespaces.h"

Meson_Common_System_BEGIN

struct ElapsedTimeState;

class ElapsedTime
{
private: // private variables
	ElapsedTimeState *m_pElapsedTimeState;

public: // public methods
	ElapsedTime(void);
	ElapsedTime(const ElapsedTime& p_elapsedTime);
	ElapsedTime(double p_dTotalSeconds);
	~ElapsedTime(void);

	int Milliseconds(void);
	int Seconds(void);
	int Minutes(void);
	int Hours(void);
	int Days(void);

	double TotalSeconds(void);
	double TotalMinutes(void);
	double TotalHours(void);
	double TotalDays(void);

	bool operator==(const ElapsedTime& p_elapsedTime);
	ElapsedTime& operator=(const ElapsedTime& p_elapsedTime);
};

Meson_Common_System_END
