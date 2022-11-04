// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef C_VERSION_STAR_H
#define C_VERSION_STAR_H

#include "includes.h"
#include "Region.h"
#include "RegionMatrix.h"

class Star {
public:
    int first, second = -1;
    char lastSign = '-';

    // position uses parsecs and velocity uses pc/year
    Star(int id, Vector position, Vector velocity, Vector acceleration, float mass, RegionMatrix* parent_region_matrix) :
            id(id), position(position), velocity(velocity), acceleration(acceleration), mass(mass), parent(parent_region_matrix) {
        this->history_position.emplace_back(this->position.x, this->position.y, this->position.z);
    }

    int id;
    float mass;
    Vector position, velocity, acceleration;
    std::vector<Vector> history_position = {};
    std::vector<Vector> history_velocity = {};
    std::vector<Vector> history_acceleration = {};

    std::vector<Region*> regions_we_are_in = {};

    RegionMatrix* parent = nullptr;

    RegionArrayT find_regions();

    double acceleration_update_stars_in_region(bool clear_accel);

    double acceleration_update_region_com(bool clear_accel);

    void velocity_update();

    void position_update();
};

#endif //C_VERSION_STAR_H
