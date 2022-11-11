// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
//
// Created by Conni Bilham on 11/11/2022.
//

#ifndef GALAXYSIMULATION_DATAEXPORTER_TEST_CC
#define GALAXYSIMULATION_DATAEXPORTER_TEST_CC

#include <gtest/gtest.h>
#include "../data_exporter.h"

TEST(DataExporter_Test, Generic_Test) {
    std::vector<Star*> *star_list = new std::vector<Star*>;
    Star* testing_star = new Star(
            0,
            Vector(1, 2, 3),
            Vector(4, 5, 6),
            Vector(7, 8, 9),
            0, nullptr, 0
    );
    star_list->emplace_back(testing_star);

    testing_star->history_acceleration.emplace_back(Vector(1, 1, 1));
    testing_star->history_velocity.emplace_back(Vector(1, 1, 1));
    testing_star->history_position.emplace_back(Vector(1, 1, 1));

    bool tmp = testing_star->history_position.size() == testing_star->history_velocity.size();
    tmp = tmp ? testing_star->history_position.size() == testing_star->history_acceleration.size() : false;
    EXPECT_TRUE(tmp);

    data_exporter* exporter = new data_exporter(star_list, "dump_test.csv");
    exporter->start_dumping();
    EXPECT_TRUE(1);
}

#endif //GALAXYSIMULATION_DATAEXPORTER_TEST_CC