// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#include <sstream>
#include <string>
#include <chrono>
#include "Vector3.h"
#include "Star.h"
#include "includes.h"
#include "Region.h"
#include <iostream>
#include <thread>

#include "logging.h"

// TODO: CLEAN UP THE CODE, lots of commented out sections that i don't know if it is needed or not
// CODE USES PARSECS, SOLAR MASS, PC/YEAR and PC/YEAR^2
std::vector<Star*> star_list = {};
// Not really sure what this does, probably made by Conni
// Yes it was made by me, and it splits a string by a delimiter.
// Example:
// Input: Hello, world!   ,
// Output: ["Hello", "world!"]
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void compute_region_com(Region* region) {
    for (Star* star : region->stars_in_region) {
        // TODO: Should consider the mass of the star when calculating the COM
        region->centreMass.position = region->centreMass.position.x == 0 ?
                star->position :
                (region->centreMass.position -
                    ((region->centreMass.position - star->position) *
                        (star->mass / region->centreMass.mass)
                ));
        region->centreMass.mass += star->mass;
    }
}

RegionMatrix regionMatrix;

int main(int arg_count, char** args) {
    std::string data_set_path = "D:\\JET BRAINS\\galaxy-in-cpp/star_data.csv";
    if (arg_count == 2) {
        data_set_path = args[1];
    }
    if (arg_count > 4) {
        data_set_path = args[1];

        std::cout << "[ Data Set Path ]    [" << data_set_path << "]" << std::endl;

        Vector startPosition = Vector(std::stof(args[2]), std::stof(args[3]), std::stof(args[4]));
        std::cout << "[ Start Position X ] [" << startPosition.x << "]" << std::endl;
        std::cout << "[ Start Position Y ] [" << startPosition.y << "]" << std::endl;
        std::cout << "[ Start Position Z ] [" << startPosition.z << "]" << std::endl;

        Vector endPosition = Vector(std::stof(args[5]), std::stof(args[6]), std::stof(args[7]));
        std::cout << "[ End Position X ]   [" << endPosition.x << "]" << std::endl;
        std::cout << "[ End Position Y ]   [" << endPosition.y << "]" << std::endl;
        std::cout << "[ End Position Z ]   [" << endPosition.z << "]" << std::endl;

        Vector divisions = Vector(std::stof(args[8]), std::stof(args[9]), std::stof(args[10]));
        std::cout << "[ divisions X ]      [" << divisions.x << "]" << std::endl;
        std::cout << "[ divisions Y ]      [" << divisions.y << "]" << std::endl;
        std::cout << "[ divisions Z ]      [" << divisions.z << "]" << std::endl;

        regionMatrix = RegionMatrix(
                startPosition, // Start position
                endPosition, // End position
                divisions               // Amount of divisions on the z, y, z
                // Overlap factor
        );
    } else {
        regionMatrix = RegionMatrix(
                Vector(-100000, -100000, -100000), // Start position
                Vector(100000, 100000, 100000), // End position
                Vector(10, 10, 10),               // Amount of divisions on the z, y, z
                0.0003f         // Overlap factor
        );
    }

    std::cout << "Hello, World!" << std::endl; // classic hello world of course <3
    logging::info("Hello, World! (FROM THE LOGGING FRAMEWORK)", "");

    logging::info("Regions: ", regionMatrix.regions.size());

//    std::ifstream infile;
//    infile.open(data_set_path);
//    std::string line;

    star_list.emplace_back(new Star(
            1,                                // ID
            Vector(0.1, 0.1, 0.1),    // Position
            Vector(0, 0, 0),    // Velocity
            Vector(0, 0, 0), // Acceleration
            3.00273e-6,
            &regionMatrix       // Parent region matrix
    ));

    star_list.emplace_back(new Star(
            2,                                // ID
            Vector(1.246e-8 + 0.1, 0.1, 0.1),    // Position
            Vector( 0, 2.02269032e-6, 0),    // Velocity
            Vector(0, 0, 0), // Acceleration
            3.69396868e-8,
            &regionMatrix       // Parent region matrix
    ));

    int stars_cnt = 0;
// I think this is making the list of stars from the file
//    while (std::getline(infile, line)) {
//        ++stars_cnt;
////        if (stars_cnt % 25000 == 0) {
////            break;
////            logging::info("Stars: ", stars_cnt);
////        }
//        std::istringstream iss(line);
//        auto split_str = split(line, ',');
//
//        star_list.emplace_back(new Star(
//                std::stoi(split_str.at(0)),     // ID
//                Vector(std::stof(split_str.at(2)), std::stof(split_str.at(3)), std::stof(split_str.at(4))), // Position
//                Vector(std::stof(split_str.at(5)), std::stof(split_str.at(6)), std::stof(split_str.at(7))), // Velocity
//                Vector(0, 0, 0), // Acceleration
//                1.0f,
//                &regionMatrix // Parent region matrix
//        )); // Mass
//    }
//    infile.close();

    Vector min = Vector();
    Vector max = Vector();
    for (auto star: star_list) {
        if (star->position.x > max.x) {
            max.x = star->position.x;
        }
        if (star->position.y > max.y) {
            max.y = star->position.y;
        }
        if (star->position.z > max.z) {
            max.z = star->position.z;
        }
        if (star->position.x < min.x) {
            min.x = star->position.x;
        }
        if (star->position.y < min.y) {
            min.y = star->position.y;
        }
        if (star->position.z < min.z) {
            min.z = star->position.z;
        }
    }
    min = min * 1.01;
    max = max * 1.01;

    regionMatrix = RegionMatrix(
            Vector(min.x, min.y, min.z), // Start position
            Vector(max.x, max.y, max.z), // End position
            Vector(10, 10, 10)               // Amount of divisions on the z, y, z
    );

    logging::info("Assigning regions.", "");
// converting data to meters (for now)
    static unsigned long averageStarRegionCount = 0;
    for (auto star: star_list) {
        for (Region *region: star->find_regions())
            region->stars_in_region.emplace_back(star);

        if (star->id % 5000 == 0) {
            averageStarRegionCount = averageStarRegionCount == 0 ? star->regions_we_are_in.size() :
                                     (averageStarRegionCount + star->regions_we_are_in.size()) / 2;

            auto progress = to_string(star->id) + "/" + to_string(star_list.size());
            auto percent = to_string((int) ((float) star->id / (float) star_list.size() * 100));
            auto message = "Progress " + percent + "% [" + progress + "] Stars, in an avrg of " +
                           to_string(averageStarRegionCount) + " regions";
            logging::verbose(message, "");
        }
    }
    // compute region coms
    for (Region *region: regionMatrix.regions) {
        compute_region_com(region);
    }

    // After updating regions stars we need to ensure we update centre of mass and total mass of the regions
//    for(Region* region : regionMatrix.regions) {
//
//    }

    logging::info("Finished assigning regions", "");
// don't know what the hell averageStarUpdateTime is, I think it is for keeping track of how long it takes
//    static long long averageAccelerationUpdateTime = -1;
    static long long averageStarUpdateTime = -1;

    int thread_count = std::thread::hardware_concurrency() - 1;

//    thread_count /= 3;

    auto work_queue = std::vector<std::vector<Star *>>{};
    auto star_count = star_list.size();
    auto left_over = star_count % thread_count;
    auto star_per_thread = (star_count - left_over) / thread_count;
    logging::info("Star count: ", star_count);
    logging::info("Thread count: ", thread_count);
    logging::info("Star per thread: ", star_per_thread);
    logging::info("Left over: ", left_over);

    const int accelCycleCount = accelCycles; // number of accelCycleCount to run
    for (int loopCnt = 0; loopCnt < accelCycleCount; ++loopCnt) {
        auto starUpdateStartTime = std::chrono::high_resolution_clock::now();

#define MULTI_THREADED 1
#define OUTPUT_EVERY_N_TASKS 1000
// multi threading stuff
#if MULTI_THREADED


        auto tmp = std::vector<Star *>{};
        for (int i = 0; i < left_over; ++i) {
            tmp.emplace_back(star_list.at(0));
            star_list.erase(star_list.begin());
        }

        work_queue.clear();
        for (int i = 0; i < thread_count; ++i) {
            work_queue.emplace_back(std::vector<Star *>{});
            for (int j = 0; j < star_per_thread; ++j) {
                work_queue.at(i).emplace_back(star_list.at(i * star_per_thread + j));
            }
        }
        for (int i = 0; i < left_over; ++i) {
            work_queue.at(0).emplace_back(tmp.at(i));
        }

        for (int i = 0; i < left_over; ++i) {
            star_list.emplace_back(tmp.at(i));
        }
        auto threads = std::vector<std::thread>{};
//goto A;
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(std::thread([&work_queue, i]() {
                int tasksComplete = 0;
                int myTasks = work_queue.at(i).size();
                double averageTime = 0.f;
                for (auto star: work_queue.at(i)) {
                    auto timeTaken = star->acceleration_update_region_com(true);
                    timeTaken += star->acceleration_update_stars_in_region(false);

                    averageTime = (averageTime == -1 ? timeTaken : (averageTime + timeTaken) / 2);

                    ++tasksComplete;
                    if (tasksComplete % (OUTPUT_EVERY_N_TASKS + (i * OUTPUT_EVERY_N_TASKS)) == 0) {
                        auto progress = to_string(tasksComplete) + "/" + to_string(myTasks);
                        auto percent = to_string((int) ((float) tasksComplete / (float) myTasks * 100));

                        logging::info(
                                "[Thread " + to_string(i) + "] Acceleration Status - " + percent + "% [" + progress +
                                "] [" + to_string(averageTime) + "ms]", "", true, true);

                        if (averageTime > 10) {
                            logging::info("Average time is too high", "", true, true);
                        }
//                        std::cout << '\r' << "Acceleration Update Status - [Thread " + to_string(i) + "] " + percent + "% [" + progress + "], Average Time Taken: " +
                        //                                           to_string(averageTime) + "ms" << std::flush;
                    }
                }
            }));
        }
        A:
        for (auto &thread: threads) {
            thread.join();
        }
#endif

        // not multithreaded star updates
#ifndef MULTI_THREADED
        for (auto star : star_list) {
            auto accelerationDuration = star->acceleration_update_region_com(true);
            accelerationDuration += star->acceleration_update_stars_in_region(false);;

            if (averageAccelerationUpdateTime == -1) {
                averageAccelerationUpdateTime = accelerationDuration;
            } else {
                averageAccelerationUpdateTime = (averageAccelerationUpdateTime + accelerationDuration) / 2;
            }

            if (star.id % 100 == 0) {
                std::cout << star.id << " / " << star_list.size() << " Complete - Average Acceleration Update Time: " << averageAccelerationUpdateTime << "ms" << std::endl;
            }
        }
#endif
        // Clear the stars in the regions
        for (auto region: regionMatrix.regions) {
            region->stars_in_region.clear();
        }
        // updating star positions and velocities and adding them to the regions
        for (auto star: star_list) {
            if (!star->acceleration.x && !star->acceleration.y && !star->acceleration.z) {
                std::cout << "Acceleration is NaN for star " << star->id << std::endl;
                continue;
            }

            star->velocity_update(); // Update the stars veloctiy
            star->position_update(); // Update the stars position

            if (!star->position.notNull()) {
                logging::verbose("Star " + to_string(star->id) + " has position ", star->position);
            }
            // star->find_regions();
            for (Region *region: star->find_regions()) { // still makes a seg fault here
                // add star to region
                region->stars_in_region.emplace_back(star);
            }
        }

        for (auto region: regionMatrix.regions) {
            compute_region_com(region);
        }

        if (loopCnt % ((int)((loopCnt / accelCycleCount) * 10)) == 0) {
            std::cout << (loopCnt / accelCycleCount) * 100 << "% Complete - Stars" << std::endl;
        }

        auto starUpdateEndTime = std::chrono::high_resolution_clock::now();
        auto starUpdateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
                starUpdateStartTime - starUpdateEndTime).count();

        if (averageStarUpdateTime == -1) {
            averageStarUpdateTime = starUpdateDuration;
        } else {
            averageStarUpdateTime = (averageStarUpdateTime + starUpdateDuration) / 2;
        }

        logging::info("Finished an acceleration cycle: ", loopCnt + 1);

        logging::info("Re assigning stars to regions", "", true, false);
        min = Vector();
        max = Vector();
        for (auto star: star_list) {
            if (star->position.isNull()) {
                logging::verbose("Star " + to_string(star->id) + " has position ", star->position);
                continue;
            }
            if (star->position.x > max.x) {
                max.x = star->position.x;
            }
            if (star->position.y > max.y) {
                max.y = star->position.y;
            }
            if (star->position.z > max.z) {
                max.z = star->position.z;
            }
            if (star->position.x < min.x) {
                min.x = star->position.x;
            }
            if (star->position.y < min.y) {
                min.y = star->position.y;
            }
            if (star->position.z < min.z) {
                min.z = star->position.z;
            }
        }
        min = min * 1.01;
        max = max * 1.01;

        regionMatrix = RegionMatrix(
                Vector(min.x, min.y, min.z), // Start position
                Vector(max.x, max.y, max.z), // End position
                Vector(10, 10, 10)               // Amount of divisions on the z, y, z
        );

        averageStarRegionCount = 0;
        for (auto star: star_list) {
            for (Region *region: star->find_regions())
                region->stars_in_region.emplace_back(star);

            if (star->id % 5000 == 0) {
                averageStarRegionCount = averageStarRegionCount == 0 ? star->regions_we_are_in.size() :
                                         (averageStarRegionCount + star->regions_we_are_in.size()) / 2;

                auto progress = to_string(star->id) + "/" + to_string(star_list.size());
                auto percent = to_string((int) ((float) star->id / (float) star_list.size() * 100));
                auto message = "Progress " + percent + "% [" + progress + "] Stars, in an avrg of " +
                               to_string(averageStarRegionCount) + " regions";
                logging::verbose(message, "");
            }
        }
        // compute region coms
        for (Region *region: regionMatrix.regions) {
            compute_region_com(region);
        }
        logging::info("Finished re assigning stars to regions", "", true, false);
    }

    logging::info("Starting to dump data", "");
    {
        ofstream fileDump("2Stars.dump.txt");
        fileDump.precision(32);

        for (auto star: star_list) {
            int cycle_id = 1;
            for (auto history : star->history_position) {
                fileDump << star->id << ',' << cycle_id << ',' << history.x << ',' << history.y << ',' << history.z << std::endl;
                ++cycle_id;
            }
        }
        fileDump.close();
    }
    logging::info("Finished dumping data", "");

    return 0;
}

