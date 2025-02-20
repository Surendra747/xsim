#include "Core.h"
#include "Memory.h"

#define XSIM_NUM_SUPPORTED_OPCODES 9
namespace xsim {

	/**********************************************************************************
	 * Opcode definition
	 * Instruction Type definition
	 * Instruction Function Pointer definition
	 **********************************************************************************/
	enum class OpCode { Add, Sub, Mul, Div, Lw, Sw, Bne, Jal, Addi };

	struct Instruction {
		OpCode opcode;
		uint32_t rd, rs1, rs2, imm;
	};

	using PFNInstruction = std::function<void(const Instruction, Core*, Memory*)>;

	/**********************************************************************************
	 * Instruction Functions Prototype
	 **********************************************************************************/
	namespace instruction {

		void add(const Instruction Instruction, Core* pCore, Memory* pMemory);
		void sub(const Instruction Instruction, Core* pCore, Memory* pMemory);
		void mul(const Instruction Instruction, Core* pCore, Memory* pMemory);
		void div(const Instruction Instruction, Core* pCore, Memory* pMemory);
		void lw(const Instruction Instruction, Core* pCore, Memory* pMemory);
		void sw(const Instruction Instruction, Core* pCore, Memory* pMemory);
		void bne(const Instruction Instruction, Core* pCore, Memory* pMemory);
		void jal(const Instruction Instruction, Core* pCore, Memory* pMemory);
		void addi(const Instruction Instruction, Core* pCore, Memory* pMemory);

	} // namespace instruction

	/**********************************************************************************
	 * Instruction Dispatch Table
	 **********************************************************************************/
	class InstructionDispatchTable {
	public:
		InstructionDispatchTable(const InstructionDispatchTable&) = delete;
		InstructionDispatchTable(InstructionDispatchTable&&) = delete;
		InstructionDispatchTable operator=(const InstructionDispatchTable&) = delete;

		InstructionDispatchTable();
		~InstructionDispatchTable() = default;

		void Dispatch(const Instruction Instruction, Core* pCore, Memory* pMemory);

	private:
		std::vector<PFNInstruction> m_DispatchTable;
	};

} // namespace xsim