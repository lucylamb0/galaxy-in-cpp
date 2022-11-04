//
// Created by User on 31/07/2022.
//

#ifndef C_VERSION_VECTOR3_H
#define C_VERSION_VECTOR3_H

#include <valarray>

struct Vector {
    Vector() = default;
    constexpr Vector(long double x, long double y, long double z) noexcept : x{ x }, y{ y }, z{ z } {}

    friend auto operator<<(std::ostream& os, Vector const& m) -> std::ostream& {
        return os << "(" << m.x << ", " << m.y << ", " << m.z << ")";
    }

    constexpr auto notNull() const noexcept
    {
        return x || y || z;
    }

    constexpr auto isNull() const noexcept
    {
        return !notNull();
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

    auto length2D() const noexcept
    {
        return std::sqrt(x * x + y * y);
    }

    constexpr auto squareLength() const noexcept
    {
        return x * x + y * y + z * z;
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

//    auto toAngle() const noexcept
//    {
//        return Vector{ Helpers::rad2deg(std::atan2(-z, std::hypot(x, y))),
//                       Helpers::rad2deg(std::atan2(y, x)),
//                       0.0f };
//    }
//
//    static auto fromAngle(const Vector& angle) noexcept
//    {
//        return Vector{ std::cos(Helpers::deg2rad(angle.x)) * std::cos(Helpers::deg2rad(angle.y)),
//                       std::cos(Helpers::deg2rad(angle.x)) * std::sin(Helpers::deg2rad(angle.y)),
//                       -std::sin(Helpers::deg2rad(angle.x)) };
//    }

    long double x, y, z;
};

#endif //C_VERSION_VECTOR3_H
