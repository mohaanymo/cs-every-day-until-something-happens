#pragma once
#include "array.hpp"
#include <stdexcept>

template <typename T> class Stack {
private:
  Array<T> data;

public:
  Stack() : data(10) {}

  bool isEmpty() { return data.size() == 0; }

  void push(T v) { data.push(v); }
  void pop() {
    if (isEmpty()) {
      throw std::out_of_range("the stack is empty");
    }
    data.pop();
  }

  T peek() {
    if (isEmpty()) {
      throw std::out_of_range("the stack is empty");
    }
    return data[data.size() - 1];
  }

  size_t size() { return data.size(); }
};
