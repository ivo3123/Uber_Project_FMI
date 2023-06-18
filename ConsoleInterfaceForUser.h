#pragma once

#include "Utilities.hpp"
#include "AddressAndPosition.h"

class ConsoleInterfaceForDriver {
private:

    void enterMinutesToArrive (unsigned int& minutesToArrive) const;
    void enterPosition (double& xCoord, double& yCoord) const;

    friend class UberTaxiDriver;
};


class ConsoleInterfaceForClient {
private:

    void enterOrder (Address& origin, Address& destination, unsigned short& numberOfPassengers) const;
    void enterAmountOfMoneyToPay (double& money, double cost, double availableMoney) const;
    void enterAmountOfMoneyToAdd (double& money) const;
    void enterRatig (unsigned short& rating) const;

    friend class UberClient;
};