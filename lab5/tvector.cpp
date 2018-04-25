#ifndef TVECTOR_CPP
#define TVECTOR_CPP

#include "tvector.h"

template<class T>
TVector<T>::TVector(const int size) {
    this->RealSize = size;
    this->LastIndex = size;
    if (0 == RealSize) {
        this->Data = nullptr;
    }
    else {
        this->Data = (T*) malloc(this->RealSize * sizeof(T));
    }
}

//template <class T>
//IVector<T>::IVector() {
//    this->IVector(this->STEP);
//}


template <class T>
TVector<T>::~TVector() {
    free(this->Data);
    this->Data = nullptr;
}

template <class T>
void TVector<T>::PushBack(T el) {
    if (this->RealSize == this->LastIndex) {
        auto tmpData = this->Data;

        if (0 == this->RealSize)
            this->RealSize = 1;
        else
            this->RealSize *= this->STEP;

        this->Data = (T*) realloc(this->Data, this->RealSize * sizeof(T));

        if (nullptr == this->Data) {
            this->Data = tmpData;
            this->RealSize /= this->STEP;
            throw std::bad_alloc();
        }
    }
    this->Data[this->LastIndex] = el;
    this->LastIndex += 1;
}

template <class T>
T &TVector<T>::operator [](unsigned int index) {
    T &res = this->Data[index];
    return res;
}

template<class T>
size_t TVector<T>::Size() {
    return this->LastIndex;
}

template<class T>
TIterator<T> TVector<T>::begin() {
    TIterator<T> res(this->Data);
    return res;
}

template<class T>
TIterator<T> TVector<T>::end() {
    TIterator<T> res(this->Data, this->LastIndex);
    return res;
}

#endif // TVECTOR_CPP
