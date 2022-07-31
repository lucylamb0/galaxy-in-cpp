//
// Created by User on 31/07/2022.
//

#ifndef C_VERSION_STAR_H
#define C_VERSION_STAR_H

#include "Vector3.h"
#include "includes.h"

class Star {
public:
    Star(int id, Vector3 position, Vector3 velocity, Vector3 acceleration) :
            id(id), position(position), velocity(velocity), acceleration(acceleration) {}

    int id;
    Vector3 position, velocity, acceleration;
    std::vector<Vector3> history_position = {};

    double acceleration_update(const std::vector<Star>& current_star_positions) {
        auto accelerationStartTime = std::chrono::high_resolution_clock::now();

        acceleration = Vector3(0, 0, 0);
        for (const auto& star : current_star_positions) {
            if (star.id == id)
                continue;
            double radius = std::sqrt(std::pow(2.0, this->position.x - this->position.x) + std::pow(2.0, this->position.y - this->position.y) + std::pow(2.0, this->position.z - this->position.z));
            double accel_from_star = (gravitationalConstant * starMass)/(std::pow(2.0, radius));

            this->acceleration.x += accel_from_star * ((this->position.x - this->position.x)/radius);
            this->acceleration.y += accel_from_star * ((this->position.y - this->position.y)/radius);
            this->acceleration.z += accel_from_star * ((this->position.z - this->position.z)/radius);
        }
        auto accelerationEndTime = std::chrono::high_resolution_clock::now();
        auto accelerationDuration = std::chrono::duration_cast<std::chrono::milliseconds>(accelerationEndTime-accelerationStartTime).count();
        return accelerationDuration;
    }

    void velocity_update() {
        this->velocity.x += this->acceleration.x * time_step;
        this->velocity.y += this->acceleration.y * time_step;
        this->velocity.z += this->acceleration.z * time_step;
    }

    void position_update() {
        this->position.x += this->velocity.x * time_step + 0.5 * this->acceleration.x * time_step * std::pow(2.0, time_step);
        this->position.y += this->velocity.y * time_step + 0.5 * this->acceleration.y * time_step * std::pow(2.0, time_step);
        this->position.z += this->velocity.z * time_step + 0.5 * this->acceleration.z * time_step * std::pow(2.0, time_step);
    }
};

#endif //C_VERSION_STAR_H
