#pragma once

#include "AddressAndPosition.h"

namespace {
    const double MINIMAL_AMOUNT_FOR_A_DRIVE = 2.49;
}

struct ClientData {
private:

    String fullName;

public:

    ClientData () = default;
    ClientData (const String& fullName);
    const String& getFullName () const;
};

struct DriverData {
private:

    String username;
    String fullName;
    String carNumber;
    String telephoneNumber;
    double rating = 0;

    void setRating (double);

public:

    DriverData () = default;
    DriverData (const String& username, const String& fullName, const String& carNumber, const String& telephoneNumber, double rating);

    const String& getUsername () const;
    const String& getFullName () const;
    const String& getCarNumber () const;
    const String& getTelephoneNumber () const;
    double getRating () const;

    friend class Executer;
};





enum class OrderStatus { uninitialised = 0, cancelled, registered, recieved, completed, paid, concluded };

class Order {
private:

    static unsigned long createID;
    unsigned long ID;

    ClientData clientData;

    OrderStatus status = OrderStatus::uninitialised;

    Address origin;
    Address destination;
    unsigned short numberOfPassengers = 0;

    unsigned int minutesToArrive = 0;
    double cost = 0;
    double moneyPaid = 0;
    unsigned short ratingGiven = 0;

    unsigned int numberOfDriversToDecline = 0;

    DriverData driverData;

public:

    Order (const Address& origin, const Address& destination, unsigned short numberOfPassengers, const ClientData& client);

    Order (unsigned long ID, int statusOfOrder, const String& clinetFullName,
           double origin_xCoord, double origin_yCoord, const String& originLocation, const String& originExtraDescription,
           double destination_xCoord, double destination_yCoord, const String& destinationLocation, const String& destinationExtraDescription,
           unsigned short numberOfPassengers, unsigned int minutesToArrive, double cost, double moneyPaid,
           const String& driverUsername, const String& driverFullName, const String& carNumber, const String& telephoneNumber, double rating);

    Order () = default; // it needs a default constructor so that it can be used in Vector

    const Address& getOrigin () const;
    const Address& getDestination () const;
    unsigned short getNumberOfPassengers () const;

    unsigned long getID () const;
    OrderStatus getStatus () const;

    unsigned int getMinutesToArrive () const;
    double getCost () const;
    double getMoneyPaid () const;
    unsigned short getRatingGiven () const;

    const ClientData& getClientData () const;
    const DriverData& getDriverData () const;

private:

    void setStatus (OrderStatus orderStatus);

    void setDriverData (const DriverData& driverData);
    void setMinutesToArrive (unsigned int);

    void setCost (double);

    void setMoneyPaid (double);
    void setRatingGiven (unsigned short);

    friend class UberTaxiDriver;
    friend class Executer;
    friend class Uber;
};