// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
//
// Created by Conni Bilham on 10/11/2022.
//

#ifndef GALAXYSIMULATION_VECTOR3_TEST_CC
#define GALAXYSIMULATION_VECTOR3_TEST_CC

#define PI 3.14159265

#include <gtest/gtest.h>
#include "../includes/Vector3.h"

TEST(Vector3_Test, SetVector_Test) {
    Vector testVector = Vector(1, 2, 3);
    EXPECT_TRUE(testVector.x == 1);
    EXPECT_TRUE(testVector.y == 2);
    EXPECT_TRUE(testVector.z == 3);
//    testVector.
}
TEST(Vector3_Test, Distance_Test) {
    Vector testVector = Vector(1, 2, 3);
    Vector testVector2 = Vector(4, 5, 6);
    EXPECT_TRUE(testVector.distTo(testVector2) == 5.196152422706632);
}

TEST(Vector3_Test, Normalise_Test) {
    Vector testVector = Vector(1, 2, 3);
    testVector.normalise();
    EXPECT_TRUE(testVector.x == 0.2672612419124244);
    EXPECT_TRUE(testVector.y == 0.5345224838248488);
    EXPECT_TRUE(testVector.z == 0.8017837257372732);
}

TEST(Vector3_Test, Rotate_Test) {
    GTEST_SKIP_("Not yet implemented");
    Vector testVector = Vector(1, 1, 1);
    testVector.rotate_about_axis(testVector, PI);
    EXPECT_TRUE(testVector.x == 0);
    EXPECT_TRUE(testVector.y == 1);
}

#endif //GALAXYSIMULATION_VECTOR3_TEST_CC
