#pragma once

#include "User.h"
#include "Orders.h"

#include "ConsoleInterfaceForUser.h"

class UberTaxiDriver : public User {
private:

    String carNumber;
    String telephoneNumber;

    Point position;

    static double tariff; // currency per kilometer // we are using 1.22 levs per kilometer tariff with a minimal price of 2.49 levs
    static double minimalPrice; // 2.49 lv
    static double percentOfIncome; // when a client pays their order, the driver take 80% of the money and the Uber system - 20%

    double income = 0; // all the income the driver has made since being created
    Vector<unsigned short> ratingsRecieved;

    bool isFreeAtTheMoment = true;

    unsigned long currentOrderID = 0;
    MySmartPtr<Order, MAX_NUMBER_OF_ORDER_HOLDERS> currentOrder;
    
    static bool isValidCarNumber (const String&);
    static bool isValidTelephoneNumber (const String&);

    double getIncome () const; // only the system needs to know the income of the driver, not everyone

public:

    bool setCarNumber (const String&);
    bool setTelephoneNumber (const String&);
    void setPosition (const Point&);

    UberTaxiDriver () = default;

    UberTaxiDriver (const String& username, const String& password, const String& firstName, const String& lastName, 
                    const String& carNumber, const String& telephoneNumber, const Point& position);

    UberTaxiDriver (const String& username, const String& password, const String& firstName, const String& lastName, 
                    const String& carNumber, const String& telephoneNumber, double xCoord, double yCoord,
                    bool isFreeAtTheMoment, double income, const Vector<unsigned short>& ratingsRecieved);

    const String& getCarNumber () const;
    const String& getTelephoneNumber () const;
    const Point& getPosition () const;
    bool getIsFreeAtTheMoment () const;
    double getRating () const;
    unsigned int getNumberOfTimesRated () const;

    // the main functionalities of the driver

    void readMesseges () const;
    void changePosition ();
    unsigned int acceptOrder () const;
    void declineOrder () const;
    void finishOrder () const;
    void acceptPayment ();

    friend class ConsoleInterfaceForSystem;
    friend class FileManager;
    friend class Executer;
    friend class Uber;
};