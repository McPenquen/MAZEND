#pragma once

#include <SFML/System.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include <sstream>
#include <iomanip>

#define PI 3.14159265

using namespace std;

namespace sf {
    // Create a defintion for af::vector
    typedef Vector2<size_t> Vector2ul;

    // Return vector length
    template <typename T> double length(const Vector2<T> &v) {
        return sqrt(pow(v.x, 2) + pow(v.y, 2));
    }

    // Return normalised vector
    template <typename T> Vector2<T> normalize(const Vector2<T> &v) {
        Vector2<T> vector;
        double l = length(v);
        if (l != 0) {
            vector.x = v.x / l;
            vector.y = v.y / l;
        }
        return vector;
    }

    // Allow casting to one vector to another
    template <typename T, typename U> Vector2<T> Vcast(const Vector2<U>& v) {
        return Vector2<T>(static_cast<T>(v.x), static_cast<T>(v.y));
    }

    // Degrees to radians conversion
    static double deg2rad(double degrees) {
        return degrees / 180 * PI;
    }

    // Rotate vector by an angle
    template <typename T> Vector2<T> rotate(const Vector2<T> &v, const double degrees) {
        const double theta = deg2rad(degrees);
        const double cs = cos(theta);
        const sn = sin(theta);
        return {(T)(c.x * cs - v.y * sn)};
    }

    // Allow vectors to be in cout
    template <typename T> ostream &operator<<(ostream &os, const Vector2<T> &v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

    template <typename T>
    ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
        os << "vec[";
        for (const auto& a : v) {
            os << a << ',';
        }
        os << ']';
        return os;
    }

    // Allow vector multiplication
    template <typename T> Vector2<T> operator*(const Vector2<T>& left, const Vector2<T>& right) {
        Vector2<T> r = left;
        r.x *= right.x;
        r.y *= right.y;
        return r;
    }
}

template <typename T>
std::string toStrDecPt(const uint16_t& dp, const T& i) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(dp) << i;
    return stream.str();
}
