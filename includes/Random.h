// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 17/11/2022.
//

#ifndef GALAXYSIMULATION_RANDOM_H
#define GALAXYSIMULATION_RANDOM_H

#include <random>

class Random {
public:
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen;
    Random() {
        this->seed();
    }

    void seed() {
        this->gen = std::mt19937(rd()); // seed the generator
    }

#define SHOULD_SEED this->seed();

    Vector_t<std::uniform_real_distribution<long double>> uniform_real_distro_vector (Vector min, Vector max) {
        std::uniform_real_distribution<long double> dis_x(min.x, max.x);
        std::uniform_real_distribution<long double> dis_y(min.y, max.y);
        std::uniform_real_distribution<long double> dis_z(min.z, max.z);
        return {dis_x, dis_y, dis_z};
    }

    std::uniform_real_distribution<long double> uniform_real_distro(long double min, long double max) {
        SHOULD_SEED
        std::uniform_real_distribution<long double> distr(min, max); // define the range
        return distr;
    }

    std::normal_distribution<long double> normal_distro(long double min, long double max) {
        SHOULD_SEED
        std::normal_distribution<long double> distr(min, max); // define the range
        return distr;
    }
};
#endif //GALAXYSIMULATION_RANDOM_H
