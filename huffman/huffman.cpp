#include <fstream>
#include <iostream>
#include <stdexcept>
#include <deque>
#include <algorithm>
#include <cstdint>
#include <array>

class t_charset {
private:
  std::array <uint32_t, 8> arrrr;
public:
  t_charset () : arrrr {0} {}
  t_charset (t_charset const & other) { arrrr = other.arrrr; }
  t_charset (unsigned char c);
  t_charset & operator = (t_charset const & other) { if (this == &other) return *this; arrrr = other.arrrr; return *this; }

  bool operator [] (unsigned char) const;
  t_charset operator | (t_charset const & other) const;

  friend std::ostream & operator << (std::ostream &, t_charset const &);
};

bool t_charset::operator [] (unsigned char c) const {
  std::size_t const arri = static_cast <std::size_t> (c) / 32;
  std::size_t const biti = static_cast <std::size_t> (c) % 32;
  
  return {((arrrr [arri]) & (std::size_t (1) << biti)) != 0};
}

t_charset::t_charset (unsigned char c) : arrrr {0} {
  std::size_t const arri = static_cast <std::size_t> (c) / 32;
  std::size_t const biti = static_cast <std::size_t> (c) % 32;
  arrrr [arri] |= (std::size_t (1) << biti);
}

t_charset t_charset::operator | (t_charset const & other) const {
  t_charset cs = *this;
  for (std::size_t i = 0; i < 8; i++) cs.arrrr[i] |= other.arrrr[i];
  return cs;
}

std::ostream & operator << (std::ostream & os, t_charset const & ch) {
  for (std::size_t i = 0; i < 256; i++) {
    if (ch[i]) {
      os << static_cast <char> (i);
    }
  }
  return os;
}

struct t_huffnode {
  t_huffnode () : freq {0}, left {nullptr}, right {nullptr} {}
  t_huffnode (unsigned char ch) : freq {1}, chars {ch}, left {nullptr}, right {nullptr} {}
  t_huffnode (t_huffnode &&) = default;
  t_huffnode (t_huffnode const &) = default;
  ~t_huffnode () = default;

  t_huffnode operator + (t_huffnode &);
  bool is_leaf () { return (left == nullptr and right == nullptr); }

  int freq;
  t_charset chars;
  t_huffnode * left;
  t_huffnode * right;
};

class t_huffman {
private:
  t_huffnode * m_root;

  void delete_rec (t_huffnode * &);
  void clear () { if (m_root != nullptr) { delete_rec (m_root); } }

public:
  t_huffman () : m_root (nullptr) {}
  ~t_huffman () { clear (); }
	
  void build (std::istream &);
  double encode (std::istream &, std::ostream &);
  bool decode (std::istream &, std::ostream &);
};

t_huffnode t_huffnode::operator + (t_huffnode & other) {
  t_huffnode huff;
  huff.freq = freq + other.freq;
  huff.chars = chars | other.chars;
  huff.left = this;
  huff.right = &other;
  return huff;
}

void t_huffman::delete_rec (t_huffnode * & root) {
	//std::cout << root->chars << "\n";
  if (root->left  != nullptr) delete_rec (root->left );
  if (root->right != nullptr) delete_rec (root->right);
  
  delete root;
  root = nullptr;
}

void t_huffman::build (std::istream & file) {
  if (not (file.good())) throw std::invalid_argument {"something's not right with the file"};

  clear ();

  std::deque <t_huffnode *> nodes;

  char ch;
  while (file.get(ch)) {
    bool found = false;
    for (auto const & node : nodes) {
      if (node->chars[ch]) {
        node->freq++;
        found = true;
        break;
      }
    }
    if (not found) {
      nodes.push_back (new t_huffnode {ch});
    }
  }

  if (nodes.size() == 0) throw std::invalid_argument ("fuck you too");

  file.clear();file.clear();file.clear();file.clear();file.clear();file.clear();file.sync();
  file.seekg (0, std::ios_base::beg);
  file.seekg (0, std::ios_base::beg);
  file.seekg (0, std::ios_base::beg);
  file.seekg (0, std::ios_base::beg);
  file.seekg (0, std::ios_base::beg);
  file.seekg (0);
  file.clear();
  file.sync();

  std::sort (nodes.begin(), nodes.end(), [] (t_huffnode * a, t_huffnode * b) { return a->freq < b->freq; });
	
	/*
  int i = 0;
  for (const auto & node : nodes) {
    std::cout << i << ": " << node->chars << ", freq: " << node->freq << "\n";
    i++;
  }
  */
	

  while (nodes.size () > 1) {
    t_huffnode * sum = new t_huffnode {*nodes[0] + *nodes[1]};
    //std::cout << sum->chars << "\n";
    //std::cout << nodes.size() << "\n";
    nodes.pop_front();
    nodes.pop_front();
    nodes.push_front (sum);
    std::sort (nodes.begin(), nodes.end(), [] (t_huffnode * a, t_huffnode * b) { return a->freq < b->freq; });
  }

  m_root = nodes.front();
}

double t_huffman::encode (std::istream & orfile, std::ostream & enfile) {
  build (orfile);

  if (not enfile.good()) throw std::invalid_argument ("something aint right");

  std::size_t or_char_length = 0;
  std::size_t en_bit_length = 0;
  
  /* 
  std::cout << orfile.good() << "\n";
  std::cout << orfile.eof() << "\n";
  std::cout << orfile.bad() << "\n";
  std::cout << orfile.fail() << "\n";
  std::cout << orfile.tellg() << "\n";
  */

  char ch;
  while (orfile.get(ch)) {
    auto walk = m_root;
    while (not walk->is_leaf()) {
      en_bit_length++;
      if (walk->left->chars[ch]) {
        walk = walk->left;
        //std::cout << ch << ": 0" << "\n";
        enfile << '0';
      } else {
        walk = walk->right;
        //std::cout << ch << ": 1" << "\n";
        enfile << '1';
      }
    }
    or_char_length++;
  }

  enfile.flush();

  enfile.clear();enfile.clear();enfile.clear();enfile.clear();enfile.clear();enfile.clear();enfile.flush();
  enfile.seekp (0, std::ios_base::beg);
  enfile.seekp (0, std::ios_base::beg);
  enfile.seekp (0, std::ios_base::beg);
  enfile.seekp (0, std::ios_base::beg);
  enfile.seekp (0, std::ios_base::beg); // i beg
  enfile.seekp (0);
  enfile.clear();
  enfile.flush();
  enfile.seekp(0);

  return double (en_bit_length) / double (or_char_length * 8);
}

bool t_huffman::decode (std::istream & enfile, std::ostream & defile) {
  if (not (enfile.good() and defile.good())) throw std::invalid_argument ("blargh what an invalid arg");
  if (m_root == nullptr) throw std::logic_error ("blargh i don't even have the tree");

  enfile.clear();enfile.clear();enfile.clear();enfile.clear();enfile.clear();enfile.clear();enfile.sync();
  enfile.seekg (0, std::ios_base::beg);
  enfile.seekg (0, std::ios_base::beg);
  enfile.seekg (0, std::ios_base::beg);
  enfile.seekg (0, std::ios_base::beg);
  enfile.seekg (0, std::ios_base::beg); // i beg
  enfile.seekg (0);
  enfile.clear();
  enfile.sync();
  enfile.seekg(0);
  
  defile.flush();

  defile.clear();defile.clear();defile.clear();defile.clear();defile.clear();defile.clear();defile.flush();
  defile.seekp (0, std::ios_base::beg);
  defile.seekp (0, std::ios_base::beg);
  defile.seekp (0, std::ios_base::beg);
  defile.seekp (0, std::ios_base::beg);
  defile.seekp (0, std::ios_base::beg); // i beg
  defile.seekp (0);
  defile.clear();
  defile.flush();
  defile.seekp(0);

  char ch;
  auto walk = m_root;

  if (walk->is_leaf()) {
    defile << walk->chars;
    return true;
  }

  while (enfile.get(ch)) {
    if (ch == '0') {
      walk = walk->left;
    } else if (ch == '1') {
      walk = walk->right;
    } else {
      return false;//throw std::invalid_argument ("beep bop can only understand 1's and 0's");
    }

    if (walk->is_leaf()) {
      defile << walk->chars;
      walk = m_root;
    }

  }

  return true;
}

int main () {
  std::fstream orfile {"orfile.txt"};
  std::fstream enfile {"enfile.txt"};
  std::fstream defile {"defile.txt"};
  t_huffman huffman;

  //huffman.build (orfile);
  std::cout << huffman.encode (orfile, enfile) << "\n";
  huffman.decode(enfile, defile);

}
