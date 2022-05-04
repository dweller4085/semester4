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
  copy (other);
}
t_binary_tree::~t_binary_tree () {
  clear();
}

t_binary_tree & t_binary_tree::operator = (t_binary_tree const & other) {
  if (&other == this) return *this;

  clear();

  copy (other);

  return *this;
}

t_binary_tree t_binary_tree::get_copy (int const index) const {
  return t_binary_tree (t_binary_tree (at_index (index)));
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
  }
  else {
    t_node * leftist = m_root;
    while (leftist -> left != nullptr) leftist = leftist -> left;
    leftist -> left = new t_node {key};
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
  for (auto const & node : breadth_first (m_root)) {
    if (std::abs (height (node->left) - height (node->right)) > 1) return false;
  }

  return true;
}

int t_binary_tree::level (int const key) const {
	
	std::vector <t_node *> vec;

	vec.push_back (m_root);

	std::size_t j = -1;

	for (std::size_t i = 0; i < vec.size(); i++) {
    if (i == (std::size_t(1) << (j + 1)) - std::size_t(1)) j++;

    if (vec[i] != nullptr) {
      if (vec[i]->key == key) return j;

      vec.push_back(vec[i]->left );
		  vec.push_back(vec[i]->right);
    }
	}
	
	return -1;
}
int t_binary_tree::height () const {
  return height (m_root);
}
int t_binary_tree::node_count () const {
  return breadth_first(m_root).size();
}

int t_binary_tree::index_of (int const key) const {
  if (m_root == nullptr) return -1;

  auto const bf = breadth_first (m_root);

  for (int i = 0; i < bf.size(); i++) if (bf[i]->key == key) return i;

  return -1;
}
int t_binary_tree::key_of (int const index) const {
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
  for (int i = 0; i < height () + 1; i++) print_level (i);
}
void t_binary_tree::print_level (int const level) const {

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

void t_binary_tree::copy (t_binary_tree const & other) {
	if (other.is_empty())
		m_root = nullptr;
	else
		copy_rec (m_root = new t_node {other.m_root->key}, other.m_root);
}

void t_binary_tree::copy_rec (t_node * const root, t_node * const other) {
	if (other->left  != nullptr) copy_rec (root->left  = new t_node (other->left ->key), other->left );
	if (other->right != nullptr) copy_rec (root->right = new t_node (other->right->key), other->right);
}

int t_binary_tree::height (t_node * const root) const {
  if (root == nullptr) return 0;

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

t_binary_tree::t_binary_tree (t_node * root) : m_root (root) {}

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
  if (index < 0 or index > bf.size()) throw std::out_of_range ("at_index (int): Index must be in range [0, node_count)!");

  return bf [index];
}

t_binary_tree::t_node * t_binary_tree::with_key (int const key) const {
  for (auto const & node : breadth_first (m_root)) if (node->key == key) return node;
  
  return nullptr;
}

bool t_binary_tree::remove_node (t_node * const node) {
  return true;
}

/*
t_binary_tree & t_binary_tree::subtree (int const index) const {
  return t_binary_tree { at_index (index) };
}

t_binary_tree & t_binary_tree::operator [] (int const index) const {
  return subtree (index);
}
*/