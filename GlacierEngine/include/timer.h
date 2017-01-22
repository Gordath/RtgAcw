#ifndef GLACIER_TIMER_H_
#define GLACIER_TIMER_H_
#include <chrono>

namespace Glacier
{
	template<typename T>
	using TimePoint = std::chrono::time_point<T>;

	using HighResolutionClock = std::chrono::high_resolution_clock;

	using HighResolutionTimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;


	class Timer {
	private:
		HighResolutionTimePoint m_start;
		HighResolutionTimePoint m_pause;

		mutable HighResolutionTimePoint m_prev;

		bool m_running;

	public:
		Timer() : m_prev{ HighResolutionClock::now() }, m_running{false}
		{
		}

		void reset() noexcept
		{
			m_pause = {};
			m_start = HighResolutionClock::now();
		}
		
		void start() noexcept
		{
			if (!is_running()) {
				m_start += HighResolutionClock::now() - m_pause;
				m_pause = {};

				m_running = true;
			}
		}

		void stop() noexcept
		{
			if (is_running()) {
				m_pause = HighResolutionClock::now();

				m_running = false;
			}
		}

		bool is_running() const noexcept
		{
			return m_running;
		}

		long long get_msec() const noexcept
		{
			using namespace std::chrono;
			if (!is_running()) {
				// in paused state...
				return duration_cast<milliseconds>(m_pause - m_start).count();
			}

			return duration_cast<milliseconds>(HighResolutionClock::now() - m_start).count();
		}

		double get_sec() const noexcept
		{
			using namespace std::chrono;
			if (!is_running()) {
				// in paused state...
				return duration<double>{ m_pause - m_start }.count();
			}

			return duration<double>{ HighResolutionClock::now() - m_start }.count();
		}

		double get_delta() const noexcept
		{
			using namespace std::chrono;

			duration<double> dur{ HighResolutionClock::now() - m_prev };
			m_prev = HighResolutionClock::now();

			return dur.count();
		}
	};
}

#endif //GLACIER_TIMER_H_
