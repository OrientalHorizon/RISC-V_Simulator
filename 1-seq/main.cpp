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
    parser.Init();
    while ("Screw you") {
        reg[0] = 0;
        unsigned int cmd = parser.GetCommand();
        if (cmd == 0x0FF00513u) break;
        actuator.Doit(cmd);
    }
    cout << (reg[10] & 0xFFu) << endl;
    return 0;
}
