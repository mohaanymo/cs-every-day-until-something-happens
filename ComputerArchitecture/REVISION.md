# Computer Architecture — Final Revision (Lectures 1–3)

> Read top to bottom once. Then read **§12 Traps** and **§13 Quick numbers** again right before the exam.

---

## 0. The 3 ideas that answer most questions

1. **ISA = contract.** It holds *exactly* what a programmer needs to write a **correct** program. Everything else is **microarchitecture**.
   **Test:** *"If this changed, would a correct program break?"* Yes → ISA. Only faster or slower → microarchitecture.
2. **Who picks the next instruction?** An instruction pointer (Von Neumann) or data readiness (Dataflow).
3. **Every ISA choice is a tradeoff.** Powerful instructions give short code and an easy compiler, but complex hardware. Simple instructions give simple, fast hardware, but longer code and more compiler work.

---

# LECTURE 1 — Introduction and Basics

## 1. Levels of transformation and abstraction

The stack, top to bottom (memorize the order):

```
Problem
Algorithm
Program / Language
Runtime System (VM, OS, MM)
ISA (Architecture)
Microarchitecture
Logic
Circuits
Electrons
```

- **"The purpose of computing is insight"** (Richard Hamming).
- **Abstraction:** a higher level only needs to know the **interface** to the lower level, not how the lower level is implemented.
  - Example: a Java programmer doesn't need to know the ISA.
- **Abstraction improves productivity:** you don't worry about the decisions made in the levels below.
- **Why cross the layers?** When things go wrong or you want better:
  - the program is **slow**, **incorrect**, or uses **too much energy**
  - the hardware is **hard to program**, or **too slow** because it lacks the right primitives
  - you want to design a **more efficient** system
- **User-centric view:** the entire stack should be optimized for the user.
- **Takeaway of Lecture 1:** breaking the abstraction layers and knowing what is underneath lets you solve problems. The DRAM unfairness and refresh examples both show this.

**Computer architecture (definition):** the science and art of designing, selecting and interconnecting hardware components, and designing the hardware/software interface, to build a computing system that meets functional, performance, energy, cost and other goals.

---

## 2. DRAM: how main memory works

### RAM vs. DRAM vs. SRAM
- **RAM** = Random Access Memory, the working memory. Any address can be read directly.
- **DRAM** (Dynamic RAM) = **main memory** (the "8 GB RAM").
  - **1 cell = 1 capacitor + 1 access transistor.** Small and cheap.
- **SRAM** (Static RAM) = used for **caches**. Bigger and more expensive, but faster.
- A **DRAM cell stores 1 bit as charge in its capacitor**: charged = 1, empty = 0.
- The capacitor **leaks**, so the charge must be rewritten periodically. That's why it's called **dynamic**, and it's the reason for **refresh** (§4).

### Why a grid?
Billions of cells can't each have their own wire. So the cells are arranged in a **2D grid of rows × columns** and you select one by **row number + column number**.
- A DRAM chip has tens of thousands of rows.

### Bank structure (all inside the DRAM chip)
```
 row address ─► ROW DECODER ─► switches on one row
                 ┌───────────────────┐
                 │   grid of cells   │   (rows × columns)
                 └─────────┬─────────┘
                 ┌─────────▼─────────┐
                 │    ROW BUFFER     │   (sense amplifiers: hold ONE row)
                 └─────────┬─────────┘
 col address ──► COLUMN MUX ─► picks the requested piece ─► out to CPU
```
- **Row decoder:** selects (opens) one row.
- **Row buffer:** made of **sense amplifiers**. A cell's charge is too weak to send out directly, so the sense amps detect it, amplify it and **hold the whole row**.
- **Column mux:** picks the requested column (piece) out of the row buffer.
- **Bank** = one grid + its own row buffer. A chip has several banks (e.g. Bank 0–3), each with **one row open at a time**.

### Row hit / empty / conflict
| Case | What happens | Speed |
|---|---|---|
| **Row hit** | the needed row is **already** in the buffer, so just read the column | **Fast** |
| **Empty** | no row is open, so open the row, then read | Medium |
| **Row conflict** | a **different** row is open: close it (write back), open the new one, read | **Slow** |

**Only the ROW decides hit vs. conflict. The column number doesn't matter.**

### 1D addresses vs. the 2D grid
- To the program, memory is a **1D array** (address 0, 1, 2, …). That's the **interface** (abstraction).
- Inside the chip it's a **2D grid**. The address is split into row and column.
- Example with 4 columns: $\text{row} = \lfloor \text{addr}/4 \rfloor$ and $\text{col} = \text{addr} \bmod 4$. Address 13 → row 3, col 1. Row 1 holds addresses 4, 5, 6, 7.
- Consequence: reading addresses **in order** stays in the same row and gives many fast hits. **Random** addresses give conflicts.

---

## 3. Memory scheduling and unfairness

### The setup
- **Core** = one full processor (own PC, registers, ALU) that runs one program.
- **Multi-core chip** = several cores running programs at the same time.
- Each core has a private cache (L2), but **all cores share one DRAM**.
- **Memory controller** = the circuit that queues requests from all cores and decides **which request DRAM serves next** (a traffic officer).

### FR-FCFS (First-Ready, First-Come-First-Served)
Priority order:
1. **Row-hit first**: requests to the currently open row go first.
2. **Oldest first**: then the oldest request.

- **Goal: maximize DRAM throughput** (most requests served per second), because hits are fast.
- It is **NOT designed for fairness**.

### Thread unfairness: STREAM vs. RANDOM
| | **STREAM** | **RANDOM** |
|---|---|---|
| Access pattern | sequential (in order) | random |
| Row-buffer hit rate | **very high (96%)** | **very low (3%)** |
| Memory intensive? | yes | similarly yes |
| Under FR-FCFS | keeps getting served | starves and slows down a lot |

- STREAM keeps producing **row hits**, and hits always jump ahead, so RANDOM's (older) conflict requests keep waiting.
- STREAM is the **memory performance hog**. Total throughput is high, but it's split unfairly.
- DRAM scheduling policies are **thread-unfair**, and the memory controller is vulnerable to **denial-of-memory-service attacks**: someone can **write a program on purpose** that exploits this unfairness.
- In the slides: matlab (hog) vs. gcc (victim). High priority vs. low priority doesn't help, because the controller doesn't know about priorities.
- **Oldest-first** alone also unfairly favors memory-intensive threads.

### The 128 calculation ⭐
- **Row size = 8 KB** = $8 \times 1024 = 8192$ bytes. The whole row sits in the row buffer.
- **Cache block = 64 B**: the piece **one request** brings to the CPU (it gets stored in the cache).
- Requests per row:
$$
\frac{\text{row size}}{\text{block size}} = \frac{8192}{64} = 128
$$
- So **128 requests of T0 (STREAM) are serviced before T1 (RANDOM)**.
- General method: convert KB to bytes (×1024), then divide by the block size. E.g. 4 KB / 32 B = 4096 / 32 = 128.

**Where should the problem be solved?** Programmer? System software? Compiler? Memory controller? DRAM? Circuits? The lecture's point is that you need to think **across layers** to answer this.

---

## 4. DRAM refresh and RAIDR

### Why refresh
- Capacitor **charge leaks over time**, so data would be lost.
- The **memory controller refreshes each row periodically**: it reads the row and writes it back at full strength.
- **Activate each row every N ms, typical N = 64 ms.** ⭐ (milliseconds, NOT µs)

### Downsides of refresh
- **Energy:** each refresh consumes energy.
- **Performance:** a bank can't serve requests while it's being refreshed.
- **QoS / predictability:** (long) pause times during refresh.
- **Refresh rate limits DRAM capacity scaling:** more rows means more refresh.
- Overhead grows with density (slides): **performance loss 8% → 46%** and **energy 15% → 47%** as DRAM capacity grows.

### RAIDR = Retention-Aware Intelligent DRAM Refresh (Liu et al., ISCA 2012)
- **Observation:** most DRAM rows can be refreshed **much less often** without losing data. Only a few rows have **weak cells**.
- **Key idea:** refresh **weak rows more frequently** and the **other rows less frequently**.
- 3 steps:
  1. **Profiling:** measure the retention time of all rows.
  2. **Binning:** store rows in bins by retention time, in the memory controller (using **Bloom filters**, only **1.25 KB for 32 GB**).
  3. **Refreshing:** refresh each bin at a different rate.
- **Results** (8-core, 32 GB): **74.6% refresh reduction**, ~16%/20% DRAM dynamic/idle power reduction, **~9% performance improvement**. The benefits **increase with DRAM capacity**.
- The general lesson: **expose information from underneath** (the retention profile) to upper layers (memory controller, OS, …).

---

# LECTURE 2 — Fundamental Concepts and ISA

## 5. Basics
- **A computer = 3 key components: Computation, Communication, Storage (memory).**
- **Moore's Law** (Moore, 1965): the number of transistors on a chip **doubles about every 2 years**.
- **Today's challenges:** power/energy, design complexity, technology scaling, the **memory wall**, the **reliability wall**, the **programmability wall**. Industry is shifting to **multi-core and beyond**.
- **Architecture = ISA + microarchitecture.**

## 6. ISA vs. microarchitecture ⭐⭐

### ISA
- The **agreed-upon interface between software and hardware**. **SW/compiler assumes, HW promises.**
- What the programmer needs to know to write and debug programs.
- **Analogy:** the gas pedal is the interface (ISA). The engine internals are the implementation (microarchitecture).

### Microarchitecture (uarch)
- A **specific implementation** of an ISA.
- **Not visible to software.**
- **Changes much faster** than the ISA. ISAs (x86, ARM) last decades; microarchitectures change every few years.

### Classification table (practice exam Q10 + more)
| Item | Belongs to | Why |
|---|---|---|
| Opcodes / opcode encoding of ADD | **ISA** | program bits depend on it |
| Number of general-purpose registers | **ISA** | programs name R0…R31 |
| Addressing modes | **ISA** | programs use them |
| Data types | **ISA** | |
| Instruction types and formats | **ISA** | |
| Condition codes | **ISA** | |
| Memory organization (address space, addressability, **endianness**) | **ISA** | changes what a program reads |
| Program counter, registers, memory (architectural state) | **ISA** | visible |
| Number of register-file **read/write ports** | **uarch** | only affects speed |
| **Latency** (cycles) of MUL | **uarch** | same result, only speed changes |
| **Pipelining**, number of pipeline stages | **uarch** | |
| **Cache** (existence, size, contents) | **uarch** | invisible |
| Out-of-order execution, branch prediction | **uarch** | |

⚠️ **"Sounds low-level" ≠ microarchitecture.** Opcode bits and endianness are ISA.

### Out-of-order execution ⭐
- The microarchitecture **can execute instructions out of order** internally, **as long as the architectural (programmer-visible) state matches the sequential semantics defined by the ISA**.
- The program must see the same results as if instructions ran one by one, in order.

## 7. Von Neumann vs. dataflow ⭐⭐

### Von Neumann model (stored-program computer)
Two key properties:
1. **Stored program**
   - Instructions are stored in a **linear memory array**.
   - **Memory is unified** between instructions and data.
   - **The interpretation of a stored value depends on the control signals.** The bits have **no tag**. A value is an **instruction** when it's **fetched via the PC** into the instruction register, and **data** when it's read during execution. The same bits can be both at different times. There's no fixed "code region" in the model.
2. **Sequential instruction processing**
   - One instruction is processed (fetched, executed, completed) at a time.
   - The **program counter (instruction pointer)** identifies the current instruction.
   - The PC advances **sequentially**, except for **control transfer instructions** (branches).
- Parts: Memory (MAR, MDR), Processing unit (ALU, TEMP), Control unit (IP, Instruction Register), Input, Output.

### Dataflow model
- An instruction is fetched and executed in **data flow order**: **when its operands are ready**.
- **There is no instruction pointer.**
- Order is set by **data dependences**.
- Each instruction specifies **"who" should receive its result**.
- A node **"fires" when all its inputs have tokens**.
- Many instructions can execute **at the same time**, so it's **inherently more parallel**.

### Example
```
v = a + b;    w = b * 2;    x = v - w;    y = v + w;    z = x * y;
```
- Von Neumann: runs line by line.
- Dataflow: `v` and `w` fire **together** (they need only a, b). Then `x` and `y` fire together. Then `z`.

### Comparison (short-answer Q11) ⭐
> **Von Neumann:** execution order is determined by the **instruction pointer** (control flow). It's sequential, one instruction at a time, and the PC advances in order except at branches.
> **Dataflow:** there is **no instruction pointer**. An instruction executes **when all its input operands are available** (data flow), so many instructions can run in parallel.

### ISA-level tradeoff: do we need an IP?
- **Yes:** control-driven, sequential execution.
- **No:** data-driven, parallel execution.
- Tradeoffs: ease of programming, ease of compilation, extraction of parallelism, hardware complexity.

⚠️ **All major ISAs today are Von Neumann**: x86, ARM, MIPS, SPARC, Alpha, POWER. Dataflow ideas are used **inside the microarchitecture** (out-of-order execution).

---

# LECTURE 3 — ISA Tradeoffs

## 8. Design point and tradeoffs
- **Design point** = the set of design considerations and their importance: **cost, performance, max power, energy (battery life), availability, reliability and correctness, time to market**.
- The design point is determined by the **"Problem" space** (applications, users, market).
- **Computer architecture is the science and art of making the appropriate tradeoffs to meet a design point.**
- **Why "art"?** We don't (fully) know the future (applications, users, market), **and the future changes**.
- Levels of tradeoffs: ISA-level, microarchitecture-level, system/task-level (dividing the work between HW and SW).
- Many ISAs over the decades: x86, PDP-11, VAX, IBM 360, CDC 6600, SIMD (CRAY-1), VLIW (IA-64), PowerPC, RISC (Alpha, MIPS, SPARC, ARM).

## 9. What is an instruction?
- The **basic element of the HW/SW interface**.
- **Opcode:** *what* the instruction does. **Operands:** *who* it is to do it to.
- **Bit steering:** one bit in the instruction determines how other bits are interpreted.

## 10. Elements of an ISA (each one is a tradeoff)

### (1) Instruction processing style: 0/1/2/3-address machines ⭐
**Count the named operands.** Task: C = A + B.

| Type | Meaning | Code | # instr |
|---|---|---|---|
| **3-address** | sources and destination separate | `ADD C, A, B` | 1 |
| **2-address** | one operand is **both source and destination** | `MOV C, A` / `ADD C, B` | 2 |
| **1-address** | **accumulator** is implicit | `LD A` / `ADD B` / `ST C` | 3 |
| **0-address** | **stack** machine | `PUSH A` / `PUSH B` / `ADD` / `POP C` | 4 |

- 1-address `ADD B` means **ACC ← ACC + B**.
- Tradeoff: **fewer operands = smaller instructions but more instructions executed**. Code size vs. execution time vs. on-chip memory.
- **Stack machine:**
  - ✅ Small instructions (no operands), simpler logic, compact code.
  - ✅ Efficient procedure calls: parameters are already on the stack.
  - ❌ Computations that don't fit **postfix** notation are hard to map.
  - ❌ Only the **top N values** are accessible, so it's not flexible.
- **PDP-11 = 2-address** (4-bit opcode, two 6-bit operand specifiers, because of limited instruction bits). Downside: **one source operand is always clobbered** by the result. To keep it, copy it first.
- **x86 = 2-address** (memory/memory). **Alpha = 3-address** (load/store).

### (2) Data types
- **Definition:** a representation of information **for which there are instructions that operate on it**.
- Examples: integer, floating point, character, binary, decimal, BCD. High-level: doubly linked list, queue, string, bit vector, stack.
  - **VAX:** INSQUEUE/REMQUEUE (doubly linked list), FINDFIRST.
  - **x86:** SCAN (strings), PUSH/POP.
- **Tradeoff:** more high-level data types are **easier for the compiler/programmer** (smaller **semantic gap**) but **harder for the microarchitect**.
- Extremes: **early RISC = only the integer type**. **Intel 432 = object data type** (capability-based machine).

### (3) Memory organization ⭐
- **Address space:** **how many** uniquely identifiable locations ($n$ address bits → $2^n$ locations).
- **Addressability:** **how much data** each location stores.
  - **Byte-addressable: most ISAs** (characters are 8 bits).
  - Bit-addressable: Burroughs 1700. 64-bit addressable: some supercomputers. 32-bit addressable: the first Alpha.
- **Endianness:** which byte of a multi-byte value goes at the **lowest address**.
  - The **big end = most significant byte = the LEFTMOST byte as written** (it's about position, not about which byte has the larger value).
  - **Big endian:** MSB at the low address, which is natural reading order.
  - **Little endian:** LSB at the low address, i.e. reversed. **x86 is little endian.**

  `0x11223344` at addresses 100–103:
  ```
  address:        100  101  102  103
  big endian:     11   22   33   44
  little endian:  44   33   22   11
  ```
  (Hex: 1 hex digit = 4 bits, 2 hex digits = 1 byte.)
- Support for **virtual memory** is also part of memory organization.

### (4) Registers
- **Why registers?** Programs have **data locality**. **Temporal locality:** a recently produced/accessed value is likely to be used again. Keeping it in a register avoids going to memory every time.
- Registers have **names** (R1, R2), **not addresses**. They're inside the CPU.
- **Evolution:** accumulator (legacy from adding machines) → accumulator + address registers (special-purpose, used for indirection) → **general-purpose registers** (all registers good for all purposes; from a few, to 32 in RISC, to 128 in IA-64).

### Programmer-visible vs. invisible state ⭐
| **Architectural (visible)** | **Microarchitectural (invisible)** |
|---|---|
| Registers (general and special purpose) | **Cache state** |
| Memory (array indexed by address) | **Pipeline registers** |
| **Program counter** (address of current instruction) | |

Instructions specify how to transform the values of the programmer-visible state.

### Instruction classes
| Class | Does | Control flow |
|---|---|---|
| **Operate** | arithmetic/logic: fetch operands, compute, store result | implicit sequential |
| **Data movement** | move data between memory, registers, I/O | implicit sequential |
| **Control flow** | **change the sequence** of executed instructions | branches/jumps |

### Branches (control flow)
- Normally **next PC = PC + 1** (or PC + 4 when counting in bytes with 4-byte instructions).
- A **branch** sets the PC somewhere else. It's how `if`, `else`, loops and calls work.
- **Offset** = how far to jump. **Target = PC + offset.** A negative offset jumps backward (loops).
  - E.g. PC = 50, offset +8, taken → 58.
- **Not taken** (condition false) → PC + 1.
- Only branches use those bits as a jump offset. In other instructions the same bits mean something else (a constant, a displacement).

### (5) Load/store vs. memory architecture ⭐
- **Load/store:** operate instructions work **only on registers**. Only LD/ST touch memory. Examples: **MIPS, ARM, most RISC**.
- **Memory architecture:** operate instructions **can operate on memory** directly. Examples: **x86, VAX, most CISC**.
  ```
  Memory arch:   ADD R1, [500]
  Load/store:    LD R2, [500]
                 ADD R1, R1, R2
  ```

### (5b) Addressing modes: how to compute the operand's address
| Mode | x86 example | Address used |
|---|---|---|
| Absolute / displacement | `mov ax, [1000]` | the constant 1000 |
| **Register indirect** | `mov ax, [esi]` | **the value inside** esi (like a pointer) |
| Based / displaced | `mov ax, [bx][si]` | base reg + index reg |
| Indexed | `mov ax, var1[esi]` | var1 + esi (arrays) |

- Register indirect example: R4 = 800 and M[800] = 15 → `LD R1, [R4]` puts **15** in R1.
- **Advantage of more modes:** better mapping of high-level constructs (arrays with autoincrement, pointer chasing, sparse matrices), so **fewer instructions and smaller code**.
- **Disadvantage:** **more work for the compiler and for the microarchitect.**

### (6) Instruction length ⭐
| | **Fixed length** | **Variable length** |
|---|---|---|
| ✅ | **Easier to decode** one instruction; **easier to decode many in parallel** (starts are at 0, 4, 8, …) | **Compact encoding** (smaller code, less memory/bandwidth) |
| ❌ | **Wasted bits**; **harder to extend** the ISA (no room for new instructions) | **More decode logic**; **harder to decode many in parallel** (instruction 2's start is unknown until instruction 1 is decoded) |

- Intel 432: Huffman-like encoding, **6 to 321 bits**.
- Tradeoffs: code size vs. hardware complexity; extensibility vs. hardware complexity.

### (7) Uniform vs. non-uniform decode ⭐
- **Uniform decode:** the **same bits mean the same thing in every instruction** (the opcode is always in the same place, same for operands and immediates). Examples: Alpha, MIPS, SPARC.
  - ✅ Easier decode, simpler hardware.
  - ✅ **Enables parallelism:** the hardware can **compute the branch target (PC + offset bits) before knowing the instruction is a branch**, in parallel with opcode decoding. If it's not a branch, the result is thrown away.
  - ❌ Restricts the format (fewer instructions?) or wastes space.
- **Non-uniform:** e.g. the **x86 opcode can be in byte 1 to 7**.
  - ✅ More compact and powerful format. ❌ More complex decode logic.
- **Uniform decode usually goes with fixed length.**

### (8) Aligned vs. unaligned access ⭐
- The CPU reads in **words** (4 bytes on a 32-bit CPU) starting at multiples of 4.
- **Aligned:** a 4-byte value starts at a multiple of 4, so 1 read.
- **Unaligned:** it straddles two words, so **up to 2× the memory transactions**. Reading 2 bytes can even be slower than reading 4.
- **x86 handles unaligned access** (slower). **MIPS allows only aligned access**; otherwise it raises an **address exception**.

### (9) Complex vs. simple instructions ⭐
- **Complex instruction:** does a lot of work (e.g. insert into a doubly linked list, string copy).
- **Simple instruction:** a small primitive (ADD, XOR, MUL) that complex operations are built from.

| Complex instructions: ✅ Advantages | Complex instructions: ❌ Disadvantages |
|---|---|
| **Denser encoding → smaller code** → better memory use, saves off-chip bandwidth, **better cache hit rate** | Larger chunks of work → **compiler has less opportunity to optimize** (fine-grained) |
| **Simpler compiler** (less need to optimize small instructions) | **More complex hardware** (translation to control signals and optimization done in HW) |

## 11. Semantic gap, RISC vs. CISC, translation

### Semantic gap = the distance between the high-level language and the ISA
- **Small gap:** ISA close to the HLL, **complex instructions** (CISC). Extreme: Java machines, LISP machines, object-oriented and capability-based machines.
- **Large gap:** ISA close to hardware control signals, **simple instructions** (RISC). Extreme: each instruction specifies all control signals.
- Example of a complex instruction: **VAX INDEX** (array access with bounds checking).

### CISC vs. RISC history
- **CISC:** motivated by **"not good enough" code generation** (weak compilers).
- **RISC:** **John Cocke, mid-1970s, IBM 801**. Goal: **enable better compiler control and optimization**. Also motivated by:
  - memory stalls (no work gets done inside a complex instruction while it waits on memory)
  - **simpler hardware → lower cost, higher frequency**
  - letting the compiler optimize better and find fine-grained parallelism to reduce stalls
- High-level-language machines **gave way to optimizing compilers**.

### Summary table ⭐
| **RISC** | **CISC** |
|---|---|
| Simple instructions | Complex instructions |
| **Fixed length** | **Variable length** |
| **Uniform decode** | **Non-uniform decode** |
| **Few addressing modes** | **Many addressing modes** |
| Load/store | Memory operands |
| Large semantic gap | Small semantic gap |
| MIPS, ARM, Alpha, SPARC | x86, VAX |

### ISA evolution and translation
- ISAs evolved to address the concerns of their day: limited memory, limited compiler technology, limited bandwidth, specialization (e.g. **MMX**).
- **Translation** (in HW or SW) lets different ISAs share similar implementations, which changes the semantic-gap tradeoff.
  - **Intel/AMD x86:** hardware translates x86 instructions into **programmer-invisible micro-operations (μops)**, which are simple, RISC-like instructions.
    ```
    ADD [100], R1   →  μop1: LOAD tmp ← M[100]
                       μop2: ADD  tmp ← tmp + R1
                       μop3: STORE M[100] ← tmp
    ```
    This doesn't break programs because μops are **microarchitecture**: the x86 ISA contract is unchanged. CISC on the outside, RISC on the inside.
  - **Transmeta:** translated x86 into "secret" **VLIW** instructions **in software** (**code morphing software**).
- **Dynamic/static interface** (contrast with the hardware/software interface).

---

## 12. ⚠️ Traps (re-read before the exam)
1. **MUL latency / pipeline stages / cache / register ports → uarch.** **Opcode encoding / endianness / number of registers / addressing modes → ISA.**
2. **MUL cycle count is NOT part of the ISA** (practice exam Q9 = **False**).
3. **Microarchitecture changes faster than the ISA** → **True**.
4. **Von Neumann: interpretation as instruction or data depends on control signals** → **True**. No tag bit and no fixed code region.
5. **Dataflow has NO instruction pointer.** Von Neumann has unified memory and a PC.
6. **All major ISAs today are Von Neumann**, not dataflow.
7. **Out-of-order is allowed only if visible state keeps sequential ISA semantics.**
8. **FR-FCFS maximizes THROUGHPUT, not fairness.**
9. **Refresh interval = 64 ms** (not µs).
10. **Big end = leftmost byte**, not the byte with the larger value. **x86 = little endian.**
11. **Load/store: operate instructions only on registers.** "Every instruction loads and stores" is wrong.
12. **Variable length = compact but HARDER parallel decode.** Fixed length = wasted bits and harder to extend.
13. `ADD C, A, B` can exist **only on a 3-address machine**. Count the named operands.
14. **Registers have names, not memory addresses.** `[R2]` means "use R2's value as an address."
15. **Moore's Law = doubles about every 2 years** (not 6 months).
16. **ISA = interface between SW and HW** (not circuit layout, OS paging or pipeline stages).
17. Complex instructions: **smaller code and simpler compiler**, but **less compiler optimization and more complex HW**.
18. Uniform decode's big win: **compute the branch target before knowing it's a branch.**

## 13. 🔢 Quick numbers
| Fact | Value |
|---|---|
| Moore's Law | ×2 transistors every **~2 years** |
| DRAM refresh interval | **64 ms** |
| STREAM / RANDOM row-hit rate | **96% / 3%** |
| Row size / cache block (hog example) | **8 KB / 64 B → 128 requests** |
| Refresh overhead (perf), now → future | **8% → 46%** |
| Refresh overhead (energy), now → future | **15% → 47%** |
| RAIDR refresh reduction | **74.6%** |
| RAIDR storage | **1.25 KB** (Bloom filters, 32 GB DRAM) |
| RAIDR power reduction | **~16% dynamic / ~20% idle** |
| RAIDR performance gain | **~9%** |
| PDP-11 ADD format | 4-bit opcode + two 6-bit operand specifiers |
| x86 opcode position | byte **1 to 7** |
| Intel 432 instruction length | **6 to 321 bits** |
| GPR count | ~32 (RISC), 128 (IA-64) |
| RISC origin | John Cocke, IBM 801, mid-1970s |

## 14. Practice exam answers (with the one-line reason)
1. **B**: ISA = agreed interface between SW and HW.
2. **A**: Von Neumann = unified memory for instructions and data.
3. **B**: dataflow fires when all input tokens are ready.
4. **B**: register file ports are microarchitecture.
5. **B**: Moore's Law = 2 years.
6. **B**: out-of-order is OK if visible state keeps sequential ISA semantics.
7. **True**: microarchitecture changes faster than the ISA.
8. **True**: control signals decide instruction vs. data.
9. **False**: MUL latency is microarchitecture.
10. a) ISA, b) ISA, c) uarch, d) uarch, e) uarch.
11. Von Neumann: order set by the IP, sequential. Dataflow: no IP, fires when operands are ready, parallel.

---
**Good luck! 🍀** Use the test *"would a correct program break?"* for ISA vs. uarch, and *"who/what decides the next instruction?"* for Von Neumann vs. dataflow. For any tradeoff question, the answer is "software ease vs. hardware simplicity."
