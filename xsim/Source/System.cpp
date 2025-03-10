#include "System.h"

namespace xsim {

    System::System(SystemConfig* pSystemConfig, std::string& rExecutable) :
        m_GlobalPC(0),
        pSystemConfig(pSystemConfig),
        m_ExecutableSS(rExecutable),
        m_Labels(),
        m_Cores(),
        m_Fetcher(),
        m_Metrics() {

        m_Cores.reserve(pSystemConfig->numCores);

        for (uint32_t i = 0; i < pSystemConfig->numCores; ++i) {

            m_Cores.emplace_back(i, &m_GlobalPC);
        }

        // Pre-Process Labels for DecodeStage
        ProcessLabels();

        // Set Stream for Fetcher
        m_Fetcher.SetStream(m_ExecutableSS);

        // Set Buffer for FetchStage & Set Labels for DecodeStage
        for (uint32_t i = 0; i < pSystemConfig->numCores; ++i) {

            m_Cores[i].GetFetchStage().SetBuffer(&m_Fetcher.GetBuffer());
            m_Cores[i].GetDecodeStage().SetLabels(m_Labels);
            m_Cores[i].GetMemoryStage().SetMemorySize(pSystemConfig->ramSize);
        }
    }

    void System::Execute() {

        bool FetchSignal = true, PendingInstructions = true;

        while (m_Fetcher.Fetch() || PendingInstructions) {

            for (uint32_t i = 0; i < pSystemConfig->numCores; ++i) {

                XSIM_INFO("[Core #" << i << "]: ");
                m_Cores[i].Cycle(&m_Metrics);
                m_Cores[i].AdvancePC();
                FetchSignal = m_Cores[i].GetFetchSignal();
                PendingInstructions = m_Cores[i].HasPendingInstructions();
            }

            m_GlobalPC += 4;

            XSIM_INFO("---------- Executed Cycle " << m_Metrics.cycles << " ----------\n");

            ++m_Metrics.cycles;

            if (FetchSignal)
                m_Fetcher.GetBuffer().pop_front();
        }

        XSIM_INFO("---------- Metrics ----------");
        XSIM_INFO("[Cycles]: " << m_Metrics.cycles);
        XSIM_INFO("[Stalls]: " << m_Metrics.stalls);
    }

    void System::ProcessLabels() {

        std::stringstream TempStream;
        std::string Line = "";
        size_t AddressCounter = 0ULL;

        while (std::getline(m_ExecutableSS, Line)) {
            size_t CommentPosition = Line.find("#");

            if (CommentPosition != std::string::npos)
                Line = Line.substr(0, CommentPosition);

            Line = TrimString(Line);

            if (Line.empty()) continue;

            if (Line.back() == ':') {
                m_Labels[Line.substr(0, Line.length() - 1)] = AddressCounter;
            }
            else {
                TempStream << Line << '\n';
                AddressCounter += 4;
            }
        }

        m_ExecutableSS.str(TempStream.str());
        m_ExecutableSS.clear();
    }

} // namespace xsim