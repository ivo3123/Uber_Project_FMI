#pragma once

#include <iostream>

#include "String.h"

// I had found a similar function to the one below in geek for geeks

template <typename T>
void enterNumber (T& num, const String& messageForEntering, const String& messageForError) {
    // I found this function somewhere in geek for geeks
    using std::cin;
    using std::cout;
    while (true) {
        cout << messageForEntering;
        cin >> num;
        if (cin.good ()) {
            std::cin.ignore ();
            break; // num is a valid number
        }
        else {
            cout << messageForError;
            cin.clear ();
            cin.ignore (2048, '\n');
        }
    }
}

template <typename T>
void enterNumber (T& num) {
    static const String EMPTY_STRING = "";
    enterNumber (num, EMPTY_STRING, EMPTY_STRING);
}