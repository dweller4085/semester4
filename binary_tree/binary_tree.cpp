#include "binary_tree.hpp"

#include <stdexcept>
#include <iostream>

t_binary_tree::t_node::t_node () :
  left (nullptr), right (nullptr), key (0)
{}

t_binary_tree::t_node::t_node (t_node * left, t_node * right, int key) :
  left (left), right (right), key (key)
{}

t_binary_tree::t_node::t_node (int key) :
  left (nullptr), right (nullptr), key (key)
{}

int t_binary_tree::t_node::children_count () const {
  return static_cast<int>(left != nullptr) + static_cast<int>(right != nullptr);
}

t_binary_tree::t_binary_tree () :
  m_root (nullptr)
{}
t_binary_tree::t_binary_tree (t_binary_tree const & other) {
  copy (other.m_root);
}
t_binary_tree::~t_binary_tree () {
  clear();
}

t_binary_tree & t_binary_tree::operator = (t_binary_tree const & other) {
  if (&other == this) return *this;

  clear();

  copy (other.m_root);

  return *this;
}

t_binary_tree t_binary_tree::get_copy (int const index) const {
  t_binary_tree tree;
  tree.copy (at_index (index));
  return tree;
}

t_binary_tree & t_binary_tree::clear () {
  clear (m_root);

  return *this;
}
t_binary_tree & t_binary_tree::clear_subtrees (int const index) {
	t_node * const node = at_index (index);
	if (node->left  != nullptr) clear (node->left );
	if (node->right != nullptr) clear (node->right);

  return *this;
}

t_binary_tree & t_binary_tree::add_node (int const key) {
  if (m_root == nullptr) {
    m_root = new t_node {key};
    return *this;
  }
  else {
    for (auto const & node : breadth_first (m_root)) {
      if (node->left == nullptr) {
        node->left = new t_node {key};
        return *this;
      } else if (node->right == nullptr) {
        node->right = new t_node {key};
        return *this;
      }
    }
  }

  return *this;
}
bool t_binary_tree::remove_node_key (int const key) {
  return remove_node (with_key (key));
}
bool t_binary_tree::remove_node_index (int const index) {
  return remove_node (at_index (index));
}
bool t_binary_tree::is_empty () const {
  return (m_root == nullptr);
}
bool t_binary_tree::is_balanced () const {
  if (m_root == nullptr) return false;

  for (auto const & node : breadth_first (m_root)) {
    if (std::abs (height (node->left) - height (node->right)) > 1) return false;
  }

  return true;
}

int t_binary_tree::level (int const key) const {
  if (m_root == nullptr) return -1;

  int level = -1;
  int curlvl = 0;

  level_rec (m_root, curlvl, level, key);

  return level;
}
void t_binary_tree::level_rec (t_node * root, int & curlvl, int & level, int const key) const {
  if (root->key == key) { level = curlvl; return; }

	if (root->left  != nullptr) level_rec (root->left, ++curlvl, level, key);
	if (root->right != nullptr) level_rec (root->right, ++curlvl, level, key);

  --curlvl;
}

int t_binary_tree::height () const {
  return height (m_root);
}
int t_binary_tree::node_count () const {
  return breadth_first(m_root).size();
}

int t_binary_tree::index (int const key) const {
  if (m_root == nullptr) return -1;

  auto const bf = breadth_first (m_root);

  for (int i = 0; i < bf.size(); i++) if (bf[i]->key == key) return i;

  return -1;
}
int t_binary_tree::key (int const index) const {
  return at_index (index) -> key;
}

int t_binary_tree::key_max () const {
  if (m_root == nullptr) return -1;

  int max = m_root->key;

  for (auto const & node : breadth_first (m_root)) if (node->key > max) max = node->key;

  return max;
}
int t_binary_tree::key_min () const {
  if (m_root == nullptr) return -1;

  int min = m_root->key;

  for (auto const & node : breadth_first (m_root)) if (node->key < min) min = node->key;

  return min;
}
int t_binary_tree::key_sum () const {

  if (m_root == nullptr) return -1;

  int sum = 0;
  for (auto const & node : breadth_first (m_root)) sum += node -> key;

  return sum;
}

std::vector<int> t_binary_tree::keys () const {
  auto const bf = breadth_first (m_root);
  std::vector <int> keys (bf.size());

  for (std::size_t i = 0; i < bf.size(); i++) keys [i] = bf [i] -> key;

  return keys;
}

void t_binary_tree::print () const {

  if (m_root == nullptr) {
    std::cout << "L0: ." << "\n";
    return;
  }

  for (int i = 0; i <= height (); i++) print_level (i);
}
void t_binary_tree::print_level (int const level) const {
  if (level < 0 or level > height ()) throw std::invalid_argument ("print_level (int): Level must be in range [0, height]!");

  struct t_nodelvl { t_node * root; int level; };
  
  std::vector <t_nodelvl> vec;

  vec.push_back ({m_root, 0});

  for (std::size_t i = 0; vec[i].level <= level; i++) {
    if (vec[i].root == nullptr) { 
      vec.push_back ({nullptr, vec[i].level + 1});
      vec.push_back ({nullptr, vec[i].level + 1});
    } else {
      vec.push_back({vec[i].root->left, vec[i].level + 1});
      vec.push_back({vec[i].root->right, vec[i].level + 1});
    }
  }

  std::cout << "L" << level << ": ";

  for (auto const & node : vec) {
    if (node.level == level) {
      if (node.root == nullptr) {
        std::cout << ". ";
      } else {
        std::cout << node.root->key << " ";
      }
    }
  }

  std::cout << "\n";
}
void t_binary_tree::print_leaves () const {
  
  for (auto const & node : breadth_first (m_root)) {
    if (node -> left == nullptr and node -> right == nullptr) {
      std::cout << node -> key << " ";
    }
  }

  std::cout << "\n";
}

void t_binary_tree::clear (t_node * & root) {
  if (root == nullptr) return;

  clear_rec (root);
}

void t_binary_tree::clear_rec (t_node * & root) {
	if (root->left  != nullptr) clear_rec (root->left );
	if (root->right != nullptr) clear_rec (root->right);
	
  delete root;
  root = nullptr;
}

void t_binary_tree::copy (t_node * const root) {
	if (root == nullptr)
		m_root = nullptr;
	else
		copy_rec (m_root = new t_node {root->key}, root);
}

void t_binary_tree::copy_rec (t_node * const root, t_node * const other) {
	if (other->left  != nullptr) copy_rec (root->left  = new t_node (other->left ->key), other->left );
	if (other->right != nullptr) copy_rec (root->right = new t_node (other->right->key), other->right);
}

int t_binary_tree::height (t_node * const root) const {
  if (root == nullptr) return -1;

  int max_depth = 0;
  int curr_depth = 0;

  height_rec (root, max_depth, curr_depth);

  return max_depth;
}

void t_binary_tree::height_rec (t_node * const root, int & max_depth, int & curr_depth) const {
  if (root->left != nullptr) height_rec (root->left, max_depth, ++curr_depth);
  if (root->right != nullptr) height_rec (root->right, max_depth, ++curr_depth);
  
  if (curr_depth > max_depth) max_depth = curr_depth;
  
  curr_depth--;
}

std::vector <t_binary_tree::t_node *> t_binary_tree::breadth_first (t_node * const root) const {
  std::vector <t_node *> vec;

  if (root == nullptr) return vec;

  vec.push_back (root);

  for (std::size_t i = 0; i < vec.size(); i++) {
    if (vec[i]->left  != nullptr) vec.push_back(vec[i]->left );
    if (vec[i]->right != nullptr) vec.push_back(vec[i]->right);
  }

  return vec;
}

t_binary_tree::t_node * t_binary_tree::at_index (int const index) const {
  auto bf = breadth_first (m_root);
  if (index < 0 or index >= bf.size()) throw std::out_of_range ("at_index (int): Index must be in range [0, node_count)!");

  return bf [index];
}

t_binary_tree::t_node * t_binary_tree::with_key (int const key) const {
  for (auto const & node : breadth_first (m_root)) if (node->key == key) return node;
  
  return nullptr;
}

bool t_binary_tree::remove_node (t_node * const node) {
  if (m_root == nullptr or node == nullptr) return false;

  if (m_root == node and m_root->children_count() == 0) {
    clear (m_root);
    return true;
  }

  if (m_root == node and m_root->children_count() != 0) {
    if (m_root->left != nullptr) {
      m_root = m_root->left;
      t_node * rightist = m_root;
      while (rightist->right != nullptr) rightist = rightist -> right;
      rightist->right = node->right;
      delete node;
      return true;
    } else {
      m_root = m_root->right;
      t_node * leftist = m_root;
      while (leftist->left != nullptr) leftist = leftist -> left;
      leftist->left = node->left;
      delete node;
      return true;
    }
  }


  for (auto const & pnode : breadth_first (m_root)) {
    if (pnode->left == node) {
      pnode->left = node->left;
      
      t_node * rightist = m_root;
      while (rightist->right != nullptr) rightist = rightist -> right;
      rightist->right = node->right;

      delete node;
      return true;
    }
    else if (pnode->right == node) {
      pnode->right = node->right;
      
      t_node * leftist = m_root;
      while (leftist->left != nullptr) leftist = leftist -> left;
      leftist->left = node->left;

      delete node;
      return true;
    }
  }

  return false;
}

bool t_binary_tree::is_search_tree () const {
  for (auto const & node : breadth_first (m_root)) {
    if (node->left != nullptr) {
      if (node->key <= node->left->key) return false;
    }
    if (node->right != nullptr) {
      if (node->key > node->right->key) return false;
    }
  }

  return true;
}