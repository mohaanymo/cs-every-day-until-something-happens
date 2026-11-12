#pragma once
#include <iostream>
#include <utility>

template <typename T>

class Array {
private:
  T *data;
  size_t cap, len = 0;

  void realloc() {
    size_t tmp_cap = cap * 2;
    T *tmp_data = new T[tmp_cap];
    for (size_t i = 0; i < len; i++) {
      tmp_data[i] = data[i];
    }
    cap = tmp_cap;
    delete[] data;
    data = tmp_data;
  }

public:
  Array(size_t capacity) {
    cap = capacity;
    if (cap <= 0) {
      cap = 1;
    }
    data = new T[cap];
  }

  // Copy constructor: the copy gets its own memory (deep copy).
  Array(const Array &other) {
    cap = other.cap;
    len = other.len;
    data = new T[cap];
    for (size_t i = 0; i < len; i++) {
      data[i] = other.data[i];
    }
  }

  ~Array() { delete[] data; }

  void swap(Array &other) noexcept {
    std::swap(cap, other.cap);
    std::swap(data, other.data);
    std::swap(len, other.len);
  }

  Array &operator=(Array other) {
    this->swap(other);
    return *this;
  }

  void push(T v) {
    if (len == cap) {
      realloc();
    }
    data[len] = v;
    len++;
  }

  void pop() {
    if (len == 0) {
      return;
    }
    len--;
  }

  void display() {
    for (size_t i = 0; i < len; i++) {
      std::cout << data[i] << '\n';
    }
  }

  size_t size() const { return len; }
  size_t capacity() const { return cap; }
  T &operator[](size_t index) { return data[index]; }
  const T &operator[](size_t index) const { return data[index]; }
};
