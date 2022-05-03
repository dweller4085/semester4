#include "../binary_tree/binary_tree.hpp"

class t_search_tree : public t_binary_tree {
public:
  t_search_tree();
  t_search_tree(t_search_tree const &);
  ~t_search_tree();

  int key_max ();
  int key_min ();
  void add_node (int);

  bool remove_node_key (int);
	bool remove_node_index (t_index);

  t_index index (int);
  int level (int);

  t_binary_tree operator = (t_binary_tree const &);
};
