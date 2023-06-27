//
// Created by iakno on 2023/6/25.
//

#ifndef RISC_V_SIMULATOR_PARSER_H
#define RISC_V_SIMULATOR_PARSER_H

#include <bits/stdc++.h>
#include "utils.h"
using std::cin;
using std::cout;
using std::endl;

extern unsigned int reg[32], mem[300003], pc;

class Parser {
public:
    void Init() {
        std::stringstream ss;
        std::string tmp;
        int addr = 0;
        while (getline(cin, tmp)) {
            // cout << tmp << endl;
            ss.clear();
            ss << tmp;
            if (tmp[0] == '@') {
                char not_used; ss >> not_used;
                ss >> std::hex >> addr;
            }
            else if (isdigit(tmp[0]) || (tmp[0] >= 'A' && tmp[0] <= 'F')) {
                unsigned int x[4];
                while (ss >> std::hex >> x[0]) {
                    mem[addr++] = x[0];
                    // cout << mem[addr - 1] << endl;
                    for (int i = 1; i <= 3; ++i) {
                        ss >> std::hex >> x[i];
                        mem[addr++] = x[i];
                        // cout << mem[addr - 1] << endl;
                    }
                }
            }
        }
    }
    unsigned int GetCommand() {
        unsigned int ret = readMemory(pc);
        pc += 4;
        return ret;
    }
};

#endif //RISC_V_SIMULATOR_PARSER_H
