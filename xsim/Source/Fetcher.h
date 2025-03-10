#pragma once

#include "Foundation.h"

namespace xsim {

	class Fetcher {

	public:

		Fetcher(const Fetcher&) = delete;
		Fetcher(Fetcher&&) = delete;
		Fetcher operator=(const Fetcher&) = delete;

		Fetcher() = default;
		~Fetcher() = default;

		void SetStream(std::istringstream& rStream) { m_ExecutableSS.str(rStream.str()); }

		bool Fetch();

		std::deque<std::string>& GetBuffer() noexcept { return m_RawInstructionBuffer; }
		bool DoneFetching() noexcept { return m_DoneFetching; }

	private:
	
		bool										m_DoneFetching;
		std::string									m_RawInstruction;
		std::deque<std::string>						m_RawInstructionBuffer;
		std::istringstream							m_ExecutableSS;
		
		void		FetchIntoBuffer();
	};
}

