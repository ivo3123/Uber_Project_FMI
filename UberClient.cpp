#include <iostream>

#include "UberClient.h"


UberClient::UberClient (const String& username, const String& password, const String& firstName, const String& lastName) : 
                User (username, password, firstName, lastName) {}

UberClient::UberClient (const String& username, const String& password, 
                const String& firstName, const String& lastName,
                unsigned long ID, double availableMoney) : UberClient (username, password, firstName, lastName) {
                    currentOrderID = ID;
                    this -> availableMoney = availableMoney;
                }

void UberClient::addMoney () {
    ConsoleInterfaceForClient interface;
    double moneyToAdd = 0;
    interface.enterAmountOfMoneyToAdd (moneyToAdd);

    availableMoney += moneyToAdd;
}

double UberClient::getMoney () const {
    return availableMoney;
}

unsigned long UberClient::getID () const {
    return currentOrderID;
}

void UberClient::readMesseges () const {
    using std::cout;
    if (!currentOrder.isInitialised () || currentOrder -> getStatus () == OrderStatus::concluded) {
        cout << "You do not have any ongoing order.\n";
        return;
    }
    OrderStatus orderStatus = currentOrder -> getStatus ();
    if (orderStatus == OrderStatus::registered) {
        cout << "You have made an order with ID - " << currentOrder -> getID () << ".\n";
        cout << "Waiting for a Driver to accept your order.\n";
    }
    else if (orderStatus == OrderStatus::recieved) {
        cout << "Your driver " << (currentOrder -> getDriverData ()).getFullName () 
             << " has accepted your order and will ariive in " << currentOrder -> getMinutesToArrive () << " minutes.\n";
    }
    else if (orderStatus == OrderStatus::completed) {
        cout << "Your order with ID - " << currentOrder -> getID () << " is completed. \n";
        cout << "It costs: " << currentOrder -> getCost () << ".\n";
    }
    else if (orderStatus == OrderStatus::paid) {
        cout << "You paid " << currentOrder -> getMoneyPaid () << " levs for your order with ID - " << currentOrder -> getID () << ".\n";
    }
    else {
        throw std::runtime_error ("Unknown error when driver reads messages!");
    }
}

void UberClient::checkOrder () const {
    if (!currentOrder.isInitialised ()) {
        throw std::runtime_error ("No current order when trying to check order!");
    }
    using std::cout;
    cout << "From: \n";
    cout << currentOrder -> getOrigin () << "\n";
    cout << "To: \n";
    cout << currentOrder -> getDestination () << "\n";
    cout << "Driver: " << (currentOrder -> getDriverData ()).getFullName () << "\n";
    cout << "Car number: " << (currentOrder -> getDriverData ()).getCarNumber () << "\n";
    cout << "Telephone number: " << (currentOrder -> getDriverData ()).getTelephoneNumber () << "\n";
}

Order UberClient::makeAnOrder () {
    ConsoleInterfaceForClient interface;
    Address origin;
    Address destination;
    unsigned short numberOfPassengers = 0;
    interface.enterOrder (origin, destination, numberOfPassengers);

    return Order (origin, destination, numberOfPassengers, ClientData (getFullName ()));
}

void UberClient::cancelOrder () {}

double UberClient::payOrder () const {
    ConsoleInterfaceForClient interface;
    double money = 0;
    interface.enterAmountOfMoneyToPay (money, currentOrder -> getCost (), availableMoney);

    return money;
}

unsigned short UberClient::rate () const {
    ConsoleInterfaceForClient interface;
    unsigned short rating = 0;
    interface.enterRatig (rating);

    return rating;
}