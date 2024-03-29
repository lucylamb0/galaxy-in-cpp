// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef C_VERSION_STAR_H
#define C_VERSION_STAR_H

#include "includes.h"
//#include "includes/Region.h"
#include "includes/RegionMatrix.h"
#include "includes/logging.h"
#include "includes/Vector3.h"

class Region;

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

class history_record_t {
public:
    Vectorr position, velocity, acceleration = {};

    history_record_t() = default;

    history_record_t(Vectorr position, Vectorr velocity, Vectorr acceleration) :
        position(position), velocity(velocity), acceleration(acceleration) {}

};
class Star {
private:
    unsigned char flags = 0;

public:
    int first, second = -1;

    // position uses parsecs and velocity uses pc/year
    Star(int id, Vectorr position, Vectorr velocity, Vectorr acceleration, float mass, RegionMatrix* parent_region_matrix, int flags = 0) :
            id(id), position(position), velocity(velocity), acceleration(acceleration), mass(mass), parent(parent_region_matrix) {

        this->history.emplace_back(
                history_record_t(this->position, this->velocity, this->acceleration)
        );
        // Push final history record

        this->flags |= flags;
    }

    int id;
    float mass;
    Vectorr position, velocity, acceleration;

    history_record_t history_tmp = history_record_t();
    std::vector<history_record_t> history = {};

//    std::vector<Vectorr> history_position = {};
//    std::vector<Vectorr> history_velocity = {};
//    std::vector<Vectorr> history_acceleration = {};

    std::vector<Region*> regions_we_are_in = {};

    RegionMatrix* parent = nullptr;

    void find_regions();

    double acceleration_update_stars_in_region(bool clear_accel);

    double acceleration_update_region_com(bool clear_accel);

    void velocity_update();

    void position_update(long double override_timestep = 0);

    bool is_static() const;

    long double kinetic_energy(int history_index = -1, bool reverse = false);
};

#endif //C_VERSION_STAR_H
