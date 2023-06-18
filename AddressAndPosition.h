#pragma once

#include <cmath>

#include "String.h"


struct Point {
public:

    double x = 0;
    double y = 0;

    Point () = default;

    Point (double x, double y);

    double getDistance (const Point& other) {
        int dx = x - other.x;
        int dy = y - other.y;
        return sqrt (dx * dx + dy * dy);
    }
};

struct Address {
public:

    Point point;
    String location;
    String extraDescription;

    Address () = default;

    Address (const Point& point, const String& location, const String& extraDescription);
    Address (double x, double y, const String& location, const String& extraDescription);

    // without the extra description
    Address (const Point& point, const String& location);
    Address (double x, double y, const String& location);

    const Point& getPoint () const;
    const String& getLocation () const;
    const String& getExtraDescription () const;
};


std::ostream& operator << (std::ostream& os, const Point& point);
std::ostream& operator << (std::ostream& os, const Address& address);