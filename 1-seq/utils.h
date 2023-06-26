//
// Created by iakno on 2023/6/26.
//

#ifndef RISC_V_SIMULATOR_UTILS_H
#define RISC_V_SIMULATOR_UTILS_H

#include <bits/stdc++.h>

extern unsigned int mem[300003];

// 实现符号位扩展
unsigned int extend(unsigned int u, unsigned int num) { // num: 最高位是几，0-based
    std::bitset<32> b(u);
    if (b[num]) {
        for (unsigned i = num + 1u; i < 32u; ++i) {
            b[i] = true;
        }
    }
    return b.to_ulong();
}
unsigned int readMemory(unsigned int addr) {
    unsigned int ret = 0u;
    unsigned cur = addr + 3;
    for (int i = 3; i >= 0; --i) {
        ret <<= 8u;
        ret |= mem[cur--];
    }
    return ret;
}

#endif //RISC_V_SIMULATOR_UTILS_H
