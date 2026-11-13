#include "cpu.hpp"
#include <iostream>

void CPU::reset() {
  r0 = 0;
  r1 = 0;
  r2 = 0;
  r3 = 0;

  pc = 0;
  ir = 0;

  zero = false;
  carry = false;
  negative = false;

  running = false;

  for (int i = 0; i<256; i++) {
    memory[i] = 0;
  }
}

void CPU::fetch() {
  ir = memory[pc];
  pc++;
}

void CPU::decode()
{
  opcode = ir; 
  operand1 = memory[pc];
  pc++;

  operand2 = memory[pc];
  pc++;

  std::cout << "opcodee = " 
    << static_cast<int>(opcode) << '\n' << "operand1 = " << static_cast<int>(operand1)
    << '\n' << "operand2 = " << static_cast<int>(operand2) << '\n';

}

void CPU::execute()
{
    switch (opcode) {

        case LOAD:
            if (operand1 == 0) {
                r0 = operand2;
            }
            else if (operand1 == 1) {
                r1 = operand2;
            }
            else if (operand1 == 2) {
                r2 = operand2;
            }
            else if (operand1 == 3) {
                r3 = operand2;
            }
            break;

        case HALT:
            running = false;
            break;

        default:
            std::cout << "Unknown instruction\n";
            running = false;
            break;
    }
}

void CPU::step() {
    fetch();
    decode();
    execute();
}
