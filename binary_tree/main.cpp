#include "btree.hpp"

#include <iostream>

template <typename T>
inline void p (T const & out) {
  std::cout << out << "\n";
}

template <> inline void p (bool const & out) {
  std::cout << ((out) ? "yes" : "no") << "\n";
}

int main () {
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

  t_binary_tree another_tree;
  another_tree = tree.copy(3);
  p("another_tree:");
  another_tree.print();

  p("tree. keymax, keymin, keysum:");
  std::cout << tree.key_max() << "\n" << tree.key_min() << "\n" << tree.key_sum() << "\n";

  p("another_tree.key(2), index (666):");
  p(another_tree.key(2));
  p(another_tree.index(666));

  p("tree node count:");
  p(tree.node_cnt());

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

  t_binary_tree empty;
  p("here's empty:");
  empty.print();

  empty.delete_children(4);
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
  p(tree.key(4));
  p("tree after removing the node 4:");
  tree.remove_node_index(4);
  tree.print();
  
}
