// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_STAR_TEST_CC
#define GALAXYSIMULATION_STAR_TEST_CC

#include <gtest/gtest.h>
#include "../Star.h"

TEST(Star, Create) {
    Star* star = new Star(
            1,
            Vectorr(1, 2, 3),
            Vectorr(4, 5, 6),
            Vectorr(7, 8, 9),
            10,
            nullptr,
            0
            );

    EXPECT_TRUE(star->id == 1);
    EXPECT_TRUE(star->position.x == 1);
    EXPECT_TRUE(star->position.y == 2);
    EXPECT_TRUE(star->position.z == 3);
    EXPECT_TRUE(star->velocity.x == 4);
    EXPECT_TRUE(star->velocity.y == 5);
    EXPECT_TRUE(star->velocity.z == 6);
    EXPECT_TRUE(star->acceleration.x == 7);
    EXPECT_TRUE(star->acceleration.y == 8);
    EXPECT_TRUE(star->acceleration.z == 9);
    EXPECT_TRUE(star->mass == 10);
    EXPECT_TRUE(star->parent == nullptr);
    EXPECT_FALSE(star->is_static());
    EXPECT_TRUE(star->regions_we_are_in.size() == 0);
}

TEST(Star, Consistant_HistoryRecords) {
    std::vector<Star*> *star_list = new std::vector<Star*>;
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
}
#endif //GALAXYSIMULATION_STAR_TEST_CC