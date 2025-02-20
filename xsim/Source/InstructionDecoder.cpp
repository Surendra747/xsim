#include "InstructionDecoder.h"

namespace xsim {

	InstructionDecoder::InstructionDecoder(std::string& Executable, size_t InstructionQueueSize)
		: Instructions(), m_InstructionQueueSize(InstructionQueueSize), 
		m_ExecutableSS(Executable) {

		std::string Line;

		for (size_t i = 0; i < InstructionQueueSize ; ++i) {
			std::getline(m_ExecutableSS, Line);

			if (!Line.empty())
				InternalDecode(Line);
		}
	}

	void InstructionDecoder::Decode(){

		std::string Line;

		Instructions.pop_front();

		std::getline(m_ExecutableSS, Line);

		if (!Line.empty())
			InternalDecode(Line);
	}

	std::optional<OpCode> InstructionDecoder::StringToOpCode(const std::string& OpcodeStr)
	{
		if (OpcodeStr == "ADD")
			return OpCode::Add;
		if (OpcodeStr == "SUB")
			return OpCode::Sub;
		if (OpcodeStr == "MUL")
			return OpCode::Mul;
		if (OpcodeStr == "DIV")
			return OpCode::Div;
		if (OpcodeStr == "LW")
			return OpCode::Lw;
		if (OpcodeStr == "SW")
			return OpCode::Sw;
		if (OpcodeStr == "BNE")
			return OpCode::Bne;
		if (OpcodeStr == "JAL")
			return OpCode::Jal;
		if (OpcodeStr == "ADDI")
			return OpCode::Addi;
		return std::nullopt;
	}
	
	void InstructionDecoder::InternalDecode(const std::string& Line) {
		// Patterns for different instruction formats
		std::regex rtype_pattern(R"(^(\w+)\s+(X\d+),\s*(X\d+),\s*(X\d+)$)");
		std::regex mem_pattern(R"(^(\w+)\s+(X\d+),\s*([-]?\d+)\((X\d+)\)$)");
		std::regex bne_pattern(R"(^BNE\s+(X\d+),\s*(X\d+),\s*(-?\d+)$)");
		std::regex jal_pattern(R"(^JAL\s+(X\d+),\s*(-?\d+)$)");
		std::regex addi_pattern(R"(^ADDI\s+(X\d+),\s*(X\d+),\s*(-?\d+)$)");

		std::smatch match;
		std::istringstream lineStream(Line);
		std::string opcodeToken;
		lineStream >> opcodeToken;

		auto maybeOp = StringToOpCode(opcodeToken);
		if (!maybeOp.has_value()) {
			std::cerr << "Unknown opcode: " << opcodeToken << "\n";
			return;
		}

		Instruction inst{};
		inst.opcode = maybeOp.value();

		// Handle each instruction format exclusively:
		if (inst.opcode == OpCode::Bne) {
			// BNE expects: "BNE X1, X2, <imm>"
			if (std::regex_match(Line, match, bne_pattern)) {
				auto mapRegister = [](const std::string& regStr) -> uint32_t {
					return std::stoul(regStr.substr(1)); // Remove 'X'
					};
				inst.rs1 = mapRegister(match.str(1));
				inst.rs2 = mapRegister(match.str(2));
				inst.imm = std::stoi(match.str(3));  // Immediate offset
				Instructions.push_back(inst);
			}
			else {
				std::cerr << "Failed to parse BNE Instruction: " << Line << "\n";
			}
		}
		else if (inst.opcode == OpCode::Jal) {
			// JAL expects: "JAL X<rd>, <imm>"
			if (std::regex_match(Line, match, jal_pattern)) {
				auto mapRegister = [](const std::string& regStr) -> uint32_t {
					return std::stoul(regStr.substr(1));
					};
				inst.rd = mapRegister(match.str(1));
				inst.imm = std::stoi(match.str(2));  // Immediate offset
				Instructions.push_back(inst);
			}
			else {
				std::cerr << "Failed to parse JAL Instruction: " << Line << "\n";
			}
		}
		else if (inst.opcode == OpCode::Addi) {
			// ADDI expects: "ADDI X<rd>, X<rs>, <imm>"
			if (std::regex_match(Line, match, addi_pattern)) {
				auto mapRegister = [](const std::string& regStr) -> uint32_t {
					return std::stoul(regStr.substr(1));
					};
				inst.rd = mapRegister(match.str(1));
				inst.rs1 = mapRegister(match.str(2));
				inst.imm = std::stoi(match.str(3));   // Immediate value
				Instructions.push_back(inst);
			}
			else {
				std::cerr << "Failed to parse ADDI Instruction: " << Line << "\n";
			}
		}
		else if (inst.opcode == OpCode::Lw || inst.opcode == OpCode::Sw) {
			// Memory instructions: "LW X<rd/rs2>, <imm>(X<rs1>)"
			if (std::regex_match(Line, match, mem_pattern)) {
				auto mapRegister = [](const std::string& regStr) -> uint32_t {
					return std::stoul(regStr.substr(1));
					};
				inst.imm = std::stoi(match.str(3));  // Immediate offset
				if (inst.opcode == OpCode::Lw) {
					inst.rd = mapRegister(match.str(2));
					inst.rs1 = mapRegister(match.str(4));
				}
				else { // SW
					inst.rs2 = mapRegister(match.str(2));
					inst.rs1 = mapRegister(match.str(4));
				}
				Instructions.push_back(inst);
			}
			else {
				std::cerr << "Failed to parse memory instruction: " << Line << "\n";
			}
		}
		else {
			// Assume R-type instructions: "ADD X<rd>, X<rs1>, X<rs2>" or similar
			if (std::regex_match(Line, match, rtype_pattern)) {
				auto mapRegister = [](const std::string& regStr) -> uint32_t {
					return std::stoul(regStr.substr(1));
					};
				inst.rd = mapRegister(match.str(2));
				inst.rs1 = mapRegister(match.str(3));
				inst.rs2 = mapRegister(match.str(4));
				Instructions.push_back(inst);
			}
			else {
				std::cerr << "Failed to parse R-type instruction: " << Line << "\n";
			}
		}
	}

}
