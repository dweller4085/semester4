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

void test2 () {
  t_search_tree st;
  st.add_node(7);
  for (int i = 0; i < 15; i+=2) {
    st.add_node(i);
    std::cout << "i: " << i << "\n";
    st.print();
    std::cout << "\n";
  }

  std::cout << "---- removing now ----\n";

  for (int i = 0; i < 15; i+=2) {
    st.remove_node_key(i);
    std::cout << "i: " << i << "\n";
    st.print();
    std::cout << "\n";
  }

}

void test3 () {
  t_search_tree st;
  st.add_node(6).add_node(4).add_node(8).add_node(3).add_node(5).add_node(7).add_node(9);

  auto c {st};
  std::cout << "\n\n---\n";
  c.print();


  std::cout << "\ndeleting leaf (7):\n";
  c.remove_node_key (7);
  c.print();

  std::cout << "\ndeleting leaf (5):\n";
  c.remove_node_key (5);
  c.print();

  auto d {c};

  std::cout << "\ndeleting a node that has only one child (8):\n";
  c.remove_node_key (8);
  c.print();

  std::cout << "\ndeleting a node that has only one child (4):\n";
  c.remove_node_key (4);
  c.print();

  std::cout << "\n\n---\n";
  d.remove_node_key (4);
  d.remove_node_key (3);
  d.print();

  std::cout << "\ndeleting a root that has only one child (6):\n";
  d.remove_node_key (6);
  d.print();

  c = st;
  std::cout << "\n\n---\n";
  c.print();

  std::cout << "\ndeleting a node that has two children and a successor by the right hand(8):\n";
  c.remove_node_key (8);
  c.print();

  c = st;
  std::cout << "\n\n---\n";
  c.print();

  std::cout << "\ndeleting a node that is also a root that has two children and a successor that is not by the right hand(8):\n";
  c.remove_node_key (6);
  c.print();

}

void test_ost () {
  std::vector <int> d = {10, 20, 30, 40};
  std::vector <int> p = {2, 1, 1, 5};
  std::vector <int> q = {1, 10, 1, 1, 10};
  
  t_search_tree ost = t_search_tree::build_optimal_search_tree (d, p, q);

  ost.print();
}

int main () {
  
}
