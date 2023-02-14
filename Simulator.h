// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_SIMULATOR_H
#define GALAXYSIMULATION_SIMULATOR_H

#include "includes/logging.h"
#include "Star.h"

class Simulator {
public:
    int thread_count = -1;
    int star_per_thread = -1;
    int left_over = -1;
    std::vector<Star*> *star_list;

    std::vector<std::vector<Star *>> *work_queue = new std::vector<std::vector<Star *>>();

    Simulator(int thread_count, std::vector<Star*> *star_list) {
        this->thread_count = thread_count;
        this->star_list = star_list;
        this->left_over = star_list->size() % thread_count;
        this->star_per_thread = (star_list->size() - left_over) / thread_count;
    }

    void output_info();

    void generateWorkQueue();
};


#endif //GALAXYSIMULATION_SIMULATOR_H
