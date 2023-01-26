// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef C_VERSION_REGION_H
#define C_VERSION_REGION_H

#include <iostream>
#include "Vector3.h"
#include "../Star.h"

// TODO: Give each region a COM
// TODO: If a region has no stars set a flag to not calculate it and to skip it when calculating star accels
class CentreMass {
public:
    bool initiated = false;

    Vectorr position = Vectorr(0, 0, 0);
    long double mass = 0;

    CentreMass() {
        this->initiated = false;
        this->position = Vectorr(0, 0, 0);
        this->mass = 0;
    }

    void reset() {
        this->initiated = false;
        this->position = Vectorr(0, 0, 0);
        this->mass = 0;
    }
};

class Region {
private:
public:

    Vectorr min, max;
    CentreMass centreMass;

    std::vector<Star*> stars_in_region = {};
    Region(Vectorr min, Vectorr max) : min(min), max(max) {}

    void reset() {
        this->stars_in_region.clear();
        this->centreMass.reset();
    }

    void computeRegionCom() {
        for (Star* star : this->stars_in_region) {
            // Set up a shorthand com for usage in this local module
            auto* centreMass = &this->centreMass;
            if(!centreMass->initiated) {
                // Set the centre of mass for the region considering the single star inside
                centreMass->position = star->position;
                centreMass->mass = star->mass;
                centreMass->initiated = true;

            } else {
                auto mass_bias = star->mass - centreMass->mass;

                // Calculate new centre of mass including the new star.
                centreMass->mass += star->mass;
                centreMass->position -= (star->position - centreMass->position) * mass_bias;
            }
        }
    }
};

#endif //C_VERSION_REGION_H
