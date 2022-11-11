#include "debug/Instrumentor.hpp"
#include "debug/Logger.hpp"

namespace rnd::debug{
	Instrumentor Instrumentor::instance;

	Instrumentor::Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

	void Instrumentor::BeginSession(const std::string& name, const std::string& filepath){
		LOG(Info, "begin instrumentor session", (name + " : " + filepath).c_str());
		m_OutputStream.open(filepath);
		WriteHeader();
		m_CurrentSession = new InstrumentationSession{ name };
	}

	void Instrumentor::EndSession(){
		LOG(Info, "end instrumentor session", m_CurrentSession->Name.c_str());
		WriteFooter();
		m_OutputStream.close();

		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	}

	void Instrumentor::WriteProfile(const ProfileResult& result){
		if (frameTarget == 0 || frameCount > frameTarget) return;

		mutex.lock();
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
		mutex.unlock();
	}

	void Instrumentor::WriteHeader(){
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}

	void Instrumentor::WriteFooter(){
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	void Instrumentor::newFrame(){
		frameCount++;
	}

	void Instrumentor::setFrameTarget(uint64_t frameCount){
		frameTarget = frameCount;
		this->frameCount = 1;
	}

	Instrumentor& Instrumentor::Get(){
		return instance;
	}
}