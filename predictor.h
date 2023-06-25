//
// Created by iakno on 2023/6/20.
//

#ifndef RISC_V_SIMULATOR_PREDICTOR_H
#define RISC_V_SIMULATOR_PREDICTOR_H
#include <bits/stdc++.h>
class Predictor {
private:
    unsigned int pred, cnt, tot;
public:
    Predictor() : pred(0u) {}
    void Jump() {
        if (pred & 2u) ++cnt;
        if (pred == 3u) return;
        ++pred;
    }
    void NotJump() {
        if (!(pred & 2u)) ++cnt;
        if (pred == 0u) return;
        --pred;
    }
    bool Predict() {
        ++tot;
        return pred & 2u;
    }
    double GetAccuracy() {
        if (!tot) return -1; // Haven't predicted yet
        return (double)cnt / tot;
    }
};

#endif //RISC_V_SIMULATOR_PREDICTOR_H
