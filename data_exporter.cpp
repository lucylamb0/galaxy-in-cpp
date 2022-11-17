// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
// Created by Conni Bilham on 07/11/2022.
//

#include "data_exporter.h"


void to_json(json& j, const Vector& v) {
    j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}
void to_json(json& j, const Star* star) {
    j = json{
            { "id", star->id },
            { "history_position", star->history_position },
            { "history_velocity", star->history_velocity },
            { "history_acceleration", star->history_acceleration }
    };
}
void from_json(const json& j, Vector& v) {
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
    j.at("z").get_to(v.z);
}

#define VEC(x) #x << "_x" << "," << #x << "_y" << "," << #x << "_z"

void data_exporter::dump_csv() {
    std::ofstream fileDump; //(file_path.c_str());
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
                            velocity.calc_energy(star->mass)
                    ))
                    << std::endl;
        }
    }
    fileDump.close();
}
