#include "InstructionDecoder.h"

namespace xsim {

	struct SystemInfo {
		uint32_t numCores;
		uint64_t ramSize;
	};

	class System {
	public:
		System(const System&) = delete;
		System(System&&) = delete;
		System operator=(const System&) = delete;

		System(SystemInfo* pSystemInfo);
		~System() = default;

		void Execute(std::string& _ExecutableFile);

	private:
		uint64_t m_PC;
		std::vector<Core> m_Cores;
		Memory m_Memory;
	};
} // namespace xsim