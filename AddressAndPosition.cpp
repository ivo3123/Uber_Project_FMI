#include <iostream>

#include "AddressAndPosition.h"


Point::Point (double x, double y) {
    this -> x = x;
    this -> y = y;
}




Address::Address (const Point& point, const String& location, const String& extraDescription) {
    this -> point = point;
    this -> location = location;
    this -> extraDescription = extraDescription;
}

Address::Address (double x, double y, const String& location, const String& extraDescription) : Address (Point (x, y), location, extraDescription) {}

// without the extra description
Address::Address (const Point& point, const String& location) : Address (point, location, "") {}

Address::Address (double x, double y, const String& location) : Address (Point (x, y), location) {}

const Point& Address::getPoint () const {
    return point;
}

const String& Address::getLocation () const {
    return location;
}

const String& Address::getExtraDescription () const {
    return extraDescription;
}





std::ostream& operator << (std::ostream& os, const Point& point) {
    return os << "(" << point.x << ", " << point.y << ")";
}

std::ostream& operator << (std::ostream& os, const Address& address) {
    os << address.location << " " << address.point;
    if (address.extraDescription != "") {
        os << " " << address.extraDescription;
    }
    return os;
}