#include "btree.hpp"
#include <stack>
#include <vector>
#include <utility>

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

std::vector<t_binary_tree::t_node * const> t_binary_tree::t_node::depth_first () {
  using pair = std::pair<t_node *, bool>;

  t_node * cur = this;
  std::stack <pair> nodestack;
  std::vector<t_node * const> vec;

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

std::vector<t_binary_tree::t_node const *> t_binary_tree::t_node::breadth_first () const {

}


int t_binary_tree::t_node::descedants_cnt () {
  return depth_first().size() - 1;
}
/////////// t_binary_tree ///////////





t_binary_tree::t_binary_tree () :
  m_root(nullptr)
{
  // test
  m_root = new t_node {new t_node {nullptr, new t_node {new t_node {nullptr, nullptr, 5}, new t_node {nullptr, nullptr, 6}, 4}, 2}, new t_node {nullptr, nullptr, 3}, 1};
}

t_binary_tree::t_binary_tree (t_binary_tree const & other) {
  //copy other tree logic
}

t_binary_tree::~t_binary_tree () {
  clear();
}



t_binary_tree t_binary_tree::operator = (t_binary_tree const & other) {
}



t_binary_tree t_binary_tree::copy (t_binary_tree::t_index const index) {
}



void t_binary_tree::clear () {
  for (auto & node : m_root->depth_first()) delete node;
  m_root = nullptr;
}

void t_binary_tree::clear (t_binary_tree::t_index const index) {
}



void t_binary_tree::add_node (int const key) {
  m_root->depth_first().front()->m_left_child = new t_node {key};
}

bool t_binary_tree::remove_node_key (int const key) {
}

bool t_binary_tree::remove_node_index (t_binary_tree::t_index const index) {
}



bool t_binary_tree::is_balanced () {
}



int t_binary_tree::level (int const key) {
}

int t_binary_tree::height () {
}

int t_binary_tree::node_cnt () {
  return m_root->descedants_cnt() + 1;
}



t_binary_tree::t_index t_binary_tree::index (int const key) {
}

int t_binary_tree::key (t_index const index) {
}



int t_binary_tree::key_max () {
  const auto vec = m_root->depth_first();
  int max = vec.front()->key();
  for (const auto & node : vec) {
    if (node->key() > max) max = node->key();
  }
  return max;
}

int t_binary_tree::key_min () {
  const auto vec = m_root->depth_first();
  int min = vec.front()->key();
  for (const auto & node : vec) {
    if (node->key() < min) min = node->key();
  }
  return min;
}

int t_binary_tree::key_sum () {
  int sum = 0;
  for (const auto & node : m_root->depth_first()) {
    sum += node->key();
  }
  return sum;
}



std::vector<int> t_binary_tree::keys () {
  //breadthfirst
}



void t_binary_tree::print () {
}

void t_binary_tree::print_level (int const level) {
}

void t_binary_tree::print_leaves () {
}
