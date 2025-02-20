#include "Foundation.h"

namespace xsim {

	class Memory {
	public:
		Memory(const Memory&) = delete;
		Memory(Memory&&) = delete;
		Memory operator=(const Memory&) = delete;
		~Memory() = default;
		Memory(size_t size) : memory(size, 0) {}

		int64_t Load(uint64_t address);
		void Store(uint64_t address, int64_t value);

	private:
		std::vector<int64_t> memory;
		std::mutex memMutex;
	};

} // namespace xsim
