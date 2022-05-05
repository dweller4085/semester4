#include "binary_tree.hpp"

#include <iostream>

int main () {
  t_binary_tree a;
  a.test_a();


  //for (auto const & key : a.keys()) std::cout << "L" << a.level(key) << ": " << key << "\n";
  std::cout << "\n";
  a.print();
  std::cout << "\n";
  a.remove_node_key (4);
  a.print();
}
