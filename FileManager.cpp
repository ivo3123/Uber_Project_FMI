#include <iostream>
#include <fstream>
#include <cstring>

#include "FileManager.h"
#include "Conversion.h"


void FileManager::clearFiles () const {
    std::ofstream file1 (CLIENTS_FILE, std::ios::trunc);
    file1.close ();
    std::ofstream file2 (DRIVERS_FILE, std::ios::trunc);
    file2.close ();
    std::ofstream file3 (ORDERS_FILE, std::ios::trunc);
    file3.close ();
    std::ofstream file4 (UBER_INCOME_FILE, std::ios::trunc);
    file4.close ();
    std::ofstream file5 (ORDER_ID_FILE, std::ios::trunc);
    file5.close ();
}

void FileManager::writeClientToFile (const UberClient& client) const {
    std::ofstream outFile (CLIENTS_FILE, std::ios::out | std::ios::app);
    if (!outFile.is_open ()) {
        return;
    }

    outFile << SEPARATOR << client.getUsername ();
    outFile << SEPARATOR << client.getPassword (); 
    outFile << SEPARATOR << client.getFirstName (); 
    outFile << SEPARATOR << client.getLastName ();
    client.currentOrder.isInitialised () ? outFile << SEPARATOR << client.currentOrder -> getID () : outFile << SEPARATOR << 0;
    outFile << SEPARATOR << client.getMoney ();
    outFile << SEPARATOR << "\n";

    outFile.close ();

    if (!client.currentOrder.isInitialised ()) {
        return;
    }

    std::ofstream outFile2 (ORDERS_FILE, std::ios::out | std::ios::app);
    if (!outFile2.is_open ()) {
        return;
    }

    outFile2 << SEPARATOR << client.currentOrder -> getID ();
    outFile2 << SEPARATOR << (int)client.currentOrder -> getStatus ();
    outFile2 << SEPARATOR << client.currentOrder -> getClientData ().getFullName (); 

    outFile2 << SEPARATOR << client.currentOrder -> getOrigin ().getPoint ().x;
    outFile2 << SEPARATOR << client.currentOrder -> getOrigin ().getPoint ().y;
    outFile2 << SEPARATOR << client.currentOrder -> getOrigin ().getLocation ();
    outFile2 << SEPARATOR << client.currentOrder -> getOrigin ().getExtraDescription ();

    outFile2 << SEPARATOR << client.currentOrder -> getDestination ().getPoint ().x;
    outFile2 << SEPARATOR << client.currentOrder -> getDestination ().getPoint ().y;
    outFile2 << SEPARATOR << client.currentOrder -> getDestination ().getLocation ();
    outFile2 << SEPARATOR << client.currentOrder -> getDestination ().getExtraDescription ();

    outFile2 << SEPARATOR << client.currentOrder -> getNumberOfPassengers ();
    outFile2 << SEPARATOR << client.currentOrder -> getMinutesToArrive ();
    outFile2 << SEPARATOR << client.currentOrder -> getCost ();
    outFile2 << SEPARATOR << client.currentOrder -> getMoneyPaid ();

    outFile2 << SEPARATOR << client.currentOrder -> getDriverData ().getUsername ();
    outFile2 << SEPARATOR << client.currentOrder -> getDriverData ().getFullName ();
    outFile2 << SEPARATOR << client.currentOrder -> getDriverData ().getCarNumber ();
    outFile2 << SEPARATOR << client.currentOrder -> getDriverData ().getTelephoneNumber ();
    outFile2 << SEPARATOR << client.currentOrder -> getDriverData ().getRating (); 

    outFile2 << SEPARATOR << "\n";

    outFile2.close ();
}

void FileManager::writeTaxiDriverToFile (const UberTaxiDriver& taxiDriver) const {
    std::ofstream outFile (DRIVERS_FILE, std::ios::out | std::ios::app);
    if (!outFile.is_open ()) {
        return;
    }

    outFile << SEPARATOR << taxiDriver.getUsername ();
    outFile << SEPARATOR << taxiDriver.getPassword (); 
    outFile << SEPARATOR << taxiDriver.getFirstName (); 
    outFile << SEPARATOR << taxiDriver.getLastName ();
    outFile << SEPARATOR << taxiDriver.getCarNumber ();
    outFile << SEPARATOR << taxiDriver.getTelephoneNumber ();
    outFile << SEPARATOR << taxiDriver.getPosition ().x;
    outFile << SEPARATOR << taxiDriver.getPosition ().y;
    outFile << SEPARATOR << taxiDriver.getIsFreeAtTheMoment ();
    outFile << SEPARATOR << taxiDriver.getIncome ();

    unsigned int count = taxiDriver.getNumberOfTimesRated ();
    for (unsigned int i = 0 ; i < count ; ++i) {
        outFile << SEPARATOR << taxiDriver.ratingsRecieved[i];
    }

    outFile << SEPARATOR << "\n";

    outFile.close ();
}

Vector<UberClient> FileManager::readClients () const {
    Vector<UberClient> arrayOfClients;
    std::ifstream inFile (CLIENTS_FILE, std::ios::in);
    if (!inFile.is_open ()) {
        throw std::runtime_error ("Error when opening Clients.uber to read from file!");
    }

    while (true) {
        inFile.ignore (); // skips the first |

        if (inFile.eof ()) {
            break;
        }

        char buffer1[1024];
        inFile.getline (buffer1, 1024, SEPARATOR);
        String username (buffer1);

        char buffer2[1024];
        inFile.getline (buffer2, 1024, SEPARATOR);
        String password (buffer2);

        char buffer3[1024];
        inFile.getline (buffer3, 1024, SEPARATOR);
        String firstName (buffer3);

        char buffer4[1024];
        inFile.getline (buffer4, 1024, SEPARATOR);
        String lastName (buffer4);

        unsigned long ID = 0;
        inFile >> ID;
        inFile.ignore ();

        double availableMoney = 0;
        inFile >> availableMoney;
        inFile.ignore ();

        inFile.ignore ();

        arrayOfClients.pushBack (UberClient (username, password, firstName, lastName, ID, availableMoney));
    }

    inFile.close ();
    return arrayOfClients;
}

Vector<UberTaxiDriver> FileManager::readDrivers () const {
    Vector<UberTaxiDriver> arrayOfDrivers;
    std::ifstream inFile (DRIVERS_FILE, std::ios::in);
    if (!inFile.is_open ()) {
        throw std::runtime_error ("Error when opening Drivers.uber to read from file!");
    }

    inFile.ignore (); // skips the first |
    while (true) {

        if (inFile.eof ()) {
            break;
        }

        char buffer1[1024];
        inFile.getline (buffer1, 1024, SEPARATOR);
        String username (buffer1);

        char buffer2[1024];
        inFile.getline (buffer2, 1024, SEPARATOR);
        String password (buffer2);

        char buffer3[1024];
        inFile.getline (buffer3, 1024, SEPARATOR);
        String firstName (buffer3);

        char buffer4[1024];
        inFile.getline (buffer4, 1024, SEPARATOR);
        String lastName (buffer4);

        char buffer5[1024];
        inFile.getline (buffer5, 1024, SEPARATOR);
        String carNumber (buffer5);

        char buffer6[1024];
        inFile.getline (buffer6, 1024, SEPARATOR);
        String telephoneNumber (buffer6);

        double xCoord = 0;
        inFile >> xCoord;
        inFile.ignore ();

        double yCoord = 0;
        inFile >> yCoord;
        inFile.ignore ();

        bool isFreeAtTheMoment;
        inFile >> isFreeAtTheMoment;
        inFile.ignore ();

        double income = 0;
        inFile >> income;
        inFile.ignore ();

        Vector<unsigned short> ratingsRecieved;

        while (true) {
            char buffer[1024];
            inFile.getline (buffer, 1024, SEPARATOR);
            String temp (buffer);
            if (temp.length () == 0 || temp == "\n") {
                break;
            }
            else {
                unsigned short currentRating = fromString (buffer);
                ratingsRecieved.pushBack (currentRating);
            }
        }

        arrayOfDrivers.pushBack (UberTaxiDriver (username, password, firstName, lastName, carNumber, telephoneNumber, xCoord, yCoord, isFreeAtTheMoment, income, ratingsRecieved));
    }

    inFile.close ();
    return arrayOfDrivers;
}

Vector<Order> FileManager::readOrders () const {
    Vector<Order> arrayOfOrders;
    std::ifstream inFile (ORDERS_FILE, std::ios::in);
    if (!inFile.is_open ()) {
        throw std::runtime_error ("Error when opening Orders.uber to read from file!");
    }

    while (true) {
        inFile.ignore (); // skips the first |

        if (inFile.eof ()) {
            break;
        }

        unsigned long ID = 0;
        inFile >> ID;
        inFile.ignore ();

        int status = 0;
        inFile >> status;
        inFile.ignore ();

        char buffer1[1024];
        inFile.getline (buffer1, 1024, SEPARATOR);
        String clinetFullName (buffer1);

        double origin_xCoord = 0;
        inFile >> origin_xCoord;
        inFile.ignore ();

        double origin_yCoord = 0;
        inFile >> origin_yCoord;
        inFile.ignore ();

        char buffer2[1024];
        inFile.getline (buffer2, 1024, SEPARATOR);
        String originLocation (buffer2);

        char buffer3[1024];
        inFile.getline (buffer3, 1024, SEPARATOR);
        String originExtraDescription (buffer3);

        double destination_xCoord = 0;
        inFile >> destination_xCoord;
        inFile.ignore ();

        double destination_yCoord = 0;
        inFile >> destination_yCoord;
        inFile.ignore ();

        char buffer4[1024];
        inFile.getline (buffer4, 1024, SEPARATOR);
        String destinationLocation (buffer4);

        char buffer5[1024];
        inFile.getline (buffer5, 1024, SEPARATOR);
        String destinationExtraDescription (buffer5);

        unsigned short numberOfPassengers = 0;
        inFile >> numberOfPassengers;
        inFile.ignore ();

        unsigned int minutesToArrive = 0;
        inFile >> minutesToArrive;
        inFile.ignore ();

        double cost = 0;
        inFile >> cost;
        inFile.ignore ();

        double moneyPaid = 0;
        inFile >> moneyPaid;
        inFile.ignore ();

        char buffer6[1024];
        inFile.getline (buffer6, 1024, SEPARATOR);
        String driverUsername (buffer6);

        char buffer7[1024];
        inFile.getline (buffer7, 1024, SEPARATOR);
        String driverFullName (buffer7);

        char buffer8[1024];
        inFile.getline (buffer8, 1024, SEPARATOR);
        String carNumber (buffer8);

        char buffer9[1024];
        inFile.getline (buffer9, 1024, SEPARATOR);
        String telephoneNumber (buffer9);

        double rating = 0;
        inFile >> rating;
        inFile.ignore ();

        inFile.ignore ();

        arrayOfOrders.pushBack (Order (ID, status, clinetFullName, origin_xCoord, origin_yCoord, originLocation, originExtraDescription,
                                       destination_xCoord, destination_yCoord, destinationLocation, destinationExtraDescription,
                                       numberOfPassengers, minutesToArrive, cost, moneyPaid,
                                       driverUsername, driverFullName, carNumber, telephoneNumber, rating));
    }

    inFile.close ();
    return arrayOfOrders;
}

void FileManager::writeOrderID (unsigned long ID) const {
    std::ofstream outFile (ORDER_ID_FILE, std::ios::out | std::ios::app);
    if (!outFile.is_open ()) {
        return;
    }

    outFile << ID;

    outFile.close ();
}

unsigned long FileManager::readOrderID () const {
    std::ifstream inFile (ORDER_ID_FILE, std::ios::in);
    if (!inFile.is_open ()) {
        throw std::runtime_error ("Error when opening file to read ID!");
    }

    unsigned long ID = 0;
    inFile >> ID;

    inFile.close ();
    return ID;
}

void FileManager::writeIncome (double uberIncome) const {
    std::ofstream outFile (UBER_INCOME_FILE, std::ios::out | std::ios::app);
    if (!outFile.is_open ()) {
        return;
    }

    outFile << uberIncome;

    outFile.close ();
}

double FileManager::readIncome () const {
    std::ifstream inFile (UBER_INCOME_FILE, std::ios::in);
    if (!inFile.is_open ()) {
        throw std::runtime_error ("Error when opening file to read uber income!");
    }

    double income = 0;
    inFile >> income;

    inFile.close ();
    return income;
}
