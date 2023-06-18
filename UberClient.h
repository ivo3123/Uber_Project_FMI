#pragma once

#include "User.h"
#include "Orders.h"

#include "ConsoleInterfaceForUser.h"


class UberClient : public User {
private:

    double availableMoney = 0;

    unsigned long currentOrderID = 0;
    MySmartPtr<Order, MAX_NUMBER_OF_ORDER_HOLDERS> currentOrder;

public:

    UberClient () = default; // we need a default constructor to be ale to put the clients in a vector (in class Uber)

    UberClient (const String& username, const String& password, 
                const String& firstName, const String& lastName);

    UberClient (const String& username, const String& password, 
                const String& firstName, const String& lastName,
                unsigned long ID, double availableMoney);

    void addMoney ();
    double getMoney () const;
    unsigned long getID () const;

    // main functionalities of client

    void readMesseges () const;
    void checkOrder () const;

    Order makeAnOrder ();
    void cancelOrder ();
    double payOrder () const;
    unsigned short rate () const;

    friend class Executer; // so that it can use the order
    friend class FileManager;
    friend class Uber;
};