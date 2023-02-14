// Copyright (c) Conni Bilham & Lucy Coward 2023, All Rights Reserved.

#include <RegionMatrix.h>
#include <Region.h>

RegionMatrix::RegionMatrix(Vector_t<long double> min, Vector_t<long double> max, Vector_t<long double> divisions,
                           float overlap_factor) :
        simulationSpaceStart(min), simulationSpaceEnd(max), divisions(divisions), overlap_factor(overlap_factor) {
    // Ensure we dont enter a value out of range
    assert(overlap_factor >= 0.f && overlap_factor <= 1.f);
    assert(divisions.x > 0 && divisions.y > 0 && divisions.z > 0);

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

void RegionMatrix::reset() {
    for (auto region : regions) {
        region->reset();
    }
}

void RegionMatrix::computeRegionComs() {
    for (auto region : regions) {
        region->computeRegionCom();
    }
}
