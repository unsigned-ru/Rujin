#include "stdafx.h"
#include "Timer.h"

rutils::Timer::Timer(std::function<void(void)> func, const int32_t interval, bool shouldRepeat, bool autoStart)
	: Timer(func, std::chrono::milliseconds(interval))
{
}

rutils::Timer::Timer(std::function<void(void)> func, const std::chrono::milliseconds& interval, bool shouldRepeat, bool autoStart)
	: m_Function{ func }
	, m_Interval{ interval }
	, m_ShouldRepeat{ shouldRepeat }
	, m_IsElapsed{ false }
	, m_IsRunning{ false }
{
	if (autoStart)
		Start();
}

rutils::Timer::~Timer()
{
	Stop();
}

void rutils::Timer::Start()
{
	m_IsRunning = true;
	m_IsElapsed = false;


	m_Thread = std::thread([&]()
		{
			do
			{
				std::this_thread::sleep_for(m_Interval);
				m_Function();
			} while (m_ShouldRepeat);

			m_IsElapsed = true;
		});

	m_Thread.detach();
}

void rutils::Timer::Stop()
{
	m_IsRunning = false;
	m_Thread.~thread();
}

void rutils::Timer::Restart()
{
	Stop();
	Start();
}

bool rutils::Timer::IsElapsed() const
{
	return m_IsElapsed;
}

bool rutils::Timer::IsRunnning() const
{
	return m_IsRunning;
}

bool rutils::Timer::IsRepeating() const
{
	return m_ShouldRepeat;
}

rutils::Timer* rutils::Timer::SetFunction(std::function<void(void)> func)
{
	m_Function = func;
	return this;
}

rutils::Timer* rutils::Timer::SetInterval(const uint32_t interval)
{
	SetInterval(std::chrono::milliseconds(interval));
	return this;
}

rutils::Timer* rutils::Timer::SetRepeating(const bool shouldRepeat)
{
	m_ShouldRepeat = shouldRepeat;
	return this;
}
