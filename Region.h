// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef C_VERSION_REGION_H
#define C_VERSION_REGION_H

#include <iostream>
#include "Vector3.h"
#include "Star.h"

// TODO: Give each region a COM
// TODO: If a region has no stars set a flag to not calculate it and to skip it when calculating star accels
class Star;
class CentreMass {
public:
    Vector position;
    float mass;
};
class Region {
public:
    Vector min, max;

    CentreMass centreMass;

    std::vector<Star*> stars_in_region = {};
    Region(Vector min, Vector max) : min(min), max(max) {}
};

#endif //C_VERSION_REGION_H
