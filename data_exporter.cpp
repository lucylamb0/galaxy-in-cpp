// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
// Created by Conni Bilham on 07/11/2022.
//

#include "data_exporter.h"


void to_json(json& j, const Vectorr& v) {
    j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}
//void to_json(json& j, const Star* star) {
//    j = json{
//            { "id", star->id },
//            { "history_position", star->history_position },
//            { "history_velocity", star->history_velocity },
//            { "history_acceleration", star->history_acceleration }
//    };
//}
void to_json(json& j, const history_record_t history) {
    j = json{
            { "position", history.position },
            { "velocity", history.velocity },
            { "acceleration", history.acceleration }
    };
}
void to_json(json& j, const Star* star) {
    j = json{
            { "id", star->id },
            { "history", star->history }
    };
}
void from_json(const json& j, Vectorr& v) {
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
    j.at("z").get_to(v.z);
}

#define VEC(x) #x << "_x" << "," << #x << "_y" << "," << #x << "_z"


#define writeVector(VAR) VAR.x << ',' << (VAR).y << ',' << (VAR).z << ','
void data_exporter::csv_full_dump_Star(Star star) {
    if(!fileDump.is_open()) {
        logging::error("[ data_exporter::csv_full_dump_Star ] File is not open");
        return;
    }
    for (int i = 0; i < simulationFrames; ++i) {

//        Vectorr position = star.history_position.at(i);
//        Vectorr velocity = star.history_velocity.at(i);
//        Vectorr acceleration = star.history_acceleration.at(i);
        history_record_t history = star.history.at(i);
        auto position = history.position;
        auto velocity = history.velocity;
        auto acceleration = history.acceleration;

        fileDump
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