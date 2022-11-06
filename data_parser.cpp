// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 06/11/2022.
//

#include "data_parser.h"

// Not really sure what this does, probably made by Conni
// Yes it was made by me, and it splits a string by a delimiter.
// Example:
// Input: Hello, world!   ,
// Output: ["Hello", "world!"]
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void data_parser::handle_csv(csv_data_template data) {
    std::ifstream infile;
    infile.open(data.file_path);
    std::string line;
    int stars_cnt = 0;
// I think this is making the list of stars from the file
    while (std::getline(infile, line)) {
        ++stars_cnt;

        std::istringstream iss(line);
        auto split_str = split(line, ',');

        int StarID = std::stoi(split_str.at(data.star_id));

        Vector position = !data.position_override ? Vector(
                std::stof(split_str.at(data.position.x)),
                std::stof(split_str.at(data.position.y)),
                std::stof(split_str.at(data.position.z))
        ) : data.position;

        Vector velocity = !data.velocity_override ? Vector(
                std::stof(split_str.at(data.velocity.x)),
                std::stof(split_str.at(data.velocity.y)),
                std::stof(split_str.at(data.velocity.z))
        ) : data.velocity;

        Vector acceleration = !data.acceleration_override ? Vector(
                std::stof(split_str.at(data.acceleration.x)),
                std::stof(split_str.at(data.acceleration.y)),
                std::stof(split_str.at(data.acceleration.z))
        ) : data.acceleration;

        star_list->emplace_back(new Star(
                StarID,     // ID
                position, // Position
                velocity, // Velocity
                acceleration, // Acceleration
                1.0f,
                data.regionMatrix_Parent // Parent region matrix
        )); // Mass
    }
    infile.close();

}
