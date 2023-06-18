#pragma once

#include <iostream>


const unsigned int INITIAL_CAPACITY = 8;

template <typename T>
class Vector {
private:

    T* data;
    size_t capacity = INITIAL_CAPACITY;
    size_t count = 0;

    void copyFrom (const Vector<T>& other);
    void moveFrom (Vector<T>&& other);
    void free ();

    void resize ();

public:

    Vector ();
    Vector (const Vector<T>&);
    Vector (Vector<T>&&);
    Vector& operator= (const Vector<T>&);
    Vector& operator= (Vector<T>&&);
    ~Vector ();

    bool isEmpty () const;
    size_t getNumberOfElements () const;

    void pushBack (const T&);
    void pushBack (T&&);

    void popBack ();

    void insert (unsigned int index, const T&);
    void insert (unsigned int index, T&&); 

    void erase (unsigned int index);

    void clear ();

    bool contains (const T&) const;

    const T& operator [] (unsigned int) const;
    T& operator [] (unsigned int);

    void print (char separator) const;
    void print () const;
};

template <typename T>
void Vector<T>::copyFrom (const Vector<T>& other) {
    capacity = other.capacity;
    count = other.count;
    data = new T [capacity];
    for (unsigned int i = 0 ; i < capacity ; ++i) {
        data[i] = std::move (other.data[i]);
    }
}

template <typename T>
void Vector<T>::moveFrom (Vector<T>&& other) {
    capacity = other.capacity;
    count = other.count;
    data = other.data;
    other.data = nullptr;
}

template <typename T>
void Vector<T>::free () {
    delete [] data;
    data = nullptr;
    capacity = INITIAL_CAPACITY;
    count = 0;
}

template <typename T>
Vector<T>::Vector () {
    data = new T [capacity];
}

template <typename T>
Vector<T>::Vector (const Vector<T>& other) {
    copyFrom (other);
}

template <typename T>
Vector<T>::Vector (Vector<T>&& other) {
    moveFrom (std::move(other));
}

template <typename T>
Vector<T>& Vector<T>::operator= (const Vector<T>& other) {
    if (this != &other) {
        free ();
        copyFrom (other);
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator= (Vector<T>&& other) {
    if (this != &other) {
        free ();
        copyFrom (std::move(other));
    }
    return *this;
}

template <typename T>
Vector<T>::~Vector () {
    free ();
}

template <typename T>
void Vector<T>::resize () {
    static const unsigned int RESIZE_COEFFICINET = 2;
    capacity *= RESIZE_COEFFICINET;
    T* temp = new T [capacity];
    for (unsigned int i = 0 ; i < count ; ++i) {
        temp[i] = std::move(data[i]);
    }
    delete [] data;
    data = temp;
}

template <typename T>
bool Vector<T>::isEmpty () const {
    return count == 0;
}

template <typename T>
size_t Vector<T>::getNumberOfElements () const {
    return count;
}

template <typename T>
void Vector<T>::pushBack (const T& element) {
    if (count == capacity) {
        resize ();
    }
    data[count++] = element;
}

template <typename T>
void Vector<T>::pushBack (T&& element) {
    if (count == capacity) {
        resize ();
    }
    data[count++] = std::move(element);
}

template <typename T>
void Vector<T>::popBack () {
    if (isEmpty ()) {
        throw std::logic_error ("Emty vector!");
    }
    --count;
}

template <typename T>
void Vector<T>::insert (unsigned int index, const T& element) {
    if (index >= capacity) {
        throw std::logic_error ("Invalid index!");
    }
    if (count == capacity) {
        resize ();
    }
    for (unsigned int i = count ; i > index ; --i) {
        data[i] = data[i - 1];
    }
    ++count;
    data[index] = element;
}

template <typename T>
void Vector<T>::insert (unsigned int index, T&& element) {
    if (index >= capacity) {
        throw std::logic_error ("Invalid index!");
    }
    if (count == capacity) {
        resize ();
    }
    for (unsigned int i = count ; i > index ; --i) {
        data[i] = data[i - 1];
    }
    ++count;
    data[index] = std::move(element);
}

template <typename T>
void Vector<T>::erase (unsigned int index) {
    if (index >= count) {
        throw std::logic_error ("Invalid index!");
    }
    for (unsigned int i = index ; i < count - 1 ; ++i) {
        data[i] = std::move(data[i + 1]);
    }
    --count;
}

template <typename T>
void Vector<T>::clear () {
    // basically free () and default constructor
    delete [] data;
    count = 0;
    capacity = INITIAL_CAPACITY;
    data = new T [capacity];
}

template <typename T>
bool Vector<T>::contains (const T& element) const {
    for (unsigned int i = 0 ; i < count ; ++i) {
        if (element == data[i]) {
            return true;
        }
    }
    return false;
}

template <typename T>
const T& Vector<T>::operator [] (unsigned int index) const {
    return data[index];
}

template <typename T>
T& Vector<T>::operator [] (unsigned int index) {
    return data[index];
}

template <typename T>
void Vector<T>::print (char separator) const { // expected input is space, new line, comma, semi-column, etc.
    if (count == 0) {
        return;
    }
    for (unsigned int i = 0 ; i < count ; ++i) {
        std::cout << data[i]; // required operator << (ostream&, const T&);
        if (i != count - 1) {
            std::cout << separator;
        }
    }
}

template <typename T>
void Vector<T>::print () const {
    static const char SEPARATOR = ' ';
    print (SEPARATOR);
}
