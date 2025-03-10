#pragma once

#include "PipelineStage.h"

namespace xsim {

	class ExecuteStage : public PipelineStage {
		
	public:

		ExecuteStage();
		~ExecuteStage() = default;

		virtual bool Cycle(
			PipelineRegister* pThisStageRegister,
			PipelineRegister* pNextStageRegister,
			PipelineMetrics* pMetrics) noexcept override;
		virtual bool Cycle2(
			PipelineRegister* pThisStageRegister,
			PipelineRegister* pNextStageRegister,
			PipelineRegister* pForwardRegister,
			PipelineMetrics* pMetrics) noexcept override;
		virtual void Reset(PipelineRegister* pThisStageRegister) noexcept override;

	private:

		void Execute(PipelineRegister* pEx);
		bool DetectHazard(PipelineRegister& rEx, PipelineRegister& rWb);

		void add(Instruction& rInstruction, RegisterFile_T* pRegisterFile);
		void sub(Instruction& rInstruction, RegisterFile_T* pRegisterFile);
		void mul(Instruction& rInstruction, RegisterFile_T* pRegisterFile);
		void div(Instruction& rInstruction, RegisterFile_T* pRegisterFile);
		void addi(Instruction& rInstruction, RegisterFile_T* pRegisterFile);

		void bne(Instruction& rInstruction, int64_t* pPC, RegisterFile_T* pRegisterFile);
		void jal(Instruction& rInstruction, int64_t* pPC, RegisterFile_T* pRegisterFile);
		void jmp(Instruction& rInstruction, int64_t* pPC, RegisterFile_T* pRegisterFile);

		void bneWithCoreID(
			Instruction& rInstruction, 
			int64_t* pLocalPC, 
			int64_t* pGlobalPC,
			RegisterFile_T* pRegisterFile);
	};
}

