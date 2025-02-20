#include "Memory.h"

namespace xsim {
	int64_t Memory::Load(uint64_t address) {

		std::lock_guard<std::mutex> lock(memMutex);
		if (address < memory.size()) {
			return memory[address];
		}
		else {
			std::cerr << "Invalid memory access at address " << address << "\n";
			return 0;
		}
	}

	void Memory::Store(uint64_t address, int64_t value) {

		std::lock_guard<std::mutex> lock(memMutex);
		if (address < memory.size()) {
			memory[address] = value;
		}
		else {
			std::cerr << "Invalid memory access at address " << address << "\n";
		}
	}

} // namespace xsim
