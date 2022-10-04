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
    std::vector<Region*> regions_we_are_in = {};

    RegionMatrix* parent = nullptr;

    std::vector<Region*> find_regions() {
#ifdef _DEBUG
        std::cout << "Star Pos: " << this->position.x << ", " << this->position.y << ", " << this->position.z << std::endl;
#endif
        // weird math that works to find the index of the region the star is in
        auto tmp = (this->position.x - this->parent->simulationSpaceStart.x) / this->parent->step.x;
        auto tmp2 = std::floor(tmp);
        auto remainder = tmp - tmp2;
        int index = (tmp2) * this->parent->divisions.y * this->parent->divisions.z; //indexing works

//        if(this->parent->debug) std::cout << "index: " << index << std::endl;

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

        tmp = (this->position.y - this->parent->simulationSpaceStart.y) / this->parent->step.y;
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
        tmp = (this->position.z - this->parent->simulationSpaceStart.z) / this->parent->step.z;
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

        if (mode_neighbours == 1) {
//            if(this->parent->debug) std::cout << "No overlapping regions found. Index of box we are in is: " << index << std::endl;
// getting the region object from the index
            regions_we_are_in.push_back(this->parent->regions[index]);
        }
        if (mode_neighbours == 2) {
//            if(this->parent->debug) std::cout << "We are in 2 regions. Index of box we are in is: " << index << std::endl;
            tmp = index;

            tmp += (neighbour_x * this->parent->divisions.y * this->parent->divisions.z);
            tmp += (neighbour_y * this->parent->divisions.z);
            tmp += neighbour_z;

//            if(this->parent->debug) std::cout << "The index of the neighbouring region is: " << tmp << std::endl;
            regions_we_are_in.emplace_back(this->parent->regions[index]);
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
        }
        if (mode_neighbours == 4) {
//            if(this->parent->debug) std::cout << "We are in 4 regions. Index of box we are in is: " << index << std::endl;
            tmp = index;

            tmp += (neighbour_x * this->parent->divisions.y * this->parent->divisions.z);
            tmp += (neighbour_y * this->parent->divisions.z);
            tmp += neighbour_z;

            regions_we_are_in.emplace_back(this->parent->regions[index]);
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//            if(this->parent->debug) std::cout << "The index of the corner neighbouring regions is: " << tmp << std::endl;

            if (neighbour_x != 0) {
                tmp = (index + (neighbour_x * this->parent->divisions.y * this->parent->divisions.z));
                regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//                if(this->parent->debug) std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
            }
            if (neighbour_y != 0) {
                tmp = (index + (neighbour_y * this->parent->divisions.z));
                regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//                if(this->parent->debug) std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
            }
            if (neighbour_z != 0) {
                tmp = (index + neighbour_z);
                regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//                if(this->parent->debug) std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
            }
        }
        if (mode_neighbours == 8) {
//            if(this->parent->debug) std::cout << "We are in 8 regions. Index of box we are in is: " << index << std::endl;
            tmp = index;

            tmp += (neighbour_x * this->parent->divisions.y * this->parent->divisions.z) + (neighbour_y * this->parent->divisions.z) + neighbour_z;
            regions_we_are_in.emplace_back(this->parent->regions[index]);
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//            if(this->parent->debug) std::cout << "The index of the corner neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_x * this->parent->divisions.y * this->parent->divisions.z);
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//            if(this->parent->debug) std::cout << "(X) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_y * this->parent->divisions.z);
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//            if(this->parent->debug) std::cout << "(Y) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + neighbour_z;
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//            if(this->parent->debug) std::cout << "(Z) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_x * this->parent->divisions.y * this->parent->divisions.z) + (neighbour_y * this->parent->divisions.z);
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//            if(this->parent->debug) std::cout << "(XY) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_x * this->parent->divisions.y * this->parent->divisions.z) + neighbour_z;
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//            if(this->parent->debug) std::cout << "(XZ) The index of one of the neighbouring regions is: " << tmp << std::endl;

            tmp = index + (neighbour_y * this->parent->divisions.z) + neighbour_z;
            regions_we_are_in.emplace_back(this->parent->regions[tmp]);
//            if(this->parent->debug) std::cout << "(YZ) The index of one of the neighbouring regions is: " << tmp << std::endl;

        }

        return regions_we_are_in;
    }

    // code for updating the acceleration of the star
    // this is the most important part of the simulation
    // TODO: make this use the regioning system to update stars only in their current and neighbouring regions
    // TODO: When a star is overlapping regions it should update the acceleration using the other stars in the overlapping regions
    // TODO: Make stars update using regions COM
    double acceleration_update_stars_in_region(bool clear_accel) { // current_star_positions is star list
        if (clear_accel) {
            acceleration = Vector(0, 0, 0);
        }
        auto accelerationStartTime = std::chrono::high_resolution_clock::now();
        for (auto region : this->regions_we_are_in) {
            for (auto star : region->stars_in_region) {
                if (star->id == id)
                    continue;

                long double r = this->position.distTo(star->position);
                long double accel_from_star = (gravitationalConstantFinal * star->mass)/(pow(r, 2)); // Now gives acceleration in pc/year^2

                this->acceleration.x += accel_from_star * ((star->position.x - this->position.x)/r);
                this->acceleration.y += accel_from_star * ((star->position.y - this->position.y)/r);
                this->acceleration.z += accel_from_star * ((star->position.z - this->position.z)/r);
            }
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::high_resolution_clock::now())-accelerationStartTime).count();
    }

    double acceleration_update_region_com(bool clear_accel) {
        if (clear_accel) {
            acceleration = Vector(0, 0, 0);
        }
        auto accelerationStartTime = std::chrono::high_resolution_clock::now();
        for (auto region : this->parent->regions){
            if (std::find(regions_we_are_in.begin(), regions_we_are_in.end(), region) != regions_we_are_in.end()) { // Conni you might want to make this your way
                continue;
            }
            long double r = this->position.distTo(region->com_position);
            long double accel_from_region = (gravitationalConstantFinal * region->com_mass)/(pow(r, 2)); // Now gives acceleration in pc/year^2

            this->acceleration.x += accel_from_region * ((region->com_position.x - this->position.x)/r);
            this->acceleration.y += accel_from_region * ((region->com_position.y - this->position.y)/r);
            this->acceleration.z += accel_from_region * ((region->com_position.z - this->position.z)/r);
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
