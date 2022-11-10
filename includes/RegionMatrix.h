// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 27/10/2022.
//

#ifndef GALAXYSIMULATION_REGIONMATRIX_H
#define GALAXYSIMULATION_REGIONMATRIX_H

#include <vector>
#include "../Vector3.h"
#include "Region.h"
#include "../includes.h"

#define RegionArrayT std::vector<Region*>
class  RegionMatrix {
public:
    // TODO: Fine tune a decent overlap factor - higher == more overlaps for stars == longer runtimes == higher == greater accuracy?
    float overlap_factor;

    RegionArrayT regions;
    Vector step, overlap, divisions;

    Vector simulationSpaceStart, simulationSpaceEnd = {};

    RegionMatrix() = default;

    RegionMatrix(Vector min, Vector max, Vector divisions, float overlap_factor = 0.0003f) :
            simulationSpaceStart(min), simulationSpaceEnd(max), divisions(divisions), overlap_factor(overlap_factor)
    {
        step = (simulationSpaceEnd - simulationSpaceStart) / divisions;
        overlap = Vector(step.x * overlap_factor, step.y * overlap_factor, step.z * overlap_factor); // overlap between regions

        for (int i = 0; i < divisions.x; i++) {
            for (int j = 0; j < divisions.y; j++) {
                for (int k = 0; k < divisions.z; k++) {
                    regions.push_back(new Region(Vector((simulationSpaceStart.x + i * step.x) - overlap.x,
                                                        (simulationSpaceStart.y + j * step.y) - overlap.y,
                                                        (simulationSpaceStart.z + k * step.z) - overlap.z
                                                 ),
                                                 Vector((simulationSpaceStart.x + (i + 1) * step.x) + overlap.x,
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
