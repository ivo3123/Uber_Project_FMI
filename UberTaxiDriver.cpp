#include <iostream>

#include "UberTaxiDriver.h"


double UberTaxiDriver::tariff = 1.22; // currency per kilometer // we are using 1.22 levs per kilometer tariff with a minimal price of 2.49 levs
double UberTaxiDriver::minimalPrice = 2.49; // 2.49 lv
double UberTaxiDriver::percentOfIncome = 0.8; // when a client pays their order, the driver take 80% of the money and the Uber system - 20%


UberTaxiDriver::UberTaxiDriver (const String& username, const String& password, const String& firstName, const String& lastName, 
                    const String& carNumber, const String& telephoneNumber, const Point& position)
                    : User (username, password, firstName, lastName) {
    setCarNumber (carNumber);
    setTelephoneNumber (telephoneNumber);
    setPosition (position);
}

UberTaxiDriver::UberTaxiDriver (const String& username, const String& password, const String& firstName, const String& lastName, 
                    const String& carNumber, const String& telephoneNumber, double xCoord, double yCoord,
                    bool isFreeAtTheMoment, double income, const Vector<unsigned short>& ratingsRecieved)
                    : UberTaxiDriver (username, password, firstName, lastName, carNumber, telephoneNumber, Point (xCoord, yCoord)) {
    this -> isFreeAtTheMoment = isFreeAtTheMoment;
    this -> income = income;
    this -> ratingsRecieved = ratingsRecieved;
}


bool UberTaxiDriver::isValidCarNumber (const String& carNumber) {
    return carNumber.length () >= 4;
}

bool UberTaxiDriver::isValidTelephoneNumber (const String& telephoneNumber) {
    return telephoneNumber.length () >= 4;
}

bool UberTaxiDriver::setCarNumber (const String& carNumber) {
    if (isValidCarNumber (carNumber)) {
        this -> carNumber = carNumber;
        return true;
    }
    return false;
}

bool UberTaxiDriver::setTelephoneNumber (const String& telephoneNumber) {
    if (isValidTelephoneNumber (telephoneNumber)) {
        this -> telephoneNumber =  telephoneNumber;
        return true;
    }
    return false;
}

void UberTaxiDriver::setPosition (const Point& position) {
    this -> position = position;
}

const String& UberTaxiDriver::getCarNumber () const {
    return carNumber;
}

const String& UberTaxiDriver::getTelephoneNumber () const {
    return telephoneNumber;
}

const Point& UberTaxiDriver::getPosition () const {
    return position;
}

bool UberTaxiDriver::getIsFreeAtTheMoment () const {
    return isFreeAtTheMoment;
}

double UberTaxiDriver::getIncome () const {
    return income;
}

double UberTaxiDriver::getRating () const {
    double sum = 0;
    for (unsigned int i = 0 ; i < ratingsRecieved.getNumberOfElements () ; ++i) {
        sum += ratingsRecieved[i];
    }
    if (ratingsRecieved.getNumberOfElements () == 0) {
        return 0; // when denominator is 0
    }
    return (sum / ratingsRecieved.getNumberOfElements ());
}

unsigned int UberTaxiDriver::getNumberOfTimesRated () const {
    return ratingsRecieved.getNumberOfElements ();
}





void UberTaxiDriver::readMesseges () const {
    using std::cout;
    if (!currentOrder.isInitialised () || currentOrder -> getStatus () == OrderStatus::concluded) {
        cout << "You do not have any ongoing order or request for an order.\n";
        return;
    }
    OrderStatus orderStatus = currentOrder -> getStatus ();
    if (orderStatus == OrderStatus::cancelled) {
        cout << (currentOrder -> getClientData ()).getFullName () << " has cancelled their order with ID - " << currentOrder -> getID () << ".\n";
    }
    else if (orderStatus == OrderStatus::registered) {
        cout << (currentOrder -> getClientData ()).getFullName () << " has made an order with ID - " << currentOrder -> getID () << ".\n";
        cout << "From: \n";
        cout << currentOrder -> getOrigin () << "\n";
        cout << "To: \n";
        cout << currentOrder -> getDestination () << "\n";
    }
    else if (orderStatus == OrderStatus::recieved) {
        cout << "You have accepted the order of " << (currentOrder -> getClientData ()).getFullName () << " with ID - " << currentOrder -> getID () << ".\n";
    }
    else if (orderStatus == OrderStatus::completed) {
        cout << "The order of " << (currentOrder -> getClientData ()).getFullName () << " with ID - " << currentOrder -> getID () << " is completed. \n";
        cout << "It costs: " << currentOrder -> getCost () << ".\n";
    }
    else if (orderStatus == OrderStatus::paid) {
        cout << (currentOrder -> getClientData ()).getFullName () << " has paid " << currentOrder -> getMoneyPaid ();
        cout << " for their order with ID - " << currentOrder -> getID () << ".\n";
    }
    else {
        throw std::runtime_error ("Unknown error when driver reads messages!");
    }
}

void UberTaxiDriver::changePosition () {
    ConsoleInterfaceForDriver interface;
    double xCoord, yCoord;
    interface.enterPosition (xCoord, yCoord);
    setPosition (Point (xCoord, yCoord));
}

unsigned int UberTaxiDriver::acceptOrder () const {
    ConsoleInterfaceForDriver interface;
    unsigned int minutesToArrive = 0;
    interface.enterMinutesToArrive (minutesToArrive);

    return minutesToArrive;
}

void UberTaxiDriver::declineOrder () const {}

void UberTaxiDriver::finishOrder () const {}

void UberTaxiDriver::acceptPayment () {
    income += (currentOrder -> getMoneyPaid ()) * percentOfIncome;
}