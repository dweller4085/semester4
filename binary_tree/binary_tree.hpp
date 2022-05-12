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

	void copy (t_node *);
	void copy_rec (t_node *, t_node *);

	int height (t_node *) const;
	void height_rec (t_node *, int &, int &) const;

	void level_rec (t_node *, int &, int &, int) const;

	t_node * at_index (int) const;
	virtual t_node * with_key (int) const;

	virtual bool remove_node (t_node *);

	std::vector <t_node *> breadth_first (t_node *) const;

	t_binary_tree (t_node * root) : m_root {root} {}

public:
	t_binary_tree ();
	t_binary_tree (t_binary_tree const &);
	t_binary_tree (t_binary_tree && other) { m_root = other.m_root; other.m_root = nullptr; }
	virtual ~t_binary_tree ();
	
	t_binary_tree & operator = (t_binary_tree const &);
	
	t_binary_tree get_copy (int) const;

	t_binary_tree & clear ();
	t_binary_tree & clear_subtrees (int);
	
	virtual t_binary_tree& add_node (int);
	bool remove_node_key (int);
	bool remove_node_index (int);
	
	bool is_empty () const;
	bool is_balanced () const;

	virtual int level (int) const;
	int height () const;
	int node_count () const;
	
	int index (int) const;
	int key (int) const;

	virtual int key_max () const;
	virtual int key_min () const;
	int key_sum () const;

	std::vector<int> keys () const;
	
	void print () const;
	void print_level (int) const;
	void print_leaves () const;

	bool is_search_tree () const;
};
