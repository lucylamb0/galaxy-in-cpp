// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#include <vector>
#include "includes.h"
#include "Star.h"

#ifndef GALAXYSIMULATION_DATA_PARSER_H
#define GALAXYSIMULATION_DATA_PARSER_H


struct data_template {
public:
    const char* file_path;
};

// TODO: Implement json data template
struct json_data_template : data_template {};
struct csv_data_template : data_template {
    int star_id;

    RegionMatrix* regionMatrix_Parent;

    bool position_override = false;
    Vectorr position;

    bool velocity_override = false;
    Vectorr velocity;

    bool acceleration_override = false;
    Vectorr acceleration;
};

class data_parser {
private:
    void handle_csv(csv_data_template data);

public:
    std::vector<Star*> *star_list;

    data_parser(std::vector<Star*> *star_list, const char* file_name) {
        this->star_list = star_list;
    }
};


#endif //GALAXYSIMULATION_DATA_PARSER_H
