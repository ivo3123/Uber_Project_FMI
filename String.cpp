#include <iostream>
#include <cstring>
#pragma warning(disable : 4996)
#include <memory>
#include <fstream>

#include "String.h"


bool String::isPossibleToOptimise (size_t length) const {
    return (length <= MAX_LENGTH_OF_SMALL_STRING);
}

void String::optimiseSmallString (const char* data) {
    strcpy (smallString, data);
    ((char*)&_length)[LAST_BYTE_OF_SIZE_T] &= 127;
}

void String::optimiseBigString () {
    ((char*)&_length)[LAST_BYTE_OF_SIZE_T] |= 128;
}

bool String::isOptimised () const {
    int result = ((char*)&_length)[LAST_BYTE_OF_SIZE_T] & 128;
    return result == 0;
}

size_t String::length () const {
    if (isOptimised ()) {
        return (strlen (smallString));
    }
    size_t result = _length;
    ((char*)&result)[LAST_BYTE_OF_SIZE_T] &= 127;
    return result;
}

String::String (size_t length) {
    if (!isPossibleToOptimise (length)) {
        _length = length;
        _data = new char [length + 1];
        optimiseBigString ();
    }
    else {
        static const char EMPTY_STRING[1] = "";
        optimiseSmallString (EMPTY_STRING);
    }
}

String::String () : String ((size_t)0) {}

void String::copyArray (const char* data, size_t length) {
    _length = length;
	_data = new char[length + 1];
	strcpy(_data, data);
    optimiseBigString ();
}

void String::copyFrom (const String& other) {
    copyArray (other._data, other.length ());
}

void String::moveFrom (String&& other) {
    _data = other._data;
	other._data = nullptr;
	_length = other._length;
    optimiseBigString ();
}

void String::free () {
	delete [] _data;
	_data = nullptr;
    _length = 0;
}

String::String (const char* data) { 
	unsigned int lenOfData = strlen (data);
    if (isPossibleToOptimise (lenOfData)) {
        optimiseSmallString (data);
    }
    else {
        copyArray (data, lenOfData);
    }
}

String::String (const String& other) { 
	if (other.isOptimised ()) {
        optimiseSmallString (other.smallString);
    }
    else {
        copyFrom (other);
    }
}

String& String::operator= (const String& other) {
    if (this == &other) {
        return *this;
    }
    unsigned int lenOfData = strlen (other.smallString);
    if (isOptimised () && other.isOptimised ()) {
        optimiseSmallString (other.smallString);
    }
    else if (!isOptimised () && other.isOptimised ()) {
        free ();
        optimiseSmallString (other.smallString);
    }
    else if (isOptimised () && !other.isOptimised ()) {
        copyFrom (other);
    }
    else if (!isOptimised () && !other.isOptimised ()) {
		free();
		copyFrom(other);
    }
	return *this;
}

String::~String () {
    if (!isOptimised ()) {
        free ();
    }
}

String::String (String&& other) {
    if (other.isOptimised ()) {
        optimiseSmallString (other.smallString);
    }
    else {
        moveFrom (std::move (other));
    }
}

String& String::operator= (String&& other) {
    if (this == &other) {
        return *this;
    }
    unsigned int lenOfData = strlen (other.smallString);
    if (isOptimised () && other.isOptimised ()) {
        optimiseSmallString (other.smallString);
    }
    else if (!isOptimised () && other.isOptimised ()) {
        free ();
        optimiseSmallString (other.smallString);
    }
    else if (isOptimised () && !other.isOptimised ()) {
        moveFrom (std::move (other));
    }
    else if (!isOptimised () && !other.isOptimised ()) {
		free();
		moveFrom (std::move (other));
    }
	return *this;
}

String String::substr (size_t begin, size_t howMany) const {
    if (begin + howMany > length ()) {
        throw std::length_error("Error, Substr out of range");
    }
    char temp[SIZE_OF_SMALL_STRING];
    String res2 (howMany);
    bool isOptimisible = isPossibleToOptimise (howMany);
    bool IsOptimised = isOptimised ();
    for (unsigned int i = 0 ; i < howMany ; ++i) {
        if (IsOptimised) {
            temp[i] = smallString[begin + i];
        }
        else {
            (isOptimisible) ? (temp[i] = _data[begin + i]) : (res2._data[i] = _data[begin + i]);
        }
    }
    (isOptimisible) ? (temp[howMany] = '\0') : (res2._data[howMany] = '\0');
    if (isOptimisible) {
        String res1;
        res1.optimiseSmallString (temp);
        return res1;
    }
    return res2;
}

void String::strcatForBigString (unsigned int lenOfResult, const String& lhs, const String& rhs) {
    unsigned int lenOfLhs = lhs.length ();
    bool lhsIsOptimised = lhs.isOptimised ();
    for (unsigned int i = 0 ; i < lenOfLhs ; ++i) {
        (lhsIsOptimised) ? (_data[i] = lhs.smallString[i]) : (_data[i] = lhs._data[i]);
    }
    unsigned int lenOfRhs = rhs.length ();
    bool rhsIsOptimised = rhs.isOptimised ();
    for (unsigned int i = 0 ; i < lenOfRhs ; ++i) {
        (rhsIsOptimised) ? (_data[i + lenOfLhs] = rhs.smallString[i]) : (_data[i + lenOfLhs] = rhs._data[i]);
    }
    _data[lenOfResult] = '\0';
}

String operator+ (const String& lhs, const String& rhs) {
    unsigned int lenOfResult = lhs.length () + rhs.length ();
    if (lhs.isPossibleToOptimise (lenOfResult)) {
        char temp[SIZE_OF_SMALL_STRING];
        temp[0] = '\0';
        strcat (temp, lhs.smallString);
        strcat (temp, rhs.smallString);
        String result;
        result.optimiseSmallString (temp);
        return result;
    }
    String nonOptimisedResult (lenOfResult);
    nonOptimisedResult.strcatForBigString (lenOfResult, lhs, rhs);
    return nonOptimisedResult;
}

String& String::operator+= (const String& other) {
    unsigned int lenOfResult = length () + other.length ();
    if (isPossibleToOptimise (lenOfResult)) {
        String result;
        result = *this + other;
        *this = result;
        return *this;
    }
    if (isOptimised ()) {
        String temp = *this;
        this -> _data = new char [lenOfResult + 1];
        this -> _length = lenOfResult;
        strcatForBigString (lenOfResult, temp, other);
        optimiseBigString ();
        return *this;
    }
    char* resultArr = new char[lenOfResult + 1];

    unsigned int lenOfLhs = length ();
    bool lhsIsOptimised = isOptimised ();
    for (unsigned int i = 0 ; i < lenOfLhs ; ++i) {
        (lhsIsOptimised) ? (resultArr[i] = smallString[i]) : (resultArr[i] = _data[i]);
    }
    unsigned int lenOfRhs = other.length ();
    bool rhsIsOptimised = other.isOptimised ();
    for (unsigned int i = 0 ; i < lenOfRhs ; ++i) {
        (rhsIsOptimised) ? (resultArr[i + lenOfLhs] = other.smallString[i]) : (resultArr[i + lenOfLhs] = other._data[i]);
    }
    resultArr[lenOfResult] = '\0';

    delete [] _data;
    _data = resultArr;
    _length = lenOfResult;
    optimiseBigString ();

    return *this;
}

char& String::operator[] (unsigned int index) {
    if (isOptimised ()) {
        return smallString[index];
    }
    return _data[index];
}

char String::operator[] (unsigned int index) const {
    if (isOptimised ()) {
        return smallString[index];
    }
	return _data[index];
}

const char* String::c_str () const {
    if (isOptimised ()) {
        return smallString;
    }
	return _data;
}

void String::saveStringToBinatyFile (std::ofstream& outFile) const {
    if (!outFile.is_open ()) {
        return;
    }
    size_t len = length ();
    outFile.write ((const char*)&len, sizeof(size_t));
    outFile.write (c_str (), len + 1);
}

void String::saveStringToBinatyFile (const char* fileName) const {
    std::ofstream outFile (fileName, std::ios::out | std::ios::binary);
    saveStringToBinatyFile (outFile);

    outFile.close ();
}

String readStringFromBinaryFile (std::ifstream& inFile) {
    if (!inFile.is_open ()) {
        return String ("");
    }
    size_t len;
    inFile.read ((char*)&len, sizeof(len));
    String result (len);
    if (result.isOptimised ()) {
        inFile.read (result.smallString, len + 1);
    }
    else {
        inFile.read (result._data, len + 1);
    }
    return result;
}

String readStringFromBinaryFile (const char* fileName) {
    std::ifstream inFile (fileName, std::ios::in | std::ios::binary);
    String result = readStringFromBinaryFile (inFile);

    inFile.close ();
    return result;
}

std::ostream& operator<< (std::ostream& os, const String& str) {
	return os << str.c_str();
}

std::istream& operator>>(std::istream& is, String& str)
{
    static const size_t MAX_SIZE_TO_ENTER_STRING = 1024;
	char buff[MAX_SIZE_TO_ENTER_STRING];
	is.getline(buff, MAX_SIZE_TO_ENTER_STRING);
    unsigned int lenOfNewString = strlen (buff);
    if (str.isOptimised () == false) {
        str.free ();
    }
    if (str.isPossibleToOptimise (lenOfNewString)) {
        str.optimiseSmallString (buff);
        return is;
    }
    str.copyArray (buff, lenOfNewString);
	return is;
}

bool operator== (const String& lhs, const String& rhs) {
    return (strcmp (lhs.c_str (), rhs.c_str ()) == 0);
}

bool operator!= (const String& lhs, const String& rhs) {
    return (strcmp (lhs.c_str (), rhs.c_str ()) != 0);
}

bool operator< (const String& lhs, const String& rhs) {
    return (strcmp (lhs.c_str (), rhs.c_str ()) < 0);
}

bool operator> (const String& lhs, const String& rhs) {
    return (strcmp (lhs.c_str (), rhs.c_str ()) > 0);
}

bool operator<= (const String& lhs, const String& rhs) {
    return (strcmp (lhs.c_str (), rhs.c_str ()) <= 0);
}

bool operator>= (const String& lhs, const String& rhs) {
    return (strcmp (lhs.c_str (), rhs.c_str ()) >= 0);
}


