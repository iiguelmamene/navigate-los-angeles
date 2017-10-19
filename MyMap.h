#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap()
	{
		m_root = nullptr;
		m_size = 0;
	}
	~MyMap()
	{
		clear();
	}
	void clear()
	{
		m_size = 0;
		Node* cur = m_root;
		freeit(cur);
	}
	int size() const
	{
		return m_size;
	}
	void associate(const KeyType& key, const ValueType& value)
	{
		if (m_root == nullptr)
		{
			m_root = new Node(key, value);
			m_size++;
			return;
		}
		Node* cur = m_root;
		for (;;)
		{
			if (key == cur->valuekt)
			{
				cur->valuevt = value;
				return;
			}
			if (key < cur->valuekt)
			{
				//cout << "hi" << endl;
				if (cur->left != nullptr)
					cur = cur->left;
				else
				{
					cur->left = new Node(key, value);
					//cout << "hiiii" << endl;
					m_size++;
					return;
				}
			}
			else if (key > cur->valuekt)
			{
				//cout << "hiii" << endl;
				if (cur->right != nullptr)
					cur = cur->right;
				else
				{
					cur->right = new Node(key, value);
					//cout << "hii" << endl;
					m_size++;
					return;
				}
			}
		}
		//m_size++;
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
	{
		Node* ptr = m_root;
		while (ptr != nullptr)
		{
			if (key == ptr->valuekt)
				return &(ptr->valuevt);
			else if (key < ptr->valuekt)
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}
		return nullptr;
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		Node(const KeyType kt, const ValueType vt)
		{
			valuekt = kt;
			valuevt = vt;
			left = nullptr;
			right = nullptr;
		}
		KeyType valuekt;
		ValueType valuevt;
		Node* left;
		Node* right;
	};

	void freeit(Node* cur)
	{
		if (cur == nullptr)
			return;
		freeit(cur->left);
		freeit(cur->right);
		delete cur;
	}

	Node* m_root;
	int m_size;
};

#endif // MYMAP_INCLUDED

