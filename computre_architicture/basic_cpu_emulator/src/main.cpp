#include "cpu.hpp"
#include <iostream>

int main() {
  CPU cpu;

  cpu.reset();

  cpu.running = true;

  // LOAD R0, 42
  cpu.memory[0] = 0x01;
  cpu.memory[1] = 0x00;
  cpu.memory[2] = 0x2A;

  cpu.memory[3] = 0x01;
  cpu.memory[4] = 0x01;
  cpu.memory[5] = 0x14;

  // Halt
  cpu.memory[6] = 0xFF;

  while (cpu.running) {
    cpu.step();
  }

  std::cout << "R0 = " << static_cast<int>(cpu.r0) << '\n';

  std::cout << "R1 = " << static_cast<int>(cpu.r1) << '\n';
  return 0;
}
