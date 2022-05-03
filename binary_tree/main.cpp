#include "binary_tree.hpp"

#include <iostream>

class t_test {
 public:
  t_test * l;
  t_test * r;
  int key;
};

int main () {
  t_test t;
  std::cout << t.key << " " << (t.l == nullptr) << std::endl;
}