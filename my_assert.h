#pragma once
#include <iostream>

inline void my_assert(const bool exp) {
    if (!exp) {

        std::cout << "assert failed" << std::endl;

        // create exception
        int *x = nullptr;
        *x = 1;
    }
}

