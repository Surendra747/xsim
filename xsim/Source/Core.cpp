
#include "Core.h"

namespace xsim {
	Core::Core(const uint32_t& _CoreID)
		: RegisterFile(32), PC(0), m_CoreID(_CoreID) {

	}

} // namespace xsim
