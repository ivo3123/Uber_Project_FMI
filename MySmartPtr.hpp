#include <iostream>


template <typename T, const unsigned int D>
class MySmartPtr {
private:

    T* data = nullptr;
    size_t* ptrCount = nullptr;

    void copyFrom (const MySmartPtr<T, D>&);
    void moveFrom (MySmartPtr<T, D>&&);
    void free ();

public:

    MySmartPtr ();
    MySmartPtr (T*);

    MySmartPtr (const MySmartPtr<T, D>&);
    MySmartPtr (MySmartPtr<T, D>&&);

    MySmartPtr<T, D>& operator= (const MySmartPtr<T, D>&);
    MySmartPtr<T, D>& operator= (MySmartPtr<T, D>&&);

    ~MySmartPtr ();

    const T& operator* () const;
    T& operator* ();

    const T* operator-> () const;
    T* operator-> ();

    void clear ();
    bool isInitialised () const;
};

template <typename T, const unsigned int D>
void MySmartPtr<T, D>::copyFrom (const MySmartPtr<T, D>& other) {
    ptrCount = other.ptrCount;
    if (*ptrCount == D) {
        throw std::logic_error ("Getting over the limit of the amount of pointers!");
    }
    data = other.data;
    if (data != nullptr) {
        *ptrCount += 1;
    }
}

template <typename T, const unsigned int D>
void MySmartPtr<T, D>::moveFrom (MySmartPtr<T, D>&& other) {
    data = other.data;
    ptrCount = other.ptrCount;
    other.data = nullptr;
}

template <typename T, const unsigned int D>
void MySmartPtr<T, D>::free () {
    if (data == nullptr && ptrCount == nullptr) {
        return;
    }
    if (*ptrCount == 1) {
        delete data;
        delete ptrCount;
    }
    else {
        *ptrCount -= 1;
    }
}

template <typename T, const unsigned int D>
MySmartPtr<T, D>::MySmartPtr () {
    data = nullptr;
    ptrCount = nullptr;
}

template <typename T, const unsigned int D>
MySmartPtr<T, D>::MySmartPtr (T* data) {
    if (D == 0) {
        throw std::logic_error ("The limit of pointers is set to zero!");
    }
    this -> data = data;
    if (this -> data) {
        ptrCount = new size_t (1);
    }
}

template <typename T, const unsigned int D>
MySmartPtr<T, D>::MySmartPtr (const MySmartPtr<T, D>& other) {
    copyFrom (other);
}

template <typename T, const unsigned int D>
MySmartPtr<T, D>::MySmartPtr (MySmartPtr<T, D>&& other) {
    moveFrom (std::move(other));
}

template <typename T, const unsigned int D>
MySmartPtr<T, D>& MySmartPtr<T, D>::operator= (const MySmartPtr<T, D>& other) {
    if (this != &other) {
        free ();
        copyFrom (other);
    }
    return *this;
}

template <typename T, const unsigned int D>
MySmartPtr<T, D>& MySmartPtr<T, D>::operator= (MySmartPtr<T, D>&& other) {
    if (this != &other) {
        free ();
        moveFrom (std::move(other));
    }
    return *this;
}

template <typename T, const unsigned int D>
MySmartPtr<T, D>::~MySmartPtr () {
    free ();
}

template <typename T, const unsigned int D>
const T& MySmartPtr<T, D>::operator* () const {
    if (data == nullptr) {
        throw std::runtime_error ("Pointer not set!");
    }
    return *data;
}

template <typename T, const unsigned int D>
T& MySmartPtr<T, D>::operator* () {
    if (data == nullptr) {
        throw std::runtime_error ("Pointer not set!");
    }
    return *data;
}

template <typename T, const unsigned int D>
const T* MySmartPtr<T, D>::operator-> () const {
    return data;
}

template <typename T, const unsigned int D>
T* MySmartPtr<T, D>::operator-> () {
    return data;
}

template <typename T, const unsigned int D>
void MySmartPtr<T,D>::clear () {
    free ();
    copyFrom (MySmartPtr<T, D> ());
}

template <typename T, const unsigned int D>
bool MySmartPtr<T,D>::isInitialised () const {
    return data != nullptr;
}