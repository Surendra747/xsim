#include "WritebackStage.h"

namespace xsim {

	WritebackStage::WritebackStage() :
		PipelineStage(PipelineStageType::Writeback) {

	}

	bool WritebackStage::Cycle(
		PipelineRegister* pThisStageRegister, 
		PipelineRegister* pNextStageRegister, 
		PipelineMetrics* pMetrics) noexcept {

		if (!pThisStageRegister->stateless) {

			Writeback(pThisStageRegister->instruction.value(), pThisStageRegister->pRegisterFile);

			*pThisStageRegister->pLocalPC = *pNextStageRegister->pLocalPC;
			
			XSIM_INFO("\t[Stage::Writeback]:\tWritten back to Register File for " 
				<< pThisStageRegister->rawInstruction.value());

			// No Forwarding & Reset this Stage
			pThisStageRegister->Reset();
		}

		return true;
	}

	bool WritebackStage::Cycle2(
		PipelineRegister* pThisStageRegister, 
		PipelineRegister* pNextStageRegister, 
		PipelineRegister* pForwardRegister, 
		PipelineMetrics* pMetrics) noexcept {

		return true;
	}

	void WritebackStage::Reset(PipelineRegister* pThisStageRegister) noexcept {

		pThisStageRegister->Reset();
	}

	void WritebackStage::Writeback(Instruction& rInstruction, RegisterFile_T* pRegisterFile) noexcept {

		if(rInstruction.rdWrite)
			(*pRegisterFile)[rInstruction.rd] = rInstruction.rdv;
		if(rInstruction.rs1Write)
			(*pRegisterFile)[rInstruction.rs1] = rInstruction.rs1v;
		if (rInstruction.rs2Write)
			(*pRegisterFile)[rInstruction.rs2] = rInstruction.rs2v;
	}
}
