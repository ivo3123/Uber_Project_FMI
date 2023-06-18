#pragma once

#include "UberClient.h"
#include "UberTaxiDriver.h"
#include "Orders.h"

namespace {
    const char CLIENTS_FILE[128] = "Clients.uber";
    const char DRIVERS_FILE[128] = "Drivers.uber";
    const char ORDERS_FILE[128] = "Orders.uber";
    const char ORDER_ID_FILE[128] = "OrderID.uber";
    const char UBER_INCOME_FILE[128] = "Income.uber";
}

namespace {
    const char SEPARATOR = '|';
}


class FileManager {
private:

    void clearFiles () const;

    void writeClientToFile (const UberClient&) const;
    void writeTaxiDriverToFile (const UberTaxiDriver&) const;

    UberClient readClientFromFile () const;
    UberTaxiDriver readTaxiDriverFromFile () const;
    Order readOrderFromFile () const;

    Vector<UberClient> readClients () const;
    Vector<UberTaxiDriver> readDrivers () const;
    Vector<Order> readOrders () const;

    void writeOrderID (unsigned long) const;
    unsigned long readOrderID () const;

    void writeIncome (double uberIncome) const;
    double readIncome () const;

    friend class Uber;
};