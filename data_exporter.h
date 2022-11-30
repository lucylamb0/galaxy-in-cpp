// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
// Created by Conni Bilham on 07/11/2022.
//

#include "Star.h"
#include "includes/logging.h"
#include "includes/json.h"
using json = nlohmann::json;

#ifndef GALAXYSIMULATION_DATA_EXPORTER_H
#define GALAXYSIMULATION_DATA_EXPORTER_H

class data_exporter {
private:
    std::vector<Star*> *star_list;
    std::string file_path;
    std::ofstream fileDump;

    void dump_csv();

public:
    data_exporter(std::vector<Star*> *star_list, std::string file_path = "2Stars.test.dump.txt") {
        this->star_list = star_list;
        this->file_path = file_path;
    }

    void full_dump();
    void setup_live_dump();

    void csv_full_dump_Star(Star star);

    void start_dumping() {
        logging::info("\n\nStarting to dump data");
        dump_csv();
        logging::info("Finished dumping data");
    }
};


#endif //GALAXYSIMULATION_DATA_EXPORTER_H
