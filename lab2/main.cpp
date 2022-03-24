#include "btree.hpp"

#include <iostream>


void t_binary_tree::test () {
  auto a = m_root->breadth_first();
  for (auto const n : a) { std::cout << n->key() << "\n"; }
}


int main () {
  t_binary_tree tree;
  tree.test();
}