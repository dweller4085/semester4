#include "btree.hpp"
#include <stack>
#include <utility>

/////////// t_binary_tree::t_node ///////////

t_binary_tree::t_node::t_node (t_node * lc, t_node * rc, int key) :
  m_left_child (lc),
  m_right_child (rc),
  m_key (key)
{}

int t_binary_tree::t_node::descedants_cnt () const {
  using pair = std::pair<t_node const *, bool>;

  int count = 0;
  t_node const * cur = this;
  std::stack <pair> nodestack;

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
          if (nodestack.empty()) return count;

          count++;
          if (nodestack.top().second) {
            cur = nodestack.top().first->m_right_child;
            nodestack.top().second = false;
            break;
          }

          nodestack.pop();
        }
      }
    }
  }
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
}



t_binary_tree t_binary_tree::operator = (t_binary_tree const & other) {
}



t_binary_tree t_binary_tree::copy (t_binary_tree::t_index const index) {
}



void t_binary_tree::clear () {
}

void t_binary_tree::clear (t_binary_tree::t_index const index) {
}



void t_binary_tree::add_node (int const key) {
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
}



t_binary_tree::t_index t_binary_tree::index (int const key) {
}

int t_binary_tree::key (t_index const index) {
}



int t_binary_tree::key_max () {
}

int t_binary_tree::key_min () {
}

int t_binary_tree::key_sum () {
}



std::vector<int> t_binary_tree::keys () {
}



void t_binary_tree::print () {
}

void t_binary_tree::print_level (int const level) {
}

void t_binary_tree::print_leaves () {
}
