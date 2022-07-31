//
// Created by User on 31/07/2022.
//

#ifndef C_VERSION_VECTOR3_H
#define C_VERSION_VECTOR3_H

class Vector3 {
public:
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    double x, y, z;

    Vector3 operator*(Vector3 other) {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }
    Vector3 operator*(float other) {
        return {x * other, y * other, z * other};
    }
    Vector3 operator*(double other) {
        return {x * other, y * other, z * other};
    }
    Vector3 operator*(double other) const {
        return {x * other, y * other, z * other};
    }

    Vector3 operator+(Vector3 other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
};

#endif //C_VERSION_VECTOR3_H
