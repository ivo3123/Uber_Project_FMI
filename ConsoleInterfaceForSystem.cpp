#include <iostream>


#include "ConsoleInterfaceForSystem.h"


void ConsoleInterfaceForSystem::enterNumberOfChoice (int& userInput, const String& messageForEntering, const String& messageForError, unsigned int lastChoice) const {
    userInput = 0;
    while (true) {
        enterNumber (userInput, messageForEntering, messageForError);
        if (userInput <= 0 || userInput > lastChoice) {
            std::cout << "Invalid number! The entered number must be between 1 and " << lastChoice << "!\n";
        }
        else {
            break;
        }
    }
}

void ConsoleInterfaceForSystem::enterComponent (String& component, const String& messageForEntering, const String& messageForError, bool (*isValidComponent)(const String&)) const {
    while (true) {
        std::cout << messageForEntering;
        std::cin >> component;
        if (isValidComponent (component) == true) {
            break;
        }
        else {
            std::cout << messageForError;
        }
    }
}

void ConsoleInterfaceForSystem::enterType (int& userInput) const {
    enterNumberOfChoice (userInput, "Type 1 if you wish to register as a client or type 2 if you wish to register as a driver: ", "Invalid input!\n", 2);
}

UberClient ConsoleInterfaceForSystem::registerClient () const {
    String username, password, firstName, lastName;

    enterComponent (username, "Enter username: ", "Invalid username!\n", User::isValidUsername);
    enterComponent (password, "Enter password: ", "Invalid passwrod!\n", User::isValidPassword);
    enterComponent (firstName, "Enter first name: ", "Invalid first name!\n", User::isValidName);
    enterComponent (lastName, "Enter last name: ", "Invalid last name!\n", User::isValidName);

    return UberClient (username, password, firstName, lastName);
}

UberTaxiDriver ConsoleInterfaceForSystem::registerDriver () const {
    String username, password, firstName, lastName, carNumber, telephoneNumber;

    enterComponent (username, "Enter username: ", "Invalid username!\n", User::isValidUsername);
    enterComponent (password, "Enter password: ", "Invalid passwrod!\n", User::isValidPassword);
    enterComponent (firstName, "Enter first name: ", "Invalid first name!\n", User::isValidName);
    enterComponent (lastName, "Enter last name: ", "Invalid last name!\n", User::isValidName);

    enterComponent (carNumber, "Enter car number: ", "Invalid car number!\n", UberTaxiDriver::isValidCarNumber);
    enterComponent (telephoneNumber, "Enter telephone number: ", "Invalid telephone number!\n", UberTaxiDriver::isValidTelephoneNumber);

    double xCoord, yCoord;
    std::cout << "Enter your location (x, y): ";
    enterNumber (xCoord);
    enterNumber (yCoord);

    return UberTaxiDriver (username, password, firstName, lastName, carNumber, telephoneNumber, Point (xCoord, yCoord));
}

void ConsoleInterfaceForSystem::logIn (String& username, String& password) const {
    std::cout << "Enter username: ";
    std::cin >> username;

    std::cout << "Enter password: ";
    std::cin >> password;
}