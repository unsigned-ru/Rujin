#include "DeltaTimer.h"
#include <cmath>

rutils::DeltaTimer::DeltaTimer(const float interval, const bool repeat, const bool startEnabled)
	: m_Interval{interval}
	, m_Repeat{repeat}
	, m_ElapsedTime{0.f}
	, m_IsEnabled{ startEnabled }
{
}

bool rutils::DeltaTimer::Tick(const float deltaTime)
{
	if (!m_IsEnabled || (!m_Repeat && IsElapsed()))
		return false;

	m_ElapsedTime += deltaTime;

	if (IsElapsed())
	{
		//we elapsed
		//check if we need to repeat
		if (m_Repeat)
		{
			//set elapsedTime to remainder
			m_ElapsedTime = fmod(m_ElapsedTime, m_Interval);
		}

		return true;
	}

	return false;
}

bool rutils::DeltaTimer::IsElapsed() const
{
	return m_ElapsedTime >= m_Interval;
}

float rutils::DeltaTimer::GetElapsed() const
{
	return m_ElapsedTime;
}

bool rutils::DeltaTimer::GetRepeating() const
{
	return m_Repeat;
}

void rutils::DeltaTimer::Start()
{
	SetEnabled(true);
}

void rutils::DeltaTimer::Stop()
{
	SetEnabled(false);
}

void rutils::DeltaTimer::SetEnabled(const bool enabled)
{
	m_IsEnabled = enabled;
}

void rutils::DeltaTimer::SetRepeating(const bool repeat)
{
	m_Repeat = repeat;
}

void rutils::DeltaTimer::SetInterval(const float interval)
{
	m_Interval = interval;
}

void rutils::DeltaTimer::Reset(const bool stop)
{
	m_ElapsedTime = 0.f;
	
	if (stop)
		Stop();
}
