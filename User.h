#pragma once

#include "String.h"

#include "MySmartPtr.hpp"
#include "Vector.hpp"

const unsigned MAX_NUMBER_OF_ORDER_HOLDERS = 3;


class User {  
private:
    
    String username;
    String password;
    String firstName;
    String lastName;
    bool isLoggedIn = false;

    static bool isValidUsername (const String& username);
    static bool isValidPassword (const String& password);

    static bool isValidName (const String& name);

public:

    bool setUsername (const String& username);
    bool setPassword (const String& password);

    bool setFirstName (const String& firstName);
    bool setLastName (const String& lastName);

    void logOut ();

    User () = default;

    User (const String& username, const String& password, const String& firstName, const String& lastName);

    bool getIsLoggedIn () const;
    const String& getFirstName () const;
    const String& getLastName () const;
    String getFullName () const;
    const String& getUsername () const;

protected:

    const String& getPassword () const;

    friend class ConsoleInterfaceForSystem;
    friend class FileManager;
    friend class Uber;
};