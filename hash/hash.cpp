#include <iostream>
#include <string>
#include <bitset>

struct t_keyval {
  unsigned key;
  std::string str;
};

struct i_hash_function {
	virtual unsigned operator () (unsigned, unsigned, unsigned) = 0;
};

struct t_hash_v1 : public i_hash_function {
	unsigned operator () (unsigned K, unsigned N, unsigned i) override {
		if (i == 0) return K % N;
		return (operator () (K, N, i - 1) + 4 * i + 4 * i * i) % N;
	}
} v1;

struct t_hash_v2 : public i_hash_function {
	unsigned operator () (unsigned K, unsigned N, unsigned i) override {
		if (i == 0) return K % N;
		return unsigned (operator () (K, N, i - 1) * N * 0.618034 ) % N;
	}
} v2;

class t_hashtable {
private:
  struct t_kvnode {
    t_keyval kv;
    t_kvnode * next;
  } * table;
  unsigned size;
  i_hash_function * hash_function;
  bool * occupied;
public:
  t_hashtable () : hash_function {&v1}, size {4}, table {new t_kvnode [4] {{{0, ""}, nullptr}}}, occupied {new bool [4] {false}} {}
  t_hashtable (t_hashtable const &);
  t_hashtable & operator = (t_hashtable const &);
  ~t_hashtable ();
  void add (t_keyval const &);
  void remove (t_keyval const &);
  bool check (t_keyval const &) const;
  void change_hash_function (i_hash_function &);
};

void t_hashtable::add (t_keyval const & kv) {
  unsigned index = (* hash_function) (kv.key, size, 1);
  if (not occupied [index]) {
    occupied [index] = true;
    table [index].kv = kv;
  } else {
    auto seek = &table [index];
    while (seek->next != nullptr) seek = seek->next;
    seek->next = new t_kvnode {kv, nullptr};
  }
}

t_hashtable::~t_hashtable () {
  for (unsigned i = 0; i < size; i++) {
    if (occupied [i]) {
      auto seek = table [i].next;
      while (seek != nullptr) {
        auto next = seek->next;
        delete seek;
        seek = next;
      }
    }
  }

  delete [] table;
  delete [] occupied;
}

void t_hashtable::remove (t_keyval const & kv) {
  unsigned index = (* hash_function) (kv.key, size, 1);
  if (not occupied [index]) {
    return;
  } else {
    //?????????????
  }
}

bool t_hashtable::check (t_keyval const & kv) const {
  unsigned index = (* hash_function) (kv.key, size, 1);
  if (not occupied [index]) {
    return false;
  } else {
    auto seek = &table [index];
    while (seek->kv.key != kv.key and seek != nullptr) seek = seek->next;
    if (seek == nullptr)
      return false;
    else
      return true;
  }
}

t_hashtable::t_hashtable (t_hashtable const & other) : 
  size {other.size},
  table {new t_kvnode [other.size] {{{0, ""}, nullptr}}},
  occupied {new bool [other.size] {false}}
{
  for (unsigned i = 0; i < size; i++) {
    if (other.occupied[i]) {
      table [i].kv = other.table[i].kv;
      auto seek = other.table[i].next;
      auto sick = & table[i];
      while (seek != nullptr) {
        sick = sick->next = new t_kvnode {seek->kv, nullptr};
        seek = seek->next;
      }
    }
  }
}

t_hashtable & t_hashtable::operator = (t_hashtable const & other) {
  for (unsigned i = 0; i < size; i++) {
    if (occupied [i]) {
      auto seek = table [i].next;
      while (seek != nullptr) {
        auto next = seek->next;
        delete seek;
        seek = next;
      }
    }
  }

  delete [] table;
  delete [] occupied;

  size = other.size;
  table = new t_kvnode [other.size] {{{0, ""}, nullptr}};
  occupied = new bool [other.size] {false};

  for (unsigned i = 0; i < size; i++) {
    if (other.occupied[i]) {
      table [i].kv = other.table[i].kv;
      auto seek = other.table[i].next;
      auto sick = & table[i];
      while (seek != nullptr) {
        sick = sick->next = new t_kvnode {seek->kv, nullptr};
        seek = seek->next;
      }
    }
  }

  return *this;
}

void t_hashtable::change_hash_function (i_hash_function & hashf) {
  t_kvnode * newtable = new t_kvnode [size] {{{0, ""}, nullptr}};
  bool * newoccupied = new bool [size] {false};
  hash_function = &hashf;

  for (unsigned i = 0; i < size; i++) {
    if (occupied [i]) {
      auto walk = & table [i];
      while (walk != nullptr) {
        {
          unsigned index = (* hash_function) (walk->kv.key, size, 1);
          if (not newoccupied [index]) {
            newoccupied [index] = true;
            newtable [index].kv = walk->kv;
          } else {
            auto seek = &newtable [index];
            while (seek->next != nullptr) seek = seek->next;
            seek->next = new t_kvnode {walk->kv, nullptr};
          }
        }
        walk = walk->next;
      }
    }
  }

  delete [] table;
  delete [] occupied;

  table = newtable;
  occupied = newoccupied;
}

int main () {
  t_hashtable ht;
  ht.add ({666, "hellooo"});
  ht.add ({666, "ayayayeyeapppaoaooaoa"});
  ht.add ({666, "AAAAAAAAA"});
  ht.add ({12, "testers"});
  ht.change_hash_function (v2);
  ht.check ({666, "whatever wont even look"});
}
