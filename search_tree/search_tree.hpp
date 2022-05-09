#include "../binary_tree/binary_tree.hpp"

class t_search_tree : public t_binary_tree {
protected:
  bool remove_node (t_node *) override;
  t_node * with_key (int) const override;

public:
  t_search_tree ();
  t_search_tree (t_search_tree const &);
  ~t_search_tree () override = default;

  t_search_tree operator = (t_search_tree const &);

  t_search_tree get_copy (int);

  t_search_tree & add_node (int);

  int level (int) const override;

  int key_max () const override;
  int key_min () const override;
};
