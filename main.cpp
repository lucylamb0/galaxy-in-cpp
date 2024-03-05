// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#include "ConfigHandler.h"
#include <string>
#include <chrono>
#include "includes/Vector3.h"
#include "Star.h"
#include "includes.h"
#include "includes/Region.h"
#include "includes/ConfigStruct.h"

#include "data_exporter.h"
//#include "data_parser.h"

#include "Simulator.h"

#include "includes/logging.h"

//#include "Plotting.h"

// TODO: CLEAN UP THE CODE, lots of commented out sections that i don't know if it is needed or not
// CODE USES PARSECS, SOLAR MASS, PC/YEAR and PC/YEAR^2
std::vector<Star*> star_list = {};
//data_exporter ExportHandler = data_exporter(&star_list, "Stars_gaussian.test.dump.txt");

// TODO: Maybe make smaller functions for this
// this function generates stars and puts them into the star list and class
// all velocities will in general go clockwise around the origin
// (for x,y plane) the pos x pos y quadrant velocity will be 0<=x, 0>=y
// (for x,y plane) the pos x neg y quadrant velocity will be 0>=x, 0>=y
// (for x,y plane) the neg x pos y quadrant velocity will be 0<=x, 0<=y
// (for x,y plane) the neg x neg y quadrant velocity will be 0>=x, 0<=y

RegionMatrix regionMatrix;

int main(int arg_count, char** args) {
    std::string pwd = std::filesystem::current_path().string();

    std::cout.precision(2);

    // Get size of file by path
//    std::ifstream in("/Users/connibilham/CLionProject/GalaxySimIMGUI/cmake-build-debug/GalaxySimulationUI", std::ifstream::ate | std::ifstream::binary);
//    std::ifstream in(pwd + "/GalaxySimulation", std::ifstream::app | std::ifstream::ate | std::ifstream::binary);
//    auto size = in.tellg();
//    in.close();
//    logging::info("Amount of floppy disks: ", size / 1440000.f);

    std::cout.precision(32);
//
    std::string default_path = pwd + "/config.json";

    std::cout << "Hello, World!" << std::endl;                          // classic hello world of course <3
    logging::info("Hello, World! (FROM THE LOGGING FRAMEWORK)", "");
    logging::info("Regions: ", regionMatrix.regions.size());
    logging::info("Overlap factor of: ", regionMatrix.overlap_factor);
    logging::info("Outputting log to: ", pwd + "/log.txt");
    logging::info("Default config path: ", default_path);

    // Get config path from args accounting for spaces in path
    std::string config_path = "";
    for (int i = 0; i < arg_count - 1; i++) {
        config_path += args[i];
        if (i != arg_count - 1) {
            config_path += " ";
        }
    }
    if (config_path == "") {
        config_path = default_path;
    }
    logging::info("Selected config [ " + config_path + " ]");

    ConfigStruct config = Config::loadConfig(config_path);

    regionMatrix = RegionMatrix(
      Vectorr { 0, 0, 0 },
      Vectorr { 400, 400, 10 },

      Vector_t<int>{ 4, 4, 1 },

      0.5f
    );

    {
//    ConfigStruct config;
//    auto c = &config.star_generation;
//    c->star_count = 100000;
//    c->stars_per_arm = 1000;
//    c->mean_mass = 10;
//    c->std_mass = 0.1;
//    c->min_position = Vectorr(-10, -10, -10);
//    c->max_position = Vectorr(10, 10, 10);
//    c->velocity_at_origin = Vectorr(0.000002556727896654, 0.000002556727896654, 0.00000002);
//    c->variation_velocity = Vectorr(0.00000001056727896654, 0.00000001056727896654, 0.00000000002);
//    c->variation_direction = Vectorr(0.0174533, 0.0174533);
//
//    c->number_of_arms = 0;
//    c->angle_of_arms = { };
//    c->arm_width = 0.1;
//    c->arm_length = 0.1;
//    c->arm_height = 0.1;
//    c->arm_offset = 0.1;
//
//    c->gaussian_mean = Vectorr(0, 0, 0);
//    c->gaussian_std = Vectorr(0.2, 0.2, 0.2);
    }


//    // Earth
//    star_list.emplace_back(new Star(
//            1,                                // ID
//            Vectorr(1, 1, 1),    // Position
////            Vectorr(-2e-8, 0, 0),    // Velocity
//            Vectorr(0, 0, 0), // Velocity
//            Vectorr(0, 0, 0), // Acceleration
//            5.97e+24,
//            &regionMatrix,       // Parent region matrix
//            0 //(int)STAR_FLAGS::STATIC
//    ));
//
//    // Moon
//    star_list.emplace_back(new Star(
//            2,                                // ID
//            Vectorr(0.4055e+6, 1, 1),    // Position
////            Vectorr(4.055e+5, 1, 1),    // Position
////            Vectorr( 1.02201e-6, 0, 0),    // Velocity
//            Vectorr( 1, 0, 0),    // Velocity
//            Vectorr(0, 0, 0), // Acceleration
//            7.34e+22 ,
//            &regionMatrix,       // Parent region matrix
//            0
//    ));

//    star_generator_gaussian(config.star_generation, &regionMatrix);

    // dump the star list to a csv file
//    data_exporter ExportHandler = data_exporter(&star_list);
//    ExportHandler.start_dumping("\"Stars.test.dump.txt\"");


    // Values to ensure NaN errors are not thrown
    Vectorr min = Vectorr(-1, -1, -1);
    Vectorr max = Vectorr(1, 1, 1);

    for (auto star: star_list)
        star->position.getBounds(min, max);

    // Scale the stars to fit in the region matrix
    // (this is done to ensure stars aren't directly on the edge of the region matrix)
    min.scale(config.region_matrix.regions_scale_min_PFit);
    max.scale(config.region_matrix.regions_scale_max_PFit);

    regionMatrix = RegionMatrix(min, max, config.region_matrix.region_divisions, config.region_matrix.overlap_factor);

    logging::info("Assigning regions.", "");

// converting data to meters (for now)

    static unsigned long averageStarRegionCount = 0;
    for (auto star : star_list) {
        star->find_regions();

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
    regionMatrix.computeRegionComs();

    logging::info("Finished assigning regions");
    static long long averageStarUpdateTime = -1;

    unsigned int thread_count = std::thread::hardware_concurrency() - 1;

    Simulator simulator = Simulator(thread_count, &star_list);
    simulator.output_info();

    auto work_queue = simulator.work_queue;
    for (int simFrame = 1; simFrame <= simulationFrames; ++simFrame) {
        auto starUpdateStartTime = std::chrono::high_resolution_clock::now();

        // Generate a work queue for the threads to handle
        simulator.generateWorkQueue();

        // begin threading
        auto threads = std::vector<std::thread>{};
        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back(std::thread([&work_queue, i]() {
                int tasksComplete = 0;
                int myTasks = work_queue->at(i).size();
                double averageTime = 0.f;

                for (auto star: work_queue->at(i)) {
                    auto timeTaken = star->acceleration_update_region_com(true);
                    timeTaken += star->acceleration_update_stars_in_region(false);

                    averageTime = (averageTime == -1 ? timeTaken : (averageTime + timeTaken) / 2);

                    ++tasksComplete;
                    if (tasksComplete % (10000 + (i * 1000)) == 0) {
                        auto progress = to_string(tasksComplete) + "/" + to_string(myTasks);
                        auto percent = to_string((int) ((float) tasksComplete / (float) myTasks * 100));

                        logging::info(
                                "[Thread " + to_string(i) + "] Acceleration Status - " + percent + "% [" + progress +
                                "] [" + to_string(averageTime) + "ms]", "", true, true);

                        if (averageTime > 1) {
                            logging::info("Average time is higher than a second", "", true, false);
                        }
//                        std::cout << '\r' << "Acceleration Update Status - [Thread " + to_string(i) + "] " + percent + "% [" + progress + "], Average Time Taken: " +
                        //                                           to_string(averageTime) + "ms" << std::flush;
                    }
                }
            }));
        }


        int thread_index = 0;
        for (auto &thread: threads) {
            thread.join();

            logging::verbose("Thread " + to_string(thread_index) + " joined");

            // Initiate live data dumps
            // ExportHandler.csv_full_dump_Star(work_queue->at(thread_index));
            ++thread_index;
        }


        // Clear the stars in the regions, so we can update the centre of masses and reassign the stars their regions
        regionMatrix.reset();

        // updating star positions and velocities and adding them to the regions
        for (auto star: star_list) {
            star->velocity_update(); // Update the stars velocity
            star->position_update(); // Update the stars position

            star->find_regions();
        }

        // compute region coms
        regionMatrix.computeRegionComs();

//        if (simFrame % 1 == 0) {
            auto progress = to_string(simFrame) + "/" + to_string(simulationFrames);
            auto percent = to_string((int) ((float) simFrame / (float) simulationFrames * 100));

            logging::info("\n [ Acceleration Cycle ] - " + percent + "% - " + progress, "", true, false);
//        }

        auto starUpdateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(starUpdateStartTime - std::chrono::high_resolution_clock::now()).count();

        averageStarUpdateTime = (averageStarUpdateTime == -1) ?
                starUpdateDuration : (averageStarUpdateTime + starUpdateDuration) / 2;


        logging::verbose("Finished an acceleration cycle: ", simFrame + 1);

        logging::verbose("Re assigning stars to regions");
        min = Vectorr(-1, -1, -1);
        max = Vectorr(1, 1, 1);
        for (auto star: star_list) {
            if (star->position.x > max.x) {
                max.x = star->position.x;
            } else if (star->position.x < min.x) {
                min.x = star->position.x;
            }
            if (star->position.y > max.y) {
                max.y = star->position.y;
            } else if (star->position.y < min.y) {
                min.y = star->position.y;
            }
            if (star->position.z > max.z) {
                max.z = star->position.z;
            } else if (star->position.z < min.z) {
                min.z = star->position.z;
            }
        }
        min = min * 1.01;
        max = max * 1.01;

        regionMatrix = RegionMatrix(
                Vectorr(min.x, min.y, min.z), // Start position
                Vectorr(max.x, max.y, max.z), // End position
                Vector_t<int>(10, 10, 10)               // Amount of divisions on the z, y, z
        );

        averageStarRegionCount = 0;
        for (auto star: star_list) {
            star->find_regions();

            averageStarRegionCount = averageStarRegionCount == 0 ? star->regions_we_are_in.size() :
                                     (averageStarRegionCount + star->regions_we_are_in.size()) / 2;

            if (star->id % 5000 == 0) {
                auto progress = to_string(star->id) + "/" + to_string(star_list.size());
                auto percent = to_string((int) ((float) star->id / (float) star_list.size() * 100));
                auto message = "Progress " + percent + "% [" + progress + "] Stars, in an avrg of " +
                               to_string(averageStarRegionCount) + " regions";
                logging::verbose(message);
            }
        }
        // compute region coms
        regionMatrix.computeRegionComs();

        logging::verbose("Finished re assigning stars to regions");
    }

    logging::info("Simulation Complete");

//    logging::info("Loading matplotlib");
//    Plotting c;
//    c.test(star_list);
//    ExportHandler.start_dumping();

    data_exporter ExportHandler2 = data_exporter(&star_list);
    ExportHandler2.start_dumping("NEW_DATA.csv");
    return 0;
}

