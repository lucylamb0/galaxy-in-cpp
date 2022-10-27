// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 27/10/2022.
//

#ifndef GALAXYSIMULATION_CSV_PARSER_H
#define GALAXYSIMULATION_CSV_PARSER_H


#include <vector>
#include <string>
#include <fstream>
#include "Star.h"

class CSV_Parser {
public:
    static std::vector<Star*> parse_csv(RegionMatrix regionMatrix, std::string filename, bool ignore_first_line = true, char delimiter = ',');
};


#endif //GALAXYSIMULATION_CSV_PARSER_H
