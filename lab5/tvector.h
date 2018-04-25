#ifndef TVECTOR_H
#define TVECTOR_H

#include <stdlib.h>
#include <exception>
#include <new>

#include "titerator.h"

template <class T>
class TVector
{
public:
    TVector(const int size);
    TVector() : TVector(0) {}
    virtual ~TVector();

    void PushBack(T el);
    T &operator [](unsigned int index);
    size_t Size();

    TIterator<T> begin();
    TIterator<T> end();

protected:
    T *Data;
    unsigned int RealSize;
    unsigned int LastIndex;
    static const unsigned int STEP = 2;
};

#include "tvector.cpp"

#endif // TVECTOR_H
