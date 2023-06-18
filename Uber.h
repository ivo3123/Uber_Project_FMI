#pragma once

#include "UberClient.h"
#include "UberTaxiDriver.h"

#include "ConsoleInterfaceForSystem.h"



class POV_CurrentUser {
private:

    unsigned short userType = 0;
    int indexInArray = 0;

public:

    POV_CurrentUser (unsigned short userType, unsigned int indexInArray);
    POV_CurrentUser () = default;

    unsigned short getUserType () const;
    int getIndexInArray () const;

    void clear ();
};

class Uber {
private:

    Vector<Order> arrayOfActiveOrders;
    Vector<UberClient> arrayOfClients;
    Vector<UberTaxiDriver> arrayOfDrivers;

    void initialiseSystem (); // calls FileManager to read from files to initialise the vectors
    void close () const; // saves all the data into files


    double totalIncomeOfCompany = 0;

    int findIndexOfDriver (const String& username) const;
    int findClosetDriver (const Order& order) const;

    int findIndexOfOrder (unsigned int ID) const;

public:

    Uber ();
    ~Uber ();

    void Register ();
    POV_CurrentUser logIn () const;

    friend class Executer;
};