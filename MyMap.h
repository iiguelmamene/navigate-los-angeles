#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

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
				if (cur->left != nullptr)
					cur = cur->left;
				else
				{
					cur->left = new Node(key, value);
					m_size++;
					return;
				}
			}
			else if (key > cur->valuekt)
			{
				if (cur->right != nullptr)
					cur = cur->right;
				else
				{
					cur->right = new Node(key, value);
					m_size++;
					return;
				}
			}
		}
	}

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

	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

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

#endif
