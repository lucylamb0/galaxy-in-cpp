// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 27/10/2022.
//

#include "CSV_Parser.h"
#include "logging.h"

#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include "Vector3.h"
#include "Star.h"
#include "includes.h"
#include "Region.h"
#include <iostream>

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

std::vector<Star*> CSV_Parser::parse_csv(RegionMatrix regionMatrix, std::string filename, bool ignore_first_line, char delimiter) {
    std::vector<Star*> star_list = {};

    std::ifstream infile;
    infile.open(filename);
    std::string line;

    int stars_cnt = 0;
    while (std::getline(infile, line)) {
        if (ignore_first_line) {
            ignore_first_line = false;
            continue;
        }
        ++stars_cnt;
        if (stars_cnt % 25000 == 0)
            break;

        std::istringstream iss(line);
        auto split_str = split(line, ',');

        star_list.emplace_back(new Star(
                std::stoi(split_str.at(0)),     // ID
                Vector(std::stof(split_str.at(2)), std::stof(split_str.at(3)), std::stof(split_str.at(4))), // Position
                Vector(std::stof(split_str.at(5)), std::stof(split_str.at(6)), std::stof(split_str.at(7))), // Velocity
                Vector(0, 0, 0), // Acceleration
                1.0f,
                &regionMatrix // Parent region matrix
        )); // Mass
    }
    infile.close();

    return star_list;
}
