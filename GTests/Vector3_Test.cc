// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.

#ifndef GALAXYSIMULATION_VECTOR3_TEST_CC
#define GALAXYSIMULATION_VECTOR3_TEST_CC

#define PI 3.14159265

#include <gtest/gtest.h>
#include "../includes/Vector3.h"

#define OUTPUT_ON_FAIL(condition, var) if(!(condition)) { std::cout << "Value: " << var << std::endl; EXPECT_TRUE(condition); }

TEST(Vector3, SetVector) {
    Vectorr testVector = Vectorr(1, 2, 3);
    EXPECT_EQ(testVector.x, 1);
    EXPECT_EQ(testVector.y, 2);
    EXPECT_EQ(testVector.z, 3);
    //    testVector.
}
TEST(Vector3, Distance) {
    Vectorr testVector = Vectorr(1, 2, 3);
    Vectorr testVector2 = Vectorr(4, 5, 6);
    float dist = testVector.distTo(testVector2);
    EXPECT_FLOAT_EQ(dist, 5.1961522);
}

TEST(Vector3, Normalise) {
    Vectorr testVector = Vectorr(1, 2, 3);
    testVector.normalise();
    EXPECT_FLOAT_EQ(testVector.x, 0.26726124);
    EXPECT_FLOAT_EQ(testVector.y, 0.53452247);
    EXPECT_FLOAT_EQ(testVector.z, 0.80178374);
}

TEST(Vector3, Rotate_Y) {
    Vectorr testVector = Vectorr(0, 1, 0);
    testVector.rotate(0, PI, 0);
    EXPECT_EQ(testVector, Vectorr(0, 1, 0));
}

TEST(Vector3, doubleMath) {
    Vectorr testVector = Vectorr(1, 2, 3);
    testVector = testVector * 2;
    EXPECT_EQ(testVector, Vectorr(2, 4, 6));
    testVector = testVector / 2;
    EXPECT_EQ(testVector, Vectorr(1, 2, 3));
    testVector = testVector + 2;
    EXPECT_EQ(testVector, Vectorr(3, 4, 5));
    testVector = testVector - 2;
    EXPECT_EQ(testVector, Vectorr(1, 2, 3));
}

TEST(Vector3, VectorMath) {
    Vectorr testVector = Vectorr(1, 2, 3);
    Vectorr testVector2 = Vectorr(4, 5, 6);
    testVector = testVector * testVector2;
    EXPECT_EQ(testVector, Vectorr(4, 10, 18));
    testVector = testVector / testVector2;
    EXPECT_EQ(testVector, Vectorr(1, 2, 3));
    testVector = testVector + testVector2;
    EXPECT_EQ(testVector, Vectorr(5, 7, 9));
    testVector = testVector - testVector2;
    EXPECT_EQ(testVector, Vectorr(1, 2, 3));
}

TEST(Vector3, VectorMath_MagSq_DotProd_Scale) {
    Vectorr testVector = Vectorr(1, 2, 3);
    EXPECT_EQ((int)testVector.magnitude_squared(), 14);

    testVector.scale(2);
    EXPECT_EQ(testVector, Vectorr(2, 4, 6));
    EXPECT_EQ(testVector.dotProduct(Vectorr(4, 5, 6)), 64);
}



#endif //GALAXYSIMULATION_VECTOR3_TEST_CC
