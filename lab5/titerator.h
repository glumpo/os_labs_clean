#ifndef TVECTORITER_H
#define TVECTORITER_H

#include <inttypes.h>

template <class T>
class TIterator
{
public:
    TIterator(T *p, uint16_t n = 0) {
        this->cur = p + n;
    }
    virtual ~TIterator() {}


    // TODO: Remove code duplication
    T& operator *() {
        T& res = *cur;
        return res;
    }

    T& operator ->() {
        T& res = *cur;
        return res;
    }

    void step_forward() {
        this->cur += 1;
    }

    TIterator operator ++() {
        step_forward();
        return *this;
    }

    TIterator operator ++(int) {
        auto cur = *this;
        this->step_forward();
        return cur;
    }

    bool operator ==(TIterator r) {
        return this->cur == r.cur;
    }

    bool operator !=(TIterator r) {
        return !(*this == r);
    }

private:
    T *cur;
};


#include "titerator.cpp"
#endif // TVECTORITER_H
