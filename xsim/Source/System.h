#pragma once
#include "Fetcher.h"
#include "Core.h"


namespace xsim {

	class System {
	
	public:
		
		System(const System&) = delete;
		System(System&&) = delete;
		System operator=(const System&) = delete;

		System(SystemConfig* pSystemConfig, std::string& rExecutable);
		~System() = default;

		void Execute();

	private:
		
		int64_t				m_GlobalPC;
		SystemConfig*		pSystemConfig;

		std::istringstream							m_ExecutableSS;
		std::unordered_map<std::string, uint64_t>	m_Labels;

		Fetcher				m_Fetcher;
		std::vector<Core>	m_Cores;

		PipelineMetrics		m_Metrics;

		void ProcessLabels();
	};
} // namespace xsim