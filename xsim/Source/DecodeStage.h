#pragma once

#include "PipelineStage.h"

namespace xsim {

	class DecodeStage : public PipelineStage {

	public:

		DecodeStage();
		~DecodeStage() = default;

		void SetLabels(std::unordered_map<std::string, uint64_t> LabelMap) noexcept { m_Labels = LabelMap; }
		
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
		
		std::optional<Instruction>					m_Instruction;
		std::deque<Instruction>						m_InstructionBuffer;
		std::unordered_map<std::string, uint64_t>	m_Labels;
	
	private:

		void			Decode(std::string& rRawInstruction) noexcept;
		uint64_t		ParseRegister(const std::string& rRegister) noexcept;
	};
}

