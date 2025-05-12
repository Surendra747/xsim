#pragma once

#include "PipelineStage.h"
#include "Memory.h"

namespace xsim {
	
	class Cache {
	private:
		// Cache parameters
		static constexpr size_t LINE_SIZE = 64;     // bytes per cache line
		static constexpr size_t CACHE_SIZE = 4096;  // total cache size in bytes
		static constexpr size_t ASSOCIATIVITY = 4;  // ways per set
		
		struct CacheLine {
			bool valid = false;
			bool dirty = false;
			uint64_t tag = 0;
			std::array<int64_t, LINE_SIZE / sizeof(int64_t)> data;
		};
		
		// Cache organization
		std::vector<std::array<CacheLine, ASSOCIATIVITY>> m_Sets;
		
		// Cache statistics
		size_t m_Hits = 0;
		size_t m_Misses = 0;
		
		// Helper methods
		size_t getSetIndex(uint64_t address) const;
		size_t getTagBits(uint64_t address) const;
		size_t getOffset(uint64_t address) const;
		int findLineInSet(size_t setIndex, uint64_t tag) const;
		int selectVictim(size_t setIndex) const;
		
	public:
		Cache();
		bool lookup(uint64_t address, int64_t& data);
		void update(uint64_t address, int64_t data);
		void flush(std::array<int64_t, MEM_SIZE>& mainMemory);
		void dumpStats() const;
	};

	class MemoryStage : public PipelineStage{

	public:

		MemoryStage();
		~MemoryStage() = default;

		void SetMemorySize(size_t& rSize) { m_Memory.resize(rSize); }

		virtual bool Cycle(
			PipelineRegister* pThisStageRegister,
			PipelineRegister* pNextStageRegister,
			PipelineMetrics* pMetrics) noexcept override;
		virtual bool Cycle2(
			PipelineRegister* pThisStageRegister,
			PipelineRegister* pNextStageRegister,
			PipelineRegister* pForwardRegister,
			PipelineMetrics* pMetrics) noexcept override;
		virtual void Reset(PipelineRegister* pThisStageRegister) noexcept override;

		void setCachePolicy(bool writeThrough, bool writeAllocate);

		void DumpCacheStats() const;
		void Dump() noexcept;

	private:

		std::vector<int64_t>	m_Memory;

		int64_t Load(uint64_t& rAddress);
		void	Store(uint64_t& rAddress, int64_t& rValue);
		void	lw(Instruction& rInstruction);
		void	sw(Instruction& rInstruction);
		void setCachePolicy(bool writeThrough, bool writeAllocate);

		Cache m_Cache;  // Add cache member
    
    // Cache policy flags
		bool m_WriteThrough = false;  // false = write-back, true = write-through
		bool m_WriteAllocate = true;  // true = write-allocate, false = write-no-allocate
		
		// Helper methods
		void handleCacheMiss(uint64_t address);
	};
}
