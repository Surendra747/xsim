
#include "System.h"

std::string LoadExecutableFromFile(const std::string& filePath) {

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return "";
    }

    std::ostringstream executable;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            executable << line << "\n"; 
        }
    }

    file.close();
    return executable.str();
}

int main(int argc, char* argv[]) {

    std::string Executable, ExecutablePath;
    uint32_t NumCores = 4U;
    uint64_t MemorySize = 1024ULL;

    for (int i = 0; i < argc; ++i) {

        if (std::string(argv[i]) == "--source" && i + 1 < argc) {

            ExecutablePath = argv[i + 1];
        }
        if (std::string(argv[i]) == "--num-cores" && i + 1 < argc) {

            NumCores = std::stoul(argv[i + 1]);
        }
        if (std::string(argv[i]) == "--memory-size" && i + 1 < argc) {

            MemorySize = std::stoull(argv[i + 1]);
        }
    }

    Executable = LoadExecutableFromFile(ExecutablePath);

	std::cout << "-------------------- XSimulator --------------------"
		<< std::endl;

    //if (Executable.empty()) {
    //    Executable =
    //        "ADDI X2, X0, 300\n"

    //        // ✅ RAW Hazard: X3 depends on X2 (Should Stall)
    //        "ADDI X3, X2, 300\n"

    //        // ✅ Another RAW Hazard: X4 depends on X3 (Should Stall)
    //        "ADDI X4, X3, 500\n"

    //        // ✅ Branch Instruction (Only Core #1 Should Take the Branch)
    //        "BNE CID, 1, LOOP\n"

    //        // ✅ X5 depends on X2 (No stall, but checks forwarding if enabled)
    //        "ADDI X5, X2, 600\n"

    //        // ✅ X6 depends on X3 (Possible stall if no forwarding)
    //        "ADDI X6, X3, 700\n"

    //        // ✅ Memory Dependency: Load X7 from address 0
    //        "LW X7, 0(X0)\n"

    //        // ✅ Store X6 at address 4
    //        "SW X6, 4(X0)\n"

    //        "LOOP:\n"
    //        // ✅ X8 depends on X4 (Core #1 skips due to branch)
    //        "ADDI X8, X4, 900\n"

    //        // ✅ X9 depends on X5 (RAW hazard test)
    //        "ADDI X9, X5, 1000\n"

    //        // ✅ Memory Read after Write (Hazard if unhandled)
    //        "LW X10, 4(X0)\n"

    //        // ✅ Store X9 at address 8 (Memory dependency check)
    //        "SW X9, 8(X0)\n"
    //        ;
    //}

    if (Executable.empty()) {
        Executable =
            // ✅ Initialize registers
            "ADDI X1, X0, 0\n"   // sum = 0
            "ADDI X2, X0, 0\n"   // i = 0
            "ADDI X10, X0, 25\n"
            // ✅ Set compute unit ID (Assume it's preloaded in CID)
            "MUL X3, CID, X10\n"   // Start index = CID * 25
            "ADDI X4, X3, 25\n"   // End index = Start index + 25

            "LOOP:\n"
            "BNE X3, X4, BODY\n"  // If start != end, continue
            "JMP EXIT\n"            // Else, exit

            "BODY:\n"
            "LW X5, 0(X3)\n"      // Load a[i] into X5
            "ADD X1, X1, X5\n"    // sum += a[i]
            "ADDI X3, X3, 4\n"    // i++
            //"JMP LOOP\n"

            "EXIT:\n"
            // ✅ Only Compute Unit 1 prints the sum
            "BNE CID, 1, DONE\n"
            "SW X1, 0(X0)\n"      // Store final sum at address 0

            "DONE:\n"
            ;
    }

	xsim::SystemConfig systemInfo{ .numCores = NumCores, .ramSize = MemorySize };
	xsim::System system(&systemInfo, Executable);

	system.Execute();

	return 0;
}
