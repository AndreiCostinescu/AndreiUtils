//
// Created by Andrei Costinescu (andrei.costinescu@tum.de) on 14.03.2021.
//

#ifndef ANDREIUTILS_CLASSES_FORKSHAREDVARIABLE_HPP
#define ANDREIUTILS_CLASSES_FORKSHAREDVARIABLE_HPP

#if defined(_WIN32) || defined(_WIN64)

#include <mman.h>

#else
#include <sys/mman.h>
#endif

template<class T>
class ForkSharedVariable {
public:
    ForkSharedVariable() {
        this->variable = (T *) mmap(nullptr, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    }

    explicit ForkSharedVariable(T initialValue) : ForkSharedVariable() {
        *(this->variable) = initialValue;
    }

    ~ForkSharedVariable() {
        munmap(this->variable, sizeof(T));
    }

    T get() {
        return *(this->variable);
    }

    void set(T value) {
        *(this->variable) = value;
    }

private:
    T *variable;
};


#endif //ANDREIUTILS_CLASSES_FORKSHAREDVARIABLE_HPP
