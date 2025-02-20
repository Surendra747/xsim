#include "System.h"

namespace xsim {
	System::System(SystemInfo* pSystemInfo) : m_Cores(), m_Memory(pSystemInfo->ramSize) {

		for (uint32_t i = 0; i < pSystemInfo->numCores; ++i)
			m_Cores.emplace_back(i);
	}
	
	void System::Execute(std::string& _ExecutableFile) {
		InstructionDecoder Decoder = {_ExecutableFile, 5};
		InstructionDispatchTable idt = {};

		for (; !Decoder.Instructions.empty(); ) {

			for (size_t j = 0; j < m_Cores.size(); ++j, ++m_PC) {
				idt.Dispatch(Decoder.Instructions[0], &m_Cores[j], &m_Memory);
			}

			if (m_PC % (Decoder.GetInstructionQueueSize() - 1) == 0) {
				Decoder.Decode();
			}
		}
			
	}
} // namespace xsim