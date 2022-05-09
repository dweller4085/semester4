#include "search_tree.hpp"

t_search_tree::t_search_tree () :
  t_binary_tree {}
{}

t_search_tree::t_search_tree (t_search_tree const & other) :
  t_binary_tree {other}
{}

t_search_tree t_search_tree::operator = (t_search_tree const & other) {
  if (this == &other) return *this;
  
  t_binary_tree::operator = (other);
  return *this;
}

t_search_tree t_search_tree::get_copy (int const index) {
  t_search_tree tree;
  tree.copy (at_index (index));
  return tree;
}

t_search_tree & t_search_tree::add_node (int const key) {

  t_node * parent {m_root};
  t_node * seek {m_root};

  while (seek != nullptr) {
    parent = seek;
    if (seek->key > key) {
      seek = seek->left;
    } else {
      seek = seek->right;
    }
  }

  if (parent == nullptr) {
    m_root = new t_node {key};
  } else {
    if (parent->key > key) {
      parent->left = new t_node {key};
    } else {
      parent->right = new t_node {key};
    }
  }

  return *this;
}

int t_search_tree::level (int const key) const {
  
  t_node * seek {m_root};
  int level {0};

  while (seek != nullptr) {
    if (seek->key == key) {
      return level;
    } else if (seek->key > key) {
      seek = seek->left;
    } else {
      seek = seek->right;
    }
    level++;
  }

  return -1;
}

int t_search_tree::key_max () const {
  if (m_root == nullptr) return -1;
  
  t_node * node {m_root};
  for (; node->right != nullptr; node = node->right);

  return node->key;
}
int t_search_tree::key_min () const {
  if (m_root == nullptr) return -1;

  t_node * node {m_root};
  for (; node->left != nullptr; node = node->left);

  return node->key;
}

bool t_search_tree::remove_node (t_binary_tree::t_node * const node) {
  return false;
}

t_binary_tree::t_node * t_search_tree::with_key (int) const {
  return nullptr;
}