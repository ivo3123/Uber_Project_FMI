#include <iostream>

#include "User.h"


bool User::isValidUsername (const String& username) {
    return username.length () >= 2;
}

bool User::isValidPassword (const String& password) {
    return password.length () >= 5;
}

bool User::isValidName (const String& name) {
    return (name.length () > 0 && (name[0] >= 'A' && name[0] <= 'Z'));
}

bool User::setUsername (const String& username) {
    if (isValidUsername (username)) {
        this -> username = username;
        return true;
    }
    return false;
}

bool User::setPassword (const String& password) {
    if (isValidPassword (password)) {
        this -> password = password;
        return true;
    }
    return false;
}

bool User::setFirstName (const String& firstName) {
    if (isValidName (firstName)) {
        this -> firstName = firstName;
        return true;
    }
    return false;
}

bool User::setLastName (const String& lastName) {
    if (isValidName (lastName)) {
        this -> lastName = lastName;
        return true;
    }
    return false;
}

void User::logOut () {
    isLoggedIn = false;
}

User::User (const String& username, const String& password, const String& firstName, const String& lastName) {
    setUsername (username);
    setPassword (password);
    setFirstName (firstName);
    setLastName (lastName);
}

const String& User::getUsername () const {
    return username;
}

const String& User::getPassword () const {
    return password;
}

const String& User::getFirstName () const {
    return firstName;
}

const String& User::getLastName () const {
    return lastName;
}

String User::getFullName () const {
    return firstName + " " + lastName;
}

bool User::getIsLoggedIn () const {
    return isLoggedIn;
}