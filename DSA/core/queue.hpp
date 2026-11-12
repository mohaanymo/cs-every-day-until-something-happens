#pragma once
#include "linkedlist.hpp"
#include <stdexcept>

template <typename T> class Queue {
private:
  LinkedList<T> data;

public:
  Queue() : data() {}
  bool isEmpty() { return data.length() == 0; }

  void enqueue(T v) { data.insert(v); }

  T dequeue() {
    if (isEmpty()) {
      throw std::out_of_range("the queue is empty");
    }
    T val = data.get(0);
    data.remove(0);
    return val;
  }
};
