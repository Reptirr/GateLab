#pragma once

inline void my_assert(const bool exp) {
    if (!exp) {
        // create exception
        int *x = nullptr;
        *x = 1;
    }
}

