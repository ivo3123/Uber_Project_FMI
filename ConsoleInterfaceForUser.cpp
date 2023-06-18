#include <iostream>

#include "ConsoleInterfaceForUser.h"
#include "AddressAndPosition.h"


void ConsoleInterfaceForDriver::enterMinutesToArrive (unsigned int& minutesToArrive) const {
    minutesToArrive = 0;
    while (minutesToArrive <= 0 || minutesToArrive >= 600) {
        enterNumber (minutesToArrive, "Enter how many minutes it will take you to arrive: ", "Invalid input!\n");
        if (minutesToArrive <= 0 || minutesToArrive >= 600) {
            std::cout << "Invalid minutes!\n";
        }
        else {
            break;
        }
    }
}

void ConsoleInterfaceForDriver::enterPosition (double& xCoord, double& yCoord) const {
    std::cout << "Enter your location (x, y): ";
    enterNumber (xCoord);
    enterNumber (yCoord);
}



void ConsoleInterfaceForClient::enterOrder (Address& origin, Address& destination, unsigned short& numberOfPassengers) const {
    std::cout << "Enter your current address: ";
    std::cin >> origin.location;
    std::cout << "Enter your location (x, y): ";
    enterNumber (origin.point.x);
    enterNumber (origin.point.y);
    std::cout << "Enter extra description about your current address (not necessary): ";
    std::cin >> origin.extraDescription;

    std::cout << "Enter your destination address: ";
    std::cin >> destination.location;
    std::cout << "Enter your destination (x, y): ";
    enterNumber (destination.point.x);
    enterNumber (destination.point.y);
    std::cout << "Enter extra description about your destination address (not necessary): ";
    std::cin >> destination.extraDescription;

    std::cout << "Enter number of passengers: ";
    enterNumber (numberOfPassengers);
}

void ConsoleInterfaceForClient::enterAmountOfMoneyToPay (double& money, double cost, double availableMoney) const {
    money = 0;
    while (true) {
        enterNumber (money, "Enter how much money you wish pay to your driver: ", "Invalid input!\n");
        if (money <= cost) {
            std::cout << "Money paid cannot be less than the cost!\n";
        }
        else if (money > availableMoney) {
            std::cout << "Money paid cannot be more than the amount of available money!\n";
        }
        else {
            break;
        }
    }
}

void ConsoleInterfaceForClient::enterAmountOfMoneyToAdd (double& money) const {
    money = 0;
    while (true) {
        enterNumber (money, "Enter how much money you wish to add to your account: ", "Invalid input!\n");
        if (money <= 0) {
            std::cout << "Money added cannot be less than 0!\n";
        }
        else if (money >= 1000) {
            std::cout << "Money added cannot be more than 1000!\n";
        }
        else {
            break;
        }
    }
}

void ConsoleInterfaceForClient::enterRatig (unsigned short& rating) const {
    rating = 0;
    while (true) {
        enterNumber (rating, "Enter what rating you would give to your driver: ", "Invalid input!\n");
        if (rating < 1 || rating > 5) {
            std::cout << "Your rating must be between 1 and 5!\n";
        }
        else {
            break;
        }
    }
}