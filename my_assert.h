#pragma once
#include <assert.h>

inline void my_assert(const bool exp) {
    if (!exp)
        assert(exp);
}

