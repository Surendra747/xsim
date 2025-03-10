#include "MemoryStage.h"

namespace xsim {

	MemoryStage::MemoryStage() :
		PipelineStage(PipelineStageType::Memory),
		m_Memory()
	{}

	bool MemoryStage::Cycle(
		PipelineRegister* pThisStageRegister,
		PipelineRegister* pNextStageRegister,
		PipelineMetrics* pMetrics) noexcept {

		if (!pThisStageRegister->stateless && pNextStageRegister->stateless) {

			switch (pThisStageRegister->instruction.value().opcode) {

			case OpCode::Lw:

				lw(pThisStageRegister->instruction.value());
				break;

			case OpCode::Sw:

				sw(pThisStageRegister->instruction.value());
				break;

			default:

				XSIM_INFO("\t[Stage::Memory]:\tNothing to Execute in Memory Stage");
				break;
			}

		}

		// Forward this State to next Stage
		*pNextStageRegister = *pThisStageRegister;
		// Reset this Stage
		pThisStageRegister->Reset();

		return true;
	}

	bool MemoryStage::Cycle2(
		PipelineRegister* pThisStageRegister,
		PipelineRegister* pNextStageRegister,
		PipelineRegister* pForwardRegister,
		PipelineMetrics* pMetrics) noexcept {

		return true;
	}

	void MemoryStage::Reset(PipelineRegister* pThisStageRegister) noexcept {
		
		for (size_t i = 0; i < m_Memory.size(); ++i)
			m_Memory[i] = 0;

		pThisStageRegister->Reset();
	}

	void MemoryStage::Dump() noexcept {

		std::cout << "---------- Memory Dump ----------" << std::endl;
		for (size_t i = 0; i < m_Memory.size(); ++i) {
			std::cout << m_Memory[i] << " ";
		}
		std::cout << std::endl;
	}

	int64_t MemoryStage::Load(uint64_t& rAddress) {

		return m_Memory[rAddress];
	}

	void MemoryStage::Store(uint64_t& rAddress, int64_t& rValue) {
	
		m_Memory[rAddress] = rValue;
	}

	void MemoryStage::lw(Instruction& rInstruction) {

		uint64_t address = (uint64_t)(rInstruction.rs1v + rInstruction.imm);

		if (address >= m_Memory.size()) {

			XSIM_INFO("\t[Address]: " << address);
			XSIM_LOG("[Stage::Memory]: Address Exceeded Memory Size");
		}

		rInstruction.rdv = Load(address);

		XSIM_INFO("\t[Stage::Memory]:\tLoaded Word from Address " << rInstruction.rs1v 
			<< " + " << rInstruction.imm);
	}

	void MemoryStage::sw(Instruction& rInstruction) {

		uint64_t address = (uint64_t(rInstruction.rs1v + rInstruction.imm));

		if (address >= m_Memory.size()) {

			XSIM_INFO("\t[Address]: " << address);
			XSIM_LOG("\t[Stage::Memory]: Address Exceeded Memory Size");
		}

		Store(address, rInstruction.rs2v);

		XSIM_INFO("\t[Stage::Memory]:\tStored " << rInstruction.rs1v 
			<< " to Address " << rInstruction.rs1v << " + " << rInstruction.imm);
	}
}
