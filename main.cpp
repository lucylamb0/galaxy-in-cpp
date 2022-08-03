#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include "Vector3.h"
#include "Star.h"
#include "includes.h"
#include "Region.h"

std::vector<Star*> star_list = {};

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

Vector playSpaceStart = Vector(-10,-10,-10);
Vector playSpaceStop = Vector(10, 10, 10);
RegionMatrix regionMatrix = RegionMatrix();
void getPointsRegions(Vector point = {-1, 5, 1}) {
    std::cout << "point: " << point.x << ", " << point.y << ", " << point.z << std::endl;
    std::vector<int> regions_we_are_in = {};

    auto tmp = (point.x - playSpaceStart.x) / regionMatrix.step.x;
    auto tmp2 = std::floor(tmp);
    auto remainder = tmp - tmp2;
    int index = (tmp2) * regionMatrix.divisions.y * regionMatrix.divisions.z; //indexing works

    std::cout << "idnex: " << index << std::endl;

    int neighbour_above_x = -1;
    int neighbour_below_x = -1;
    int neighbour_above_y = -1;
    int neighbour_below_y = -1;
    int neighbour_above_z = -1;
    int neighbour_below_z = -1;
    int mode_neighbours = 1; // 1 = 1 region, 2 = 2 regions, 4 = 4 regions, 8 = 8 regions

    if(remainder <= regionMatrix.overlap_factor) { // need to check all 3 directions to see if we are in a neighbour region
        // We are overlapping the below region
        neighbour_below_x = index + (-1 * regionMatrix.divisions.y * regionMatrix.divisions.z);
//        if(regionMatrix.regions.at(neighbour_below).contains_x(point)) {
//            regions_we_are_in.push_back(neighbour_below);
//            std::cout << "We are definetly overlapping the below region: " << neighbour_below << std::endl;
//        } else {
//            std::cout << "We are not overlapping the below region: " << neighbour_below << std::endl;
//        }
    }
    else if (remainder >= 1 - regionMatrix.overlap_factor) {
        // We are overlapping the above region
        neighbour_above_x = index + (regionMatrix.divisions.y * regionMatrix.divisions.z);
//        if(regionMatrix.regions.at(neighbour_above).contains_x(point)) {
//            regions_we_are_in.push_back(neighbour_above);
//            std::cout << "We are definetly overlapping the above region: " << neighbour_above << std::endl;
//        } else {
//            std::cout << "We are not overlapping the above region: " << neighbour_above << std::endl;
//        }
    }

    tmp = (point.y - playSpaceStart.y) / regionMatrix.step.y;
    tmp2 = std::floor(tmp);
    remainder = tmp - tmp2;
    index += (tmp2) * regionMatrix.divisions.z;

    regions_we_are_in.push_back(index);

    neighbour_above = -1;
    neighbour_below = -1;

    if(remainder <= regionMatrix.overlap_factor) {
        // We are overlapping the below region
        neighbour_below = index + (-1 * regionMatrix.divisions.z);
//        if(regionMatrix.regions.at(neighbour_below).contains_y(point)) {
//            regions_we_are_in.push_back(neighbour_below);
//            std::cout << "We are definetly overlapping the below region: " << neighbour_below << std::endl;
//        } else {
//            std::cout << "We are not overlapping the below region: " << neighbour_below << std::endl;
//        }
    }
    else if (remainder >= 1 - regionMatrix.overlap_factor) {
        // We are overlapping the above region
        neighbour_above = index + (regionMatrix.divisions.z);
//        if(regionMatrix.regions.at(neighbour_above).contains_y(point)) {
//            regions_we_are_in.push_back(neighbour_above);
//            std::cout << "We are definetly overlapping the above region: " << neighbour_above << std::endl;
//        } else {
//            std::cout << "We are not overlapping the above region: " << neighbour_above << std::endl;
//        }
    }


    tmp = (point.z - playSpaceStart.z) / regionMatrix.step.z;
    tmp2 = std::floor(tmp);
    remainder = tmp - tmp2;
    index += (tmp2);


    neighbour_above = -1;
    neighbour_below = -1;

    if(remainder <= regionMatrix.overlap_factor) {
        // We are overlapping the below region
        neighbour_below = index + -1;
//        if(regionMatrix.regions.at(neighbour_below).contains_z(point)) {
//            regions_we_are_in.push_back(neighbour_below);
//            std::cout << "We are definetly overlapping the below region: " << neighbour_below << std::endl;
//        } else {
//            std::cout << "We are not overlapping the below region: " << neighbour_below << std::endl;
//        }
    }
    else if (remainder >= 1 - regionMatrix.overlap_factor) {
        // We are overlapping the above region
        neighbour_above = index + 1;
//        if(regionMatrix.regions.at(neighbour_above).contains_z(point)) {
//            regions_we_are_in.push_back(neighbour_above);
//            std::cout << "We are definetly overlapping the above region: " << neighbour_above << std::endl;
//        } else {
//            std::cout << "We are not overlapping the above region: " << neighbour_above << std::endl;
//        }
    }
    std::cout << index << std::endl;
    regions_we_are_in.push_back(index);

    int startingX = 0;
//    startingX

    int x = 0;
    for(auto region : regionMatrix.regions) {
        std::cout << x << ": " << region.min.x << " " << region.min.y << " " << region.min.z << " --- " << region.max.x << " " << region.max.y << " " << region.max.z << std::endl;
        ++x;
    }
    int p = 0;
    std::cout << "We are in the following regions: " << std::endl;
    for(auto region : regionMatrix.regions) {
        if(region.contains(point)){
            std::cout << "Point is def in region: " << p << std::endl;
        }
        ++p;
    }
    std::cout << "============================" << std::endl;

//    for(int index : regions_we_are_in) {
//        std::cout << "Overlapping region: " << index << std::endl;
//        if(regionMatrix.regions.at(index).contains(point)) {
//            std::cout << "Point is in region: " << index << std::endl;
//        }
//    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    regionMatrix = RegionMatrix(playSpaceStart, playSpaceStop, Vector(2, 2, 2));

    getPointsRegions();

    return 0;

//    std::vector<Region> regions = regionMatrix.getRegions(point);
    std::cout << "Regions: " << regionMatrix.regions.size() << std::endl;

    std::cout << regionMatrix.regions.size() << std::endl;

//    std::cout << regionMatrix.
//    int outPointsXStartsAt =

    Region region(Vector(-100, -100, -100), Vector(100, 100, 100));
    if(region.contains(Vector(0, 0, 0))) {
        std::cout << "Region contains origin" << std::endl;
    } else {
        std::cout << "Region does not contain origin" << std::endl;
    }

    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;
    std::ifstream infile(L"C:\\Users\\User\\Documents\\coding\\Star Stuff\\test\\C version\\star_data.csv");
    std::string line;

//    int cnt = 0;
    while (std::getline(infile, line))
    {
//        ++cnt; if(cnt > 10) break;

        std::istringstream iss(line);
        auto split_str = split(line, ',');

        star_list.emplace_back( new Star(
                std::stoi(split_str.at(0)),     // ID
    Vector(std::stof(split_str.at(2)), std::stof(split_str.at(3)), std::stof(split_str.at(4))), // Position
    Vector(std::stof(split_str.at(5)), std::stof(split_str.at(6)), std::stof(split_str.at(7))), // Velocity
    Vector(0,0,0))); // Acceleration

    }

    std::cout << "Finished reading file" << std::endl;

    for (auto star : star_list) {
        star->position = star->position * parsec;
        star->velocity = star->velocity * parsec_per_year;
        star->history_position.emplace_back(star->position.x, star->position.y, star->position.z);
    }

    std::cout << "Finished converting data to parsecs" << std::endl;

//    static long long averageAccelerationUpdateTime = -1;
    static long long averageStarUpdateTime = -1;

    const int loops = 10000;
    for (int i = 0; i < loops; ++i) {
        auto starUpdateStartTime = std::chrono::high_resolution_clock::now();

#define MULTI_THREADED 1

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
//        for (int i = 0; i < thread_count; ++i) {
//            threads.emplace_back(std::thread([&work_queue, i]() {
//                int tasksComplete = 0;
//                int myTasks = work_queue.at(i).size();
//                int averageTime = -1;
//                for (auto star : work_queue.at(i)) {
//                    auto timeTaken = star->acceleration_update(star_list);
//                    averageTime = averageTime == -1 ? timeTaken : (averageTime + timeTaken) / 2;
//                    ++tasksComplete;
//#define OUTPUT_EVERY_N_TASKS 100
//                    if(tasksComplete % (OUTPUT_EVERY_N_TASKS + (i * OUTPUT_EVERY_N_TASKS)) == 0) {
//                        std::cout << "Thread " << i << ": " << tasksComplete << "/" << myTasks << " - Average Time Taken: " << averageTime << "ms" << std::endl;
//                    }
//                }
//            }));
//        }

        for (auto &thread : threads) {
            thread.join();
        }
#endif
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

        for (auto star : star_list) {
            star->velocity_update();
            star->position_update();
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

