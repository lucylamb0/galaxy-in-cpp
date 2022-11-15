// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 07/11/2022.
//

#include "data_exporter.h"
#include "includes/json.h"

using json = nlohmann::json;

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