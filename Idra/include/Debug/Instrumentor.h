#pragma once

#include "Core/Core.h"
#include "Core/Timer.h"

#include <fstream>
#include <filesystem>
#include <string>

#include <thread>

namespace Idra {
	using Path = std::filesystem::path;

	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentorSession
	{
		std::string Name;
	};

	class IDRA_API Instrumentor
	{
	public:
		Instrumentor();

		void BeginSession(const std::string& name, const Path& filepath = "profile.json");
		void EndSession();

		void WriteProfile(const ProfileResult& result);

		static Instrumentor& Get();

	private:
		void WriteHeader();
		void WriteFooter();

	private:
		InstrumentorSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount = 0;
	};

	class IDRA_API InstrumentorTimer
	{
	public:
		InstrumentorTimer(const char* name);
		~InstrumentorTimer();

		void Reset();
		void Stop();

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Running = true;
	};
}

#define IDRA_PROFILE 1
#if IDRA_PROFILE
	#define IDRA_PROFILE_BEGIN_SESSION(name, filepath) ::Idra::Instrumentor::Get().BeginSession(name, filepath)
	#define IDRA_PROFILE_END_SESSION() ::Idra::Instrumentor::Get().EndSession()
	#define IDRA_PROFILE_SCOPE(name) ::Idra::InstrumentorTimer timer##__LINE__(name)
	#define IDRA_PROFILE_FUNCTION() IDRA_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define IDRA_PROFILE_BEGIN_SESSION(name, filepath)
	#define IDRA_PROFILE_END_SESSION()
	#define IDRA_PROFILE_SCOPE(name)
	#define IDRA_PROFILE_FUNCTION()
#endif