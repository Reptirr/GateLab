#pragma once

#include <map>
#include <stdexcept>
#include <type_traits>

template<typename K, typename V>
class BiMap {
    static_assert(!std::is_same_v<K, V>,
                  "BiMap requires K and V to be distinct types for overload resolution");

    std::map<K, V> forward;
    std::map<V, K> reverse;

public:
    bool insert(const K &key, const V &value) {
        if (forward.contains(key) || reverse.contains(value))
            return false;

        forward.emplace(key, value);
        reverse.emplace(value, key);

        return true;
    }

    bool erase(const K &key) {
        auto it = forward.find(key);

        if (it == forward.end())
            return false;

        reverse.erase(it->second);
        forward.erase(it);

        return true;
    }

    bool erase(const V &value) {
        auto it = reverse.find(value);

        if (it == reverse.end())
            return false;

        forward.erase(it->second);
        reverse.erase(it);

        return true;
    }

    V &at(const K &key) { return forward.at(key); }
    const V &at(const K &key) const { return forward.at(key); }

    K &at(const V &value) { return reverse.at(value); }
    const K &at(const V &value) const { return reverse.at(value); }

    bool contains(const K &key) const { return forward.contains(key); }
    bool contains(const V &value) const { return reverse.contains(value); }

    size_t size() const { return forward.size(); }

    void clear() { forward.clear(); reverse.clear(); }
};