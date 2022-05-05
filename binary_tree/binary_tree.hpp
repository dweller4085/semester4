#include <vector>

class t_binary_tree {
protected:
	class t_node {
	public:
		t_node * left;
		t_node * right;
		int key;

		t_node ();
		t_node (t_node *, t_node *, int);
		t_node (int);
		~t_node () = default;

		int children_count () const;

	} * m_root;

	void clear (t_node * &);
	void clear_rec (t_node * &);

	void copy (t_binary_tree const &);
	void copy_rec (t_node *, t_node *);

	int height (t_node *) const;
	void height_rec (t_node *, int &, int &) const;

	void level_rec (t_node *, int &, int &, int) const;

	t_node * at_index (int) const;
	t_node * with_key (int) const;

	bool remove_node (t_node *);

	t_binary_tree (t_node *);

	std::vector <t_node *> breadth_first (t_node *) const;

public:

	void test_a () {
		/*
		0
		4 99
		41 42 98 97
		5 . 6 . . . . .
		*/
		m_root = new t_node (0);
		m_root -> left = new t_node (new t_node {41}, new t_node {42}, 4);
		m_root -> right = new t_node {new t_node {98}, new t_node {97}, 99};
		m_root -> left -> left -> left = new t_node {5};
		m_root -> left -> right -> left = new t_node {6};
	}

	t_binary_tree ();
	t_binary_tree (t_binary_tree const &);
	~t_binary_tree ();
	
	t_binary_tree & operator = (t_binary_tree const &);
	
	t_binary_tree get_copy (int) const;

	t_binary_tree & clear ();
	t_binary_tree & clear_subtrees (int);
	
	t_binary_tree & add_node (int);
	bool remove_node_key (int);
	bool remove_node_index (int);
	
	bool is_empty () const;
	bool is_balanced () const;

	int level (int) const;
	int height () const;
	int node_count () const;
	
	int index_of (int) const;
	int key_of (int) const;

	int key_max () const;
	int key_min () const;
	int key_sum () const;

	std::vector<int> keys () const;
	
	void print () const;
	void print_level (int) const;
	void print_leaves () const;

	//t_binary_tree & subtree (int) const;
	//t_binary_tree & operator [] (int) const;
};
