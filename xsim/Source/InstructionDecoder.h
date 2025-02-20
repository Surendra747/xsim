#pragma once

#include "Instruction.h"

namespace xsim {
	class InstructionDecoder {
	public:

		InstructionDecoder(const InstructionDecoder&) = delete;
		InstructionDecoder(InstructionDecoder&&) = delete;
		InstructionDecoder operator= (const InstructionDecoder&) = delete;

		InstructionDecoder(std::string& Executable, size_t InstructionQueueSize);
		~InstructionDecoder() = default;

		void Decode();

		size_t& GetInstructionQueueSize() noexcept { return m_InstructionQueueSize; }

		std::deque<Instruction> Instructions;

	private:
		size_t				m_InstructionQueueSize;
		std::stringstream	m_ExecutableSS;

	private:
		std::optional<OpCode> StringToOpCode(const std::string& OpcodeStr);
		void InternalDecode(const std::string& Line);
	};
}

