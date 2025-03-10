#include "DecodeStage.h"

namespace xsim {

	DecodeStage::DecodeStage() :
		PipelineStage(PipelineStageType::Decode),
		m_Instruction(),
		m_InstructionBuffer(),
		m_Labels(){

	}

	bool DecodeStage::Cycle(
		PipelineRegister* pThisStageRegister, 
		PipelineRegister* pNextStageRegister, 
		PipelineMetrics* pMetrics) noexcept {

		if (!pThisStageRegister->stateless && pNextStageRegister->stateless) {

			if (!pThisStageRegister->rawInstruction.has_value())
				return true;

			Decode(pThisStageRegister->rawInstruction.value());
			pThisStageRegister->instruction = m_Instruction;

			XSIM_INFO("\t[Stage::Decode]:\tDecoded " << pThisStageRegister->rawInstruction.value());

			// Forward state to next Stage
			*pNextStageRegister = *pThisStageRegister;
			// Reset this Stage
			pThisStageRegister->Reset();
		}

		return true;
	}

	bool DecodeStage::Cycle2(
		PipelineRegister* pThisStageRegister, 
		PipelineRegister* pNextStageRegister, 
		PipelineRegister* pForwardRegister, 
		PipelineMetrics* pMetrics) noexcept {
		return true;
	}

	void DecodeStage::Reset(PipelineRegister* pThisStageRegister) noexcept {

		m_Instruction.reset();
		m_InstructionBuffer.clear();
		pThisStageRegister->Reset();
	}
	
	void DecodeStage::Decode(std::string& rInstruction) noexcept {

		static const std::regex RegexPattern(R"(^(\w+)\s+([^,]+)(?:,\s*([^,]+))?(?:,\s*(.+))?$)");
		std::smatch Match;

		if (!std::regex_match(rInstruction, Match, RegexPattern)) {

			XSIM_LOG("[Stage::Decode]: Unknown Instruction");
			return;
		}
		
		Instruction _Instruction;
		std::string _OpCode = Match[1];
		_Instruction.opcode = StringToOpCode(_OpCode);

		if (_OpCode == "ADD" || _OpCode == "SUB" || _OpCode == "MUL" || _OpCode == "DIV") {

			_Instruction.rd = ParseRegister(Match[2]);
			Match[3] == "CID" ?
				_Instruction.rs1 = XSIM_CID_INDEX : _Instruction.rs1 = ParseRegister(Match[3]);
			Match[4] == "CID" ?
				_Instruction.rs2 = XSIM_CID_INDEX : _Instruction.rs2 = ParseRegister(Match[4]);
			_Instruction.rdWrite = true;
			_Instruction.rs1Write = true;
			_Instruction.rs2Write = true;
		}
		else if (_OpCode == "ADDI") {

			_Instruction.rd = ParseRegister(Match[2]);
			_Instruction.rs1 = ParseRegister(Match[3]);
			_Instruction.imm = std::stoi(Match[4]);
			_Instruction.rs1Write = true;
			_Instruction.rdWrite = true;
		}
		else if (_OpCode == "LW" || _OpCode == "SW") {

			std::string SecondPart = Match[3];
			std::regex	MemoryPattern(R"((-?\d+)\(([^\)]+)\))");
			std::smatch MemoryMatch;

			if (std::regex_match(SecondPart, MemoryMatch, MemoryPattern)) {

				if (_OpCode == "LW") {

					_Instruction.rd = ParseRegister(Match[2]);
					_Instruction.rdWrite = true;
				}
				else {

					_Instruction.rs2 = ParseRegister(Match[2]);
					_Instruction.rs2Write = true;
				}

				_Instruction.imm = std::stoi(MemoryMatch[1]);
				_Instruction.rs1 = ParseRegister(MemoryMatch[2]);
				_Instruction.rs1Write = true;
			}
		}
		else if (_OpCode == "BNE") {

			if (Match[2] == "cid" || Match[2] == "CID") {

				_Instruction.isCIDBranch = true;
				_Instruction.rs2 = std::stoul(Match[3]);
				_Instruction.rs1 = XSIM_CID_INDEX;
			}
			else {

				_Instruction.rs1 = ParseRegister(Match[2]);
				_Instruction.rs2 = ParseRegister(Match[3]);
				std::string Label = Match[4];
			}
			std::string Label = Match[4];
			if (m_Labels.find(Label) != m_Labels.end())
				_Instruction.imm = m_Labels[Label] - (m_InstructionBuffer.size() * 4);

			_Instruction.rs1Write = true;
			_Instruction.rs2Write = true;
		}
		else if (_OpCode == "JMP") {

			std::string Label = Match[2];
			if (m_Labels.find(Label) != m_Labels.end())
				_Instruction.imm = m_Labels[Label] - (m_InstructionBuffer.size() * 4);
		}

		m_Instruction = _Instruction;
	}

	uint64_t DecodeStage::ParseRegister(const std::string& rRegister) noexcept {

		if (rRegister[0] == 'x' || rRegister[0] == 'X')
			return std::stoul(rRegister.substr(1));

		return UINT64_MAX;
	}
}
