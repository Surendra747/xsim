#pragma once

#include "PipelineStage.h"

namespace xsim {

	class FetchStage : public PipelineStage {

	public:

		FetchStage();
		~FetchStage() = default;

		void SetBuffer(std::deque<std::string>* pBuffer)	noexcept { pRawInstructionBuffer = pBuffer; }

		virtual bool Cycle(
			PipelineRegister* pThisStageRegister,
			PipelineRegister* pNextStageRegister,
			PipelineMetrics* pMetrics)	noexcept override;
		virtual bool Cycle2(
			PipelineRegister* pThisStageRegister,
			PipelineRegister* pNextStageRegister,
			PipelineRegister* pForwardRegister,
			PipelineMetrics* pMetrics) noexcept override;
		virtual void Reset(PipelineRegister* pThisStageRegister) noexcept override;

	private:

		bool						m_DoneFetchingIntoBuffer;
		std::deque<std::string>*	pRawInstructionBuffer;
		std::optional<std::string>	m_RawInstruction;

	private:

		void Fetch()			noexcept;
	};
}

