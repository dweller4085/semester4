#include "btree.hpp"
#include <stack>
#include <map>
#include <vector>
#include <queue>
#include <utility>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <string>

/////////// t_binary_tree::t_node ///////////

t_binary_tree::t_node::t_node (t_node * lc, t_node * rc, int key) :
  m_left_child (lc),
  m_right_child (rc),
  m_key (key)
{}

t_binary_tree::t_node::t_node (int const key) :
  m_key(key),
  m_left_child(nullptr),
  m_right_child(nullptr)
{}

std::vector<t_binary_tree::t_node *> t_binary_tree::t_node::depth_first () {
  using pair = std::pair<t_node *, bool>;

  t_node * cur = this;
  std::stack <pair> nodestack;
  std::vector<t_node *> vec;

  while (true) {
    if (cur->m_left_child != nullptr) { // 1 .
      nodestack.push (pair {cur, (cur->m_right_child != nullptr)});
      cur = cur->m_left_child;
    }
    else { // 0 .
      if (cur->m_right_child != nullptr) { // 0 1
        nodestack.push (pair {cur, false});
        cur = cur->m_right_child;
      }
      else { // 0 0 -- reached a leaf : deadend. popping until we find a fresh branch or stack is over
        while (true) {

          vec.push_back(cur);

          if (nodestack.empty()) return vec;

          if (nodestack.top().second) {
            cur = nodestack.top().first->m_right_child;
            nodestack.top().second = false;
            break;
          }

          cur = nodestack.top().first;
          nodestack.pop();
        }
      }
    }
  }
}


std::vector<t_binary_tree::t_node *> t_binary_tree::t_node::breadth_first () {
  std::vector <t_node *> vec;

  vec.push_back(this);

  for (std::size_t i = 0; i < vec.size(); i++) {
    if (vec[i]->left_child() != nullptr) vec.push_back(vec[i]->left_child());
    if (vec[i]->right_child() != nullptr) vec.push_back(vec[i]->right_child());
  }

  return vec;
}


int t_binary_tree::t_node::descedants_cnt () {
  return depth_first().size() - 1;
}


/////////// t_binary_tree ///////////



t_binary_tree::t_binary_tree () :
  m_root(nullptr)
{}



t_binary_tree::t_binary_tree (t_binary_tree const & other) {
  if (other.is_empty()) {
    m_root = nullptr;
    return;
  }

  std::queue <t_node *> queue;
  std::map <t_node *, t_node *> map;

  queue.push(other.m_root);
  map.insert(std::pair<t_node *, t_node *>{other.m_root, new t_node {other.m_root->key()}});

  m_root = map [other.m_root];

  while (not queue.empty()) {
    if (queue.front()->left_child() != nullptr) {
      map.insert(std::pair<t_node *, t_node *>{queue.front()->left_child(), new t_node {queue.front()->left_child()->key()}});
      map [queue.front()] -> left_child () = map [queue.front()->left_child()];
      queue.push(queue.front()->left_child());
    }

    if (queue.front()->right_child() != nullptr) {
      map.insert(std::pair<t_node *, t_node *>{queue.front()->right_child(), new t_node {queue.front()->right_child()->key()}});
      map [queue.front()] -> right_child () = map [queue.front()->right_child()];
      queue.push(queue.front()->right_child());
    }

    queue.pop();
  }

}



t_binary_tree t_binary_tree::operator = (t_binary_tree const & other) {

  if (&other == this) return *this;

  clear();

  if (other.is_empty()) {
    m_root = nullptr;
    return * this;
  }

  std::queue <t_node *> queue;
  std::map <t_node *, t_node *> map;

  queue.push(other.m_root);
  map.insert(std::pair<t_node *, t_node *>{other.m_root, new t_node {other.m_root->key()}});

  m_root = map [other.m_root];

  while (not queue.empty()) {
    if (queue.front()->left_child() != nullptr) {
      map.insert(std::pair<t_node *, t_node *>{queue.front()->left_child(), new t_node {queue.front()->left_child()->key()}});
      map [queue.front()] -> left_child () = map [queue.front()->left_child()];
      queue.push(queue.front()->left_child());
    }

    if (queue.front()->right_child() != nullptr) {
      map.insert(std::pair<t_node *, t_node *>{queue.front()->right_child(), new t_node {queue.front()->right_child()->key()}});
      map [queue.front()] -> right_child () = map [queue.front()->right_child()];
      queue.push(queue.front()->right_child());
    }

    queue.pop();
  }


  return * this;
}



t_binary_tree t_binary_tree::copy (t_binary_tree::t_index const index) {
  std::queue <t_node *> queue;
  std::map <t_node *, t_node *> map;

  t_node * const root = get_node_at_index(index); 

  if (root == nullptr) {
    return t_binary_tree {};
  }

  queue.push(root);
  map.insert(std::pair <t_node *, t_node *> {root, new t_node {root->key()}});

  while (not queue.empty()) {
    if (queue.front()->left_child() != nullptr) {
      map.insert(std::pair<t_node *, t_node *>{queue.front()->left_child(), new t_node {queue.front()->left_child()->key()}});
      map [queue.front()] -> left_child () = map [queue.front()->left_child()];
      queue.push(queue.front()->left_child());
    }

    if (queue.front()->right_child() != nullptr) {
      map.insert(std::pair<t_node *, t_node *>{queue.front()->right_child(), new t_node {queue.front()->right_child()->key()}});
      map [queue.front()] -> right_child () = map [queue.front()->right_child()];
      queue.push(queue.front()->right_child());
    }

    queue.pop();
  }

  return t_binary_tree {map [root]};
}


t_binary_tree::~t_binary_tree () {
  clear();
}

t_binary_tree::t_binary_tree (t_node * const root) :
  m_root (root)
{}

t_binary_tree::t_node * t_binary_tree::get_node_at_index (t_index index) {
  std::vector <t_node *> const vec = m_root->breadth_first();
  if (index >= vec.size() or index < 0) return nullptr;
  return vec[index];
}

t_binary_tree::t_node * t_binary_tree::get_node_with_key (int key) {
  std::vector <t_node *> const vec = m_root->breadth_first();
  for (auto const & node : vec) {
    if (node->key() == key) return node;
  }
  return nullptr;
}

void t_binary_tree::clear () {
  if (m_root == nullptr) return;
  for (auto & node : m_root->depth_first()) delete node;
  m_root = nullptr;
}

void t_binary_tree::delete_children (t_binary_tree::t_index const index) {
  t_node * at_index = get_node_at_index(index);
  if (at_index->m_left_child != nullptr) {
    for (auto & node : at_index->m_left_child->depth_first()) delete node;
    at_index->m_left_child = nullptr;
  }

  if (at_index->m_right_child != nullptr) {
    for (auto & node : at_index->m_right_child->depth_first()) delete node;
    at_index->m_right_child = nullptr;
  }
}



void t_binary_tree::add_node (int const key) {
  if (m_root == nullptr) {
    m_root = new t_node (key);
  } else {
    m_root->depth_first().front()->m_left_child = new t_node {key};
  }
}

bool t_binary_tree::remove_node_key (int const key) {
  t_node * node = get_node_with_key (key);
  if (node == nullptr) return false;
  remove_node (node);
  return true;
}

bool t_binary_tree::remove_node_index (t_binary_tree::t_index const index) {
  t_node * node = get_node_at_index (index);
  if (node == nullptr) return false;
  remove_node (node);
  return true;
}

void t_binary_tree::remove_node (t_node * deletee) {
  const auto nodes = m_root->depth_first();

  t_node * lc = deletee->left_child();
  t_node * rc = deletee->right_child();

  for (auto & node : nodes) {
    if (node->left_child() == deletee) {
      node->left_child() = nullptr;
      delete deletee;
      break;
    }
    if (node->right_child() == deletee) {
      node->right_child() = nullptr;
      delete deletee;
      break;
    }
  }

  const auto leaves = get_leaves();
  leaves.back()->left_child() = lc;
  leaves.back()->right_child() = rc;
}


bool t_binary_tree::is_balanced () {
  const auto nodes = m_root->breadth_first();
  for (const auto & node : nodes) {
    if (std::abs(height(node->left_child()) - height(node->right_child())) >= 2) {
      return false;
    }
  } 
  return true;
}



int t_binary_tree::level (int const key) {
using pair = std::pair <t_node *, int>;
  std::queue <pair> queue;

  queue.push(pair{m_root, 0});

  while (not queue.empty()) {
    if (queue.front().first->key() == key) return queue.front().second;

    if (queue.front().first->left_child() != nullptr) {
      queue.push(pair{queue.front().first->left_child(), queue.front().second + 1});
    }

    if (queue.front().first->right_child() != nullptr) {
      queue.push(pair{queue.front().first->right_child(), queue.front().second + 1});
    }

    queue.pop();
  }

  return -1;
}

int t_binary_tree::height () {
  return height(m_root);
}

int t_binary_tree::height (t_node * node) {

  if (node == nullptr) return 0;

  using pair = std::pair<t_node *, bool>;

  t_node * cur = node;
  std::stack <pair> nodestack;

  int max = 0;

  bool getmeoutofhere = false;
  while (not getmeoutofhere) {
    if (cur->m_left_child != nullptr) { // 1 .
      nodestack.push (pair {cur, (cur->m_right_child != nullptr)});
      cur = cur->m_left_child;
    }
    else { // 0 .
      if (cur->m_right_child != nullptr) { // 0 1
        nodestack.push (pair {cur, false});
        cur = cur->m_right_child;
      }
      else { // 0 0 -- reached a leaf : deadend. popping until we find a fresh branch or stack is over
        if (nodestack.size() > max) max = nodestack.size();

        while (true) {
          if (nodestack.empty()) {
            getmeoutofhere = true;
            break;
          }

          if (nodestack.top().second) {
            cur = nodestack.top().first->m_right_child;
            nodestack.top().second = false;
            break;
          }

          cur = nodestack.top().first;
          nodestack.pop();
        }
      }
    }
  }

  return max + 1;
}

int t_binary_tree::node_cnt () {
  if (is_empty()) {
    return 0;
  }
  else {
    return m_root->descedants_cnt() + 1;
  }
}



t_binary_tree::t_index t_binary_tree::index (int const key) {
  auto const vec = m_root->breadth_first();
  for (t_index i = 0; i < vec.size(); i++) {
    if (vec[i]->m_key == key) return i;
  }
  return -1;
}

int t_binary_tree::key (t_index const index) {
  t_node * at_index = get_node_at_index(index);
  return at_index->m_key;
}



int t_binary_tree::key_max () {
  if (m_root == nullptr) return 0;
  const auto vec = m_root->depth_first();
  int max = vec.front()->key();
  for (const auto & node : vec) {
    if (node->key() > max) max = node->key();
  }
  return max;
}

int t_binary_tree::key_min () {
  if (m_root == nullptr) return 0;
  const auto vec = m_root->depth_first();
  int min = vec.front()->key();
  for (const auto & node : vec) {
    if (node->key() < min) min = node->key();
  }
  return min;
}

int t_binary_tree::key_sum () {
  if (m_root == nullptr) return 0;
  int sum = 0;
  for (const auto & node : m_root->depth_first()) {
    sum += node->key();
  }
  return sum;
}



std::vector<int> t_binary_tree::keys () {
  auto const pointervec = m_root->breadth_first();

  std::vector<int> vec;
  for (auto const pnode : pointervec) {
    vec.push_back(pnode->m_key);
  }

  return vec;
}



void t_binary_tree::print () {
  if (m_root == nullptr) {
    std::cout << "L0: ." << "\n";
    return;
  }

  for (int i = 0; i < height(); i++) {
    print_level (i);
  }
}

void t_binary_tree::print_level (int const level) {

  if (level < 0 or level >= height ()) throw std::invalid_argument("Invalid level: must be in range [0; height)");

  if (level == 0) {
    if (is_empty()) {
      std::cout << "L0: ." << "\n";
    }
    else {
      std::cout << "L0: " << m_root->key() << "\n";
    }
    return;
  }

  using pair = std::pair <t_node *, int>;

  std::queue <pair> queue;
  std::string out;


  queue.push(pair{m_root, 0});

  while (not queue.empty()) {

    if (queue.front().second == level - 1) {
      if (queue.front().first->left_child() != nullptr) {
        queue.push(pair{queue.front().first->left_child(), queue.front().second + 1});
        out.append(std::to_string(queue.front().first->left_child()->key()) + " ");
      } else {
        out.append(". ");
      }

      if (queue.front().first->right_child() != nullptr) {
        queue.push(pair{queue.front().first->right_child(), queue.front().second + 1});
        out.append(std::to_string(queue.front().first->right_child()->key()) + " ");
      } else {
        out.append(". ");
      }
    }
    else {
      if (queue.front().first->left_child() != nullptr) {
        queue.push(pair{queue.front().first->left_child(), queue.front().second + 1});
      }

      if (queue.front().first->right_child() != nullptr) {
        queue.push(pair{queue.front().first->right_child(), queue.front().second + 1});
      }
    }



    queue.pop();
  }

   std::cout << "L" + std::to_string(level) + ": " << out << "\n";
}

void t_binary_tree::print_leaves () {
  const auto leaves = get_leaves();
  for (const auto & leaf : leaves) {
    std::cout << leaf->key() << " ";
  }
  std::cout << "\n";
}


std::vector<t_binary_tree::t_node *> t_binary_tree::get_leaves () {
  using pair = std::pair<t_node *, bool>;

  t_node * cur = m_root;
  std::stack <pair> nodestack;
  std::vector<t_node *> vec;

  while (true) {
    if (cur->m_left_child != nullptr) { // 1 .
      nodestack.push (pair {cur, (cur->m_right_child != nullptr)});
      cur = cur->m_left_child;
    }
    else { // 0 .
      if (cur->m_right_child != nullptr) { // 0 1
        nodestack.push (pair {cur, false});
        cur = cur->m_right_child;
      }
      else { // 0 0 -- reached a leaf : deadend. popping until we find a fresh branch or stack is over
        
        vec.push_back(cur);
        
        while (true) {

          if (nodestack.empty()) return vec;

          if (nodestack.top().second) {
            cur = nodestack.top().first->m_right_child;
            nodestack.top().second = false;
            break;
          }

          cur = nodestack.top().first;
          nodestack.pop();
        }
      }
    }
  }
}
