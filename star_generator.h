// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_STAR_GENERATOR_H
#define GALAXYSIMULATION_STAR_GENERATOR_H

#include "RegionMatrix.h"
#include "ConfigStruct.h"

class star_generator {
public:
    star_generator() = default;

    void generate_gaussian(ConfigStruct::StarGenerator config, RegionMatrix* parent_region_matrix);
    void generate_ga
    void generate_uniform();

    void generate_random();

};
#endif //GALAXYSIMULATION_STAR_GENERATOR_H
