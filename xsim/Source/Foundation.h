#pragma once

#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <source_location>
#include <array>
#include <fstream>

#define XSIM_FETCH_BUFFER_LENGTH	25
#define XSIM_NUM_SUPPORTED_OPCODES	9
#define XSIM_CID_INDEX				32
#define XSIM_PC_INDEX				33
#define XSIM_NUM_REGISTERS			34

#define XSIM_LOG(_MESSAGE_)														\
	std::cout << _MESSAGE_														\
	<< "\n [Function]: " << std::source_location::current().function_name()		\
	<< "\n [File]: " << std::source_location::current().file_name()				\
	<< "\n [Line]: " << std::source_location::current().line()					\
	<< std::endl; __debugbreak();

#define XSIM_INFO(_MESSAGE_)													\
	std::cout << _MESSAGE_														\
	<< std::endl;


namespace xsim {
	
	typedef std::vector<int64_t> RegisterFile_T;

	enum class OpCode { Unknown, Add, Sub, Mul, Div, Lw, Sw, Bne, Jal, Jmp, Addi };

	struct SystemConfig {

		uint32_t									numCores;
		uint64_t									ramSize;
		std::unordered_map<std::string, uint64_t>	opLatencies;
	};

	struct Instruction {

		OpCode		opcode;
		uint64_t	rd, rs1, rs2, imm;
		int64_t		rdv, rs1v, rs2v;
		bool		rdWrite, rs1Write, rs2Write;
		bool		isCIDBranch;

		Instruction() :
			opcode(OpCode::Unknown),
			rd(0ULL), rs1(0ULL), rs2(0ULL), imm(0ULL),
			rdv(0), rs1v(0), rs2v(0),
			rdWrite(false), rs1Write(false), rs2Write(false),
			isCIDBranch(false) 
		{};
	};

	

	static OpCode StringToOpCode(const std::string& rOpCode) {

		static const std::unordered_map<std::string, OpCode> OpCodeMap = {
			{"ADD", OpCode::Add},
			{"SUB", OpCode::Sub},
			{"MUL", OpCode::Mul},
			{"DIV", OpCode::Div},
			{"LW", OpCode::Lw},
			{"SW", OpCode::Sw},
			{"BNE", OpCode::Bne},
			{"JAL", OpCode::Jal},
			{"JMP", OpCode::Jmp},
			{"ADDI", OpCode::Addi}
		};

		return OpCodeMap.at(rOpCode);
	}

	static std::string OpCodeToString(OpCode opcode) {

		static const std::unordered_map<OpCode, std::string> StringMap = {
			{OpCode::Add, "ADD"},
			{OpCode::Sub, "SUB"},
			{OpCode::Mul, "MUL"},
			{OpCode::Div, "DIV"},
			{OpCode::Lw, "LW"},
			{OpCode::Sw, "SW"},
			{OpCode::Bne, "BNE"},
			{OpCode::Jal, "JAL"},
			{OpCode::Jmp, "JMP"},
			{OpCode::Addi, "ADDI"}
		};

		return StringMap.at(opcode);
	}

	static std::string TrimString(const std::string& rString) {

		const std::string Whitespace = " \t\r\n";

		size_t Start = rString.find_first_not_of(Whitespace);
		if (Start == std::string::npos) return "";

		size_t End = rString.find_last_not_of(Whitespace);
		return rString.substr(Start, End - Start + 1);
	}
}
