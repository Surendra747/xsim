
#include "System.h"

int main(int argc, char* argv[]) {

	std::string Executable;

	for (int i = 0; i < argc; ++i) {
		if (std::string(argv[i]) == "-s" && i + 1 < argc) {
			Executable = argv[i + 1];
		}
	}

	std::cout << "-------------------- XSimulator --------------------"
		<< std::endl;

	xsim::SystemInfo systemInfo{ .numCores = 4, .ramSize = 1024 };
	xsim::System system(&systemInfo);

	if (Executable.empty()) {
		Executable =
			"ADDI X1, X0, 5\n"   // X1 = 5
			"ADDI X2, X0, 10\n"  // X2 = 10
			"ADD X3, X1, X2\n"   // X3 = X1 + X2 = 15
			"SUB X4, X3, X1\n"   // X4 = X3 - X1 = 10
			"MUL X5, X4, X1\n"   // X5 = X4 * X1 = 50
			"DIV X6, X5, X1\n"   // X6 = X5 / X1 = 10
			"SW X6, 0(X0)\n"     // Store X6 to memory address 0
			"LW X7, 0(X0)\n"     // Load memory address 0 into X7
			"BNE X7, X2, 8\n"    // If X7 != X2, skip next instruction
			"ADD X8, X0, X0\n"   // X8 = 0 (skipped if branch taken)
			"ADDI X9, X0, 100\n" // X9 = 100
			"JAL X0, 16\n"       // Jump to the end of the program
			"SUB X9, X9, X9\n";  // X9 = 0 (should be skipped by JAL)
	}


	system.Execute(Executable);

	return 0;
}
