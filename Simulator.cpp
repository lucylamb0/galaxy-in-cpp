// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

//
// Created by Conni Bilham on 14/11/2022.
//

#include "Simulator.h"

void Simulator::generateWorkQueue() {
    auto left_over = star_list->size() % thread_count;
    auto overflowList = new std::vector<Star*>();

    // Remove the overflowing stars from the star_list and place them in a temp array
    //      This should never have more than thread_count stars within it
    for(int index = 0; index < left_over; ++index) {
        overflowList->emplace_back(star_list->at(0));
        star_list->erase(star_list->begin());
    }

    // Populate the work queue with the stars
    work_queue->clear();
    for(int thread_queue_index = 0; thread_queue_index < thread_count; ++thread_queue_index) {
        work_queue->emplace_back(*star_list);
        for (int star_index = 0; star_index < star_per_thread; ++star_index) {
            int final_star_index = thread_queue_index * star_per_thread + star_index;
            work_queue->at(thread_queue_index).emplace_back(star_list->at(final_star_index));
        }
    }

    // Add the overflow stars to the work queue
    for(int index = 0; index < left_over; ++index) {
        work_queue->at(0).emplace_back(overflowList->at(index));
    }

    // Add the overflow stars back to the main star_list
    for(int index = 0; index < left_over; ++index) {
        star_list->emplace_back(overflowList->at(index));
    }
}

void multithreadedWorkHandler() {

}

void singlethreadedWorkHandler() {

}

void Simulator::output_info() {
    logging::info("---------------------------");
    logging::info("Thread count: ", std::to_string(thread_count));
    logging::info("Star count: ", star_list->size());
    logging::info("Stars per thread: ", std::to_string(star_per_thread));
    if(left_over > 0) {
        logging::info("--- Moving left over stars into thread 0 ---");
        logging::info("Left over stars: ", std::to_string(left_over));
    }
    logging::info("---------------------------");
}
