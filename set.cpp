// set.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#define BOOST_TEST_MODULE testSet
#include <boost/test/unit_test.hpp>

struct set
{
private:

	struct node
	{
		friend set;

	private : 

		int __val;

	public :

		node(int value, node* parent)
		{
			this->__val = value;
			this->parent = parent;
			left = right = nullptr;
		}

		int value() {
			return __val;
		}

		node *left, *right, *parent;
	};

	node* __root;
	int __count;

	void dispose(node* n)
	{
		if (n != nullptr)
		{
			dispose(n->left);
			dispose(n->right);
			delete n;
		}
	}

public:
	enum exception
	{
		getRootOnEmpty,
		noNextItem,
		noPrevItem,
		noSuchItem
	};

	int count()
	{
		return __count;
	}

	struct iterator;

	iterator const root()
	{
		return iterator(__root);
	}

	iterator const find(int value)
	{
		if (__root == nullptr) return iterator(nullptr);
		node* result = __root;
		while (result != nullptr && result->__val != value)
		{
			if (value > result->__val)
				result = result -> right;
			if (value < result->__val)
				result = result ->left;
		}
		return iterator(result);
	}

	iterator const begin()
	{
		node* result = __root;
		while (result != nullptr && result -> left != nullptr)
		{
			result = result->left;
		}
		return (result);
	}

	iterator const end()
	{
		node* result = __root;
		while (result != nullptr && result -> right != nullptr)
		{
			result = result->right;
		}
		return (result);
	}

	void insert(int value)
	{
		if (__root == nullptr)
			__root = new node(value, nullptr);
		else
		{
			if (__root->__val == value) return;
			node* parent = __root;
			node** place = __root->__val > value ? &__root->left : &__root->right;
			do
			{
				if (*place != nullptr)
					parent = *place;
				else
					break;
				if (parent != nullptr)
				{
					if (parent->__val > value)
						place = &parent->left;
					if (parent->__val < value)
						place = &parent->right;
					if (parent->__val == value)
						return;
				}
			}
			while (*place != nullptr);
			*place = new node(value, parent);
		}
	}

	void erase(iterator item)
	{
		item.assert_exists();
		node** place = ((item.target->parent == nullptr) ? &__root : item.target->parent->left == item.target ? &item.target->parent->left : &item.target->parent->right);
		if (item.target->left == nullptr)
		{
			*place = item.target->right;
			delete item.target;
			return;
		}
		if (item.target->right == nullptr)
		{
			*place = item.target->left;
			delete item.target;
			return;
		}
		iterator a(item.target);
		a = a++;
		item.target->__val = (a.target)->__val;
		erase(a);
	}

	set()
	{
		__root = nullptr;
	}

	set(set const& s)
	{
		__root = nullptr;
		set result = set();
		iterator i = iterator(s.__root);
		while (i.exists())
		{
			insert(*i);
			i++;
		}
	}

	~set()
	{
		dispose(__root);
	}

	set& operator= (set const& s)
	{
		__root = nullptr;
		set result = set();
		iterator i = iterator(s.__root);
		while (i.exists())
		{
			insert(*i);
			i++;
		}
	}

	struct iterator
	{
	private:
		iterator();
		iterator(node* const target)
		{
			this->target = target;
		}

		node* target;

	public:
		friend set;

		void assert_exists()
		{
			if (!exists()) throw noSuchItem;
		}

		bool exists()
		{
			return target != nullptr;
		}

		int value()
		{
			assert_exists();
			return target->__val;
		}

		iterator operator++()
		{
			assert_exists();
			node* result = target;
			if (result -> right == nullptr)
			{
				node* tmp = nullptr;
				while (result -> parent != nullptr && (tmp == nullptr || result->right == tmp))
				{
					tmp = result;
					result = result -> parent;
				}
				if (tmp == nullptr || result -> parent == nullptr && result -> right == tmp)
					target = nullptr;
				else
					target = result;
				return *this;
			}
			else
			{
				result = result -> right;
				while (result -> left != nullptr)
					result = result -> left;
				target = result;
				return *this;
			}	
		}

		iterator operator++(int)
		{
			assert_exists();
			node* result = target;
			if (result -> right == nullptr)
			{
				node* tmp = nullptr;
				while (result -> parent != nullptr && (tmp == nullptr || result->right == tmp))
				{
					tmp = result;
					result = result -> parent;
				}
				if (tmp == nullptr || result -> parent == nullptr && result -> right == tmp)
					target = nullptr;
				else
					target = result;
				return *this;
			}
			else
			{
				result = result -> right;
				while (result -> left != nullptr)
					result = result -> left;
				target = result;
				return *this;
			}	
		}

		iterator operator--()
		{
			assert_exists();
			node* result = target;
			if (result -> left == nullptr)
			{
				node* tmp = nullptr;
				while (result -> parent != nullptr && (tmp == nullptr || result->left == tmp))
				{
					tmp = result;
					result = result -> parent;
				}
				if (tmp == nullptr || result -> parent == nullptr && result -> left == tmp)
					target = nullptr;
				else
					target = result;
				return *this;
			}
			else
			{
				result = result -> left;
				while (result -> right != nullptr)
					result = result -> right;
				target = result;
				return *this;
			}	
		}

		iterator operator--(int)
		{
			assert_exists();
			node* result = target;
			if (result -> left == nullptr)
			{
				node* tmp = nullptr;
				while (result -> parent != nullptr && (tmp == nullptr || result->left == tmp))
				{
					tmp = result;
					result = result -> parent;
				}
				if (tmp == nullptr || result -> parent == nullptr && result -> left == tmp)
					target = nullptr;
				else
					target = result;
				return *this;
			}
			else
			{
				result = result -> left;
				while (result -> right != nullptr)
					result = result -> right;
				target = result;
				return *this;
			}	
		}

		bool operator==(iterator const& with) const
		{
			return target == with.target;
		}

		bool operator!=(iterator const& with) const
		{
			return target != with.target;
		}

		int operator*()
		{
			assert_exists();
			return target->__val;
		}
	};
};

BOOST_AUTO_TEST_CASE(testSet)
{
	set s = set();
	s.insert(0);
	s.insert(2);
	s.insert(1);
	s.insert(3);
	s.insert(-2);
	s.insert(-1);
	s.insert(-3); 
	//-3 -2 -1  0  1  2  3

	set::iterator i = s.begin();
	for (int j = -3; j <= 3; j++)
	{
		BOOST_CHECK_EQUAL(*i, j);
		i = i++;
	}

	s.erase(s.find(2));
	//-3 -2 -1  0  1  3

	i = s.find(1);
	//-3 -2 -1  0  1  3
	//             ^
	BOOST_CHECK_EQUAL(*i, 1);

	i++;
	//-3 -2 -1  0  1  3
	//             >> ^
	BOOST_CHECK_EQUAL(*i, 3);

	i = s.end();
	i--;
	i--;
	i--;
	i--;
	//-3 -2 -1  0  1  3
	//    ^ << << << <<
	BOOST_CHECK_EQUAL(*i, -2);

	set c = s;
	c.insert(2);
	i = c.find(1);
	i++;
	// c :
	//-3 -2 -1  0  1  2  3
	//             >> ^
	BOOST_CHECK_EQUAL(*i, 2);
	// s :
	//-3 -2 -1  0  1  3
	//             >> ^
	i = s.find(1);
	i++;
	BOOST_CHECK_EQUAL(*i, 3);

	i = s.root();
	i++;
	set::iterator j = s.root();
	j++;
	BOOST_CHECK_EQUAL(i == j, true);
	
	i = s.root();
	j = c.root();
	BOOST_CHECK_EQUAL(i == j, false);
}