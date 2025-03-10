#pragma once

#include "PipelineStage.h"

namespace xsim {
	
	class WritebackStage : public PipelineStage {

	public:

		WritebackStage();
		~WritebackStage() = default;

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

		void Writeback(Instruction& rInstruction, RegisterFile_T* pRegisterFile) noexcept;
	};
}

