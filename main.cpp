#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include "Vector3.h"
#include "Star.h"
#include "includes.h"
#include "Region.h"
#include <iostream>

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

#include <filesystem>
#include <thread>

void threadFunc(int threadID, std::vector<std::vector<Star*>> work_queue) {
    int tasksComplete = 0;
    auto myTasks = work_queue.at(threadID).size();
    double averageTime = -1.0;
    for (auto star : work_queue.at(threadID)) {
        auto timeTaken = star->acceleration_update(star_list);
        averageTime = averageTime == -1.0 ? timeTaken : (averageTime + timeTaken) / 2;
        ++tasksComplete;
        if(tasksComplete % (100 + (threadID * 100)) == 0) {
//            auto progress = to_string(tasksComplete) + "/" + to_string(myTasks);
//            auto percent = to_string((int)((float)tasksComplete / (float)myTasks * 100));
//            auto message = "[Thread " + to_string(threadID) + "] " + percent + "% [" + progress + "] Accel Updates, Average Time Taken: " +
//                    to_string(averageTime) + "ms";
//            l.verbose(message, "");

            std::cout << "Thread " << threadID << ": " << tasksComplete << "/" << myTasks << " - Average Time Taken: " << averageTime << "ms" << std::endl;
        }
    }
}

RegionMatrix regionMatrix;

int main(int arg_count, char** args) {
    std::string data_set_path; // = args[1];
    if(arg_count == 2) {
        data_set_path = args[1];
    }
    if(arg_count > 2) {
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
        );
    } else {
        regionMatrix = RegionMatrix(
                Vector(-1000000,-1000000,-1000000), // Start position
                Vector(1000000, 1000000, 1000000), // End position
                Vector(100, 100, 100)               // Amount of divisions on the z, y, z
        );
    }

    std::cout << "Hello, World!" << std::endl; // classic hello world of course <3
    logging::info("Hello, World! (FROM THE LOGGING FRAMEWORK)", "");

//    getPointsRegions(); /

    // return 0;

//    std::vector<Region> regions = regionMatrix.getRegions(point);
    logging::info("Regions: ", regionMatrix.regions.size());

//    std::cout << regionMatrix.
//    int outPointsXStartsAt =

//    Region region(Vector(-100, -100, -100), Vector(100, 100, 100)); // what the hell this doing? vibing i guess
//    if(region.contains(Vector(0, 0, 0))) {
//        std::cout << "Region contains origin" << std::endl;
//    } else {
//        std::cout << "Region does not contain origin" << std::endl;
//    }

    std::ifstream infile;
    infile.open(data_set_path);
    std::string line;

//    int cnt = 0;
// I think this is making the list of stars from the file
    while (std::getline(infile, line))
    {
//        ++cnt; if(cnt > 10000000) break;

        std::istringstream iss(line);
        auto split_str = split(line, ',');

        star_list.emplace_back( new Star(
                std::stoi(split_str.at(0)),     // ID
            Vector(std::stof(split_str.at(2)), std::stof(split_str.at(3)), std::stof(split_str.at(4))), // Position
            Vector(std::stof(split_str.at(5)), std::stof(split_str.at(6)), std::stof(split_str.at(7))), // Velocity
        Vector(0,0,0), // Acceleration
             1.0f,
 &regionMatrix // Parent region matrix
        )); // Mass
    }

    logging::info("Finished reading file", "");
    logging::info("Assigning regions.", "");
// converting data to meters (for now)
    static unsigned long averageStarRegionCount = 0;
    for (auto star : star_list) {
//        star->position = star->position * parsec;  // no longer need to convert to meters
//        star->velocity = star->velocity * parsec_per_year;

        // On star initialisation region all the stars
        for (int region_index : star->find_regions()) {
            Region* region = regionMatrix.regions.at(region_index);
            // add star to region
            region->stars_in_region.emplace_back(star->id);
//             std::cout << "Added star " << star->id << " to region " << region_index << std::endl; // for debugging TODO: remove this
//            std::cout << "Stars in region: [";
//            for (int star_in_region : region->stars_in_region) {
////                if( std::cout << "(" << region_index << ", " << star_in_region << ")";
//            }
        }
        if(star->id % 5000 == 0) {
            averageStarRegionCount = averageStarRegionCount == 0 ? star->regions_we_are_in.size() : (averageStarRegionCount + star->regions_we_are_in.size()) / 2;

            auto progress = to_string(star->id) + "/" + to_string(star_list.size());
            auto percent = to_string((int)((float)star->id / (float)star_list.size() * 100));
            auto message = "Progress " + percent + "% [" + progress + "] Stars, in an avrg of " +
                      to_string(averageStarRegionCount) + " regions";
            logging::verbose(message, "");
//            std::cout   << "[" << star->id << "] Finished assigning regions to " << 5000 << " stars out of " << star_list.size() << std::endl
//                        << "---- stars in an average of " << averageStarRegionCount << " regions" <<  std::endl;
        }
    }
    // After updating regions stars we need to ensure we update centre of mass and total mass of the regions
//    for(Region* region : regionMatrix.regions) {
//
//    }

    std::cout << "Finished assigning regions" << std::endl;
// don't know what the hell averageStarUpdateTime is, I think it is for keeping track of how long it takes
//    static long long averageAccelerationUpdateTime = -1;
    static long long averageStarUpdateTime = -1;

    int thread_count = std::thread::hardware_concurrency() - 1;

    thread_count /= 3;

    const int loops = 10000; // number of loops to run
    for (int loopCnt = 0; loopCnt < loops; ++loopCnt)
    {
        auto starUpdateStartTime = std::chrono::high_resolution_clock::now();

#define MULTI_THREADED 1
// multi threading stuff
#if MULTI_THREADED
        auto work_queue = std::vector<std::vector<Star*>>{};
        auto star_count = star_list.size();
        auto star_per_thread = star_count / thread_count;
        std::cout << "Star count: " << star_count << std::endl;
        std::cout << "Thread count: " << thread_count << std::endl;
        std::cout << "Star per thread: " << star_per_thread << std::endl;
        for (int i = 0; i < thread_count; ++i) {
            work_queue.emplace_back(std::vector<Star*>{});
            for (int j = 0; j < star_per_thread; ++j) {
                work_queue.at(i).emplace_back(star_list.at(i * star_per_thread + j));
            }
        }
        auto threads = std::vector<std::thread>{};
//        for (int threadID = 0; threadID < thread_count; ++threadID) {
//            threads.emplace_back(std::thread(threadFunc, threadID, work_queue));
//        }
//         don't know what this is
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(std::thread([&work_queue, i]()
            {
                int tasksComplete = 0;
                int myTasks = work_queue.at(i).size();
                double averageTime = -1;
                for (auto star : work_queue.at(i)) {
                    auto timeTaken = star->acceleration_update(star_list);
                    averageTime = averageTime == -1 ? timeTaken : (averageTime + timeTaken) / 2;
                    ++tasksComplete;
#define OUTPUT_EVERY_N_TASKS 100
                    if(tasksComplete % (OUTPUT_EVERY_N_TASKS + (i * OUTPUT_EVERY_N_TASKS)) == 0) {
                        auto progress = to_string(tasksComplete) + "/" + to_string(myTasks);
                        auto percent = to_string((int)((float)tasksComplete / (float)myTasks * 100));
                        logging::verbose("[Thread " + to_string(i) + "] " + percent + "% [" + progress + "] Accel Updates, Average Time Taken: " +
                                  to_string(averageTime) + "ms", "");
                    }
                }
            }));
        }

        for (auto &thread : threads) {
            thread.join();
        }
#endif
        // not multithreaded star updates
#ifndef MULTI_THREADED
        for (auto star : star_list) {
            auto accelerationDuration = star.acceleration_update(star_list);

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
        for (auto region : regionMatrix.regions) {
            region->stars_in_region.clear();
        }
        // updating star positions and velocities and adding them to the regions
        for (auto star : star_list) {
            star->velocity_update(); // Update the stars veloctiy
            star->position_update();

            // I believe this should be moved inside the acceleration updater.
//            star->find_regions(regionMatrix, playSpaceStart);
//            for (int region_index : star->find_regions(regionMatrix, playSpaceStart)) {
//                // add star to region
//
//                // TODO: use regions to speed up acceleration update
//                // TODO: Check we are not already within the region
//                regionMatrix.regions.at(region_index).stars_in_region.emplace_back(star->id);
//            }
        }
        if (loopCnt % 10 == 0) {
            std::cout << (loopCnt / loops) * 100 << "% Complete - Stars" << std::endl;
        }

        auto starUpdateEndTime = std::chrono::high_resolution_clock::now();
        auto starUpdateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(starUpdateStartTime-starUpdateEndTime).count();

        if (averageStarUpdateTime == -1) {
            averageStarUpdateTime = starUpdateDuration;
        } else {
            averageStarUpdateTime = (averageStarUpdateTime + starUpdateDuration) / 2;
        }
        std::cout << "Finished a stars acceleration update: " << (loopCnt + 1) << std::endl;
    }
    return 0;
}

