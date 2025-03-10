
#include "Core.h"

namespace xsim {

    Core::Core(uint32_t& rCoreID, int64_t* pGlobalPC)
        :
        m_CoreID(rCoreID),
        pGlobalPC(pGlobalPC),
        m_LocalPC(0),
        m_StallSignal(false),
        m_RegisterFile(XSIM_NUM_REGISTERS, 0),
        m_FetchStage(),
        m_DecodeStage(),
        m_ExecuteStage(),
        m_MemoryStage(),
        m_WritebackStage(),
        m_IF(&m_LocalPC, pGlobalPC, &m_RegisterFile),
        m_ID(&m_LocalPC, pGlobalPC, &m_RegisterFile),
        m_EX(&m_LocalPC, pGlobalPC, &m_RegisterFile),
        m_MEM(&m_LocalPC, pGlobalPC, &m_RegisterFile),
        m_WB(&m_LocalPC, pGlobalPC, &m_RegisterFile) {

        m_RegisterFile[XSIM_CID_INDEX] = rCoreID;
    }

    void Core::Cycle(PipelineMetrics* pMetrics) {

        bool NotStalledCycle = true;

        if (NotStalledCycle) {
            NotStalledCycle = m_WritebackStage.Cycle(&m_WB, &m_IF, pMetrics);
            m_StallSignal = NotStalledCycle;
        }
        if (NotStalledCycle) {
            NotStalledCycle = m_MemoryStage.Cycle(&m_MEM, &m_WB, pMetrics);
            m_StallSignal = NotStalledCycle;
        }
        if (NotStalledCycle) {
            NotStalledCycle = m_ExecuteStage.Cycle2(&m_EX, &m_MEM, &m_WB, pMetrics);
            m_StallSignal = NotStalledCycle;
        }
        if (NotStalledCycle) {
            NotStalledCycle = m_DecodeStage.Cycle(&m_ID, &m_EX, pMetrics);
            m_StallSignal = NotStalledCycle;
        }
        if (NotStalledCycle) {
            
            m_FetchSignal = m_FetchStage.Cycle(&m_IF, &m_ID, pMetrics);
            m_StallSignal = NotStalledCycle;
        }
    }

    bool Core::HasPendingInstructions() const {

        return  (!m_IF.stateless) ||
            (!m_ID.stateless) ||
            (!m_EX.stateless) ||
            (!m_MEM.stateless) ||
            (!m_WB.stateless);
    }

    void Core::ResetState() noexcept {

        m_IF.Reset();
        m_ID.Reset();
        m_EX.Reset();
        m_MEM.Reset();
        m_WB.Reset();
    }


} // namespace xsim
