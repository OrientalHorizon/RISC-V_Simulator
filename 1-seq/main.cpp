//
// Created by iakno on 2023/6/20.
//

#include <bits/stdc++.h>
#include "parser.h"
#include "actuator.h"
using std::cin;
using std::cout;
using std::endl;
unsigned int reg[32], mem[300003], pc;
Parser parser;
Actuator actuator;

signed main() {
    // freopen("bulgarian.data", "r", stdin);
    // freopen("array_test.out", "w", stdout);
    pc = 0u;
    parser.Init();
    // cout << "fuck";
    while ("Screw you") {
        // cout << std::dec << reg[12] << endl;
        reg[0] = 0;
        // cout << std::dec << pc << endl;
        unsigned int cmd = parser.GetCommand();
        // cout << std::setfill('0') <<  std::setw(8) << std::hex << std::uppercase << cmd << endl;
        if (cmd == 0x0FF00513u) break;
//        if (cmd == 0x00161613) {
//            std::cout << "yes" << std::endl;
//        }
        actuator.Doit(cmd);
        // cout << 1 << endl;
    }
    cout << std::dec << (reg[10] & 0xFFu) << endl;
    return 0;
}
