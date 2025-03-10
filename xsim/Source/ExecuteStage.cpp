#include "ExecuteStage.h"

namespace xsim {

	ExecuteStage::ExecuteStage() :
		PipelineStage(PipelineStageType::Execute)
	{}

	bool ExecuteStage::Cycle(
		PipelineRegister* pThisStageRegister, 
		PipelineRegister* pNextStageRegister, 
		PipelineMetrics* pMetrics) noexcept {

		return true;
	}

	bool ExecuteStage::Cycle2(
		PipelineRegister* pThisStageRegister, 
		PipelineRegister* pNextStageRegister,
		PipelineRegister* pForwardRegister,
		PipelineMetrics* pMetrics) noexcept {

		if (!pThisStageRegister->stateless && pNextStageRegister->stateless) {

			// If RAW Hazard is detected, stall this stage!
			if (DetectHazard(*pThisStageRegister, *pForwardRegister)) {

				++pMetrics->stalls;
				return false;
			}

			Execute(pThisStageRegister);

			XSIM_INFO("\t[Stage::Execute]:\tExecuted " << pThisStageRegister->rawInstruction.value());

			// Forward this State to next Stage
			*pNextStageRegister = *pThisStageRegister;
			// Reset this Stage
			pThisStageRegister->Reset();
		}

		return true;
	}

	void ExecuteStage::Reset(PipelineRegister* pThisStageRegister) noexcept {

		pThisStageRegister->Reset();
	}

	void ExecuteStage::Execute(PipelineRegister* pEx) {

		XSIM_INFO("\t[Stage::Execute]:\tLocalPC = " << *(pEx->pLocalPC));
		XSIM_INFO("\t[Stage::Execute]:\tGlobalPC = " << *(pEx->pGlobalPC));

		if (*(pEx->pGlobalPC) != *(pEx->pLocalPC)) {

			XSIM_INFO("\t[Stage::Execute]:\t Skipping Execution for this Core!, PC's dont Match");
			return;
		}

		switch (pEx->instruction.value().opcode)
		{
		case OpCode::Add:	

			add(pEx->instruction.value(), pEx->pRegisterFile); 
			break;

		case OpCode::Sub:	
			
			sub(pEx->instruction.value(), pEx->pRegisterFile); 
			break;

		case OpCode::Mul:	
			
			mul(pEx->instruction.value(), pEx->pRegisterFile); 
			break;

		case OpCode::Div:	
			
			div(pEx->instruction.value(), pEx->pRegisterFile); 
			break;

		case OpCode::Addi:	
			
			addi(pEx->instruction.value(), pEx->pRegisterFile); 
			break;

		case OpCode::Jal:	
			
			jal(pEx->instruction.value(), pEx->pLocalPC, pEx->pRegisterFile);
			break;

		case OpCode::Jmp:

			jmp(pEx->instruction.value(), pEx->pLocalPC, pEx->pRegisterFile);
			break;
			
		case OpCode::Bne:
			if (pEx->instruction.value().rs1 == XSIM_CID_INDEX)
				bneWithCoreID(
					pEx->instruction.value(), 
					pEx->pLocalPC, 
					pEx->pGlobalPC, 
					pEx->pRegisterFile);
			else
				bne(pEx->instruction.value(), pEx->pLocalPC, pEx->pRegisterFile);
			break;
		
		default:
			break;
		}
	}

	bool ExecuteStage::DetectHazard(PipelineRegister& rEx, PipelineRegister& rWb) {

		if (!rWb.stateless && rWb.instruction.has_value()) {

			Instruction& wbInstr = rWb.instruction.value();
			Instruction& exInstr = rEx.instruction.value();

			if (wbInstr.rd != UINT64_MAX &&
				(exInstr.rs1 == wbInstr.rd || exInstr.rs2 == wbInstr.rd)) {

				XSIM_INFO("\t[Stage::Execute]:")
				XSIM_INFO("\t\t[Hazard]: RAW Hazard detected");
				XSIM_INFO("\t\t[Pending Instruction in WB]:\t" << rWb.rawInstruction.value());
				XSIM_INFO("\t\t[Dependent Instruction in EX]:\t" << rEx.rawInstruction.value());
				return true;
			}
		}

		return false;
	}


	void ExecuteStage::add(Instruction& rInstruction, RegisterFile_T* pRegisterFile) {

		rInstruction.rdv =
			(*pRegisterFile)[rInstruction.rs1] + (*pRegisterFile)[rInstruction.rs2];
	}

	void ExecuteStage::sub(Instruction& rInstruction, RegisterFile_T* pRegisterFile) {

		rInstruction.rdv =
			(*pRegisterFile)[rInstruction.rs1] - (*pRegisterFile)[rInstruction.rs2];
	}

	void ExecuteStage::mul(Instruction& rInstruction, RegisterFile_T* pRegisterFile) {

		rInstruction.rdv =
			(*pRegisterFile)[rInstruction.rs1] * (*pRegisterFile)[rInstruction.rs2];
	}

	void ExecuteStage::div(Instruction& rInstruction, RegisterFile_T* pRegisterFile) {

		rInstruction.rdv =
			(*pRegisterFile)[rInstruction.rs1] / (*pRegisterFile)[rInstruction.rs2];
	}

	void ExecuteStage::addi(Instruction& rInstruction, RegisterFile_T* pRegisterFile) {

		rInstruction.rdv =
			(*pRegisterFile)[rInstruction.rs1] + rInstruction.imm;
	}

	void ExecuteStage::bne(Instruction& rInstruction, int64_t* pPC, RegisterFile_T* pRegisterFile) {

		rInstruction.rs1v != rInstruction.rs2v ?
			(*pPC) += rInstruction.imm : (*pPC) += 4;
	}

	void ExecuteStage::jmp(Instruction& rInstruction, int64_t* pPC, RegisterFile_T* pRegisterFile) {
		
		(*pPC) += rInstruction.imm;
	}

	void ExecuteStage::jal(Instruction& rInstruction, int64_t* pPC, RegisterFile_T* pRegisterFile) {

		rInstruction.rdv = (XSIM_PC_INDEX) + 4;
		(*pPC) += rInstruction.imm;
	}

	void ExecuteStage::bneWithCoreID(
		Instruction& rInstruction, 
		int64_t* pLocalPC, 
		int64_t* pGlobalPC, 
		RegisterFile_T* pRegisterFile) {

		if ((*pRegisterFile)[XSIM_CID_INDEX] != rInstruction.rs2) {

			*pLocalPC = *pGlobalPC + rInstruction.imm;
			XSIM_INFO("\t[ExecutionUnit]: This Core Should Branch");
			XSIM_INFO("\t[ExecutionUnit]: Branch Target: " << rInstruction.imm);
		}
		else {

			*pLocalPC += 4;
			XSIM_INFO("\t[ExecutionUnit]: This Core Should Continue Sequentially");
		}

	}
}
