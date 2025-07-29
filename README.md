# D16 CPU Simulator 🚀

**D16** is an educational 8-bit CPU simulator written from scratch in C.  
It interprets and executes its own custom Assembly language and features a web-based GUI to visualize the CPU's internal state in real-time.

This project is a great starting point for learning how low-level programming and basic computer architecture work.

---

## ✨ Features

- **8-bit Architecture:** Simple and comprehensible.
- **4 General-Purpose Registers:** Named A, B, C, and D.
- **256-byte RAM:** Memory for programs and data.
- **Conditional Logic:** Use `CMP` and `JNZ` for branching.
- **Interactive I/O:** `IN` and `OUT` instructions for input/output.
- **Live Web GUI:** Modern, HTML-based monitor showing registers, flags, and RAM.
- **Auto Web Server:** Launches a web server automatically to serve the GUI.
- **Modular Design:** Decoupled CPU core, GUI, and program files.

---

## 🧠 Architecture

D16 follows the Von Neumann architecture—code and data share the same memory (RAM).

**Registers:**
- `A`, `B`, `C`, `D`: General-purpose 8-bit
- `PC`: Program Counter (instruction pointer)

**Flag:**
- `ZF`: Zero Flag (set by `CMP`, used by `JNZ`)

**Memory:**
- 256 bytes of addressable RAM

**Instruction Format (3 bytes):**
```
Byte 0: Opcode
Byte 1: Arg1 (register/address)
Byte 2: Arg2 (register/value)
```

---

## 🧾 Instruction Set

| Mnemonic | Opcode | Arg 1     | Arg 2     | Description                              |
|----------|--------|-----------|-----------|------------------------------------------|
| HALT     | 0      | -         | -         | Stops execution                          |
| LOAD     | 1      | Register  | Value     | Loads a value into a register            |
| STORE    | 2      | Register  | Address   | Stores register value into RAM           |
| ADD      | 3      | Reg       | Reg       | Adds two registers, result in Arg1       |
| JMP      | 4      | Address   | -         | Jumps to a RAM address                   |
| CMP      | 5      | Reg       | Reg       | Compares two registers (sets ZF)         |
| JNZ      | 6      | Address   | -         | Jumps if ZF == 0                         |
| IN       | 7      | Register  | -         | Reads user input into a register         |
| OUT      | 8      | Register  | -         | Prints register value                    |
| SUB      | 9      | Reg       | Reg       | Subtracts Arg2 from Arg1                 |

---

## 🖥️ Usage

### 1. Prerequisites
- `gcc` (C compiler)
- `python3` (for GUI web server)

### 2. Clone the Repository
```bash
git clone https://github.com/Handsomeskull/D16-CPU.git
cd D16-CPU
```
- Compile
```
gcc d16.c -o D16
```
- Run
```
./D16 sample_programs/fibonacci.txt
```
When started, it will print a URL like:
```
http://localhost:8000/d16_monitor.html
```
Open it in your browser to view the simulation.


### ✍️ Writing D16 Assembly

Assembly programs are written in plain .txt files.

    Syntax: MNEMONIC ARG1, ARG2

    Comments: Begin with ;

  Examples:
  ```
; Load value 50 into A
LOAD A, 50

; Add A and B, result in A
ADD A, B

; Jump to address 12 if not equal
JNZ 12
; Load value 50 into A
LOAD A, 50

; Add A and B, result in A
ADD A, B

; Jump to address 12 if not equal
JNZ 12
```

### 📂 Example: fibonacci.txt

This example calculates the Fibonacci sequence and stores the results in RAM starting at address 200:
```
LOAD A, 0       ; n-2
LOAD B, 1       ; n-1
LOAD C, 200     ; pointer in RAM
LOAD D, 10      ; number of elements

; --- Loop (Address 12) ---
STORE A, C
ADD B, A
SUB B, A
JMP 12
HALT
```
Watch the values update live in the GUI!





 ### To Do.

1-Stack support (PUSH, POP)

2-Function calls (CALL, RET)

3-A real assembler (d16-asm)

4-GUI controls: Pause / Resume / Step

5-Dockerfile for easy setup


                           



### 🤝 Contributing

Contributions are welcome!

# Fork & clone,
```
git checkout -b feature/new-instruction
```
# Make changes and commit
```
git commit -m "Add: PUSH instruction"
```
# Push and open a PR


![MIT License](https://img.shields.io/badge/License-MIT-green.svg)
![Made with C](https://img.shields.io/badge/Made%20with-C-blue)


### 📜 License

Licensed under the MIT License. See the LICENSE file for details.
