#include <iostream>

#include "Orders.h"


ClientData::ClientData (const String& fullName) {
    this -> fullName = fullName;
}

const String& ClientData::getFullName () const {
    return fullName;
}





DriverData::DriverData (const String& username, const String& fullName, const String& carNumber, const String& telephoneNumber, double rating) {
    this -> username = username;
    this -> fullName = fullName;
    this -> carNumber = carNumber;
    this -> telephoneNumber = telephoneNumber;
    this -> rating = rating;
}

void DriverData::setRating (double rating) {
    this -> rating = rating;
}

const String& DriverData::getUsername () const {
    return username;
}

const String& DriverData::getFullName () const {
    return fullName;
}

const String& DriverData::getCarNumber () const {
    return carNumber;
}

const String& DriverData::getTelephoneNumber () const {
    return telephoneNumber;
}

double DriverData::getRating () const {
    return rating;
}






unsigned long Order::createID = 0;

Order::Order (const Address& origin, const Address& destination, unsigned short numberOfPassengers, const ClientData& client) {
    ++createID;
    ID = createID;
    this -> origin = origin;
    this -> destination = destination;
    this -> numberOfPassengers = numberOfPassengers;
    clientData = client;
    status = OrderStatus::registered;
}

Order::Order (unsigned long ID, int statusOfOrder, const String& clinetFullName,
        double origin_xCoord, double origin_yCoord, const String& originLocation, const String& originExtraDescription,
        double destination_xCoord, double destination_yCoord, const String& destinationLocation, const String& destinationExtraDescription,
        unsigned short numberOfPassengers, unsigned int minutesToArrive, double cost, double moneyPaid,
        const String& driverUsername, const String& driverFullName, const String& carNumber, const String& telephoneNumber, double rating)
        : Order (Address (origin_xCoord, origin_yCoord, originLocation, originExtraDescription),
                 Address (destination_xCoord, destination_yCoord, destinationLocation, destinationExtraDescription),
                 numberOfPassengers, ClientData (clinetFullName)) {
    this -> ID = ID;
    this -> minutesToArrive = minutesToArrive;
    this -> cost = cost;
    this -> moneyPaid = moneyPaid;
    this -> driverData = DriverData (driverUsername, driverFullName, carNumber, telephoneNumber, rating);
    switch (statusOfOrder) {
        case 0 : status = OrderStatus::uninitialised; break;
        case 1 : status = OrderStatus::cancelled; break;
        case 2 : status = OrderStatus::registered; break;
        case 3 : status = OrderStatus::recieved; break;
        case 4 : status = OrderStatus::completed; break;
        case 5 : status = OrderStatus::paid; break;
        case 6 : status = OrderStatus::concluded; break;
        default : status = OrderStatus::uninitialised; break;
    }
}

const Address& Order::getOrigin () const {
    return origin;
}

const Address& Order::getDestination () const {
    return destination;
}

unsigned short Order::getNumberOfPassengers () const {
    return numberOfPassengers;
}

unsigned long Order::getID () const {
    return ID;
}

OrderStatus Order::getStatus () const {
    return status;
}

unsigned int Order::getMinutesToArrive () const {
    return minutesToArrive;
}

double Order::getCost () const {
    return cost;
}

double Order::getMoneyPaid () const {
    return moneyPaid;
}

unsigned short Order::getRatingGiven () const {
    return ratingGiven;
}

const ClientData& Order::getClientData () const {
    return clientData;
}

const DriverData& Order::getDriverData () const {
    return driverData;
}

void Order::setStatus (OrderStatus orderStatus) {
    status = orderStatus;
}

void Order::setDriverData (const DriverData& driverData) {
    this -> driverData = driverData;
}

void Order::setMinutesToArrive (unsigned int minutesToArrive) {
    this -> minutesToArrive = minutesToArrive;
}

void Order::setCost (double cost) {
    this -> cost = cost;
}

void Order::setMoneyPaid (double moneyPaid) {
    this -> moneyPaid = moneyPaid;
}

void Order::setRatingGiven (unsigned short ratigGiven) {
    this -> ratingGiven = ratigGiven;
}