// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 07/11/2022.
//

#include "includes/logging.h"
#include "Star.h"

#ifndef GALAXYSIMULATION_DATA_EXPORTER_H
#define GALAXYSIMULATION_DATA_EXPORTER_H

class data_exporter {
private:
    std::vector<Star*> *star_list;

    void handle_csv();
    void dump_csv(const char *filename) {
//        const char *filename = "Stars.test.dump.txt";

        std::ofstream fileDump(filename);
        // output file location on system full location
        std::cout << "Output file location: " << filename << std::endl;

        fileDump.precision(32);

        // Output header
        fileDump << "Star ID, " << "Accel ID, "
                 << "X Position, " << "Y Position, " << "Z Position, "
                 << "X Velocity, " << "Y Velocity, " << "Z Velocity, "
                 << "X Accel, " << "Y Accel, " << "Z Accel"
                 << std::endl;

        for (auto star: *star_list) {
            star->history_position.erase(star->history_position.begin());

#define writeVector(VAR) VAR.x << ',' << (VAR).y << ',' << (VAR).z << ','
            for (int i = 0; i <= simulationFrames; ++i) {

                Vector position = star->history_position.at(i);
                Vector velocity = star->history_velocity.at(i);
                Vector acceleration = star->history_acceleration.at(i);

                fileDump
                        << star->id << ',' << (i + 1) << ','
                        << writeVector(position)
                        << writeVector(velocity)
                        << writeVector(acceleration)
                        << std::endl;
            }
        }
        fileDump.close();

    }

public:

    data_exporter(std::vector<Star*> *star_list) {
        this->star_list = star_list;
    }

    void start_dumping(const char *filename) {
        logging::info("\n\nStarting to dump data to: ", filename);
        dump_csv(filename);
        logging::info("Finished dumping data", "");
    }
} ;


#endif //GALAXYSIMULATION_DATA_EXPORTER_H
