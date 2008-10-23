#include "ElapsedTime.h"

#include "Maths.h"

using namespace Meson::Common::System;
using namespace Meson::Common::Maths;

Meson_Common_System_BEGIN

const int MILLISECONDS_PER_SECOND = 1000;
const int SECONDS_PER_MINUTE = 60;
const int MINUTES_PER_HOUR = 60;
const int HOURS_PER_DAY = 24;

const int SECONDS_PER_DAY = 86400;

const double MINUTE_INVERSE = 1.0 / 60.0;
const double HOUR_INVERSE = 1.0 / 3600.0;
const double DAY_INVERSE = 1.0 / 86400.0;

struct ElapsedTimeState
{
	double m_dTotalSeconds;

	ElapsedTimeState(void) : m_dTotalSeconds(0.0) { }

	ElapsedTimeState(const ElapsedTimeState &p_elapsedTimeState)
		: m_dTotalSeconds(p_elapsedTimeState.m_dTotalSeconds) { }

	ElapsedTimeState(double p_dTotalSeconds)
		: m_dTotalSeconds(p_dTotalSeconds) { }
};

Meson_Common_System_END

ElapsedTime::ElapsedTime(void)
	: m_pElapsedTimeState(NULL)
{
	m_pElapsedTimeState = new ElapsedTimeState();
}

ElapsedTime::ElapsedTime(const ElapsedTime &p_elapsedTime)
	: m_pElapsedTimeState(NULL)
{
	m_pElapsedTimeState = new ElapsedTimeState(*p_elapsedTime.m_pElapsedTimeState);
}

ElapsedTime::ElapsedTime(double p_dTotalSeconds)
	: m_pElapsedTimeState(NULL)
{
	m_pElapsedTimeState = new ElapsedTimeState(p_dTotalSeconds);
}


ElapsedTime::~ElapsedTime(void)
{
	delete m_pElapsedTimeState;
}

int ElapsedTime::Milliseconds(void)
{
	double dSecondFraction
		= m_pElapsedTimeState->m_dTotalSeconds;
	dSecondFraction -= (int) dSecondFraction;
	return (int) (dSecondFraction * MILLISECONDS_PER_SECOND);
}

int ElapsedTime::Seconds(void)
{
	return (int) m_pElapsedTimeState->m_dTotalSeconds % SECONDS_PER_MINUTE;
}

int ElapsedTime::Minutes(void)
{
	return (int) (m_pElapsedTimeState->m_dTotalSeconds * MINUTE_INVERSE)
		% MINUTES_PER_HOUR;
}

int ElapsedTime::Hours(void)
{
	return (int) (m_pElapsedTimeState->m_dTotalSeconds * HOUR_INVERSE)
		% HOURS_PER_DAY;
}

int ElapsedTime::Days(void)
{
	return (int) m_pElapsedTimeState->m_dTotalSeconds
		/ SECONDS_PER_DAY;
}

double ElapsedTime::TotalSeconds(void)
{
	return m_pElapsedTimeState->m_dTotalSeconds;
}

double ElapsedTime::TotalMinutes(void)
{
	return m_pElapsedTimeState->m_dTotalSeconds * MINUTE_INVERSE;
}

double ElapsedTime::TotalHours(void)
{
	return m_pElapsedTimeState->m_dTotalSeconds * HOUR_INVERSE;
}

double ElapsedTime::TotalDays(void)
{
	return m_pElapsedTimeState->m_dTotalSeconds * DAY_INVERSE;
}

bool ElapsedTime::operator==(const ElapsedTime& p_elapsedTime)
{
	return TMaths<double>::Equals(
		m_pElapsedTimeState->m_dTotalSeconds,
		p_elapsedTime.m_pElapsedTimeState->m_dTotalSeconds);
}

ElapsedTime& ElapsedTime::operator=(const ElapsedTime& p_elapsedTime)
{
	m_pElapsedTimeState->m_dTotalSeconds
		= p_elapsedTime.m_pElapsedTimeState->m_dTotalSeconds;
	return *this;
}
