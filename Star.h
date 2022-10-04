//
// Created by User on 31/07/2022.
//

#ifndef C_VERSION_STAR_H
#define C_VERSION_STAR_H

#include "Vector3.h"
#include "includes.h"
#include "Region.h"

class Star {
public:
    Star(int id, Vector position, Vector velocity, Vector acceleration, float mass, RegionMatrix* parent_region_matrix) : // position uses parsecs and velocity uses pc/year
            id(id), position(position), velocity(velocity), acceleration(acceleration), mass(mass), parent(parent_region_matrix) {

        this->history_position.emplace_back(this->position.x, this->position.y, this->position.z);
    }

    int id;
    float mass;
    Vector position, velocity, acceleration;
    std::vector<Vector> history_position = {};
    std::vector<int> regions_we_are_in = {};

    RegionMatrix* parent = nullptr;

    std::vector<int> find_regions(Vector playSpaceStart) {
        const bool DEBUG = false;
        if(DEBUG) std::cout << "Star Pos: " << this->position.x << ", " << this->position.y << ", " << this->position.z << std::endl;
        // weird math that works to find the index of the region the star is in
        auto tmp = (this->position.x - playSpaceStart.x) / this->parent->step.x;
        auto tmp2 = std::floor(tmp);
        auto remainder = tmp - tmp2;
        int index = (tmp2) * this->parent->divisions.y * this->parent->divisions.z; //indexing works

        if(DEBUG) std::cout << "index: " << index << std::endl;

        int neighbour_x = 0;
        int neighbour_y = 0;
        int neighbour_z = 0;
        int mode_neighbours = 1; // 1 = 1 region, 2 = 2 regions, 4 = 4 regions, 8 = 8 regions

        if(remainder <= this->parent->overlap_factor) { // need to check all 3 directions to see if we are in a neighbour region
            // We are overlapping the below region
            neighbour_x = -1;
            mode_neighbours *= 2;
        }
        else if (remainder >= 1 - this->parent->overlap_factor) {
            // We are overlapping the above region
            neighbour_x = 1;
            mode_neighbours *= 2;
        }

        tmp = (this->position.y - playSpaceStart.y) / this->parent->step.y;
        tmp2 = std::floor(tmp);
        remainder = tmp - tmp2;
        index += (tmp2) * this->parent->divisions.z;


        if(remainder <= this->parent->overlap_factor) {
            // We are overlapping the below region
            neighbour_y = -1;
            mode_neighbours *= 2;
        }
        else if (remainder >= 1 - this->parent->overlap_factor) {
            // We are overlapping the above region
            neighbour_y = 1;
            mode_neighbours *= 2;
        }

// TODO: Will need to check weather the type of int/float work correctly when Star pos and playspace are large
        tmp = (this->position.z - playSpaceStart.z) / this->parent->step.z;
        tmp2 = std::floor(tmp);
        remainder = tmp - tmp2;
        index += (tmp2);

        if(remainder <= this->parent->overlap_factor) {
            // We are overlapping the below region
            neighbour_z = -1;
            mode_neighbours *= 2;
        }
        else if (remainder >= 1 - this->parent->overlap_factor) {
            // We are overlapping the above region
            neighbour_z = 1;
            mode_neighbours *= 2;
        }


//    std::list<int> neighbour_list = (neighbour_x, neighbour_y, neighbour_z);
//    Vector neighbour_vector = Vector(neighbour_x,neighbour_y,neighbour_z);


// TODO: Link debug flag to a global constant for logging
        if (mode_neighbours == 1) {
            if(DEBUG) std::cout << "No overlapping regions found. Index of box we are in is: " << index << std::endl;
            regions_we_are_in.push_back(index);
        }
        if (mode_neighbours == 2) {
            if(DEBUG) std::cout << "We are in 2 regions. Index of box we are in is: " << index << std::endl;
            tmp = index;

            tmp += (neighbour_x * this->parent->divisions.y * this->parent->divisions.z);
            tmp += (neighbour_y * this->parent->divisions.z);
            tmp += neighbour_z;

            if(DEBUG) std::cout << "The index of the neighbouring region is: " << tmp << std::endl;
            regions_we_are_in.emplace_back(index);
            regions_we_are_in.emplace_back(tmp);
        }
        if (mode_neighbours == 4) {
            if(DEBUG) std::cout << "We are in 4 regions. Index of box we are in is: " << index << std::endl;
            tmp = index;

            tmp += (neighbour_x * this->parent->divisions.y * this->parent->divisions.z);
            tmp += (neighbour_y * this->parent->divisions.z);
            tmp += neighbour_z;

            regions_we_are_in.emplace_back(index);
            regions_we_are_in.emplace_back(tmp);
            if(DEBUG) std::cout << "The index of the corner neighbouring regions is: " << tmp << std::endl;

            if (neighbour_x != 0) {
                tmp = (index + (neighbour_x * this->parent->divisions.y * this->parent->divisions.z));
                regions_we_are_in.emplace_back(tmp);
                if(DEBUG) std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
            }
            if (neighbour_y != 0) {
                tmp = (index + (neighbour_y * this->parent->divisions.z));
                regions_we_are_in.emplace_back(tmp);
                if(DEBUG) std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
            }
            if (neighbour_z != 0) {
                tmp = (index + neighbour_z);
                regions_we_are_in.emplace_back(tmp);
                if(DEBUG) std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
            }
        }
        if (mode_neighbours == 8) {
            if(DEBUG) std::cout << "We are in 8 regions. Index of box we are in is: " << index << std::endl;
            tmp = index;

            tmp += (neighbour_x * this->parent->divisions.y * this->parent->divisions.z) + (neighbour_y * this->parent->divisions.z) + neighbour_z;
            regions_we_are_in.emplace_back(index);
            regions_we_are_in.emplace_back(tmp);
            if(DEBUG) std::cout << "The index of the corner neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_x * this->parent->divisions.y * this->parent->divisions.z);
            regions_we_are_in.emplace_back(tmp);
            if(DEBUG) std::cout << "(X) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_y * this->parent->divisions.z);
            regions_we_are_in.emplace_back(tmp);
            if(DEBUG) std::cout << "(Y) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + neighbour_z;
            regions_we_are_in.emplace_back(tmp);
            if(DEBUG) std::cout << "(Z) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_x * this->parent->divisions.y * this->parent->divisions.z) + (neighbour_y * this->parent->divisions.z);
            regions_we_are_in.emplace_back(tmp);
            if(DEBUG) std::cout << "(XY) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_x * this->parent->divisions.y * this->parent->divisions.z) + neighbour_z;
            regions_we_are_in.emplace_back(tmp);
            if(DEBUG) std::cout << "(XZ) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_y * this->parent->divisions.z) + neighbour_z;
            regions_we_are_in.emplace_back(tmp);
            if(DEBUG) std::cout << "(YZ) The index of one of the neighbouring regions is: " << tmp << std::endl;

        }

        return regions_we_are_in;
    }

    // code for updating the acceleration of the star
    // this is the most important part of the simulation
    // TODO: make this use the regioning system to update stars only in their current and neighbouring regions
    // TODO: When a star is overlapping regions it should update the acceleration using the other stars in the overlapping regions
    // TODO: Make stars update using regions COM
    double acceleration_update(const std::vector<Star*>& current_star_positions) {
        auto accelerationStartTime = std::chrono::high_resolution_clock::now();

        acceleration = Vector(0, 0, 0);
        for (const auto& star : current_star_positions) {
            if (star->id == id)
                continue;

            long double r = this->position.distTo(star->position);
            long double accel_from_star = (gravitationalConstantFinal * star->mass)/(pow(r, 2)); // Now gives acceleration in pc/year^2

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
