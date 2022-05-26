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
    t_kvnode () : kv {0, ""}, next {nullptr} {}
    t_kvnode (t_keyval const & _kv, t_kvnode * _next) : kv {_kv}, next {_next} {}
  } * table;
  unsigned size;
  i_hash_function * hash_function;
  bool * occupied;
public:
  t_hashtable (unsigned _size) :
    hash_function {&v1},
    size {_size},
    table {new t_kvnode [_size] ()},
    occupied {new bool [_size]}
  {
    for (unsigned i = 0; i < _size; i++) occupied[i] = false;
  }
  t_hashtable (t_hashtable const &);
  t_hashtable & operator = (t_hashtable const &);
  ~t_hashtable ();
  void add (t_keyval const &);
  void remove (t_keyval const &);
  bool check (t_keyval const &) const;
  void change_hash_function (i_hash_function &);
  void print (std::string str = "") const {
    std::cout << "------" << str << "--------" << "\n\n";
    for (unsigned i = 0; i < size; i++) {
      std::cout << i << ": ";
      if (occupied[i]) {
        auto seek = & table[i];
        while (seek != nullptr) {
          std::cout << seek->kv.key;
          seek = seek->next;
          if (seek) {
            std::cout << ", ";
          } else {
            std::cout << ".";
          }
        }
      }
      std::cout << "\n";
    }
  }
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
    if (table[index].kv.key == kv.key) {
      if (table[index].next == nullptr) {
        occupied[index] = false;
      } else {
        table[index] = *table[index].next;
      }
    } else {
      auto seek = table[index].next;
      auto prev = &table[index];
      while (seek != nullptr and seek->kv.key != kv.key) {
        prev = seek;
        seek = seek->next;
      }
      if (seek == nullptr) {
        return;
      } else {
        prev->next = seek->next;
        delete seek;
      }
    }
  }
}

bool t_hashtable::check (t_keyval const & kv) const {
  unsigned index = (* hash_function) (kv.key, size, 1);
  if (not occupied [index]) {
    return false;
  } else {
    auto seek = &table [index];
    while (seek != nullptr and seek->kv.key != kv.key) seek = seek->next;
    if (seek == nullptr)
      return false;
    else
      return true;
  }
}

t_hashtable::t_hashtable (t_hashtable const & other) : 
  size {other.size},
  table {new t_kvnode [other.size] ()},
  occupied {new bool [other.size]},
  hash_function {other.hash_function}
{
  for (unsigned i = 0; i < size; i++) {
    occupied[i] = false;
  }

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
  if (this == &other) return *this;

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


  hash_function = other.hash_function;
  size = other.size;
  table = new t_kvnode [other.size] ();
  occupied = new bool [other.size];
  for (unsigned i = 0; i < size; i++) {
    occupied[i] = other.occupied[i];
  }

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
  t_kvnode * newtable = new t_kvnode [size] ();
  bool * newoccupied = new bool [size];
  for (unsigned i = 0; i < size; i++) {
    newoccupied[i] = false;
  }
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
  t_hashtable ht {4};
  ht.add ({666, "hellooo"});
  ht.add ({734, "ayayayeyeapppaoaooaoa"});
  ht.add ({404, "AAAAAAAAA"});
  ht.add ({12, "testers"});
  ht.print("constructed ht {666, 734, 404, 12}, ht:");


  ht.change_hash_function (v2);
  ht.print("changed hf in ht, ht:");


  std::cout << "\n is 666 in ht: " << ht.check ({666, "whatever wont even look"}) << "\n";

  ht.remove ({12, "testers"});
  ht.print("removed 12 from ht, ht:");

  t_hashtable nht {4};
  nht = ht;
  nht.print("copied ht to nht, nht:");


  nht.change_hash_function (v1);
  nht.change_hash_function (v2);
  ht.change_hash_function (v1);

  ht.print("ht had hf v2, changed to v1, ht:");
  nht.print("nht had hf v2, changed to v1 and back to v2, nht:");

  std::cout << "-----------------------------------\n\n"; 


  for (unsigned i = 0; i < 4; i++) {
    ht.add({i, ""});
  }

  ht.print("addded {0, 1, 2, 3} to ht, ht:");

  ht.change_hash_function (v2);

  ht.print("ht had hf v1, changed to v2, ht:");

  for (unsigned i = 0; i < 4; i++) {
    ht.add({i+4, ""});
  }

  ht.print("added {4, 5, 6, 7} to ht, ht:");

  for (unsigned i = 0; i < 4; i++) {
    ht.remove({i, ""});
    ht.remove({i+4, ""});
  }

  ht.print("removed {0 .. 7} from ht, ht:");

}
