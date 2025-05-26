#pragma once

#include "Core/Core.h"

#include <chrono>
#include <string>

namespace Idra {
	template<typename Fn>
	class IDRA_API Timer
	{
	public:
		Timer(const char* name, Fn&& func)
			: m_Name(name), m_Function(func)
		{
			Reset();
		}

		~Timer()
		{
			if (m_Running)
				Stop();
		}

		void Reset()
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
			m_Running = true;
		}

		void Stop()
		{
			// Stop the timer and record the end time
			auto endTimePoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::duration_cast<std::chrono::microseconds>(m_StartTimepoint.time_since_epoch()).count();
			long long end = std::chrono::duration_cast<std::chrono::microseconds>(endTimePoint.time_since_epoch()).count();
			m_Running = false;

			float duration = (end - start) / 1000.0f; // Convert to milliseconds
			m_Function({ m_Name, duration });
		}

	private:
		const char* m_Name;
		Fn m_Function;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Running = true;
	};
} // namespace Idra