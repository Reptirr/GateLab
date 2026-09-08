#pragma once
#include <memory>

template <typename T>
struct WeakPtrComparator {
    bool operator()(
        const std::weak_ptr<T>& ptr1,
        const std::weak_ptr<T>& ptr2
    ) const {
        return ptr1.owner_before(ptr2);
    }
};