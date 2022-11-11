// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
//
// Created by Conni Bilham on 10/11/2022.
//

#ifndef GALAXYSIMULATION_STAR_TEST_CC
#define GALAXYSIMULATION_STAR_TEST_CC

#include <gtest/gtest.h>
#include "../Star.h"

TEST(Star_Test, Create_Star_Test) {
    Star* star = new Star(
            1,
            Vector(1, 2, 3),
            Vector(4, 5, 6),
            Vector(7, 8, 9),
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

TEST(Star_Test, Consistant_HistoryRecords) {
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
}
#endif //GALAXYSIMULATION_STAR_TEST_CC