// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_STAR_GENERATOR_H
#define GALAXYSIMULATION_STAR_GENERATOR_H

#include "RegionMatrix.h"
#include "ConfigStruct.h"
#include "../Star.h"
#include "includes/Random.h"

class star_generator {
public:
    star_generator() = default;

    void gaussian(std::vector<Star*> *star_list, ConfigStruct::StarGenerator config, RegionMatrix* parent_region_matrix);

    void uniform(
        RegionMatrix* parent_region_matrix, // the region matrix that the stars are in
        std::vector<Star*> *star_list,

        long int number_of_stars,
        float mean_mass, // in solar masses
        float std_dev_mass, // in solar masses
        Vectorr min_position, // minimum position of a star can have
        Vectorr max_position, // maximum position of a star can have
        Vectorr velocity_at_origin, // average velocity of a star can have at the origin pc/year
        Vectorr variation_velocity, // variation of velocity of a star can have pc/year
        Vectorr variation_in_direction, // the variation in direction of the velocity in radians +-
        int arms, // number of arms in the galaxy
        int number_of_stars_per_arm, // number of stars per arm
        std::vector<float> arm_positions, // the positions of the arms in radians
        float arm_width, // the width of the arms in parsecs
        float arm_height, // the height of the arms in parsecs
        float arm_length, // the length of the arms in pc
        float arm_offset // the offset of the arms in radians
    );

    void random(

            );

};
#endif //GALAXYSIMULATION_STAR_GENERATOR_H
