#pragma once

#include "Foundation.h"


namespace xsim {

	enum class PipelineStageType {

		Fetch,
		Decode,
		Execute,
		Memory,
		Writeback,
		Unknown
	};

	enum class PipelineRegisterState {

		Valid,
		Invalid
	};

	struct PipelineRegister {
		
		bool										stateless;
		std::optional<std::string>					rawInstruction;
		std::optional<Instruction>					instruction;
		int64_t*									pLocalPC;
		int64_t*									pGlobalPC;
		RegisterFile_T*								pRegisterFile;

		PipelineRegister(int64_t* _pLocalPC, int64_t* _pGlobalPC, RegisterFile_T* _pRegisterFile) :
			stateless(true),
			rawInstruction(std::nullopt),
			instruction(std::nullopt),
			pLocalPC(_pLocalPC),
			pGlobalPC(_pGlobalPC),
			pRegisterFile(_pRegisterFile)
		{};

		void Reset() noexcept {

			stateless = true;
			rawInstruction.reset();
			instruction.reset();
		}
	};

	struct PipelineMetrics {

		uint64_t	stalls;
		uint64_t	cycles;
	};

	class PipelineStage {

	public:

		PipelineStage(PipelineStageType Type);
		~PipelineStage() = default;

		virtual bool Cycle(
			PipelineRegister* pThisStageRegister, 
			PipelineRegister* pNextStageRegister,
			PipelineMetrics* pMetrics) noexcept = 0;
		virtual bool Cycle2(
			PipelineRegister* pThisStageRegister,
			PipelineRegister* pNextStageRegister,
			PipelineRegister* pForwardRegister,
			PipelineMetrics* pMetrics
			) noexcept = 0;
		virtual void Reset(PipelineRegister* pThisStageRegister) noexcept = 0;

	protected:

		PipelineStageType	m_PipelineStage;
	};
}

