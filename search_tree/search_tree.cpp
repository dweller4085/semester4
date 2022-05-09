#include "search_tree.hpp"

#include <stdexcept>

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

t_search_tree t_search_tree::get_copy (int const index) const {
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

  while (node->right != nullptr) node = node->right;

  return node->key;
}
int t_search_tree::key_min () const {
  if (m_root == nullptr) return -1;

  t_node * node {m_root};

  while (node->left != nullptr) node = node->left;

  return node->key;
}

bool t_search_tree::remove_node (t_binary_tree::t_node * const node) {

  if (m_root == nullptr or node == nullptr) return false;

  t_node * const parent {parent_of (node)};

  auto shift_to = [this, &parent, &node] (t_node * const what) {
    if (parent != nullptr) {
      if (parent->left == node) {
        parent->left = what;
      } else {
        parent->right = what;
      }
    } else {
      m_root = what;
    }
  };



  if (node->left == nullptr and node->right == nullptr) {
    shift_to (nullptr);
  } else

  if (node->left == nullptr and node->right != nullptr) {
    shift_to (node->right);
  } else

  if (node->left != nullptr and node->right == nullptr) {
    shift_to (node->left);
  } else



  if (node->left != nullptr and node->right != nullptr) {
    t_node * const successor {successor_of (node)};

    if (node->right == successor) {
      successor->left = node->left;
    }
    
    else {
      parent_of (successor) -> left = successor -> right;
      successor->right = node->right;
      successor->left = node->left;
    }

    shift_to (successor);
  }

  delete node;
  return true;
}

t_binary_tree::t_node * t_search_tree::with_key (int const key) const {
  
  t_node * seek {m_root};
  
  while (seek != nullptr) {
    if (seek->key == key) {
      return seek;
    } else if (seek->key > key) {
      seek = seek->left;
    } else {
      seek = seek->right;
    }
  }

  return nullptr;
}

t_binary_tree::t_node * t_search_tree::parent_of (t_node * const node) const {
  
  if (m_root == nullptr or node == nullptr) throw std::invalid_argument ( \
    "t_node * t_search_tree::parent_of (t_node * node):\n" \
    "[m_root] and [node] cannot be nullptr!" \
  );

  t_node * parent {nullptr};
  t_node * seek {m_root};

  while (seek != nullptr) {
    if (seek == node) {
      return parent;
    } else {
      parent = seek;
      if (seek->key > node->key) {
        seek = seek->left;
      } else {
        seek = seek->right;
      }
    }
  }

  // Accounting for bogus scenario when [node] doesn't even belong to the [*this] tree
  throw std::invalid_argument ( \
    "t_node * t_search_tree::parent_of (t_node * node):\n" \
    "[node] does not, but really ought to belond to the (*this) tree." \
  );
}

t_binary_tree::t_node * t_search_tree::successor_of (t_node * node) const {
  
  if (m_root == nullptr) return nullptr;

  if (node->right != nullptr) {
    t_node * seek {node->right};
    while (seek->left != nullptr) seek = seek->left;
    return seek;
  }
  
  else {
    t_node * parent {parent_of (node)};
    while (parent != nullptr and parent->right == node) {
      node = parent->right;
      parent = parent_of (node);
    }
    return parent;
  }
}
