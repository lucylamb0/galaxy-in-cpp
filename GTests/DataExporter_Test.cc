// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
//
// Created by Conni Bilham on 11/11/2022.
//

#ifndef GALAXYSIMULATION_DATAEXPORTER_TEST_CC
#define GALAXYSIMULATION_DATAEXPORTER_TEST_CC

#include <gtest/gtest.h>
#include "../data_exporter.h"

TEST(DataExporter, Generic) {
    auto star_list = new std::vector<Star*>;
    Star* testing_star = new Star(
            0,
            Vectorr(1, 2, 3),
            Vectorr(4, 5, 6),
            Vectorr(7, 8, 9),
            0, nullptr, 0
    );
    star_list->emplace_back(testing_star);

    testing_star->history.emplace_back(
            Vectorr(1, 2, 3),
            Vectorr(4, 5, 6),
            Vectorr(7, 8, 9)
    );

    EXPECT_EQ(testing_star->history.size(), 2);

    GTEST_SKIP();

    auto exporter = new data_exporter(star_list, "dump_test.csv");
    exporter->start_dumping();
    EXPECT_TRUE(1);
}

#endif //GALAXYSIMULATION_DATAEXPORTER_TEST_CC