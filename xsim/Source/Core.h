#pragma once

#include "FetchStage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "WritebackStage.h"

#include "Memory.h"

namespace xsim {

	class Core {

	public:
		
		Core(uint32_t& rCoreID, int64_t* pGlobalPC);
		~Core() = default;
		
		void Cycle(PipelineMetrics* pMetrics);
		bool HasPendingInstructions() const;

		void ResetState() noexcept;

		const uint32_t&		GetCoreID() const	noexcept { return m_CoreID; }
		bool				GetStallSignal()	noexcept { return m_StallSignal; }
		bool				GetFetchSignal()	noexcept { return m_FetchSignal; }
		DecodeStage&		GetDecodeStage()	noexcept { return m_DecodeStage; }
		FetchStage&			GetFetchStage()		noexcept { return m_FetchStage; }
		ExecuteStage&		GetExecuteStage()	noexcept { return m_ExecuteStage; }
		MemoryStage&		GetMemoryStage()	noexcept { return m_MemoryStage; }
		WritebackStage&		GetWritebackStage() noexcept { return m_WritebackStage; }
		PipelineRegister&	GetIF()				noexcept { return m_IF; }

		void AdvancePC() noexcept { m_LocalPC += 4; }

	private:

		uint32_t		m_CoreID;
		int64_t*		pGlobalPC;
		int64_t			m_LocalPC;
		bool			m_StallSignal;
		bool			m_FetchSignal;

		RegisterFile_T	m_RegisterFile;

		FetchStage		m_FetchStage;
		DecodeStage		m_DecodeStage;
		ExecuteStage	m_ExecuteStage;
		MemoryStage		m_MemoryStage;
		WritebackStage	m_WritebackStage;

		PipelineRegister	m_IF, m_ID, m_EX, m_MEM, m_WB;
	};

} // namespace xsim
