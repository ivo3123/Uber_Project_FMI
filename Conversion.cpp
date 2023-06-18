#include <iostream>
#include "Conversion.h"


// the 2 functions below are taken from https://github.com/Angeld55/Introduction_to_programming_FMI/blob/main/Sem.%2008/fromString.cpp

unsigned short convertFromChar (char ch) {
	if (ch < '0' || ch > '9') {
        throw std::runtime_error ("Not a digit");
    }
	return ch - '0';
}

unsigned short fromString (const char* str) {
	unsigned int result = 0;
	while (*str != '\0') {
        short currentDigit = convertFromChar(*str);
        (result *= 10) += currentDigit;
        str++;
	}
	return result;
}
