#pragma once

const unsigned int SIZE_OF_SMALL_STRING = sizeof (char*) + sizeof (size_t);
const unsigned int MAX_LENGTH_OF_SMALL_STRING = SIZE_OF_SMALL_STRING - 1;
const size_t MAX_LENGTH_OF_BIG_STRING = 2147483648; // 2^31
const size_t LAST_BYTE_OF_SIZE_T = sizeof(size_t) - 1;

class String {
private:
    union {
        char smallString[SIZE_OF_SMALL_STRING];
        struct {
            char* _data;
            size_t _length;
        };
    };

    bool isPossibleToOptimise (size_t) const;
    void optimiseSmallString (const char*);
    void optimiseBigString ();
    bool isOptimised () const;

    void strcatForBigString (unsigned int, const String&, const String&);

    explicit String (size_t);

    void copyArray (const char*, size_t);
    void copyFrom (const String&);
    void moveFrom (String&&);
	void free();

public:

    String();
	String(const char*);
	String(const String&);
	String& operator=(const String&);
	~String();
    String (String&&);
    String& operator= (String&&);

	size_t length() const;

    String substr(size_t, size_t) const;

	friend String operator+(const String&, const String&);
	String& operator+=(const String&);

	char& operator[](unsigned int);
	char operator[](unsigned int) const;

    const char* c_str() const;

    void saveStringToBinatyFile (std::ofstream&) const;
    void saveStringToBinatyFile (const char* fileName) const;
    friend String readStringFromBinaryFile (std::ifstream&);
    friend String readStringFromBinaryFile (const char* fileName);
	
	friend std::istream& operator>> (std::istream&, String&);
};

String readStringFromBinaryFile (std::ifstream&);
String readStringFromBinaryFile (const char* fileName);

std::ostream& operator<< (std::ostream&, const String&);

bool operator< (const String&, const String&);
bool operator<= (const String&, const String&);
bool operator>= (const String&, const String&);
bool operator> (const String&, const String&);
bool operator== (const String&, const String&);
bool operator!= (const String&, const String&);