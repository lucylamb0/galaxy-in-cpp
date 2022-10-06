//
// Created by User on 01/08/2022.
//

#ifndef C_VERSION_REGION_H
#define C_VERSION_REGION_H

#include <iostream>
#include "Vector3.h"
class Star;
class Region {
public:
    Vector min, max;
    Vector com_position = Vector(0, 0, 0);
    float com_mass = 0;

    Vector center;
//    long double totalMass;

    std::vector<Star*> stars_in_region = {};
    Region(Vector min, Vector max) : min(min), max(max), center((min + max) / 2) {}

    bool contains_x(Vector v) const {
        return v.x >= min.x && v.x <= max.x;
    }
    bool contains_y(Vector v) const {
        return v.y >= min.y && v.y <= max.y;
    }
    bool contains_z(Vector v) const {
        return v.z >= min.z && v.z <= max.z;
    }

    bool contains(Vector v) {
        return v.x >= min.x && v.x <= max.x &&
                v.y >= min.y && v.y <= max.y &&
                v.z >= min.z && v.z <= max.z;
    }
};
// TODO: Give each region a COM
// TODO: If a region has no stars set a flag to not calculate it and to skip it when calculating star accels

#define RegionArrayT std::vector<Region*>
class RegionMatrix {
public:
    // TODO: Fine tune a decent overlap factor - higher == more overlaps for stars == longer runtimes == higher == greater accuracy?
    float overlap_factor;

    RegionArrayT regions;
    Vector step, overlap, divisions;

    Vector simulationSpaceStart, simulationSpaceEnd = {};

    RegionMatrix(Vector min, Vector max, Vector divisions, float overlap_factor = 0.0003f) :

                simulationSpaceStart(min), simulationSpaceEnd(max), divisions(divisions), overlap_factor(overlap_factor)
    {
        setup_regions();
    }

    // Dynamic matrix
    RegionMatrix() {
        setup_regions();
    }

    bool setup_regions() {
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
 };

#endif //C_VERSION_REGION_H
