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
    std::string file_path;

    void handle_csv();
    void dump_csv() {

        std::ofstream fileDump(file_path.c_str());
        fileDump.precision(32);

        // Output header
        fileDump << "Star ID, " << "Accel ID, "
                 << "X Position, " << "Y Position, " << "Z Position, "
                 << "X Velocity, " << "Y Velocity, " << "Z Velocity, "
                 << "X Accel, " << "Y Accel, " << "Z Accel,"
                 << "Total Energy 1, " << "Total Energy 2, " << "Total Energy 3"
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
                        << writeVector(Vector(
                                velocity.size(),
                                velocity.length(),
                                velocity.squareLength()
                                ))
                        << std::endl;
            }
        }
        fileDump.close();
    }

public:
    data_exporter(std::vector<Star*> *star_list, std::string file_path = "2Stars.test.dump.txt") {
        this->star_list = star_list;
        this->file_path = file_path;
    }

    void start_dumping() {
        logging::info("\n\nStarting to dump data", "");
        dump_csv();
        logging::info("Finished dumping data", "");
    }
};


#endif //GALAXYSIMULATION_DATA_EXPORTER_H
