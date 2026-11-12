#pragma once
#include "array.hpp"
#include "linkedlist.hpp"
#include <cstddef>
#include <string>

template <typename T> struct Entry {
  std::string key;
  T value;
};

template <typename T> class Map {
private:
  static constexpr double MAX_LOAD_FACTOR = 0.75;
  static constexpr size_t DEFAULT_CAPACITY = 8;

  Array<LinkedList<Entry<T>>> buckets;
  size_t size;

  size_t hash(const std::string &key, size_t capacity) {
    size_t hashValue = 0;
    for (char c : key) {
      hashValue = hashValue * 31 + c;
    }
    return hashValue % capacity;
  }

  void rehash() {
    size_t newCapacity = buckets.size() * 2;
    Array<LinkedList<Entry<T>>> newBuckets(newCapacity);
    for (size_t i = 0; i < newCapacity; i++) {
      newBuckets.push(LinkedList<Entry<T>>());
    }

    for (size_t i = 0; i < buckets.size(); i++) {
      LinkedList<Entry<T>> &bucketList = buckets[i];

      for (size_t j = 0; j < bucketList.length(); j++) {
        Entry<T> entry = bucketList.get(j);
        size_t newIndex = hash(entry.key, newCapacity);
        newBuckets[newIndex].insert(entry);
      }
    }

    buckets.swap(newBuckets);
  }

public:
  Map() : Map(DEFAULT_CAPACITY) {}
  Map(size_t initial_cap) : buckets(initial_cap), size(0) {
    for (size_t i = 0; i < initial_cap; i++) {
      buckets.push(LinkedList<Entry<T>>());
    }
  }

  void insert(const std::string &key, T val) {
    double currentLoadFactor = static_cast<double>(size) / buckets.size();
    if (currentLoadFactor >= MAX_LOAD_FACTOR) {
      rehash();
    }

    size_t index = hash(key, buckets.size());
    Entry<T> entry{key, val};
    for (size_t i = 0; i < buckets[index].length(); i++) {
      if (buckets[index].get(i).key == key) {
        buckets[index].set(i, entry);
        return;
      }
    }
    buckets[index].insert(entry);
    size++;
  }

  void remove(std::string &key) {
    size_t index = hash(key, buckets.size());
    LinkedList<Entry<T>> &bucketList = buckets[index];

    for (size_t i = 0; i < bucketList.length(); i++) {
      Entry<T> e = bucketList.get(i);
      if (e.key == key) {
        bucketList.remove(i);
        return;
      }
    }
  }
};
