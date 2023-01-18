// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 27/10/2022.
//

#ifndef GALAXYSIMULATION_REGIONMATRIX_H
#define GALAXYSIMULATION_REGIONMATRIX_H

#include <vector>
#include "Vector3.h"
#include "Region.h"
#include "../includes.h"

class  RegionMatrix {
public:
    // TODO: Fine tune a decent overlap factor - higher == more overlaps for stars == longer runtimes == higher == greater accuracy?
    float overlap_factor;

    std::vector<Region*> regions;
    Vectorr step, overlap, divisions, simulationSpaceStart, simulationSpaceEnd = {};

    RegionMatrix() = default;

    RegionMatrix(Vectorr min, Vectorr max, Vectorr divisions, float overlap_factor = 0.0003f) :
            simulationSpaceStart(min), simulationSpaceEnd(max), divisions(divisions), overlap_factor(overlap_factor)
    {
        // Ensure we dont enter a value out of range
        assert(overlap_factor >= 0.f && overlap_factor <= 1.f);

        step = (simulationSpaceEnd - simulationSpaceStart) / divisions;
        overlap = Vectorr(step.x * overlap_factor, step.y * overlap_factor, step.z * overlap_factor); // overlap between regions

        for (int i = 0; i < divisions.x; i++) {
            for (int j = 0; j < divisions.y; j++) {
                for (int k = 0; k < divisions.z; k++) {
                    regions.push_back(new Region(Vectorr((simulationSpaceStart.x + i * step.x) - overlap.x,
                                                        (simulationSpaceStart.y + j * step.y) - overlap.y,
                                                        (simulationSpaceStart.z + k * step.z) - overlap.z
                                                 ),
                                                 Vectorr((simulationSpaceStart.x + (i + 1) * step.x) + overlap.x,
                                                        (simulationSpaceStart.y + (j + 1) * step.y) + overlap.y,
                                                        (simulationSpaceStart.z + (k + 1) * step.z) + overlap.z
                                                 )));
                }
            }
        }
    }

#ifdef WINDOWS_or_LINUX
    ~RegionMatrix() {
        for (const auto &item: this->regions) {
            if(item) delete(item);
        }
    }
#endif
};

#endif //GALAXYSIMULATION_REGIONMATRIX_H
