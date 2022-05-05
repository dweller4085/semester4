#include "binary_tree.hpp"

#include <iostream>

int main () {
  t_binary_tree a;
  for (int i = 0; i < 1000; i++) {
    a.add_node(i);
    t_binary_tree b = a;
  }
}
