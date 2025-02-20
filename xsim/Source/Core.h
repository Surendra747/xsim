
#include "Foundation.h"

namespace xsim {

	class Core {
	public:
		Core(const Core&) = default;
		Core(Core&&) = default;
		~Core() = default;

		Core(const uint32_t& _CoreID);

		const uint32_t& GetCoreID() const noexcept { return m_CoreID; }

		uint64_t				PC;
		std::vector<int64_t>	RegisterFile;
		
	private:
		uint32_t				m_CoreID;
	};

} // namespace xsim
