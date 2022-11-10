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

    Vector position = Vector(0,0,0);
    long double mass = 0;

    CentreMass() {
        this->initiated = false;
        this->position = Vector(0,0,0);
        this->mass = 0;
    }

    void reset() {
        this->initiated = false;
        this->position = Vector(0,0,0);
        this->mass = 0;
    }
};

class Star;
class Region {
private:
public:

    Vector min, max;
    CentreMass centreMass;

    std::vector<Star*> stars_in_region = {};
    Region(Vector min, Vector max) : min(min), max(max) {}
};

#endif //C_VERSION_REGION_H
