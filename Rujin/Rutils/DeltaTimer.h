#ifndef DELTA_TIMER_H
#define DELTA_TIMER_H

namespace rutils
{
	class DeltaTimer final
	{
	public:
		DeltaTimer(const float interval, const bool repeat = false, const bool startEnabled = true);
		~DeltaTimer() = default;

		bool Tick(const float deltaTime);

		bool IsElapsed() const;
		float GetElapsed() const;
		bool GetRepeating() const;

		void Start();
		void Stop();

		void SetEnabled(const bool enabled);
		void SetRepeating(const bool repeat);
		void SetInterval(const float interval);
		void Reset(const bool stop = false);

	private:
		bool m_IsEnabled;
		bool m_Repeat;
		float m_Interval;
		float m_ElapsedTime;

	};
}



#endif // Include Guard: DELTA_TIMER_H
