#include "FetchStage.h"

namespace xsim {

	FetchStage::FetchStage() :
		PipelineStage(PipelineStageType::Fetch),
		m_DoneFetchingIntoBuffer(false),
		pRawInstructionBuffer(nullptr),
		m_RawInstruction() {

	}

	bool FetchStage::Cycle(
		PipelineRegister* pThisStageRegister,
		PipelineRegister* pNextStageRegister,
		PipelineMetrics* pMetrics) noexcept {

		if (pRawInstructionBuffer->empty()) return false;

		if (pThisStageRegister->stateless) {

			Fetch();
			
			pThisStageRegister->rawInstruction = m_RawInstruction;
			pThisStageRegister->stateless = false;
			
			XSIM_INFO("\t[Stage::Fetch]:\t\tFetched " << pThisStageRegister->rawInstruction.value());

			// Forward state to next Stage
			*pNextStageRegister = *pThisStageRegister;
			// Reset this Stage
			pThisStageRegister->Reset();

			return true;
		}
	}

	bool FetchStage::Cycle2(
		PipelineRegister* pThisStageRegister, 
		PipelineRegister* pNextStageRegister, 
		PipelineRegister* pForwardRegister, 
		PipelineMetrics* pMetrics) noexcept {
		return true;
	}

	void FetchStage::Reset(PipelineRegister* pThisStageRegister) noexcept {

		pThisStageRegister->rawInstruction = std::nullopt;
		pThisStageRegister->stateless = true;

		m_RawInstruction.reset();
	}

	void FetchStage::Fetch() noexcept {

		if (!pRawInstructionBuffer->empty())
			m_RawInstruction = pRawInstructionBuffer->front();
	}
	
}
