# XSimulator

## Overview
XSimulator is a simple instruction-level simulator for executing a given instruction stream on a multi-core system. The simulator reads an instruction file, converts it into an executable format, and executes it using a predefined system configuration.

## Features
- Supports basic RISC-V-like instructions.
- Handles RAW (Read After Write) hazards with stalling or forwarding.
- Supports branching and memory operations.
- Can execute instruction streams defined in a file.
- Multi-core simulation with conditional execution based on core ID.

## Usage
Run the simulator with an instruction file as input:
```sh
./xsimulator -s instructions.txt
```

If no file is provided, a default instruction set will be used.

## Instruction Format
Instructions should be written in an assembly-like format. Example:
```assembly
ADDI X1, X0, 0    # sum = 0
ADDI X2, X0, 0    # i = 0
MUL X3, CID, X10  # Start index = CID * 25
ADDI X4, X3, 25   # End index = Start index + 25

LOOP:
BNE X3, X4, BODY  # If start != end, continue
JMP EXIT          # Else, exit

BODY:
LW X5, 0(X3)      # Load a[i] into X5
ADD X1, X1, X5    # sum += a[i]
ADDI X3, X3, 4    # i++

EXIT:
BNE CID, 1, DONE  # Only Compute Unit 1 prints the sum
SW X1, 0(X0)      # Store final sum at address 0

DONE:
```


## System Configuration (Default)
The simulator operates with a default system configuration:
- **Number of Cores**: 4
- **RAM Size**: 64 KB

