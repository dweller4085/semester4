#include "search_tree.hpp"

#include <iostream>

void test1 () {
  t_search_tree st;
  st.add_node(3).add_node(5).add_node(6).add_node(2).add_node(1);

  std::cout << "st: " << "\n";
  st.print();

  std::cout << "\n";

  t_search_tree stcopy {st};

  std::cout << "stcopy:" << "\n";
  stcopy.print();

  std::cout << "stcopy keymax: " << stcopy.key_max() << "\n";
  std::cout << "stcopy keymin: " << stcopy.key_min() << "\n";

  std::cout << "Level of (1) in stcopy: " << stcopy.level(1) << "\n";

  std::cout << "\n";

  std::cout << "st after add_node(2):" << "\n";
  st.add_node(666).print();

  stcopy = st.get_copy (2);

  std::cout << "stcopy after = st.get_copy (2):" << "\n";
  stcopy.print();
}

int main () {

}
