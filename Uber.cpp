#include <iostream>


#include "Uber.h"

const unsigned short CLIENT = 1;
const unsigned short DRIVER = 2;


POV_CurrentUser::POV_CurrentUser (unsigned short userType, unsigned int indexInArray) {
    if (userType == CLIENT || userType == DRIVER) {
        this -> userType = userType;
    }
    else {
        this -> userType = 3;
    }
    this -> indexInArray = indexInArray;
}

unsigned short POV_CurrentUser::getUserType () const {
    return userType;
}

int POV_CurrentUser::getIndexInArray () const {
    return indexInArray;
}

void POV_CurrentUser::clear () {
    userType = 0;
    indexInArray = 0;
}


int Uber::findIndexOfDriver (const String& username) const {
    for (unsigned int i = 0 ; i < arrayOfDrivers.getNumberOfElements () ; ++i) {
        if (username == arrayOfDrivers[i].getUsername ()) {
            return i;
        }
    }
    return -1;
}

int Uber::findClosetDriver (const Order& order) const {
    for (unsigned int i = 0 ; i < arrayOfDrivers.getNumberOfElements () ; ++i) {
        if (arrayOfDrivers[i].getIsFreeAtTheMoment () == false) {
            continue;
        }
        return i;
    }
    return -1;
}

int Uber::findIndexOfOrder (unsigned int ID) const {
    for (unsigned int i = 0 ; i < arrayOfActiveOrders.getNumberOfElements () ; ++i) {
        if (arrayOfActiveOrders[i].getID () == ID) {
            return i;
        }
    }
    return -1;
}


void Uber::Register () {
    ConsoleInterfaceForSystem interface;
    int type;
    interface.enterType (type);
    if (type == CLIENT) {
        
        arrayOfClients.pushBack (interface.registerClient ());
    }
    else if (type == DRIVER) {
        arrayOfDrivers.pushBack (interface.registerDriver ());
    }
}

POV_CurrentUser Uber::logIn () const {
    ConsoleInterfaceForSystem interface;
    String username, password;
    interface.logIn (username, password);
    for (unsigned int i = 0 ; i < arrayOfClients.getNumberOfElements () ; ++i) {
        if (arrayOfClients[i].getUsername () == username && arrayOfClients[i].getPassword () == password) {
            return POV_CurrentUser (CLIENT, i);
        }
    }
    for (unsigned int i = 0 ; i < arrayOfDrivers.getNumberOfElements () ; ++i) {
        if (arrayOfDrivers[i].getUsername () == username && arrayOfDrivers[i].getPassword () == password) {
            return POV_CurrentUser (DRIVER, i);
        }
    }
    return POV_CurrentUser (3, 0);
}

void Uber::initialiseSystem () {
    FileManager fileManager;
    arrayOfDrivers = fileManager.readDrivers ();
    arrayOfClients = fileManager.readClients ();
    arrayOfActiveOrders = fileManager.readOrders ();
    totalIncomeOfCompany += fileManager.readIncome ();
    Order::createID += fileManager.readOrderID ();

    for (unsigned int i = 0 ; i < arrayOfClients.getNumberOfElements () ; ++i) {
        int indexOfOrderOfCurrentClient = findIndexOfOrder (arrayOfClients[i].getID ());
        if (indexOfOrderOfCurrentClient != -1) {
            MySmartPtr<Order, MAX_NUMBER_OF_ORDER_HOLDERS> ptr = MySmartPtr<Order, MAX_NUMBER_OF_ORDER_HOLDERS> (new Order (arrayOfActiveOrders[indexOfOrderOfCurrentClient]));
            arrayOfClients[i].currentOrder = ptr;
            String driverUsername = (ptr -> getDriverData ()).getUsername ();
            if (driverUsername == "") {
                int indexOfClosestDriver = findClosetDriver (*ptr);
                if (indexOfClosestDriver != -1) {
                    arrayOfDrivers[indexOfClosestDriver].currentOrder = ptr;
                }
            }
            int indexOfDriverWithSameOrder = findIndexOfDriver (driverUsername);
            if (indexOfDriverWithSameOrder != -1) {
                arrayOfDrivers[indexOfDriverWithSameOrder].currentOrder = ptr;
            }
        }
    }
}

void Uber::close () const {
    FileManager fileManager;
    fileManager.clearFiles ();
    for (unsigned int i = 0 ; i < arrayOfClients.getNumberOfElements () ; ++i) {
        fileManager.writeClientToFile (arrayOfClients[i]);
    }
    for (unsigned int i = 0 ; i < arrayOfDrivers.getNumberOfElements () ; ++i) {
        fileManager.writeTaxiDriverToFile (arrayOfDrivers[i]);
    }
    fileManager.writeIncome (totalIncomeOfCompany);
    fileManager.writeOrderID (Order::createID);
}

Uber::Uber () {
    initialiseSystem ();
}

Uber::~Uber () {
    close ();
}