// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#include "data_exporter.h"

#define VEC(x) #x << "_x" << "," << #x << "_y" << "," << #x << "_z"


#define writeVector(VAR) VAR.x << ',' << (VAR).y << ',' << (VAR).z << ','
void data_exporter::csv_full_dump_Star(Star star) {
    if(!fileDump.is_open()) {
        logging::error("[ data_exporter::csv_full_dump_Star ] File is not open");
        return;
    }
    for (int i = 0; i < simulationFrames; ++i) {
        history_record_t history = star.history.at(i);
        auto position = history.position;
        auto velocity = history.velocity;
        auto acceleration = history.acceleration;

        fileDump
                << std::fixed << std::setprecision(16)
                << star.id << ',' << (i + 1) << ','
                << writeVector(position)
                << writeVector(velocity)
                << writeVector(acceleration)
                << writeVector(Vectorr(
                        velocity.size(),
                        velocity.length(),
                        star.kinetic_energy()
                ))
                << std::endl;
    }
}

void data_exporter::dump_csv() {
    fileDump.open(file_path.c_str());
    fileDump.precision(32);

    // Output header
    fileDump    << "Star ID,"
                << "Accel ID,"
                << VEC(position) << ","
                << VEC(velocity) << ","
                << VEC(acceleration) << ","
                << "Total Energy 1,"
                << "Total Energy 2,"
                << "Total Energy 3"
                << std::endl;

    for (auto star: *star_list) {
//        star->history_position.erase(star->history_position.begin());
        this->csv_full_dump_Star(*star);
    }
    fileDump.close();
}