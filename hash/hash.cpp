#include <iostream>
#include <string>

struct t_keyval {
  unsigned K;
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
  i_hash_function * hash_function = &v1;
public:
  void add (t_keyval &);
  void remove (t_keyval &);
  bool check (t_keyval &);
  void change_hash_function (i_hash_function &);
};

int main () {}
