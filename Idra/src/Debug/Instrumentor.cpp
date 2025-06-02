#include "IdraPCH.h"

#include "Debug/Instrumentor.h"

namespace Idra {
	Instrumentor::Instrumentor()
		: m_CurrentSession(nullptr), m_ProfileCount(0)
	{

	}

	void Instrumentor::BeginSession(const std::string& name, const Path& filepath)
	{
		m_OutputStream.open(filepath);
		WriteHeader();
		m_CurrentSession = new InstrumentorSession{ name };
	}

    void Instrumentor::EndSession()
    {
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void Instrumentor::WriteProfile(const ProfileResult& result)
    {
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void Instrumentor::WriteHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void Instrumentor::WriteFooter()
    {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

	Instrumentor& Instrumentor::Get()
	{
		static Instrumentor instance;
		return instance;
	}

    //----------------------------------------------------------------------
	// InstrumentorTimer Implementation
	//----------------------------------------------------------------------

	InstrumentorTimer::InstrumentorTimer(const char* name)
		: m_Name(name), m_Running(true)
	{
		Reset();
	}

    InstrumentorTimer::~InstrumentorTimer()
    {
        if (m_Running)
            Stop();
    }

    void InstrumentorTimer::Reset()
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
        m_Running = true;
    }

    void InstrumentorTimer::Stop()
    {
        // Stop the timer and record the end time
        auto endTimePoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::duration_cast<std::chrono::microseconds>(m_StartTimepoint.time_since_epoch()).count();
        long long end = std::chrono::duration_cast<std::chrono::microseconds>(endTimePoint.time_since_epoch()).count();
        m_Running = false;

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });
    }
}