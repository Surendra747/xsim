#pragma once

#include "PipelineStage.h"
#include "Memory.h"

namespace xsim {
	
	class MemoryStage : public PipelineStage{

	public:

		MemoryStage();
		~MemoryStage() = default;

		void SetMemorySize(size_t& rSize) { m_Memory.resize(rSize); }

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

		void Dump() noexcept;

	private:

		std::vector<int64_t>	m_Memory;

		int64_t Load(uint64_t& rAddress);
		void	Store(uint64_t& rAddress, int64_t& rValue);
		void	lw(Instruction& rInstruction);
		void	sw(Instruction& rInstruction);
	};
}
