// Copyright (c) Conni Bilham & Lucy Coward 2023, All Rights Reserved.
// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_SIMULATION_TEST_CC
#define GALAXYSIMULATION_SIMULATION_TEST_CC

#include <gtest/gtest.h>
#include "../Star.h"

#define VECTOR_EQ(ACT, EXP) EXPECT_FLOAT_EQ(ACT.x, EXP.x); EXPECT_FLOAT_EQ(ACT.y, EXP.y); EXPECT_FLOAT_EQ(ACT.y, EXP.y)

TEST(Simulation, Position_Update) {
    Star testStar = Star(
        2,                                  // ID
        Vectorr(0, 0, 0),                   // Position
        Vectorr(1, 0, 0),                   // Velocity
        Vectorr(0, 0, 0),                   // Acceleration
        0,
        nullptr,       // Parent region matrix
        0
    );
    logging::info("Starting Position: ", testStar.position);

    testStar.position = {0,0,0};
    testStar.velocity = {0,0,0};
    {
        logging::info("Testing some 0 values");
        logging::info("Velocity of xyz0 and a time step of  ");
        testStar.velocity = { 0,0,0};
        testStar.position_update(0);
        Vectorr expected = { 0, 0, 0 };
        VECTOR_EQ(testStar.position, expected);
        logging::info("==============================");
    }

    testStar.position = {0,0,0};
    {
        logging::info("Testing some non extreme ints");
        logging::info("Velocity of xyz1 and a time step of 3");
        testStar.velocity = { 1,1,1};
        testStar.position_update(3);
        Vectorr expected = { 3, 3, 3 };
        VECTOR_EQ(testStar.position, expected);
        logging::info("==============================");
    }

    testStar.position = {0,0,0};
    {
        logging::info("Testing some non extreme decimal numbers");
        logging::info("Velocity of xyz 0.2 and a time step of 0.5");
        testStar.velocity = { 0.2, 0.2, 0.2 };
        testStar.position_update(0.5);
        logging::info("Resulting pos: ", testStar.position);
        Vectorr expected = { 0.1, 0.1, 0.1 };
        VECTOR_EQ(testStar.position, expected);
        logging::info("========================================");
    }

    testStar.position = {0.0101,-2231,2.141};
    {
        logging::info("Testing some non extreme decimal numbers");
        logging::info("Velocity of xyz 0.2 and a time step of 0.5");
        testStar.velocity = { -0.1024, 1232, 2.141 };
        testStar.position_update(0.5);
        logging::info("Resulting pos: ", testStar.position);
        Vectorr expected = { -0.041099999, -1615, -1615 };
        VECTOR_EQ(testStar.position, expected);
        logging::info("========================================");
    }
}

#endif //GALAXYSIMULATION_SIMULATION_TEST_CC