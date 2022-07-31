#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include "Vector3.h"
#include "Star.h"
#include "includes.h"

std::vector<Star> star_list = {};

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

int main() {
    std::cout << "Hello, World!" << std::endl;
    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;
    std::ifstream infile(L"C:\\Users\\User\\Documents\\coding\\Star Stuff\\test\\C version\\star_data.csv");
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        auto split_str = split(line, ',');

        star_list.emplace_back(
                std::stoi(split_str.at(0)),     // ID
    Vector3(std::stof(split_str.at(2)), std::stof(split_str.at(3)), std::stof(split_str.at(4))), // Position
    Vector3(std::stof(split_str.at(5)), std::stof(split_str.at(6)), std::stof(split_str.at(7))), // Velocity
    Vector3(0,0,0)); // Acceleration

    }

    std::cout << "Finished reading file" << std::endl;

    for (auto star : star_list) {
        star.position = star.position * parsec;
        star.velocity = star.velocity * parsec_per_year;
        star.history_position.emplace_back(star.position.x, star.position.y, star.position.z);
    }

    std::cout << "Finished converting data to parsecs" << std::endl;

    static long long averageAccelerationUpdateTime = -1;
    static long long averageStarUpdateTime = -1;

    const int loops = 10000;
    for (int i = 0; i < loops; ++i) {
        auto starUpdateStartTime = std::chrono::high_resolution_clock::now();

#define MULTI_THREADED 1

#if MULTI_THREADED
        auto work_queue = std::vector<std::vector<Star>>{};
        auto thread_count = std::thread::hardware_concurrency() - 1;
        auto star_count = star_list.size();
        auto star_per_thread = star_count / thread_count;
        std::cout << "Star count: " << star_count << std::endl;
        std::cout << "Thread count: " << thread_count << std::endl;
        std::cout << "Star per thread: " << star_per_thread << std::endl;
        for (int i = 0; i < thread_count; ++i) {
            work_queue.emplace_back(std::vector<Star>{});
            for (int j = 0; j < star_per_thread; ++j) {
                work_queue.at(i).emplace_back(star_list.at(i * star_per_thread + j));
            }
        }
        auto threads = std::vector<std::thread>{};
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(std::thread([&work_queue, i]() {
                int tasksComplete = 0;
                int myTasks = work_queue.at(i).size();
                int averageTime = -1;
                for (auto star : work_queue.at(i)) {
                    auto timeTaken = star.acceleration_update(star_list);
                    averageTime = averageTime == -1 ? timeTaken : (averageTime + timeTaken) / 2;
                    ++tasksComplete;
                    if(tasksComplete % (1000 + (i * 100)) == 0) {
                        std::cout << "Thread " << i << ": " << tasksComplete << "/" << myTasks << " - Average Time Taken: " << averageTime << "ms" << std::endl;
                    }
                }
            }));
        }
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
            star.velocity_update();
            star.position_update();
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

