#include<cstdint>


enum Opcode {
    NOP  = 0x00,
    LOAD = 0x01,
    STORE = 0x02,
    MOV  = 0x03,
    ADD  = 0x04,
    SUB  = 0x05,
    JMP  = 0x06,
    JZ   = 0x07,
    HALT = 0xFF
};

class CPU {
  public:
    // General purpose regesiters
    uint8_t r0;
    uint8_t r1;
    uint8_t r2;
    uint8_t r3;

    // Special registers
    uint16_t pc;
    uint8_t ir;

    uint8_t opcode;
    uint8_t operand1;
    uint8_t operand2;

    // Flags
    bool zero, carry, negative;

    // Memory (256 bytes)
    uint8_t memory[256];

    // CPU state
    bool running;

    void reset();
    void fetch();
    void decode();
    void execute();
    void step();
};
