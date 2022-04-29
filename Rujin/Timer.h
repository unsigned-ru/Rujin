#pragma once
#include <chrono>
#include <thread>
#include <functional>

namespace rutils
{
	class Timer final
	{

	public:
#pragma region Constructors
		Timer() = default;
		Timer(std::function<void(void)> func, const int32_t interval, bool shouldRepeat = false, bool autoStart = true);
		template <class ChronoDuration>
		Timer(std::function<void(void)> func, const ChronoDuration& interval, bool shouldRepeat = false, bool autoStart = true)
			: Timer(func, std::chrono::duration_cast<std::chrono::milliseconds>(interval))
		{}
		Timer(std::function<void(void)> func, const std::chrono::milliseconds& interval, bool shouldRepeat = false, bool autoStart = true);
		~Timer();
#pragma endregion

		void Start();
		void Stop();
		void Restart();

		bool IsElapsed() const;
		bool IsRunnning() const;
		bool IsRepeating() const;


		Timer* SetFunction(std::function<void(void)> func);
		Timer* SetInterval(const uint32_t interval);
		Timer* SetRepeating(const bool shouldRepeat);

		template <class ChronoDuration>
		ChronoDuration GetInterval() const
		{
			return std::chrono::duration_cast<ChronoDuration>(m_Interval);
		}

		template <class ChronoDuration>
		Timer* SetInterval(const ChronoDuration& interval)
		{
			m_Interval = std::chrono::duration_cast<ChronoDuration>(interval);
			return this;
		}

	private:
		std::function<void(void)> m_Function;
		std::chrono::milliseconds m_Interval;
		std::thread m_Thread;
		bool m_ShouldRepeat;
		bool m_IsRunning;
		bool m_IsElapsed;
	};
}