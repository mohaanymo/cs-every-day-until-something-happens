#include "core/hashmap.hpp"
#include "core/linkedlist.hpp"
#include "core/queue.hpp"
#include "core/stack.hpp"

int main() {
  // Map<std::string> map(10);
  // map.insert("name", "mohamed");
  // map.insert("name", "omar");
  // std::cout << map.get("name") << std::endl;
  LinkedList<int> l;
  l.insert(10);
  l.insert(20);
  l.insert(30);
  l.traverse();
  return 0;
}
