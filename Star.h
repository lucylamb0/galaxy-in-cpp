// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef C_VERSION_STAR_H
#define C_VERSION_STAR_H

#include "includes.h"
#include "includes/Region.h"
#include "includes/RegionMatrix.h"

enum class STAR_FLAGS
{
    STATIC = 1 << 0, // 1
    Flag2 = 1 << 1, // 2
    Flag3 = 1 << 2, // 4
    Flag4 = 1 << 3, // 8
    Flag5 = 1 << 4, // 16
    Flag6 = 1 << 5, // 32
    Flag7 = 1 << 6, // 64
    Flag8 = 1 << 7  //128
};

class Star {
private:
    unsigned char flags = 0;

public:
    int first, second = -1;

    char lastSign = '-';

    // position uses parsecs and velocity uses pc/year
    Star(int id, Vector position, Vector velocity, Vector acceleration, float mass, RegionMatrix* parent_region_matrix, int flags = 0) :
            id(id), position(position), velocity(velocity), acceleration(acceleration), mass(mass), parent(parent_region_matrix) {
        this->history_position.emplace_back(this->position.x, this->position.y, this->position.z);
        this->history_velocity.emplace_back(this->velocity.x, this->velocity.y, this->velocity.z);
        this->history_acceleration.emplace_back(this->acceleration.x, this->acceleration.y, this->acceleration.z);

        this->flags |= flags;
    }

    int id;
    float mass;
    Vector position, velocity, acceleration;
    std::vector<Vector> history_position = {};
    std::vector<Vector> history_velocity = {};
    std::vector<Vector> history_acceleration = {};

    std::vector<Region*> regions_we_are_in = {};

    RegionMatrix* parent = nullptr;

    void find_regions();

    double acceleration_update_stars_in_region(bool clear_accel);

    double acceleration_update_region_com(bool clear_accel);

    void velocity_update();

    void position_update();

    bool is_static() {
        return this->flags & (int)STAR_FLAGS::STATIC;
    }

    long double kinetic_energy() {
        return (0.5 * this->mass) * this->velocity.magnitude_squared();
    }
};

#endif //C_VERSION_STAR_H
