// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
// Created by Conni on 31/07/2022.
// With help from - https://github.com/danielkrupinski/Osiris/blob/640e00f34d4233dc6d74d9327463c50248acc1dd/Source/SDK/Vector.h
//

#ifndef C_VERSION_VECTOR3_H
#define C_VERSION_VECTOR3_H

#include <valarray>
#include <sstream>

struct Vector {
    Vector() = default;
    constexpr Vector(long double x, long double y, long double z) noexcept : x{ x }, y{ y }, z{ z } {}

    friend auto operator<<(std::ostream& os, Vector const& m) -> std::ostream& {
        return os << "(" << m.x << ", " << m.y << ", " << m.z << ")";
    }

    friend constexpr auto operator>(Vector const& a, Vector const& b) noexcept
    {
        return a.x > b.x && a.y > b.y && a.z > b.z;
    }

    friend constexpr auto operator<(Vector const& a, Vector const& b) noexcept
    {
        return a.x < b.x && a.y < b.y && a.z < b.z;
    }

    friend constexpr auto operator==(const Vector& a, const Vector& b) noexcept
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    friend constexpr auto operator!=(const Vector& a, const Vector& b) noexcept
    {
        return !(a == b);
    }

    constexpr Vector& operator=(const long double array[3]) noexcept
    {
        x = array[0];
        y = array[1];
        z = array[2];
        return *this;
    }

    constexpr Vector& operator+=(const Vector& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    constexpr Vector& operator+=(long double f) noexcept
    {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    constexpr Vector& operator-=(const Vector& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    constexpr Vector& operator-=(long double f) noexcept
    {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    friend constexpr auto operator-(const Vector& a, const Vector& b) noexcept
    {
        return Vector{ a.x - b.x, a.y - b.y, a.z - b.z };
    }

    friend constexpr auto operator+(const Vector& a, const Vector& b) noexcept
    {
        return Vector{ a.x + b.x, a.y + b.y, a.z + b.z };
    }

    friend constexpr auto operator*(const Vector& a, const Vector& b) noexcept
    {
        return Vector{ a.x * b.x, a.y * b.y, a.z * b.z };
    }

    constexpr Vector& operator/=(long double div) noexcept
    {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    constexpr Vector& operator/(int div) noexcept
    {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    constexpr Vector& operator/(long double div) noexcept
    {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    constexpr Vector& operator/(Vector v) noexcept
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    constexpr auto operator*(long double mul) const noexcept
    {
        return Vector{ x * mul, y * mul, z * mul };
    }

    constexpr auto operator-(long double sub) const noexcept
    {
        return Vector{ x - sub, y - sub, z - sub };
    }

    constexpr auto operator+(long double add) const noexcept
    {
        return Vector{ x + add, y + add, z + add };
    }

    Vector& normalize() noexcept
    {
        x = std::isfinite(x) ? std::remainder(x, 360.0f) : 0.0f;
        y = std::isfinite(y) ? std::remainder(y, 360.0f) : 0.0f;
        z = std::isfinite(z) ? std::remainder(z, 360.0f) : 0.0f;
        return *this;
    }

    auto length() const noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    constexpr auto squareLength() const noexcept
    {
        return (x * x) + (y * y) + (z * z);
    }

    constexpr auto size() const noexcept
    {
        return x * y * z;
    }

    constexpr auto dotProduct(const Vector& v) const noexcept
    {
        return x * v.x + y * v.y + z * v.z;
    }

//    constexpr auto transform(const matrix3x4& mat) const noexcept;

    auto distTo(const Vector& v) const noexcept
    {
        return (*this - v).length();
    }

    long double x, y, z;

    // rotate vector by angle (x angle = a, y angle = b, z angle = c)
    void rotate(long double a, long double b, long double c) {
        long double cos_a = std::cos(a);
        long double sin_a = std::sin(a);
        long double cos_b = std::cos(b);
        long double sin_b = std::sin(b);
        long double cos_c = std::cos(c);
        long double sin_c = std::sin(c);

        long double sina_sinb = sin_a * sin_b;
        long double cosa_sinb = cos_a * sin_b;

        this->x = (cos_b * cos_c) * this->x + (sina_sinb * cos_c - cos_a * sin_c) * this->y + (cosa_sinb * cos_c + sin_a * sin_c) * this->z;
        this->y = (cos_b * sin_c) * this->x + (sina_sinb * sin_c + cos_a * cos_c) * this->y + (cosa_sinb * sin_c - sin_a * cos_c) * this->z;
        this->z = (-sin_b) * this->x + (sin_a * cos_b) * this->y + (cos_a * cos_b) * this->z;
    }

    void normalise() {
        long double l = this->length();
        if (l != 0.0f) {
            this->x /= l;
            this->y /= l;
            this->z /= l;
        }
    }

    void scale(float d) {
        x *= d;
        y *= d;
        z *= d;
    }
};

#endif //C_VERSION_VECTOR3_H
