#pragma once

#include "Map.h"
#include "MesonException.h"

#pragma warning(disable:4355)

Meson_Common_Collections_BEGIN

Meson::Common::Memory::MemoryManager *GetTreeMapMemoryManager(void);

Meson::Common::Memory::MemoryManager *GetTreeMapNodeMemoryManager(void);

template<class TKey, class TValue>
struct TTreeMapNode
{
	bool m_bRed;
	TTreeMapNode* m_pChild[2];
	TKeyValuePair<TKey, TValue> m_keyValuePair;

	TTreeMapNode(void)
		: m_bRed(true)
		, m_keyValuePair()
	{
		m_pChild[0] = m_pChild[1] = NULL;
	}

	TTreeMapNode(const TKey& p_key, const TValue& p_value)
		: m_bRed(true)
		, m_keyValuePair(p_key, p_value)
	{
		m_pChild[0] = m_pChild[1] = NULL;
	}

	void* operator new(size_t p_unSize)
	{
		return GetTreeMapNodeMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}
};

// forward declaration
template<class TKey, class TValue>
class TTreeMap;

template<class TKey, class TValue>
class TTreeMapPairEnumerator
	: public TEnumerator< TKeyValuePair<TKey, TValue> >
{
private:
	TTreeMap<TKey, TValue>* m_pTreeMap;
	bool* m_pbEnumerating;
	TTreeMapNode<TKey, TValue>* m_enumaratorStack[sizeof(size_t) * 8];
	size_t m_unEnumeratorStackTop;
	TTreeMapNode<TKey, TValue>* m_pNodeEnumerator;
	size_t m_unAvailable;

public:
	TTreeMapPairEnumerator(
		TTreeMap<TKey, TValue>* p_pTreeMap,
		bool* p_pbEnumerating)
		: m_pTreeMap(p_pTreeMap)
		, m_pbEnumerating(p_pbEnumerating)
		, m_unEnumeratorStackTop(0)
		, m_pNodeEnumerator(NULL)
		, m_unAvailable(p_pTreeMap->Size())
	{
		Reset();
	}

	bool HasMoreElements(void)
	{
		if( !*m_pbEnumerating )
			throw new MesonException("Map was modified during enumeration",
				__FILE__, __LINE__);

		return m_pNodeEnumerator != NULL;
	}

	void Reset(void)
	{
		m_pNodeEnumerator = m_pTreeMap->m_pRoot;
		m_unEnumeratorStackTop = 0;

		while (m_pNodeEnumerator != NULL)
		{
		    if (m_pNodeEnumerator->m_pChild[1] != NULL)
				m_enumaratorStack[m_unEnumeratorStackTop++]
					= m_pNodeEnumerator->m_pChild[1];
		    m_enumaratorStack[m_unEnumeratorStackTop++]
				= m_pNodeEnumerator;
			m_pNodeEnumerator = m_pNodeEnumerator->m_pChild[0];
		}

		if (m_unEnumeratorStackTop > 0)
			m_pNodeEnumerator = m_enumaratorStack[--m_unEnumeratorStackTop];

		m_unAvailable = m_pTreeMap->Size();
		*m_pbEnumerating = true;
	}

	TKeyValuePair<TKey, TValue>& NextElement(void)
	{
		if( !*m_pbEnumerating )
			throw new MesonException("Map was modified during enumeration",
				__FILE__, __LINE__);

		if( m_pNodeEnumerator == NULL )
			throw new MesonException("No more elements to enumerate.",
				__FILE__, __LINE__);

		--m_unAvailable;

		if (m_unEnumeratorStackTop > 0)
		{
			if (m_pNodeEnumerator->m_pChild[1] == NULL)
			{
				TKeyValuePair<TKey, TValue>& keyValuePair = m_pNodeEnumerator->m_keyValuePair;
				m_pNodeEnumerator = m_enumaratorStack[--m_unEnumeratorStackTop];
				return keyValuePair;
			}
			else
			{
				TKeyValuePair<TKey, TValue>& keyValuePair = m_pNodeEnumerator->m_keyValuePair;
				m_pNodeEnumerator = m_enumaratorStack[--m_unEnumeratorStackTop];
				while (m_pNodeEnumerator != NULL)
				{
					if (m_pNodeEnumerator->m_pChild[1] != NULL)
						m_enumaratorStack[m_unEnumeratorStackTop++]
							= m_pNodeEnumerator->m_pChild[1];
					m_enumaratorStack[m_unEnumeratorStackTop++]
						= m_pNodeEnumerator;
					m_pNodeEnumerator = m_pNodeEnumerator->m_pChild[0];
				}
				m_pNodeEnumerator = m_enumaratorStack[--m_unEnumeratorStackTop];
				return keyValuePair;
			}
		}
		else
		{
			TKeyValuePair<TKey, TValue>& keyValuePair = m_pNodeEnumerator->m_keyValuePair;
			m_pNodeEnumerator = NULL;
			return keyValuePair;
		}
	}

	size_t Size(void) const
	{
		return m_pTreeMap->Size();
	}

	size_t Available(void) const
	{
		return m_unAvailable;
	}
};

template<class TKey, class TValue>
class TTreeMapKeyEnumerator
	: public TEnumerator<TKey>
{
private:
	TTreeMapPairEnumerator<TKey, TValue> m_treeMapPairEnumerator;

public:
	TTreeMapKeyEnumerator(
		TTreeMap<TKey, TValue>* p_pTreeMap, bool* p_pbEnumerating)
		: m_treeMapPairEnumerator(p_pTreeMap, p_pbEnumerating)
	{
	}

	~TTreeMapKeyEnumerator(void)
	{
	}

	bool HasMoreElements(void)
	{
		return m_treeMapPairEnumerator.HasMoreElements();
	}

	void Reset(void)
	{
		return m_treeMapPairEnumerator.Reset();
	}

	TKey& NextElement(void)
	{
		return m_treeMapPairEnumerator.NextElement().Key;
	}

	size_t Size(void) const
	{
		return m_treeMapPairEnumerator.Size();
	}

	size_t Available(void) const
	{
		return m_treeMapPairEnumerator.Available();
	}
};

template<class TKey, class TValue>
class TTreeMapValueEnumerator
	: public TEnumerator<TValue>
{
private:
	TTreeMapPairEnumerator<TKey, TValue> m_treeMapPairEnumerator;

public:
	TTreeMapValueEnumerator(
		TTreeMap<TKey, TValue>* p_pTreeMap, bool* p_pbEnumerating)
		: m_treeMapPairEnumerator(p_pTreeMap, p_pbEnumerating)
	{
	}

	~TTreeMapValueEnumerator(void)
	{
	}

	bool HasMoreElements(void)
	{
		return m_treeMapPairEnumerator.HasMoreElements();
	}

	void Reset(void)
	{
		return m_treeMapPairEnumerator.Reset();
	}

	TValue& NextElement(void)
	{
		return m_treeMapPairEnumerator.NextElement().Value;
	}

	size_t Size(void) const
	{
		return m_treeMapPairEnumerator.Size();
	}

	size_t Available(void) const
	{
		return m_treeMapPairEnumerator.Available();
	}
};


template<class TKey, class TValue>
class TTreeMap
	: public TMap<TKey, TValue>
{
	friend class TTreeMapPairEnumerator<TKey, TValue>;

private: // private variables
	TTreeMapNode<TKey, TValue>* m_pRoot;
	size_t m_unSize;
	bool m_bEnumerating;
	TTreeMapPairEnumerator<TKey, TValue> m_treeMapPairEnumerator;
	TTreeMapKeyEnumerator<TKey, TValue> m_treeMapKeyEnumerator;
	TTreeMapValueEnumerator<TKey, TValue> m_treeMapValueEnumerator;

private: // private methods
	inline bool IsRedNode(TTreeMapNode<TKey, TValue>* p_pTreeMapNode)
	{
		return p_pTreeMapNode != NULL && p_pTreeMapNode->m_bRed;
	}

	TTreeMapNode<TKey, TValue>* DoSingleRotation(
		TTreeMapNode<TKey, TValue>* p_pTreeMapNode,
		int nChild)
	{
		TTreeMapNode<TKey, TValue>* pNodeTemp = p_pTreeMapNode->m_pChild[!nChild];
		p_pTreeMapNode->m_pChild[!nChild] = pNodeTemp->m_pChild[nChild];
		pNodeTemp->m_pChild[nChild] = p_pTreeMapNode;
		p_pTreeMapNode->m_bRed = true;
		pNodeTemp->m_bRed = false;
		return pNodeTemp;
	}

	TTreeMapNode<TKey, TValue>* DoDoubleRotation(
		TTreeMapNode<TKey, TValue>* p_pTreeMapNode,
		int nChild)
	{
		p_pTreeMapNode->m_pChild[!nChild]
			= DoSingleRotation(p_pTreeMapNode->m_pChild[!nChild], !nChild);
		return DoSingleRotation(p_pTreeMapNode, nChild);
	}

	TTreeMapNode<TKey, TValue>* CloneNode(TTreeMapNode<TKey, TValue>* p_pNode)
	{
		TTreeMapNode<TKey, TValue> *pNodeClone
			= new TTreeMapNode<TKey, TValue>(p_pNode->m_keyValuePair.Key, p_pNode->m_keyValuePair.Value);
		if (p_pNode->m_pChild[0] != NULL)
			pNodeClone->m_pChild[0] = CloneNode(p_pNode->m_pChild[0]);
		if (p_pNode->m_pChild[1] != NULL)
			pNodeClone->m_pChild[1] = CloneNode(p_pNode->m_pChild[1]);
		return pNodeClone;
	}

public: // public methods
	TTreeMap(void)
		: m_pRoot(NULL)
		, m_unSize(0)
		, m_bEnumerating(false)
		, m_treeMapPairEnumerator(this, &m_bEnumerating)
		, m_treeMapKeyEnumerator(this, &m_bEnumerating)
		, m_treeMapValueEnumerator(this, &m_bEnumerating)
	{
	}

	TTreeMap(const TImmutableMap<TKey, TValue>& p_map)
		: m_pRoot(NULL)
		, m_unSize(p_map.Size())
		, m_bEnumerating(false)
		, m_treeMapPairEnumerator(this, &m_bEnumerating)
		, m_treeMapKeyEnumerator(this, &m_bEnumerating)
		, m_treeMapValueEnumerator(this, &m_bEnumerating)
	{
		InsertMap(p_map);
	}

	TTreeMap(const TMap<TKey, TValue>& p_map)
		: m_pRoot(NULL)
		, m_unSize(p_map.Size())
		, m_bEnumerating(false)
		, m_treeMapPairEnumerator(this, &m_bEnumerating)
		, m_treeMapKeyEnumerator(this, &m_bEnumerating)
		, m_treeMapValueEnumerator(this, &m_bEnumerating)
	{
		InsertMap(p_map);
	}

	TTreeMap(const TTreeMap<TKey, TValue>& p_map)
		: m_pRoot(NULL)
		, m_unSize(p_map.m_unSize)
		, m_bEnumerating(false)
		, m_treeMapPairEnumerator(this, &m_bEnumerating)
		, m_treeMapKeyEnumerator(this, &m_bEnumerating)
		, m_treeMapValueEnumerator(this, &m_bEnumerating)
	{
		if (p_map.m_pRoot != NULL)
			m_pRoot = CloneNode(p_map.m_pRoot);
	}

	~TTreeMap(void)
	{
		Clear();
	}

	void* operator new(size_t p_unSize)
	{
		return GetTreeMapMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}							

	inline size_t Size(void) const
	{
		return m_unSize;
	}

	inline bool ContainsKey(const TKey& p_key) const
	{
		TTreeMapNode<TKey, TValue>* pNode = m_pRoot;
		while (pNode != NULL)
		{
			if (pNode->m_keyValuePair.Key == p_key) return true;
			pNode = pNode->m_pChild[pNode->m_keyValuePair.Key < p_key];
		}
		return false;
	}

	inline bool ContainsKey(const TKey& p_key, TValue &p_value) const
	{
		TTreeMapNode<TKey, TValue>* pNode = m_pRoot;
		while (pNode != NULL)
		{
			if (pNode->m_keyValuePair.Key == p_key)
			{
				p_value = pNode->m_keyValuePair.Value;
				return true;
			}
			pNode = pNode->m_pChild[pNode->m_keyValuePair.Key < p_key];
		}
		return false;
	}

	inline const TValue& ValueAt(const TKey& p_key) const
	{
		TTreeMapNode<TKey, TValue>* pNode = m_pRoot;
		while (pNode != NULL)
		{
			if (pNode->m_keyValuePair.Key == p_key) return pNode->m_keyValuePair.Value;
			pNode = pNode->m_pChild[pNode->m_keyValuePair.Key < p_key];
		}
		
		throw new MesonException("Key not found in TreeMap.",
			__FILE__, __LINE__);
	}

	TEnumerator<TKey>& GetKeyEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_treeMapKeyEnumerator.Reset();
		m_bEnumerating = true;
		return m_treeMapKeyEnumerator;
	}

	inline typename TEnumerator<TKey>::Ptr CreateKeyEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator<TKey>::Ptr(
			new TTreeMapKeyEnumerator<TKey, TValue>(this, &m_bEnumerating));
	}

	TEnumerator<TValue>& GetValueEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_treeMapValueEnumerator.Reset();
		m_bEnumerating = true;
		return m_treeMapValueEnumerator;
	}

	inline typename TEnumerator<TValue>::Ptr CreateValueEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator<TValue>::Ptr(
			new TTreeMapValueEnumerator<TKey, TValue>(this, &m_bEnumerating));
	}

	TEnumerator<TKeyValuePair<TKey, TValue>>& GetEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_treeMapPairEnumerator.Reset();
		m_bEnumerating = true;
		return m_treeMapPairEnumerator;
	}

	inline typename TEnumerator<TKeyValuePair<TKey, TValue>>::Ptr CreateEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator< TKeyValuePair<TKey, TValue> >::Ptr(
			new TTreeMapPairEnumerator<TKey, TValue>(this, &m_bEnumerating));
	}

	inline TMap<TKey, TValue>& operator=(const TMap<TKey, TValue>& p_map)
	{
		if (&p_map == this) return *this;

		Clear();
		InsertMap(p_map);
		return *this;
	}

	inline TMap<TKey, TValue>& operator=(const TTreeMap<TKey, TValue>& p_map)
	{
		if (&p_map == this) return *this;

		Clear();
		InsertMap(p_map);
		return *this;
	}

	inline TTreeMap<TKey, TValue> operator-(const TKey& p_key)
	{
		TTreeMap<TKey, TValue> mapNew;
		mapNew.InsertMap(*this);
		mapNew.Remove(p_key);
		return mapNew;
	}

	inline void Clear(void)
	{
		TTreeMapNode<TKey, TValue>* pNode = m_pRoot;
		TTreeMapNode<TKey, TValue>* pNodeTemp;

		while (pNode != NULL)
		{
			if (pNode->m_pChild[0] != NULL)
			{
				// if node has left child, do right rotation
				pNodeTemp = pNode->m_pChild[0];
				pNode->m_pChild[0] = pNodeTemp->m_pChild[1];
				pNodeTemp->m_pChild[1] = pNode;
			}
			else
			{
				// otherwise delete degenerate node
				pNodeTemp = pNode->m_pChild[1];
				delete pNode;
			}
			// advance to next child
			pNode = pNodeTemp;
		}
		m_pRoot = NULL;
		m_unSize = 0;
		m_bEnumerating = false;
	}

	inline TValue& operator[](const TKey& p_key)
	{
		TTreeMapNode<TKey, TValue>* pNode = m_pRoot;
		while (pNode != NULL)
		{
			if (pNode->m_keyValuePair.Key == p_key) return pNode->m_keyValuePair.Value;
			pNode = pNode->m_pChild[pNode->m_keyValuePair.Key < p_key];
		}

		TValue value;
		Insert(p_key, value);

		pNode = m_pRoot;
		while (true)
		{
			if (pNode->m_keyValuePair.Key == p_key) return pNode->m_keyValuePair.Value;
			pNode = pNode->m_pChild[pNode->m_keyValuePair.Key < p_key];
		}
	}

	inline TValue& operator()(const TKey& p_key)
	{
		TTreeMapNode<TKey, TValue>* pNode = m_pRoot;
		while (pNode != NULL)
		{
			if (pNode->m_keyValuePair.Key == p_key) return pNode->m_keyValuePair.Value;
			pNode = pNode->m_pChild[pNode->m_keyValuePair.Key < p_key];
		}

		TValue value;
		Insert(p_key, value);

		pNode = m_pRoot;
		while (true)
		{
			if (pNode->m_keyValuePair.Key == p_key) return pNode->m_keyValuePair.Value;
			pNode = pNode->m_pChild[pNode->m_keyValuePair.Key < p_key];
		}
	}

	void Insert(const TKey& p_key, const TValue& p_value)
	{
		if (m_pRoot == NULL)
		{
			// empty tree case
			m_pRoot = new TTreeMapNode<TKey, TValue>(p_key, p_value);
			m_pRoot->m_bRed = false;
			m_unSize = 1;
			m_bEnumerating = false;
			return;
		}
		
		// other cases
		TTreeMapNode<TKey, TValue> nodeHead; // false tree root

		TTreeMapNode<TKey, TValue> *pNodeGrandparent;
		TTreeMapNode<TKey, TValue> *pNodeParent;
		TTreeMapNode<TKey, TValue> *pIterator;
		TTreeMapNode<TKey, TValue> *pIteratorParent;
		int nChild = 0;
		int nLast;

		// set up relative nodes
		pNodeParent = &nodeHead;
		pNodeGrandparent = pIterator = NULL;
		pIteratorParent = pNodeParent->m_pChild[1] = m_pRoot;

		// search down the tree
		while(true)
		{
			if (pIteratorParent == NULL)
			{
				// insert new node at the bottom
				pIterator->m_pChild[nChild] = pIteratorParent
					= new TTreeMapNode<TKey, TValue>(p_key, p_value);
				++m_unSize;
				m_bEnumerating = false;
			}
			else if (IsRedNode(pIteratorParent->m_pChild[0])
				&& IsRedNode(pIteratorParent->m_pChild[1]))
			{
				// colour flip
				pIteratorParent->m_bRed = true;
				pIteratorParent->m_pChild[0]->m_bRed = false;
				pIteratorParent->m_pChild[1]->m_bRed = false;
			}

			// fix red violation
			if (IsRedNode(pIteratorParent) && IsRedNode(pIterator))
			{
				int nChildInner
					= pNodeParent->m_pChild[1] == pNodeGrandparent;

				if (pIteratorParent == pIterator->m_pChild[nLast])
					pNodeParent->m_pChild[nChildInner] = DoSingleRotation(pNodeGrandparent, !nLast);
				else
					pNodeParent->m_pChild[nChildInner] = DoDoubleRotation(pNodeGrandparent, !nLast);
			}

			// stop search if found
			if ( pIteratorParent->m_keyValuePair.Key == p_key )
			{
				// update existing node
				pIteratorParent->m_keyValuePair.Value = p_value;
				m_bEnumerating = false;
				break;
			}

			nLast = nChild;
			nChild = pIteratorParent->m_keyValuePair.Key < p_key;

			// update relative nodes
			if ( pNodeGrandparent != NULL )
				pNodeParent = pNodeGrandparent;
			pNodeGrandparent = pIterator;
			pIterator = pIteratorParent;
			pIteratorParent = pIteratorParent->m_pChild[nChild];
		}

		// update root node and make black
		m_pRoot = nodeHead.m_pChild[1];
		m_pRoot->m_bRed = false;
	}

	inline void InsertMap(const TImmutableMap<TKey, TValue>& p_map)
	{
		for (TEnumerator<TKey> &enumerator
			= ((TImmutableMap<TKey, TValue> &)p_map).GetKeyEnumerator();
			enumerator.HasMoreElements();)
		{
			TKey &key = enumerator.NextElement();
			Insert(key, p_map.ValueAt(key));
		}
	}

	void RemoveKey(const TKey& p_key)
	{
		if (m_pRoot == NULL) return;

		TTreeMapNode<TKey, TValue> nodeHead; // false tree root
		TTreeMapNode<TKey, TValue> *pNodeIterator,
			*pNodeParent, *pNodeGrandparent; // relative nodes
		TTreeMapNode<TKey, TValue> *pNodeFound = NULL;  // found item
		int nChild = 1;

		// set up relative nodes
		pNodeIterator = &nodeHead;
		pNodeGrandparent = pNodeParent = NULL;
		pNodeIterator->m_pChild[1] = m_pRoot;

		// search and push a red down
		while (pNodeIterator->m_pChild[nChild] != NULL)
		{
			int nLast = nChild;

			// update relative nodes
			pNodeGrandparent = pNodeParent;
			pNodeParent = pNodeIterator;
			pNodeIterator = pNodeIterator->m_pChild[nChild];
			nChild = pNodeIterator->m_keyValuePair.Key < p_key;

			// save found node
			if (pNodeIterator->m_keyValuePair.Key == p_key)
				pNodeFound = pNodeIterator;

			if (IsRedNode(pNodeIterator)
				|| IsRedNode(pNodeIterator->m_pChild[nChild]))
				continue;

			// push the red node down
			if (IsRedNode(pNodeIterator->m_pChild[!nChild]))
				pNodeParent = pNodeParent->m_pChild[nLast]
					= DoSingleRotation(pNodeIterator, nChild);
			else if (!IsRedNode(pNodeIterator->m_pChild[!nChild]))
			{
				TTreeMapNode<TKey, TValue>* pNodeLast
					= pNodeParent->m_pChild[!nLast];

				if (pNodeLast != NULL)
				{
					if (!IsRedNode(pNodeLast->m_pChild[!nLast])
						&& !IsRedNode(pNodeLast->m_pChild[nLast]))
					{
						// color flip
						pNodeParent->m_bRed = false;
						pNodeLast->m_bRed = true;
						pNodeIterator->m_bRed = true;
					}
					else
					{
						int nChildInner = pNodeGrandparent->m_pChild[1] == pNodeParent;

						if (IsRedNode(pNodeLast->m_pChild[nLast]))
							pNodeGrandparent->m_pChild[nChildInner]
								= DoDoubleRotation(pNodeParent, nLast);
						else if (IsRedNode(pNodeLast->m_pChild[!nLast]))
							pNodeGrandparent->m_pChild[nChildInner]
								= DoSingleRotation(pNodeParent, nLast);

						// ensure correct coloring
						pNodeIterator->m_bRed = pNodeGrandparent->m_pChild[nChildInner]->m_bRed = true;
						pNodeGrandparent->m_pChild[nChildInner]->m_pChild[0]->m_bRed = false;
						pNodeGrandparent->m_pChild[nChildInner]->m_pChild[1]->m_bRed = false;
					}
				}
			}			
		}

		// replace and remove if found
		if (pNodeFound != NULL)
		{
			pNodeFound->m_keyValuePair.Key = pNodeIterator->m_keyValuePair.Key;
			pNodeFound->m_keyValuePair.Value = pNodeIterator->m_keyValuePair.Value;
			pNodeParent->m_pChild[pNodeParent->m_pChild[1] == pNodeIterator] =
				pNodeIterator->m_pChild[pNodeIterator->m_pChild[0] == NULL];
			delete pNodeIterator;

			--m_unSize;
			m_bEnumerating = false;
		}

		// update root and make it black
		m_pRoot = nodeHead.m_pChild[1];
		if (m_pRoot != NULL)
			m_pRoot->m_bRed = false;
	}

	void RemoveValue(const TValue& p_value)
	{
		TEnumerator<TKey>& enumKeys = GetKeyEnumerator();
		while(enumKeys.HasMoreElements())
		{
			TKey& key = enumKeys.NextElement();
			if (ValueAt(key) == p_value)
			{
				RemoveKey(key);
				enumKeys.Reset();
			}
		}
	}

	inline TImmutableMap<TKey, TValue>& AsImmutableMap(void) const
	{
		return (TImmutableMap<TKey, TValue>&) *this;
	}
};

Meson_Common_Collections_END
