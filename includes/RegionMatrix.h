// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_REGIONMATRIX_H
#define GALAXYSIMULATION_REGIONMATRIX_H

#include <vector>
#include "Vector3.h"
#include "../includes.h"

class Region;
class RegionMatrix {
public:
    // TODO: Fine tune a decent overlap factor - higher == more overlaps for stars == longer runtimes == higher == greater accuracy?
    float overlap_factor{};

    std::vector<Region*> regions;
    Vectorr step{}, overlap{}, divisions{}, simulationSpaceStart{}, simulationSpaceEnd = {};

    RegionMatrix() = default;

    RegionMatrix(Vectorr min, Vectorr max, Vectorr divisions, float overlap_factor = 0.0003f);

    void reset();

    void computeRegionComs();

#ifdef WINDOWS_or_LINUX
    ~RegionMatrix() {
        for (const auto &item: this->regions) {
            if(item) delete(item);
        }
    }
#endif
};

#endif //GALAXYSIMULATION_REGIONMATRIX_H
