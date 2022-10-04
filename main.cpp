#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include "Vector3.h"
#include "Star.h"
#include "includes.h"
#include "Region.h"
// TODO: CLEAN UP THE CODE, lots of commented out sections that i don't know if it is needed or not
// CODE USES PARSECS, SOLAR MASS, PC/YEAR and PC/YEAR^2
std::vector<Star*> star_list = {};
// Not really sure what this does, probably made by Conni
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
#include <io.h>
#include <thread>
// thread function
void threadFunc(int threadID, std::vector<std::vector<Star*>> work_queue) {
    int tasksComplete = 0;
    int myTasks = work_queue.at(threadID).size();
    double averageTime = -1.0;
    for (auto star : work_queue.at(threadID)) {
        auto timeTaken = star->acceleration_update(star_list);
        averageTime = averageTime == -1.0 ? timeTaken : (averageTime + timeTaken) / 2;
        ++tasksComplete;
        if(tasksComplete % (1000 + (threadID * 100)) == 0) {
            std::cout << "Thread " << threadID << ": " << tasksComplete << "/" << myTasks <<
                      " - Average Time Taken: " << averageTime << "ms" << std::endl;
        }
    }
}
// TODO: Put these variables somewhere else
Vector playSpaceStart = Vector(-1000000,-1000000,-1000000);
Vector playSpaceStop = Vector(1000000, 1000000, 1000000);
RegionMatrix regionMatrix = RegionMatrix();

// This function is for finding which region a star is in and if it overlaps with any other regions
//void getPointsRegions(Vector point) { // point is the star location fyi
//    std::cout << "point: " << point.x << ", " << point.y << ", " << point.z << std::endl;
//    std::vector<int> regions_we_are_in = {};
//    // weird math that works to find the index of the region the star is in
//    auto tmp = (point.x - playSpaceStart.x) / regionMatrix.step.x;
//    auto tmp2 = std::floor(tmp);
//    auto remainder = tmp - tmp2;
//    int index = (tmp2) * regionMatrix.divisions.y * regionMatrix.divisions.z; //indexing works
//
//    std::cout << "index: " << index << std::endl;
//
//    int neighbour_x = 0;
//    int neighbour_y = 0;
//    int neighbour_z = 0;
//    int mode_neighbours = 1; // 1 = 1 region, 2 = 2 regions, 4 = 4 regions, 8 = 8 regions
//
//    if(remainder <= regionMatrix.overlap_factor) { // need to check all 3 directions to see if we are in a neighbour region
//        // We are overlapping the below region
//        neighbour_x = -1;
//        mode_neighbours *= 2;
//    }
//    else if (remainder >= 1 - regionMatrix.overlap_factor) {
//        // We are overlapping the above region
//        neighbour_x = 1;
//        mode_neighbours *= 2;
//    }
//
//    tmp = (point.y - playSpaceStart.y) / regionMatrix.step.y;
//    tmp2 = std::floor(tmp);
//    remainder = tmp - tmp2;
//    index += (tmp2) * regionMatrix.divisions.z;
//
//
//    if(remainder <= regionMatrix.overlap_factor) {
//        // We are overlapping the below region
//        neighbour_y = -1;
//        mode_neighbours *= 2;
//    }
//    else if (remainder >= 1 - regionMatrix.overlap_factor) {
//        // We are overlapping the above region
//        neighbour_y = 1;
//        mode_neighbours *= 2;
//    }
//
//// TODO: Will need to check weather the type of int/float work correctly when point and playspace are large
//    tmp = (point.z - playSpaceStart.z) / regionMatrix.step.z;
//    tmp2 = std::floor(tmp);
//    remainder = tmp - tmp2;
//    index += (tmp2);
//
//    if(remainder <= regionMatrix.overlap_factor) {
//        // We are overlapping the below region
//        neighbour_z = -1;
//        mode_neighbours *= 2;
//    }
//    else if (remainder >= 1 - regionMatrix.overlap_factor) {
//        // We are overlapping the above region
//        neighbour_z = 1;
//        mode_neighbours *= 2;
//    }
//
//
////    std::list<int> neighbour_list = (neighbour_x, neighbour_y, neighbour_z);
////    Vector neighbour_vector = Vector(neighbour_x,neighbour_y,neighbour_z);
//
//
//// TODO: Remove std::couts after testing
//    if (mode_neighbours == 1) {
//        std::cout << "No overlapping regions found. Index of box we are in is: " << index << std::endl;
//        regions_we_are_in.push_back(index);
//    }
//    if (mode_neighbours == 2) {
//        std::cout << "We are in 2 regions. Index of box we are in is: " << index << std::endl;
//        tmp = index;
//
//        tmp += (neighbour_x * regionMatrix.divisions.y * regionMatrix.divisions.z);
//        tmp += (neighbour_y * regionMatrix.divisions.z);
//        tmp += neighbour_z;
//
//        std::cout << "The index of the neighbouring region is: " << tmp << std::endl;
//        regions_we_are_in.emplace_back(index);
//        regions_we_are_in.emplace_back(tmp);
//    }
//    if (mode_neighbours == 4) {
//        std::cout << "We are in 4 regions. Index of box we are in is: " << index << std::endl;
//        tmp = index;
//
//        tmp += (neighbour_x * regionMatrix.divisions.y * regionMatrix.divisions.z);
//        tmp += (neighbour_y * regionMatrix.divisions.z);
//        tmp += neighbour_z;
//
//        regions_we_are_in.emplace_back(index);
//        regions_we_are_in.emplace_back(tmp);
//        std::cout << "The index of the corner neighbouring regions is: " << tmp << std::endl;
//
//        if (neighbour_x != 0) {
//            tmp = (index + (neighbour_x * regionMatrix.divisions.y * regionMatrix.divisions.z));
//            regions_we_are_in.emplace_back(tmp);
//            std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
//        }
//        if (neighbour_y != 0) {
//            tmp = (index + (neighbour_y * regionMatrix.divisions.z));
//            regions_we_are_in.emplace_back(tmp);
//            std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
//        }
//        if (neighbour_z != 0) {
//            tmp = (index + neighbour_z);
//            regions_we_are_in.emplace_back(tmp);
//            std::cout << "The index of one of the neighbouring regions is: " << tmp << std::endl;
//        }
//    }
//    if (mode_neighbours == 8) {
//        std::cout << "We are in 8 regions. Index of box we are in is: " << index << std::endl;
//        tmp = index;
//
//        tmp += (neighbour_x * regionMatrix.divisions.y * regionMatrix.divisions.z) + (neighbour_y * regionMatrix.divisions.z) + neighbour_z;
//        regions_we_are_in.emplace_back(index);
//        regions_we_are_in.emplace_back(tmp);
//        std::cout << "The index of the corner neighbouring regions is: " << tmp << std::endl;
//
//        tmp = index + (neighbour_x * regionMatrix.divisions.y * regionMatrix.divisions.z);
//        regions_we_are_in.emplace_back(tmp);
//        std::cout << "(X) The index of one of the neighbouring regions is: " << tmp << std::endl;
//
//        tmp = index + (neighbour_y * regionMatrix.divisions.z);
//        regions_we_are_in.emplace_back(tmp);
//        std::cout << "(Y) The index of one of the neighbouring regions is: " << tmp << std::endl;
//
//        tmp = index + neighbour_z;
//        regions_we_are_in.emplace_back(tmp);
//        std::cout << "(Z) The index of one of the neighbouring regions is: " << tmp << std::endl;
//
//        tmp = index + (neighbour_x * regionMatrix.divisions.y * regionMatrix.divisions.z) + (neighbour_y * regionMatrix.divisions.z);
//        regions_we_are_in.emplace_back(tmp);
//        std::cout << "(XY) The index of one of the neighbouring regions is: " << tmp << std::endl;
//
//        tmp = index + (neighbour_x * regionMatrix.divisions.y * regionMatrix.divisions.z) + neighbour_z;
//        regions_we_are_in.emplace_back(tmp);
//        std::cout << "(XZ) The index of one of the neighbouring regions is: " << tmp << std::endl;
//
//        tmp = index + (neighbour_y * regionMatrix.divisions.z) + neighbour_z;
//        regions_we_are_in.emplace_back(tmp);
//        std::cout << "(YZ) The index of one of the neighbouring regions is: " << tmp << std::endl;
//
//    }
//



//    int startingX = 0;
////    startingX ?
//// code for finding the region we are in i think the slow way? maybe? perchance?
//    int x = 0;
//    for(auto region : regionMatrix.regions) {
//        std::cout << x << ": " << region.min.x << " " << region.min.y << " " << region.min.z << " --- " << region.max.x << " " << region.max.y << " " << region.max.z << std::endl;
//        ++x;
//    }
//    int p = 0;
//    std::cout << "We are in the following regions: " << std::endl;
//    for(auto region : regionMatrix.regions) {
//        if(region.contains(point)){
//            std::cout << "Point is def in region: " << p << std::endl;
//        }
//        ++p;
//    }
//    std::cout << "============================" << std::endl;
//// code for finding regions that point is in slow way
////    for(int index : regions_we_are_in) {
////        std::cout << "Overlapping region: " << index << std::endl;
////        if(regionMatrix.regions.at(index).contains(point)) {
////            std::cout << "Point is in region: " << index << std::endl;
////        }
////    }
//}


int main() {
    // for specifing how many regions (will be cubed)
    long int no_of_region_div = 100;
    std::cout << "Hello, World!" << std::endl; // classic hello world of course
    // making region matrixies
    regionMatrix = RegionMatrix(playSpaceStart, playSpaceStop, Vector(no_of_region_div, no_of_region_div, no_of_region_div));

//    getPointsRegions(); // TODO: remove this and the above region matrix to work more sustainably with the rest of the code now that we have the regioning system working

    // return 0;

//    std::vector<Region> regions = regionMatrix.getRegions(point);
    std::cout << "Regions: " << regionMatrix.regions.size() << std::endl;

    std::cout << regionMatrix.regions.size() << std::endl;

//    std::cout << regionMatrix.
//    int outPointsXStartsAt =

    Region region(Vector(-100, -100, -100), Vector(100, 100, 100)); // what the hell this doing? vibing i guess
    if(region.contains(Vector(0, 0, 0))) {
        std::cout << "Region contains origin" << std::endl;
    } else {
        std::cout << "Region does not contain origin" << std::endl;
    }

    // for reading in data
    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;
    std::ifstream infile(L"D:\\JET BRAINS\\galaxy-in-cpp\\star_data.csv");
    std::string line;

//    int cnt = 0;
// I think this is making the list of stars from the file
    while (std::getline(infile, line))
    {
//        ++cnt; if(cnt > 10) break;

        std::istringstream iss(line);
        auto split_str = split(line, ',');

        star_list.emplace_back( new Star(
                std::stoi(split_str.at(0)),     // ID
    Vector(std::stof(split_str.at(2)), std::stof(split_str.at(3)), std::stof(split_str.at(4))), // Position
    Vector(std::stof(split_str.at(5)), std::stof(split_str.at(6)), std::stof(split_str.at(7))), // Velocity
    Vector(0,0,0), // Acceleration
    1.0f)); // Mass

    }

    std::cout << "Finished reading file" << std::endl;
// converting data to meters (for now)
    for (auto star : star_list) {
//        star->position = star->position * parsec;  // no longer need to convert to meters
//        star->velocity = star->velocity * parsec_per_year;
        star->history_position.emplace_back(star->position.x, star->position.y, star->position.z);
        for (int region_index : star->find_regions(regionMatrix, playSpaceStart)) {
            // add star to region
            regionMatrix.regions.at(region_index).stars_in_region.emplace_back(star->id);
            std::cout << "Added star " << star->id << " to region " << region_index << std::endl; // for debugging TODO: remove this
            for (int star_in_region : regionMatrix.regions.at(region_index).stars_in_region) {
                std::cout << "Stars in region: "<< region_index << ":" << star_in_region << std::endl;
            }
        }
    }

    std::cout << "Finished converting data from parsecs" << std::endl;
// don't know what the hell averageStarUpdateTime is, I think it is for keeping track of how long it takes
//    static long long averageAccelerationUpdateTime = -1;
    static long long averageStarUpdateTime = -1;

    const int loops = 10000; // number of loops to run
    for (int i = 0; i < loops; ++i)
    {
        auto starUpdateStartTime = std::chrono::high_resolution_clock::now();

#define MULTI_THREADED 1
// multi threading stuff
#if MULTI_THREADED
        auto work_queue = std::vector<std::vector<Star*>>{};
        auto thread_count = std::thread::hardware_concurrency() - 1;
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
        for (int threadID = 0; threadID < thread_count; ++threadID) {
            threads.emplace_back(std::thread(threadFunc, threadID, work_queue));
        }
//         don't know what this is
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(std::thread([&work_queue, i]() {
                int tasksComplete = 0;
                int myTasks = work_queue.at(i).size();
                int averageTime = -1;
                for (auto star : work_queue.at(i)) {
                    auto timeTaken = star->acceleration_update(star_list);
                    averageTime = averageTime == -1 ? timeTaken : (averageTime + timeTaken) / 2;
                    ++tasksComplete;
#define OUTPUT_EVERY_N_TASKS 100
                    if(tasksComplete % (OUTPUT_EVERY_N_TASKS + (i * OUTPUT_EVERY_N_TASKS)) == 0) {
                        std::cout << "Thread " << i << ": " << tasksComplete << "/" << myTasks << " - Average Time Taken: " << averageTime << "ms" << std::endl;
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
            region.stars_in_region.clear();
        }
        // updating star positions and velocities and adding them to the regions
        for (auto star : star_list) {
            star->velocity_update();
            star->position_update();
            star->find_regions(regionMatrix, playSpaceStart);
            for (int region_index : star->find_regions(regionMatrix, playSpaceStart)) {
                // add star to region
                regionMatrix.regions.at(region_index).stars_in_region.emplace_back(star->id); // TODO: use regions to speed up acceleration update
            }
        }
        if (i % 10 == 0) {
            std::cout << (i / loops) * 100 << "% Complete - Stars" << std::endl;
        }

        auto starUpdateEndTime = std::chrono::high_resolution_clock::now();
        auto starUpdateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(starUpdateStartTime-starUpdateEndTime).count();

        if (averageStarUpdateTime == -1) {
            averageStarUpdateTime = starUpdateDuration;
        } else {
            averageStarUpdateTime = (averageStarUpdateTime + starUpdateDuration) / 2;
        }
        std::cout << "Finished a stars acceleration update: " << i << std::endl;
    }
    return 0;
}

