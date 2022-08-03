//
// Created by User on 31/07/2022.
//

#ifndef C_VERSION_STAR_H
#define C_VERSION_STAR_H

#include "Vector3.h"
#include "includes.h"

class Star {
public:
    Star(int id, Vector position, Vector velocity, Vector acceleration) :
            id(id), position(position), velocity(velocity), acceleration(acceleration) {}

    int id;
    Vector position, velocity, acceleration;
    std::vector<Vector> history_position = {};

    double acceleration_update(const std::vector<Star*>& current_star_positions) {
        auto accelerationStartTime = std::chrono::high_resolution_clock::now();

        acceleration = Vector(0, 0, 0);
        for (const auto& star : current_star_positions) {
            if (star->id == id)
                continue;

            long double r = this->position.distTo(star->position);
            long double accel_from_star = (gravitationalConstant * starMass)/(pow(r, 2));

            this->acceleration.x += accel_from_star * ((star->position.x - this->position.x)/r);
            this->acceleration.y += accel_from_star * ((star->position.y - this->position.y)/r);
            this->acceleration.z += accel_from_star * ((star->position.z - this->position.z)/r);
        }
        auto accelerationEndTime = std::chrono::high_resolution_clock::now();
        auto accelerationDuration = std::chrono::duration_cast<std::chrono::milliseconds>(accelerationEndTime-accelerationStartTime).count();
        return accelerationDuration;
    }

    inline void velocity_update() {
        this->velocity += this->acceleration * time_step;
    }

    inline void position_update() {
        this->position.x += this->velocity.x * time_step + 0.5 * this->acceleration.x * time_step * std::pow(time_step, 2);
        this->position.y += this->velocity.y * time_step + 0.5 * this->acceleration.y * time_step * std::pow(time_step, 2);
        this->position.z += this->velocity.z * time_step + 0.5 * this->acceleration.z * time_step * std::pow(time_step, 2);
    }

//    void find_star_region(reg)
};

#endif //C_VERSION_STAR_H
