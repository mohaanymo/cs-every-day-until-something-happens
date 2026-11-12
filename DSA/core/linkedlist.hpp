#pragma once
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T> struct Node {
  T val;
  Node *next;
  Node *prev;
};

template <typename T> class LinkedList {
private:
  Node<T> *head;
  Node<T> *tail;
  size_t len;

public:
  LinkedList() : head(nullptr), tail(nullptr), len(0) {}

  LinkedList(const LinkedList &other) : head(nullptr), tail(nullptr), len(0) {
    Node<T> *tmp = other.head;
    while (tmp != nullptr) {
      insert(tmp->val);
      tmp = tmp->next;
    }
  }

  void swap(LinkedList &other) noexcept {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(len, other.len);
  }

  LinkedList &operator=(LinkedList other) {
    this->swap(other);
    return *this;
  }
  ~LinkedList() {

    while (head != nullptr) {
      Node<T> *tmp = head;
      head = head->next;
      delete tmp;
    }
  }

  size_t length() const { return len; }
  bool isEmpty() const { return head == nullptr; }
  // O(1)
  void insert(T val) {
    Node<T> *newNode = new Node<T>{val, nullptr, nullptr};
    len++;

    if (head == nullptr) {
      head = newNode;
      return;
    }

    if (tail == nullptr) {
      tail = newNode;
      head->next = tail;
      tail->prev = head;
      return;
    }

    Node<T> *prevNode = tail->prev;
    prevNode->next = newNode;
    newNode->next = tail;
    tail->prev = newNode;
  }

  void remove(size_t index) {
    if (head == nullptr) {
      return;
    }

    Node<T> *tmp = head;
    for (size_t i = 0; i < index; i++) {
      if (tmp->next == nullptr) {
        break;
      }
      tmp = tmp->next;
    }

    if (tmp->prev) {
      tmp->prev->next = tmp->next;
    } else {
      head = tmp->next;
    }

    if (tmp->next) {
      tmp->next->prev = tmp->prev;
    }
    len--;
    delete tmp;
  }
  void set(size_t index, T val) {
    if (head == nullptr) {
      throw std::out_of_range("linkedlist is empty");
    }
    if (index > length() - 1) {
      throw std::out_of_range("this index does not exist");
    }
    Node<T> *tmp = head;
    for (size_t i = 0; i < index; i++) {
      if (tmp->next == nullptr) {
        break;
      }
      tmp = tmp->next;
    }
    tmp->val = val;
  }
  T get(size_t index) const {
    if (head == nullptr) {
      throw std::out_of_range("linkedlist is empty");
    }
    Node<T> *tmp = head;
    for (size_t i = 0; i < index; i++) {
      if (tmp->next == nullptr) {
        break;
      }
      tmp = tmp->next;
    }
    return tmp->val;
  }

  void traverse() const {
    Node<T> *tmp = head;
    while (tmp != nullptr) {
      std::cout << tmp->val << std::endl;
      tmp = tmp->next;
    }
  }
};
