// Copyright (c) Conni Bilham & Lucy Coward 2022, All Rights Reserved.
//
// Created by Conni on 31/07/2022.
// With help from - https://github.com/danielkrupinski/Osiris/blob/640e00f34d4233dc6d74d9327463c50248acc1dd/Source/SDK/Vector.h
//

#ifndef C_VERSION_VECTOR3_H
#define C_VERSION_VECTOR3_H

#include <valarray>
#include <sstream>

#include "json.h"
using json = nlohmann::json;

#define Vectorr Vector_t<long double>
template<typename TYPE>
struct Vector_t {
    Vector_t() = default;
    constexpr Vector_t(TYPE x, TYPE y) noexcept : x{ x }, y{ y }, z( 0 ) {}

    constexpr Vector_t(TYPE x, TYPE y, TYPE z) noexcept : x{ x }, y{ y }, z{ z } {}

    constexpr Vector_t(TYPE x, TYPE y, TYPE z, float scale_factor) noexcept : x{ x }, y{ y }, z{ z } {
        if(scale_factor != 0)
            this->scale(scale_factor);
    }

    void getBounds(Vector_t<TYPE> &min, Vector_t<TYPE> &max) {
        if(x > max.x) max.x = x;
        else if (x < min.x) min.x = x;
        if(y > max.y) max.y = y;
        else if (y < min.y) min.y = y;
        if(z > max.z) max.z = z;
        else if (z < min.z) min.z = z;
    }

    friend auto operator<<(std::ostream& os, Vector_t const& m) -> std::ostream& {
        return os << "(" << m.x << ", " << m.y << ", " << m.z << ")";
    }

    friend constexpr auto operator>(Vector_t const& a, Vector_t const& b) noexcept -> bool {
        return a.x > b.x && a.y > b.y && a.z > b.z;
    }

    friend constexpr auto operator<(Vector_t const& a, Vector_t const& b) noexcept -> bool {
        return a.x < b.x && a.y < b.y && a.z < b.z;
    }

    friend constexpr auto operator>=(Vector_t const& a, Vector_t const& b) noexcept -> bool {
        return a.x >= b.x && a.y >= b.y && a.z >= b.z;
    }

    friend constexpr auto operator<=(Vector_t const& a, Vector_t const& b) noexcept -> bool {
        return a.x <= b.x && a.y <= b.y && a.z <= b.z;
    }

    friend constexpr auto operator==(const Vector_t& a, const Vector_t& b) noexcept
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    friend constexpr auto operator!=(const Vector_t& a, const Vector_t& b) noexcept {
        return !(a == b);
    }

    constexpr Vector_t& operator=(const TYPE array[3]) noexcept {
        x = array[0];
        y = array[1];
        z = array[2];
        return *this;
    }

    constexpr Vector_t& operator+=(const Vector_t& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    constexpr Vector_t& operator+=(TYPE f) noexcept {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    constexpr Vector_t& operator-=(const Vector_t& v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    constexpr Vector_t& operator-=(TYPE f) noexcept {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    friend constexpr auto operator-(const Vector_t& a, const Vector_t& b) noexcept {
        return Vector_t{ a.x - b.x, a.y - b.y, a.z - b.z };
    }

    friend constexpr auto operator+(const Vector_t& a, const Vector_t& b) noexcept {
        return Vector_t{ a.x + b.x, a.y + b.y, a.z + b.z };
    }

    friend constexpr auto operator*(const Vector_t& a, const Vector_t& b) noexcept {
        return Vector_t{ a.x * b.x, a.y * b.y, a.z * b.z };
    }

    constexpr Vector_t& operator/=(TYPE div) noexcept {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    constexpr Vector_t& operator/(TYPE div) noexcept {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    constexpr Vector_t& operator/(Vector_t v) noexcept {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    constexpr auto operator*(TYPE mul) const noexcept
    {
        return Vector_t{ x * mul, y * mul, z * mul };
    }

    constexpr auto operator-(TYPE sub) const noexcept
    {
        return Vector_t{ x - sub, y - sub, z - sub };
    }

    constexpr auto operator+(TYPE add) const noexcept
    {
        return Vector_t{ x + add, y + add, z + add };
    }

    Vector_t& normalize() noexcept
    {
        x = std::isfinite(x) ? std::remainder(x, 360.0f) : 0.0f;
        y = std::isfinite(y) ? std::remainder(y, 360.0f) : 0.0f;
        z = std::isfinite(z) ? std::remainder(z, 360.0f) : 0.0f;
        return *this;
    }

    auto abs() const noexcept
    {
        return Vector_t{ std::abs(x), std::abs(y), std::abs(z) };
    }

    auto length() const noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Sqrt of the sum of the squares of the components, squared
    // (sqrt(x^2 + y^2 + z^2))^2
    auto magnitude_squared() const noexcept {
        auto tmp = length();
        return tmp * tmp;
    }

    constexpr auto squareLength() const noexcept
    {
        return (x * x) + (y * y) + (z * z);
    }

    constexpr auto size() const noexcept
    {
        return x * y * z;
    }

    constexpr auto dotProduct(const Vector_t& v) const noexcept
    {
        return x * v.x + y * v.y + z * v.z;
    }

    auto distTo(const Vector_t& v) const noexcept
    {
        return (*this - v).length();
    }

    TYPE x = 0, y = 0, z = 0;

    // rotate Vector_t by angle (x angle = a, y angle = b, z angle = c)
    void rotate(TYPE a, TYPE b, TYPE c) {
        TYPE cos_a = std::cos(a);
        TYPE sin_a = std::sin(a);
        TYPE cos_b = std::cos(b);
        TYPE sin_b = std::sin(b);
        TYPE cos_c = std::cos(c);
        TYPE sin_c = std::sin(c);

        TYPE sina_sinb = sin_a * sin_b;
        TYPE cosa_sinb = cos_a * sin_b;

        this->x = (cos_b * cos_c) * this->x + (sina_sinb * cos_c - cos_a * sin_c) * this->y + (cosa_sinb * cos_c + sin_a * sin_c) * this->z;
        this->y = (cos_b * sin_c) * this->x + (sina_sinb * sin_c + cos_a * cos_c) * this->y + (cosa_sinb * sin_c - sin_a * cos_c) * this->z;
        this->z = (-sin_b) * this->x + (sin_a * cos_b) * this->y + (cos_a * cos_b) * this->z;
    }

    void normalise() {
        TYPE l = this->length();
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

#pragma region Vector Serialization
template<typename T>
void to_json(json& j, const Vector_t<T> v) {
    j = json{
            { "x", v.x },
            { "y", v.x },
            { "z", v.x }
    };
}
template<typename T>
void from_json(const json& j, Vector_t<T> v) {
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
    j.at("z").get_to(v.z);
}
#pragma endregion

#endif //C_VERSION_Vector_t3_H
