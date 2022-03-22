#include <vector>

class t_binary_tree {
public:
	using t_index = int;
private:
	class t_node {
	public:
		t_node * m_left_child;
		t_node * m_right_child;
		int m_key;
	
	public:
		t_node () = default;
		t_node (t_node *, t_node *, int);
		t_node (int);
		~t_node () = default;

		int key () const {return m_key;}
		int descedants_cnt ();

		t_node *& left_child () { return m_left_child; }
		t_node *& right_child () { return m_right_child; }
		int & key () { return m_key; }

		std::vector<t_node * const> depth_first ();
		std::vector<t_node const *> breadth_first () const;
		t_node * find_node (t_index);

	} * m_root;
	
public:

	void test ();
	t_binary_tree ();
	t_binary_tree (t_binary_tree const &);
	~t_binary_tree ();
	
	t_binary_tree operator = (t_binary_tree const &);

	t_binary_tree copy (t_index);

	void clear ();
	void clear (t_index);
	
	void add_node (int);
	bool remove_node_key (int);
	bool remove_node_index (t_index);
	
	bool is_empty () { return (m_root == nullptr); }
	bool is_balanced ();

	int level (int);
	int height ();
	int node_cnt ();
	
	t_index index (int);
	int key (t_index);

	int key_max ();
	int key_min ();
	int key_sum ();

	std::vector<int> keys ();
	
	void print ();
	void print_level (int);
	void print_leaves ();
	
};
