#pragma once

#include <map>
#include <stdexcept>

template<typename K, typename V>
class BiMap {
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

    bool eraseByKey(const K &key) {
        auto it = forward.find(key);

        if (it == forward.end())
            return false;

        reverse.erase(it->second);
        forward.erase(it);

        return true;
    }

    bool eraseByValue(const V &value) {
        auto it = reverse.find(value);

        if (it == reverse.end())
            return false;

        forward.erase(it->second);
        reverse.erase(it);

        return true;
    }

    V &atKey(const K &key) {
        return forward.at(key);
    }

    const V &atKey(const K &key) const {
        return forward.at(key);
    }

    K &atValue(const V &value) {
        return reverse.at(value);
    }

    const K &atValue(const V &value) const {
        return reverse.at(value);
    }

    bool containsKey(const K &key) const {
        return forward.contains(key);
    }

    bool containsValue(const V &value) const {
        return reverse.contains(value);
    }

    size_t size() const {
        return forward.size();
    }

    void clear() {
        forward.clear();
        reverse.clear();
    }
};
