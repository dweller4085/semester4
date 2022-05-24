#include <iostream>
#include <string>

class i_hashtable {
  virtual void add (t_keyval &) = 0;
  virtual void remove (t_keyval &) = 0;
  virtual bool check (t_keyval &) = 0;
};

struct t_keyval {
  std::size_t K;
  std::string str;
};

std::size_t hash_v1 (std::size_t K, std::size_t N, std::size_t i = 1) {
	if (i == 0) return 0;
  return (hash_v1 (K, N, i - 1) + 4 * i + 4 * i * i) % N;
}

class t_hashtable_v2 : public i_hashtable {
private:
	struct t_kvnode {
		t_keyval kv;
		t_kvnode * next;
	} * table;
  std::size_t size;
  std::size_t (*hash_function) (std::size_t, std::size_t, std::size_t) = hash_v1;

public:
};

int main () {}
