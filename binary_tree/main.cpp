#include "binary_tree.hpp"

#include <iostream>



template <typename T>
inline void p (T const & out) {
  std::cout << out << "\n";
}

template <> inline void p (bool const & out) {
  std::cout << ((out) ? "yes" : "no") << "\n";
}

void test1 () {
  t_binary_tree tree;
  p("height:");
  p(tree.height());
  p("tree:");
  tree.print ();
  tree.add_node (666);
  p("after add_node:");
  tree.print ();
  p("tree leaves:");
  tree.print_leaves();

  tree.add_node(10).add_node(20).add_node(40);

  t_binary_tree another_tree;
  another_tree = tree;
  p("another_tree:");
  another_tree.print();

  p("tree. keymax, keymin, keysum:");
  std::cout << tree.key_max() << "\n" << tree.key_min() << "\n" << tree.key_sum() << "\n";

  p("another_tree.key(2), index (666):");
  p(another_tree.key_of(2));
  p(another_tree.index_of(666));

  p("tree node count:");
  p(tree.node_count());

  p("level of 666 in another_tree:");
  p(another_tree.level(666));

  p("is another_tree balanced?");
  p(another_tree.is_balanced());

  another_tree.remove_node_key(666);

  p("another_tree after amputation:");
  another_tree.print();

  p("is another_tree balanced now?");
  p(another_tree.is_balanced());

  p("another_tree's keys:");
  for (auto key : another_tree.keys()) std::cout << key << " ";
  std::cout << "\n";

  t_binary_tree empty = another_tree.add_node(555).add_node(222);
  p("here's empty:");
  empty.print();

  empty.clear_subtrees(1);
  p("empty got cleared:");
  empty.print();

  p("is empty really empty?");
  p(empty.is_empty());

  p("clearing empty:");
  empty.clear();
  empty.print();

  p("is empty now empty?");
  p(empty.is_empty());

  p("is tree empty?");
  p(tree.is_empty());

  p("tree at this point:");
  tree.print();
  p("node at index 4 has key:");
  p(tree.key_of(3));
  p("tree after removing the node 4:");
  tree.remove_node_index(3);
  tree.print();
}

void test2 () {
  t_binary_tree a;

  std::cout << "Have an empty tree:" << "\n";
  a.print();
  std::cout << "\n\n";

  for (int i = 0; i < 63; i++) {
    a.add_node(i);
  }

  std::cout << "After 63 add_node(i), i = 0..62" << "\n";
  a.print();
  std::cout << "\n\n";

  for (int i = 0; i < 63; i++) {
    a.remove_node_key(i);
  }

  std::cout << "After 63 remove_node_key(i), i = 0..62" << "\n";
  a.print();
}

int main () {
  
}
