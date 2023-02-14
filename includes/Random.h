// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

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

#define SHOULD_SEED // this->seed();

    Vector_t<std::normal_distribution<long double>> normal_distribution_vector(Vectorr mean, Vectorr stddev) {
        return Vector_t<std::normal_distribution<long double>>(
                normal_distro(mean.x, stddev.x),
                normal_distro(mean.y, stddev.y),
                normal_distro(mean.z, stddev.z)
        );
    }

    Vector_t<std::uniform_real_distribution<long double>> uniform_real_distro_vector (Vectorr min, Vectorr max) {
        return Vector_t<std::uniform_real_distribution<long double>>(
                uniform_real_distro(min.x, max.x),
                uniform_real_distro(min.y, max.y),
                uniform_real_distro(min.z, max.z)
        );
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
