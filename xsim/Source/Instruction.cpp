#include "Instruction.h"

namespace xsim {

	/**********************************************************************************
	 * Instructions Functions Definition
	 **********************************************************************************/
	namespace instruction {

		void add(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			pCore->RegisterFile[Instruction.rd] = pCore->RegisterFile[Instruction.rs1] +
				pCore->RegisterFile[Instruction.rs2];

			std::cout << "Executing ADD on Core #" << pCore->GetCoreID() <<
				" rd(" << Instruction.rd << ") rs1(" << Instruction.rs1 << ")"
				<< " rs2(" << Instruction.rs2 << ")" << std::endl;
		}

		void sub(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			pCore->RegisterFile[Instruction.rd] = pCore->RegisterFile[Instruction.rs1] -
				pCore->RegisterFile[Instruction.rs2];

			std::cout << "Executing SUB on Core #" << pCore->GetCoreID() <<
				" rd(" << Instruction.rd << ") rs1(" << Instruction.rs1 << ")"
				<< " rs2(" << Instruction.rs2 << ")" << std::endl;
		}

		void mul(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			pCore->RegisterFile[Instruction.rd] = pCore->RegisterFile[Instruction.rs1] *
				pCore->RegisterFile[Instruction.rs2];

			std::cout << "Executing MUL on Core #" << pCore->GetCoreID() <<
				" rd(" << Instruction.rd << ") rs1(" << Instruction.rs1 << ")"
				<< " rs2(" << Instruction.rs2 << ")" << std::endl;
		}

		void div(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			pCore->RegisterFile[Instruction.rd] = pCore->RegisterFile[Instruction.rs1] /
				pCore->RegisterFile[Instruction.rs2];

			std::cout << "Executing DIV on Core #" << pCore->GetCoreID() <<
				" rd(" << Instruction.rd << ") rs1(" << Instruction.rs1 << ")"
				<< " rs2(" << Instruction.rs2 << ")" << std::endl;
		}

		void lw(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			uint32_t effectiveAddress = pCore->RegisterFile[Instruction.rs1] + Instruction.imm;
			int32_t loadedValue = pMemory->Load(effectiveAddress);

			pCore->RegisterFile[Instruction.rd] = loadedValue;

			std::cout << "Executing LW on Core #" << pCore->GetCoreID() <<
				" EFFECTIVE ADDRESS (" << effectiveAddress << ")" << std::endl;
		}

		void sw(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			uint32_t effectiveAddress = pCore->RegisterFile[Instruction.rs1] + Instruction.imm;
			int32_t storeValue = pCore->RegisterFile[Instruction.rs2];
			pMemory->Store(effectiveAddress, storeValue);

			std::cout << "Executing SW on Core #" << pCore->GetCoreID() <<
				" EFFECTIVE ADDRESS (" << effectiveAddress << ")" << std::endl;
		}

		void bne(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			if (pCore->RegisterFile[Instruction.rs1] != pCore->RegisterFile[Instruction.rs2]) {
				pCore->PC += Instruction.imm * 2; 
			}
			else {
				pCore->PC += 4;
			}
			std::cout << "Executing BNE on Core #" << pCore->GetCoreID() <<
				" BRANCHED TO(" << pCore->PC << ")" << std::endl;
		}

		void jal(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			pCore->RegisterFile[Instruction.rd] = pCore->PC + 4;  
			pCore->PC += Instruction.imm * 2;  

			std::cout << "Executing JAL on Core #" << pCore->GetCoreID() <<
				" JUMPED TO(" << pCore->PC << ")" << std::endl;
		}

		void addi(const Instruction Instruction, Core* pCore, Memory* pMemory) {
			pCore->RegisterFile[Instruction.rd] = pCore->RegisterFile[Instruction.rs1] +
				Instruction.imm;

			std::cout << "Executing ADDI on Core #" << pCore->GetCoreID() <<
				" rd(" << Instruction.rd << ") rs1(" << Instruction.rs1 << ")"
				<< " imm(" << Instruction.imm << ")" << std::endl;
		}


	} // namespace instruction

	/**********************************************************************************
	 * Instruction Dispatch Table Initialization
	 **********************************************************************************/
	InstructionDispatchTable::InstructionDispatchTable()
		: m_DispatchTable(XSIM_NUM_SUPPORTED_OPCODES) {

		m_DispatchTable[static_cast<size_t>(OpCode::Add)] = instruction::add;
		m_DispatchTable[static_cast<size_t>(OpCode::Sub)] = instruction::sub;
		m_DispatchTable[static_cast<size_t>(OpCode::Mul)] = instruction::mul;
		m_DispatchTable[static_cast<size_t>(OpCode::Div)] = instruction::div;

		m_DispatchTable[static_cast<size_t>(OpCode::Lw)] = instruction::lw;
		m_DispatchTable[static_cast<size_t>(OpCode::Sw)] = instruction::sw;

		m_DispatchTable[static_cast<size_t>(OpCode::Bne)] = instruction::bne;
		m_DispatchTable[static_cast<size_t>(OpCode::Jal)] = instruction::jal;
		m_DispatchTable[static_cast<size_t>(OpCode::Addi)] = instruction::addi;
	}

	void InstructionDispatchTable::Dispatch(const Instruction Instruction, 
		Core* pCore, Memory* pMemory) {

		m_DispatchTable[static_cast<size_t>(Instruction.opcode)](Instruction, pCore, pMemory);
	}

} // namespace xsim