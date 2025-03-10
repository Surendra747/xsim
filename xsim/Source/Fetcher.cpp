#include "Fetcher.h"

namespace xsim {

	bool Fetcher::Fetch() {

		if (m_RawInstructionBuffer.empty()) {

			FetchIntoBuffer();
		}

		if (m_RawInstructionBuffer.empty()) {
			
			m_RawInstruction = "";
			return false;
		}

		//m_RawInstruction = std::move(m_RawInstructionBuffer.front());
		//m_RawInstructionBuffer.pop_front();

		return true;
	}

	void Fetcher::FetchIntoBuffer() {

		if (m_DoneFetching) return;

		if (m_ExecutableSS.eof() || m_ExecutableSS.fail()) {
			m_ExecutableSS.clear();
			m_ExecutableSS.seekg(0, std::ios::beg);
		}

		std::string Line;

		for (size_t i = 0; i < XSIM_FETCH_BUFFER_LENGTH; ++i) {
			if ((!m_ExecutableSS.eof()) && std::getline(m_ExecutableSS, Line)) {
				m_RawInstructionBuffer.emplace_back(Line);
				//XSIM_INFO("[Fetcher]: Fetching <" << Line << "> into buffer");
			}
			else if (m_ExecutableSS.eof()) {
				m_DoneFetching = true;
			}
			else {
				XSIM_LOG("[Fetcher]: Failed std::getline()! \n Stream State: "
					<< "\n\t EOF: " << m_ExecutableSS.eof()
					<< "\n\t BAD: " << m_ExecutableSS.bad()
					<< "\n\t FAIL: " << m_ExecutableSS.fail()
					<< "\n\t POS: " << m_ExecutableSS.tellg());
			}
		}
	}
}
