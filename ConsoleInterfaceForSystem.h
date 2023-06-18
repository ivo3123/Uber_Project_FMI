#pragma once

#include "Utilities.hpp"

#include "UberClient.h"
#include "UberTaxiDriver.h"

#include "FileManager.h"

class ConsoleInterfaceForSystem {
private:

    void enterNumberOfChoice (int& userInput, const String& messageForEntering, const String& messageForError, unsigned int lastChoice) const;

    void enterComponent (String& component, const String& messageForEntering, const String& messageForError, bool (*isValidComponent)(const String&)) const;

    void enterType (int& userInput) const;

    UberClient registerClient () const;
    UberTaxiDriver registerDriver () const;

    void logIn (String& username, String& password) const;
    
    void goToMainScreen () const;

    void ClientScreen () const;

    friend class Uber;
    friend class Executer;
};