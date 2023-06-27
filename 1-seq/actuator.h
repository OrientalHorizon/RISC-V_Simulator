//
// Created by iakno on 2023/6/25.
//

#ifndef RISC_V_SIMULATOR_ACTUATOR_H
#define RISC_V_SIMULATOR_ACTUATOR_H

#include <bits/stdc++.h>
#include "utils.h"
extern unsigned int reg[32], mem[300003], pc;
class Actuator {
private:

public:
    void Doit(const unsigned int cmd) {
        unsigned int opcode = cmd & 0x7Fu;
        switch (opcode) {
            case 0b0110111: {
                // LUI
                unsigned int rd = (cmd >> 7u) & 0x1Fu;
                unsigned int imm = (cmd >> 12u) << 12u;
                reg[rd] = imm;
            }
            break;

            case 0b0010111: {
                // AUIPC
                unsigned int rd = (cmd >> 7u) & 0x1Fu;
                unsigned int imm = (cmd >> 12u) << 12u;
                reg[rd] = imm + (unsigned)pc;
            }
            break;

            case 0b1101111: {
                // JAL
                unsigned int rd = (cmd >> 7u) & 0x1Fu;
                std::bitset<32> tmp_bs(cmd), imm(0);
                for (int i = 1; i <= 10; ++i) {
                    imm[i] = tmp_bs[i + 20];
                }
                imm[11] = tmp_bs[20];
                for (int i = 12; i <= 19; ++i) {
                    imm[i] = tmp_bs[i];
                }
                imm[20] = tmp_bs[31];
                reg[rd] = pc;
                unsigned tmp = extend(imm.to_ulong(), 20);
                pc += tmp - 4;
            }
            break;

            case 0b1100111: {
                // JALR
                // cout << cmd << endl;
                unsigned int rd = (cmd >> 7u) & 0x1Fu;
                unsigned int rs1 = (cmd >> 15u) & 0x1Fu;
                unsigned int imm = (cmd >> 20u) & 0xFFFu;
                imm = extend(imm, 11);
                unsigned int t = pc;
                pc = reg[rs1] + imm;
                pc >>= 1; pc <<= 1;
                // pc -= 4;
                if (!rd) rd = reg[1];
                reg[rd] = t;
            }
            break;

            case 0b1100011: {
                // BEQ, BNE, BLT, BGE, BLTU, BGEU
                unsigned int rs1 = (cmd >> 15u) & 0x1Fu;
                unsigned int rs2 = (cmd >> 20u) & 0x1Fu;
                std::bitset<32> tmp_bs(cmd), imm(0);
                for (int i = 1; i <= 4; ++i) {
                    int j = i + 7;
                    imm[i] = tmp_bs[j];
                }
                imm[11] = tmp_bs[7];
                for (int i = 5; i <= 10; ++i) {
                    int j = i + 20;
                    imm[i] = tmp_bs[j];
                }
                imm[12] = tmp_bs[31];
                unsigned tmp = extend(imm.to_ulong(), 12);
                unsigned func = (cmd >> 12u) & 0x7u;
                switch (func) {
                    case 0x0u: {
                        // BEQ
                        if (reg[rs1] == reg[rs2]) pc += tmp - 4;
                        break;
                    }
                    case 0x1u: {
                        // BNE
                        if (reg[rs1] != reg[rs2]) pc += tmp - 4;
                        break;
                    }
                    case 0x4u: {
                        // BLT
                        if ((signed)reg[rs1] < (signed)reg[rs2]) pc += tmp - 4;
                        break;
                    }
                    case 0x5u: {
                        // BGE
                        if ((signed)reg[rs1] >= (signed)reg[rs2]) pc += tmp - 4;
                        break;
                    }
                    case 0x6u: {
                        // BLTU
                        if (reg[rs1] < reg[rs2]) pc += tmp - 4;
                        // cout << std::dec << reg[rs1] << " " << reg[rs2] << endl;
                        break;
                    }
                    case 0x7u: {
                        // BGEU
                        if (reg[rs1] >= reg[rs2]) pc += tmp - 4;
                        break;
                    }
                }
            }
            break;

            case 0b0000011: {
                // LB, LH, LW, LBU, LHU
                unsigned int rd = (cmd >> 7u) & 0x1Fu;
                unsigned int rs1 = (cmd >> 15u) & 0x1Fu;
                unsigned int imm = (cmd >> 20u) & 0xFFFu;
                imm = extend(imm, 11);
                unsigned int func = (cmd >> 12u) & (0x7u);
                switch (func) {
                    case 0x0u: {
                        // LB
                        reg[rd] = extend(mem[reg[rs1] + imm], 7);
                        break;
                    }
                    case 0x1u: {
                        // LH
                        // 读两个字节
                        unsigned addr = reg[rs1] + imm;
                        unsigned ret = mem[addr] + (mem[addr + 1] << 8u);
                        reg[rd] = extend(ret,15);
                        break;
                    }
                    case 0x2u: {
                        // LW
                        reg[rd] = readMemory(reg[rs1] + imm);
                        break;
                    }
                    case 0x4u: {
                        // LBU
                        reg[rd] = mem[reg[rs1] + imm] & 0xFFu;
                        break;
                    }
                    case 0x5u: {
                        // LHU
                        unsigned addr = reg[rs1] + imm;
                        unsigned ret = mem[addr] + (mem[addr + 1] << 8u);
                        reg[rd] = ret & 0xFFFFu;
                        break;
                    }
                }
            }
            break;

            case 0b0100011: {
                // SB, SH, SW
                unsigned int rs1 = (cmd >> 15u) & 0x1Fu;
                unsigned int rs2 = (cmd >> 20u) & 0x1Fu;
                std::bitset<32> tmp_bs(cmd), imm(0);
                for (int i = 0; i <= 4; ++i) {
                    int j = i + 7;
                    imm[i] = tmp_bs[j];
                }
                for (int i = 5; i <= 11; ++i) {
                    int j = i + 20;
                    imm[i] = tmp_bs[j];
                }
                unsigned tmp = extend(imm.to_ulong(), 11);
                unsigned addr = reg[rs1] + tmp;
                unsigned func = (cmd >> 12u) & 0x7u;
                switch (func) {
                    case 0x0u: {
                        // SB
                        mem[addr] = reg[rs2] & 0xFFu;
                        break;
                    }
                    case 0x1u: {
                        // SH
                        mem[addr] = reg[rs2] & 0xFFu;
                        mem[addr + 1] = (reg[rs2] >> 8u) & 0xFFu;
                        break;
                    }
                    case 0x2u: {
                        // SW
                        mem[addr] = reg[rs2] & 0xFFu;
                        mem[addr + 1] = (reg[rs2] >> 8u) & 0xFFu;
                        mem[addr + 2] = (reg[rs2] >> 16u) & 0xFFu;
                        mem[addr + 3] = (reg[rs2] >> 24u) & 0xFFu;
                        break;
                    }
                }
            }
            break;

            case 0b0010011: {
                // ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI
                unsigned int rd = (cmd >> 7u) & 0x1Fu;
                unsigned int rs1 = (cmd >> 15u) & 0x1Fu;
                unsigned int imm = (cmd >> 20u) & 0xFFFu;
                unsigned int func = (cmd >> 12u) & (0x7u);
                switch (func) {
                    case 0x0u: {
                        // ADDI
                        reg[rd] = reg[rs1] + extend(imm, 11);
                        break;
                    }
                    case 0x2u: {
                        // SLTI
                        unsigned tmp = extend(imm, 11);
                        reg[rd] = ((signed)reg[rs1] < (signed)tmp) ? 1u : 0u;
                        break;
                    }
                    case 0x3u: {
                        // SLTIU
                        unsigned tmp = extend(imm, 11);
                        reg[rd] = (reg[rs1] < tmp) ? 1u : 0u;
                        break;
                    }
                    case 0x4u: {
                        // XORI
                        reg[rd] = reg[rs1] ^ extend(imm, 11);
                        break;
                    }
                    case 0x6u: {
                        // ORI
                        reg[rd] = reg[rs1] | extend(imm, 11);
                        break;
                    }
                    case 0x7u: {
                        // ANDI
                        reg[rd] = reg[rs1] & extend(imm, 11);
                        break;
                    }
                    case 0x1u: {
                        // SLLI
                        unsigned shamt = imm & 0b111111u;
                        // cout << rd << " " << rs1 << " " << shamt << endl;
                        reg[rd] = reg[rs1] << shamt;
                        break;
                    }
                    case 0x5u: {
                        // SRLI, SRAI
                        unsigned shamt = imm & 0b111111u;
                        unsigned func2 = cmd >> 26u;
                        if (func2 == 0) {
                            // SRLI
                            reg[rd] = reg[rs1] >> shamt;
                        } else {
                            // SRAI
                            reg[rd] = (signed)reg[rs1] >> (signed)shamt;
                        }
                        break;
                    }
                }
            }
            break;

            case 0b0110011: {
                // ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
                unsigned int rd = (cmd >> 7u) & 0x1Fu;
                unsigned int rs1 = (cmd >> 15u) & 0x1Fu;
                unsigned int rs2 = (cmd >> 20u) & 0x1Fu;
                unsigned int func = (cmd >> 12u) & (0x7u);
                unsigned int func2 = cmd >> 25u;
                switch (func) {
                    case 0x0u: {
                        if (func2 == 0x0u) {
                            // ADD
                            reg[rd] = reg[rs1] + reg[rs2];
                        } else {
                            // SUB
                            reg[rd] = reg[rs1] - reg[rs2];
                        }
                        break;
                    }
                    case 0x1u: {
                        // SLL
                        unsigned shamt = reg[rs2] & 0b11111u;
                        reg[rd] = reg[rs1] << shamt;
                        break;
                    }
                    case 0x2u: {
                        // SLT
                        reg[rd] = ((signed)reg[rs1] < (signed)reg[rs2]) ? 1u : 0u;
                        break;
                    }
                    case 0x3u: {
                        // SLTU
                        reg[rd] = (reg[rs1] < reg[rs2]) ? 1u : 0u;
                        break;
                    }
                    case 0x4u: {
                        // XOR
                        reg[rd] = reg[rs1] ^ reg[rs2];
                        break;
                    }
                    case 0x5u: {
                        if (func2 == 0x0u) {
                            // SRL
                            unsigned shamt = reg[rs2] & 0b11111u;
                            reg[rd] = reg[rs1] >> shamt;
                        } else {
                            // SRA
                            unsigned shamt = reg[rs2] & 0b11111u;
                            reg[rd] = (signed)reg[rs1] >> (signed)shamt;
                        }
                        break;
                    }
                    case 0x6u: {
                        // OR
                        reg[rd] = reg[rs1] | reg[rs2];
                        break;
                    }
                    case 0x7u: {
                        // AND
                        reg[rd] = reg[rs1] & reg[rs2];
                        break;
                    }
                }
            }
            break;
        }
    }
};

#endif //RISC_V_SIMULATOR_ACTUATOR_H
