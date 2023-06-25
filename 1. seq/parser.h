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

extern int reg[32], mem[300003], pc;

class Parser {
public:
    void Init() {
        std::stringstream ss;
        std::string tmp;
        int addr = 0;
        while (getline(cin, tmp)) {
            ss.clear();
            ss << tmp;
            if (tmp[0] == '@') {
                char not_used; ss >> not_used;
                ss >> std::hex >> addr;
            }
            else if (isdigit(tmp[0]) || (tmp[0] >= 'A' && tmp[0] <= 'F')) {
                int x[4];
                while (ss >> std::hex >> x[0]) {
                    ss >> std::hex >> x[1];
                    ss >> std::hex >> x[2];
                    ss >> std::hex >> x[3];
                    mem[addr++] = (x[3] << 24) | (x[2] << 16) | (x[1] << 8) | x[0];
                }
            }
        }
    }
    void Interpret() {

    }
};

#endif //RISC_V_SIMULATOR_PARSER_H
