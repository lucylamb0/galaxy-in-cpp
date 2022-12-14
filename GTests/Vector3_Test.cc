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

#define OUTPUT_ON_FAIL(condition, var) if(!(condition)) { std::cout << "Value: " << var << std::endl; EXPECT_TRUE(condition); }

TEST(Vector3_Test, SetVector_Test) {
    Vector testVector = Vector(1, 2, 3);
    EXPECT_EQ(testVector.x, 1);
    EXPECT_EQ(testVector.y, 2);
    EXPECT_EQ(testVector.z, 3);
    //    testVector.
}
TEST(Vector3_Test, Distance_Test) {
    Vector testVector = Vector(1, 2, 3);
    Vector testVector2 = Vector(4, 5, 6);
    float dist = testVector.distTo(testVector2);
    EXPECT_FLOAT_EQ(dist, 5.1961522);
}

TEST(Vector3_Test, Normalise_Test) {
    Vector testVector = Vector(1, 2, 3);
    testVector.normalise();
    EXPECT_FLOAT_EQ(testVector.x, 0.26726124);
    EXPECT_FLOAT_EQ(testVector.y, 0.53452247);
    EXPECT_FLOAT_EQ(testVector.z, 0.80178374);
}

TEST(Vector3_Test, Rotate_Test) {
    GTEST_SKIP_("Not yet implemented");
    Vector testVector = Vector(1, 1, 1);
    testVector.rotate_about_axis(testVector, PI);
    EXPECT_TRUE(testVector.x == 0);
    EXPECT_TRUE(testVector.y == 1);
}

#endif //GALAXYSIMULATION_VECTOR3_TEST_CC
