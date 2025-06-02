#pragma once

#include "Core/Core.h"

#include <chrono>
#include <string>

namespace Idra {
	class IDRA_API Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		float Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_StartTimepoint).count() * 0.001f * 0.001f * 0.001f;
		}

		float ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	};
} // namespace Idra